#pragma once
#include <vector>
#include <string>
#include "Util.h"
#include <fftw3.h>
#include "Song.h"

class loader
{
public:
	static std::vector<std::string> getSongs();
	static std::vector<std::string> getHash();
	static std::unordered_map<int, std::vector<double>> getHash(std::vector<short> pcmData, int sampleRate);
	static void saveHash();
	static std::vector<Util::HashTable> loadHash();
	static fftw_complex* getFFT(std::vector<short> pcmData);
	static fftw_complex* getFFT(std::vector<double> pcmData);
	static std::vector<short> getPCMData(std::string path);
	static Util::WAVE_HEADER getHdr(std::string filePath);
	static void compareSongs(std::vector<Song>& loadedHashes,std::vector<std::string>& filenames );
};
