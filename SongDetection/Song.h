#pragma once
#include <unordered_map>
#include "Util.h"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/vector.hpp>


class Song
{
private:
	friend class boost::serialization::access;

	template<class Archive>
	void serialize(Archive& ar, const unsigned int version)
	{
		ar& hash;
		ar& sampleRate;
		ar& channels;
		ar& name;
	}

	std::unordered_map<int, std::vector<double>> hash;
	int sampleRate;
	int channels;
	std::string name;

public:
	Song(std::unordered_map<int,std::vector<double>> table, Util::WAVE_HEADER hdr,std::string name)
	{
		hash = table;
		sampleRate = hdr.SampleRate;
		channels = hdr.NumChannels;
		this->name = name;
	}
	Song() = default;
	std::unordered_map<int, std::vector<double>> getHash();
	int getSampleRate();
	std::string getName();


};