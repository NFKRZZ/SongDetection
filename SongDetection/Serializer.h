#pragma once
#include "Song.h"
class Serializer
{
public:
	static void serialize(Song s);
	static Song deserialize(std::string filePath);
};