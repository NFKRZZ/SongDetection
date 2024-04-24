#include "WAVEFILE.h"


std::vector<short> WAVEFILE::getPcmData()
{
	if (isShort)
	{
		return shortDat;
	}
	else
	{

	}
}
Util::WAVE_HEADER WAVEFILE::getHeader()
{
	return wavDat;
}
int WAVEFILE::getSize()
{
	if (isShort)
	{
		return shortDat.size();
	}
	else
	{
		return doubleDat.size();
	}
}
std::string WAVEFILE::getName()
{
	return name;
}