#include "Util.h"
#include <iostream>
#include <fstream>

void Util::createWavFileMono(std::vector<short> pcmData,std::string& filename)
{
	WAVE_HEADER header;
	header.Chunk[0] = 'R';
	header.Chunk[1] = 'I';
	header.Chunk[2] = 'F';
	header.Chunk[3] = 'F';

	header.ChunkSize = pcmData.size() * sizeof(short) + sizeof(WAVE_HEADER) - 8;

	header.format[0] = 'W';
	header.format[1] = 'A';
	header.format[2] = 'V';
	header.format[3] = 'E';

	header.Sub_chunk1ID[0] = 'f';
	header.Sub_chunk1ID[1] = 'm';
	header.Sub_chunk1ID[2] = 't';
	header.Sub_chunk1ID[3] = ' ';

	header.Sub_chunk1Size = 16;

	header.AudioFormat = 1;

	header.NumChannels = 1;

	header.SampleRate = 48000;

	header.ByteRate = 96000;

	header.BlockAlign = 4;

	header.BitsPerSample = 16;

	header.Sub_chunk2ID[0] = 'd';
	header.Sub_chunk2ID[1] = 'a';
	header.Sub_chunk2ID[2] = 't';
	header.Sub_chunk2ID[3] = 'a';

	header.Sub_chunk2Size = 0;

	std::ofstream outFile(filename, std::ios::binary);
	outFile.write(reinterpret_cast<char*>(&header), sizeof(header));
	outFile.write(reinterpret_cast<const char*>(pcmData.data()), pcmData.size() * sizeof(short));

	header.ChunkSize = pcmData.size() * sizeof(short) + sizeof(WAVE_HEADER) - 8;
	header.Sub_chunk2Size = pcmData.size() * sizeof(short);

	outFile.seekp(0, std::ios::beg);
	outFile.write(reinterpret_cast<char*>(&header), sizeof(header));
}

std::vector<short> Util::TwoChannelToMono(std::vector<short> pcmData)
{
	std::pair < std::vector<short>, std::vector<short>> leftright = split(pcmData);
	auto cons = Consolidate(leftright.first, leftright.second);
	return cons;
}

std::pair<std::vector<short>, std::vector<short>> Util::split(std::vector<short> pcmData)
{
	std::vector<short int> left;
	std::vector<short int> right;
	int size = pcmData.size();
	for (int i = 0; i < size; i++)
	{
		if (i % 2 == 0)
		{
			left.push_back(pcmData[i]);
		}
		else
		{
			right.push_back(pcmData[i]);
		}
	}
	return make_pair(left, right);
}

std::vector<short> Util::Consolidate(std::vector<short> left, std::vector<short> right)
{
	std::vector<short int> consolidated;
	int size = (left.size() + right.size()) / 2;

	for (int i = 0; i < size; i++)
	{
		consolidated.push_back((left[i] + right[i]) / 2);
	}
	return consolidated;
}

