// SongDetection.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include <map>
#include "Microphone.h"
#include "loader.h"
#include "GLOBAL.h"
#include "WAVEFILE.h"
#include <unordered_map>
#include "Song.h"
#include "Serializer.h"
#include <algorithm>
#include "AudioMatcher.h"

int main()
{

    GLOBAL::sampleRate = 48000;

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 9);
    std::cout << "Initializing Song Detection!\n";

    std::vector<std::string> hashList = loader::getHash();
    std::vector<Song> preloadedSongs;
    for (std::string s : hashList)                          //WE DESEREALIZE ALL PREVIOUS HASH DATA THEN PROCEDE TO HASH NEW SONGS
    {
        Song son;
        son = Serializer::deserialize(s);
        preloadedSongs.push_back(son);           
    }
    // We Then Should match the songs against the wav files and see which is already loaded
    SetConsoleTextAttribute(hConsole, 12);
    std::cout << "Loaded All Hashes\n";
    std::cout << "This is preloadedSongs size: " << preloadedSongs.size() << std::endl;

    std::vector<std::string> songs = loader::getSongs();
    std::vector<WAVEFILE> songList;



    std::cout << "GOING TO COMPARE NOW!\n";
   // loader::compareSongs(preloadedSongs, songs);
    std::cout << "Done Comparing\n";


    std::vector<Song> songVec;

    for (std::string s : songs)
    {
        std::vector<short> pcmData = loader::getPCMData(s);
        Util::WAVE_HEADER hdr = loader::getHdr(s);
        WAVEFILE song(hdr, pcmData,s);
        Song s();
        songList.push_back(song);
    }
    std::cout << "Loaded All Songs into memory!!!!\n";
    

    for (WAVEFILE song : songList)
    {
        std::vector<short> pcmData = Util::TwoChannelToMono(song.getPcmData());
        Util::WAVE_HEADER hdr = song.getHeader();
        std::unordered_map<int, std::vector<double>> hashTable;
        hashTable = loader::getHash(pcmData, hdr.SampleRate); // i need to alter the getHash function so that it returns frequency pairs
        Song s(hashTable, hdr, song.getName());
        songVec.push_back(s);
    }
    //Filled up Song vector with hashtable data
    std::cout << "Loaded " << songVec.size() << " Songs!\n";


    //need to merge preloadedSongs with newly Loaded Songs


 /*   std::vector<short> micData = Microphone::init();

    fftw_complex* data = loader::getFFT(micData);
    auto map = loader::getHash(micData, 48000);

    Song bestmatch = AudioMatcher::findBestMatch(map, 48000 * 10, preloadedSongs, map.size());

    std::cout << "This is the closest match! " << bestmatch.getName() << std::endl;*/

    //do this when ending
    std::cout << "Going to serialize all songs Now!" << std::endl;
    songList.clear();
    for(Song s : songVec)
    {

        Serializer::serialize(s);

    }



    std::cout << "HELLO!\n";

    
    
}
