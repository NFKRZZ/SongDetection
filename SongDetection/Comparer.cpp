#include "Comparer.h"
#include <iostream>

double Comparer::cosineSimilarity(fftw_complex* fftwData, size_t size, Song& song)
{
    std::cout << "CALLED COSINESIMILARITY\n";
    double dotProduct = 0.0;
    double normFftwData = 0.0;
    double normSongSet = 0.0;
    double timeStep = 1.0 / song.getSampleRate(); // Time difference between each frequency bin

    // Calculate dot product and norms
    for (int i = 0; i < size; ++i) 
    {
        double frequency = static_cast<double>(fftwData[i][0]); 
        double time = static_cast<double>(i) * timeStep; 

        dotProduct += frequency * (song.getHash().find(frequency) != song.getHash().end()) * time;
        std::cout << "DID DOT PRODUCT" << std::endl;
        normFftwData += fftwData[i][0] * fftwData[i][0] + fftwData[i][1] * fftwData[i][1];
        normSongSet += (song.getHash().find(frequency) != song.getHash().end()) * time;
    }

    // Avoid division by zero
    if (normFftwData == 0.0 || normSongSet == 0.0) 
    {
        return 0.0;
    }

    // Calculate cosine similarity
    return dotProduct / (sqrt(normFftwData) * sqrt(normSongSet));
}

Song Comparer::findClosestMatch(fftw_complex* queryData, size_t size, std::vector<Song>& songs)
{
	double maxSimilarity = 0.0;
	Song bestMatch;

	for (Song song : songs)
	{
		double similarity = cosineSimilarity(queryData, size, song);
		if (similarity > maxSimilarity)
		{
			maxSimilarity = similarity;
			bestMatch = song;
		}
	}

    std::cout << "Found Best Match with similarity of " << maxSimilarity << "%" << std::endl;
	return bestMatch;
}
