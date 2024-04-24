#pragma once
#include "Util.h"
class WAVEFILE
{
	bool isShort;
	std::vector<short> shortDat;
	std::vector<double> doubleDat;
	Util::WAVE_HEADER wavDat;
	std::string name;
	
public:
	WAVEFILE(Util::WAVE_HEADER hdr, std::vector<short> pcmData,std::string name)
	{	

		wavDat = hdr;
		shortDat = pcmData;
		isShort = true;
		this->name = name;

	}
	WAVEFILE(Util::WAVE_HEADER hdr, std::vector<double> pcmData)
	{

		wavDat = hdr;
		doubleDat = pcmData;
		isShort = false;

	}
	std::vector<short> getPcmData();
	Util::WAVE_HEADER getHeader();
	int getSize();
	std::string getName();
};