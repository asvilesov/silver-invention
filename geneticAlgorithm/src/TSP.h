#pragma once
#include <string>
#include <vector>
#include <random>

struct Location
{
	std::string mName;
	double mLatitude = 0;
	double mLongitude = 0;
};

struct Population
{
	std::vector<std::vector<int>> mMembers;
};

std::vector<Location> readLocations(std::string inputFile);

std::vector<int> createSequence(int size, std::mt19937& g1);

Population createPopulation(int popSize, int size, std::mt19937& g1);

std::vector<std::pair<int, double>> fitnessCalculation(const Population& v, const std::vector<Location>& locations);

std::vector<std::pair<int, double>> sortFitness(std::vector<std::pair<int, double>>& fitness);

std::vector<double> probParent(std::vector<std::pair<int, double>> sortedFitness);

std::vector<std::pair<int, int>> genCrossOverPairs(const std::vector<double>& probVec, int popSize, std::mt19937& g1);

Population genCrossover(std::vector<std::pair<int, int>> crossPairs, Population original, double mutationChance, std::mt19937& g1);

void genOutputFile(const Population& v, std::vector<Location>& x, std::vector<std::pair<int, double>>& fitness, const std::vector<std::pair<int, int>>& crossOverPairs, int generation, bool last = false);