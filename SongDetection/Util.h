#pragma once
#include <vector>
#include <string>
#include <ios>
#include <fstream>
class Util
{
public:
	static void createWavFileMono(std::vector<short> pcmData, std::string& filename);
	static std::vector<short> TwoChannelToMono(std::vector<short> pcmData);
	static std::pair<std::vector<short>, std::vector<short>> split(std::vector<short> pcmData);
	static std::vector<short> Consolidate(std::vector<short> left, std::vector<short> right);
	static void deserializeSong(std::string filePath);
	struct WAVE_HEADER
	{
		char Chunk[4];
		int ChunkSize;
		char format[4];
		char Sub_chunk1ID[4];
		int Sub_chunk1Size;
		short int AudioFormat;
		short int NumChannels;
		int SampleRate;
		int ByteRate;
		short int BlockAlign;
		short int BitsPerSample;
		char Sub_chunk2ID[4];
		int Sub_chunk2Size;
	};
	struct LIST
	{
		char ChunkID[4];
		int ChunkSize;
		char ListTypeID[4];
	};

	struct Ht_item
	{
		int key; //timestamp
		double value; //frequency
	};
	struct HashTable
	{
		Ht_item** items;
		int size;
		int count;
	};


};
