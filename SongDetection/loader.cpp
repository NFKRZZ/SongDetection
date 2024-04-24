#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
#include "loader.h"
#include <filesystem>
#include <iostream>
#include <fftw3.h>
#include <Windows.h>
#include <cmath>
#include <algorithm>



std::vector<std::string> loader::getSongs()
{
	std::vector<std::string> files;

	std::string folderPath = "Songs";
	try
	{
		for (const auto& entry : std::filesystem::directory_iterator(folderPath))
		{
			if (std::filesystem::is_regular_file(entry.path()))
			{
				std::string file = entry.path().filename().string();
				file = "Songs/" + file;
				Util::WAVE_HEADER wav = getHdr(file);
				std::cout << file <<" Sample Rate: " << wav.SampleRate <<" Hz" << std::endl;

				files.push_back(file);
				
			}
		}
		
	}
	catch(int e)
	{

	}
	return files;

}

std::vector<std::string> loader::getHash()
{
	std::vector<std::string> files;

	std::string folderPath = "Hashes";
	try
	{
		for (const auto& entry : std::filesystem::directory_iterator(folderPath))
		{
			if (std::filesystem::is_regular_file(entry.path()))
			{
				std::string file = entry.path().filename().string();
				file = "Hashes/" + file;
				

				files.push_back(file);

			}
		}

	}
	catch (int e)
	{

	}
	return files;
}

std::unordered_map<int, std::vector<double>> loader::getHash(std::vector<short> pcmData,int sampleRate)
{
	std::vector<short> pcm = Util::TwoChannelToMono(pcmData);
	int length = sampleRate / 10; //EXPERIMENT WITH HALF SECOND LENGTH SO /2
	int numOfChunks = pcm.size() / length;
	std::vector<std::vector<double>> sampleChunks;
	sampleChunks.resize(numOfChunks);
	std::unordered_map<int, std::vector<double>> hashTable;

	for (int i = 0; i < numOfChunks; i++)
	{
		for (int j = 0; j < length; j++)
		{
			sampleChunks[i].push_back(pcm[(i * length) + j]);
		}
	}
	for (int i = 0; i < sampleChunks.size(); i++)
	{
		int N = sampleChunks[i].size();
		fftw_complex* fftDat = loader::getFFT(sampleChunks[i]);
		//Find top 5 frequencies below 500 Hz

		std::vector<double> frequencies;
		std::vector<double> finalFreq;
		for (int l = 0; l < N; l++) 
		{
			double magnitude = sqrt(fftDat[l][0] * fftDat[l][0] + fftDat[l][1] * fftDat[l][1]);
			double frequency = l * (sampleRate / N);

			if (frequency <= 500.0) 
			{
				frequencies.push_back(frequency);
			}
		}
		std::sort(frequencies.begin(), frequencies.end(), std::greater<double>());
		int numTopFrequencies = (frequencies.size() < 5) ? frequencies.size() : 5;
		finalFreq.assign(frequencies.begin(), frequencies.begin() + numTopFrequencies);

		//Find and insert the next top 10 frequencies past 500 Hz
		frequencies.clear();

		for (int l = 0; l < N; l++)
		{

			double mag = sqrt(fftDat[l][0] * fftDat[l][0] + fftDat[l][1] * fftDat[l][1]);
			double freq = l * (sampleRate / N);
			if (freq > 500.0 && freq <= sampleRate / 2)
			{
				continue;
			}
			frequencies.push_back(freq);
		}
		std::sort(frequencies.begin(), frequencies.end(), std::greater<double>());
		int numTopFrequencieSec = (frequencies.size() < 10) ? frequencies.size() : 10;
		finalFreq.insert(finalFreq.end(), frequencies.begin(), frequencies.begin() + numTopFrequencieSec);
		hashTable.insert(std::make_pair(i, finalFreq));
	}
	return hashTable;
}

void loader::saveHash()
{
	
}

std::vector<Util::HashTable> loader::loadHash()
{
	return std::vector<Util::HashTable>();
}

fftw_complex* loader::getFFT(std::vector<short> pcmData)
{

	fftw_complex* in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * pcmData.size());
	fftw_complex* out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * pcmData.size());

	fftw_plan p = fftw_plan_dft_1d(pcmData.size(), in, out, FFTW_FORWARD, FFTW_ESTIMATE);

	for (int i = 0; i < pcmData.size(); ++i) 
	{
		in[i][0] = pcmData[i];
		in[i][1] = 0.0;
	}
	fftw_execute(p);
	fftw_free(in);

	return out;
}
fftw_complex* loader::getFFT(std::vector<double> pcmData)
{
	fftw_complex* in = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * pcmData.size());
	fftw_complex* out = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * pcmData.size());

	fftw_plan p = fftw_plan_dft_1d(pcmData.size(), in, out, FFTW_FORWARD, FFTW_ESTIMATE);

	for (int i = 0; i < pcmData.size(); ++i)
	{
		in[i][0] = pcmData[i];
		in[i][1] = 0.0;
	}
	fftw_execute(p);
	fftw_free(in);

	return out;
}

