#include "Serializer.h"
#include <iostream>

void Serializer::serialize(Song s)
{
	std::string name = s.getName(); 
	if (name.compare(0, 6, "Songs/") == 0 && name.compare(name.length() - 4, 4, ".wav") == 0) 
	{
		name.erase(0, 6);
		name.erase(name.length() - 4);
	}

	std::string path = "Hashes/" + name + ".txt";
	std::ofstream ofs(path); // NEED TO MAKE SURE THE PATH EXISTS ALREADY OR ELSE BOOST THROWS ERROR
	boost::archive::text_oarchive oarchive(ofs);
	oarchive << s;
}
Song Serializer::deserialize(std::string filePath)
{
	std::ifstream ifs(filePath);
	boost::archive::text_iarchive iarchive(ifs);

	Song song;
	iarchive >> song;
	return song;
}