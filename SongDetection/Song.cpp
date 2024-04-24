#include "Song.h"

std::unordered_map<int, std::vector<double>> Song::getHash()
{
	return hash;
}
int Song::getSampleRate()
{
	return sampleRate;
}
std::string Song::getName()
{
	return name;
}
