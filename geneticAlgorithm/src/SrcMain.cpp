#include "SrcMain.h"
#include <iostream>
#include <random>
#include "TSP.h"
#include <fstream>
#include <algorithm>

void ProcessCommandArgs(int argc, const char* argv[])
{
	std::string fileName = argv[1];
	int popSize = std::stoi(argv[2]);
	int generations = std::stoi(argv[3]);
	int mutationChance = std::stoi(argv[4]);
	int seed = std::stoi(argv[5]);
	std::mt19937 genRandom(seed);

	auto locations = readLocations(fileName);
	auto trips = createPopulation(popSize, locations.size(), genRandom);
	auto fitness = fitnessCalculation(trips, locations);
	auto sortedFit = sortFitness(fitness);
	auto probVec = probParent(sortedFit);
	auto crossOver = genCrossOverPairs(probVec, fitness.size(), genRandom);
	genOutputFile(trips, locations, fitness, crossOver, 0);
	auto newPop = genCrossover(crossOver, trips, mutationChance, genRandom);

	for (auto i = 1; i < generations; i++) {
		fitness = fitnessCalculation(newPop, locations);
		sortedFit = sortFitness(fitness);
		probVec = probParent(sortedFit);
		crossOver = genCrossOverPairs(probVec, fitness.size(), genRandom);
		genOutputFile(newPop, locations, fitness, crossOver, i);
		newPop = genCrossover(crossOver, newPop, mutationChance, genRandom);
	}

	fitness = fitnessCalculation(newPop, locations);
	genOutputFile(newPop, locations, fitness, crossOver, generations, true);
	newPop = genCrossover(crossOver, newPop, mutationChance, genRandom);

	
	


}
