#include "AudioMatcher.h"
#include <iostream>

Song AudioMatcher::findBestMatch(std::unordered_map<int,std::vector<double>> recordedClip, size_t clipSize, std::vector<Song>& database, size_t windowSize)
{
	//double maxSimilarity = 0.0;
	//Song bestMatch;

	//for (Song song : database)
	//{
	//	std::cout << "This is size " << song.getHash().size() - windowSize << "\n";
	//	for (size_t i = 0; i <= song.getHash().size() - windowSize; i++)
	//	{
	//		std::unordered_map<int, double> window;
	//		for (size_t j = i; j < i + windowSize; j++)
	//		{
	//			//std::cout << "This is size of song hash " << song.getHash().size() << " This is val of j " << j << std::endl;
	//			int timeIncrement = static_cast<int>(j);
	//			window[timeIncrement] = song.getHash().at(timeIncrement);
	//		}
	//		//CONVERT RECORDED CLIP TO HASH
	//		//std::cout << "gonna do cosineSimilarity\n";
	//		double similarity = cosineSimilarity(window, recordedClip);
	//		//std::cout << "Did it\n";
	//		if (similarity > maxSimilarity)
	//		{
	//			maxSimilarity = similarity;
	//			bestMatch = song;
	//		}
	//	}
	//	std::cout << "Did it, max Sim at "<<maxSimilarity<<" with current guess at Song: "<<song.getName()<<"\n";
	//}
	//std::cout << "Found Best Match with similarity of " << maxSimilarity << "%" << std::endl;
	//return bestMatch;

	double maxSimilarity = 0.0;
	Song bestMatch;
	for (Song song : database)
	{
		std::cout << "This is size " << song.getHash().size() << "\n";
		for (size_t i = 0; i <= song.getHash().size(); i++)
		{
			std::unordered_map<int, std::vector<double>> window;
			for (size_t j = i; j < i; j++)
			{
				int timeIncrement = j;
				window[timeIncrement] = song.getHash().at(timeIncrement);
			}

			double similarity = cosineSimilarity(window, recordedClip);
			if (similarity > maxSimilarity)
			{
				maxSimilarity = similarity;
				bestMatch = song;
			}
		}


	}



	return bestMatch;
}

double AudioMatcher::cosineSimilarity(std::unordered_map<int, std::vector<double>> map1, std::unordered_map<int, std::vector<double>> map2)
{
	//double dotProduct = 0.0;
	//double normMap1 = 0.0;
	//double normMap2 = 0.0;

	//// Find common time increments in both maps
	//for (const auto& entry : map1) {
	//	int timeIncrement = entry.first;

	//	// Check if the corresponding entry exists in map2
	//	auto iter = map2.find(timeIncrement);
	//	if (iter != map2.end()) {
	//		dotProduct += entry.second * iter->second;
	//		normMap1 += entry.second * entry.second;
	//		normMap2 += iter->second * iter->second;
	//	}
	//}

	//// Avoid division by zero
	//if (normMap1 == 0.0 || normMap2 == 0.0) {
	//	return 0.0;
	//}

	//// Calculate cosine similarity
	//return dotProduct / (sqrt(normMap1) * sqrt(normMap2));
	return double();
}
