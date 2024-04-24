#pragma once

#include "Song.h"
#include <fftw3.h>

class AudioMatcher
{
public:
	static Song findBestMatch(std::unordered_map<int, std::vector<double>> recordedClip, size_t clipSize, std::vector<Song>& database, size_t windowSize);
	static double cosineSimilarity(std::unordered_map<int, std::vector<double>>map1, std::unordered_map<int, std::vector<double>> map2);
};