std::vector<short> loader::getPCMData(std::string file)
{
	std::vector<short int> data;
	Util::WAVE_HEADER waveheader;
	FILE* sound;
	bool foundList = false;
	sound = fopen(file.c_str(), "rb");
	short int D;
	fread(&waveheader, sizeof(waveheader), 1, sound);

	/*std::cout << "Chunk: " << waveheader.Chunk << std::endl;
	std::cout << "Chunk Size: " << waveheader.ChunkSize << std::endl;
	std::cout << "Format: " << waveheader.format << std::endl;

	std::cout << "SubChunk1ID: " << waveheader.Sub_chunk1ID << std::endl;
	std::cout << "SubChunk1Size: " << waveheader.Sub_chunk1Size << std::endl;
	std::cout << "Audio Format: " << waveheader.AudioFormat << std::endl;
	std::cout << "Num of Channels: " << waveheader.NumChannels << std::endl;
	std::cout << "SampleRate: " << waveheader.SampleRate << std::endl;
	std::cout << "ByteRate: " << waveheader.ByteRate << std::endl;
	std::cout << "Block Align: " << waveheader.BlockAlign << std::endl;
	std::cout << "Bits Per Sample:" << waveheader.BitsPerSample << std::endl;

	std::cout << "SubChunk2ID: " << waveheader.Sub_chunk2ID << std::endl;
	std::cout << "SubChunk2Size: " << waveheader.Sub_chunk2Size << std::endl;
	std::cout << "sizeof " << sizeof(waveheader.BitsPerSample) << " reg " << waveheader.BitsPerSample << std::endl;*/

	std::wstring temp = std::wstring(file.begin(), file.end());
	LPCWSTR ws = temp.c_str();
	HANDLE h = CreateFile(ws, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);	//get Size
	int size = GetFileSize(h, NULL);

	char lol[4];
	int location = 0;
	std::ifstream read;
	read.open(file);
	read.seekg(0, read.beg);
	for (int i = 0; i < size; i++)
	{
		try
		{
			read.seekg(i);
			read.read(reinterpret_cast<char*>(&lol), sizeof(lol));
			if (lol[0] == 'L' && lol[1] == 'I' && lol[2] == 'S' && lol[3] == 'T')
			{
				location = i;
				foundList = true;
				//std::cout << "FOUND LIST AT: " << i << std::endl;
				break;
			}
		}
		catch (std::exception e)
		{
		}
	}
	//std::cout << "Finished looking for LIST" << std::endl;
	int listSize = 0;
	int positon = 0;
	if (foundList)
	{
		read.seekg(location + 4);
		read.read(reinterpret_cast<char*>(&listSize), sizeof(listSize));
		//std::cout << listSize << std::endl;
		read.seekg(location + 8 + listSize);
		positon = location + 8 + listSize;
	}
	else
	{
		positon = 44;
	}
	fseek(sound, positon, SEEK_SET);
	std::chrono::system_clock::time_point now1 = std::chrono::system_clock::now();
	
	while (!feof(sound))
	{
		fread(&D, waveheader.BitsPerSample / 8, 1, sound);
		data.push_back(D);
	}
	std::chrono::system_clock::time_point now2 = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed = now2 - now1;
	//std::cout << "Took " << elapsed.count() << " seconds" << std::endl;
	//std::cout << "Finished populating vector" << std::endl;
	return data;


}

Util::WAVE_HEADER loader::getHdr(std::string filePath)
{
	Util::WAVE_HEADER hdr;
	FILE* l;
	l = fopen(filePath.c_str(), "rb");
	short D;
	fread(&hdr, sizeof(hdr), 1, l);
	return hdr;
}

void loader::compareSongs(std::vector<Song>& loadedHashes, std::vector<std::string>& filenames)
{
	std::vector<std::string> names;

	for (Song s : loadedHashes)
	{
		names.push_back(s.getName());
	}
	for (std::string& filePath : filenames) 
	{
		size_t dotWavPos = filePath.find(".wav");
		if (dotWavPos != std::string::npos) {
			filePath.erase(dotWavPos, 4); 
		}
		size_t songsPos = filePath.find("Songs/");
		if (songsPos != std::string::npos) {
			filePath.erase(songsPos, 6); 
		}
	}
	for (std::string& song : names)
	{
		size_t dotWavPos = song.find(".wav");
		if (dotWavPos != std::string::npos) {
			song.erase(dotWavPos, 4);
		}
		size_t songsPos = song.find("Songs/");
		if (songsPos != std::string::npos) {
			song.erase(songsPos, 6);
		}
	}

	for (auto it = filenames.begin(); it != filenames.end();) 
	{
		bool found = false;

		for (const std::string& name : names) 
		{
			if (*it == name) 
			{
				std::cout << "We Found a DUPLICATE!!! This is name " << name << " this is filename " << *it << std::endl;
				found = true;
				break;
			}
		}

		if (found) 
		{
			it = filenames.erase(it);
		}
		else 
		{
			++it;
		}
	}
}















