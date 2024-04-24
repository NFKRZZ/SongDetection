#include "Microphone.h"
#include <windows.h>
#include <mmsystem.h>
#include <iostream>
#include <iterator>
#include "Util.h"
#include "GLOBAL.h"



std::vector<short> Microphone::init()
{
	std::cout << "STARTING AUDIO RECORDING!!!\n";

	const int NUMPTS = 48000 * 10;

	int sampleRate = GLOBAL::sampleRate;

	short int waveIn[NUMPTS];

	


	HWAVEIN hWaveIn;
	WAVEHDR waveInHdr;
	MMRESULT result;

	//recording parameters
	WAVEFORMATEX pFormat;
	pFormat.wFormatTag = WAVE_FORMAT_PCM;
	pFormat.nChannels = 1;
	pFormat.nSamplesPerSec = sampleRate;
	pFormat.nAvgBytesPerSec = sampleRate * 2;
	pFormat.nBlockAlign = 2;
	pFormat.wBitsPerSample = 16;
	pFormat.cbSize = 0;

	result = waveInOpen(&hWaveIn, WAVE_MAPPER, &pFormat, 0L, 0L, WAVE_FORMAT_DIRECT);

	if (result)
	{
		std::cout << "FAILED TO OPEN WAVEFORM INPUT DEVICE\n";
		return std::vector<short>();
	}

	waveInHdr.lpData = (LPSTR)waveIn;
	waveInHdr.dwBufferLength = NUMPTS * 2;
	waveInHdr.dwBytesRecorded = 0;
	waveInHdr.dwUser = 0L;
	waveInHdr.dwFlags = 0L;
	waveInHdr.dwLoops = 0L;

	waveInPrepareHeader(hWaveIn, &waveInHdr, sizeof(WAVEHDR));

	result = waveInAddBuffer(hWaveIn, &waveInHdr, sizeof(WAVEHDR));


	if (result)
	{
		std::cout << "FAILED TO READ BLOCK FROM MICROPHONE\n";
		return std::vector<short>();
	}

	result = waveInStart(hWaveIn);
	if (result)
	{
		std::cout << "FAILED TO START RECORDING\n";
	}
	do {} while (waveInUnprepareHeader(hWaveIn, &waveInHdr, sizeof(WAVEHDR)) == WAVERR_STILLPLAYING);

	waveInClose(hWaveIn);
	std::cout << "Finished Recording\n";
	std::vector<short> pcmData(std::begin(waveIn), std::end(waveIn));
	std::string fName = "mic.wav";

	Util::createWavFileMono(pcmData, fName);

	return pcmData;

}

void Microphone::setSettings()
{

}
