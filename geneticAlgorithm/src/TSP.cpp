#include "TSP.h"
#include <fstream>
#include <algorithm>
#include <cmath>
#include <iterator>
#include <numeric>
#include <iostream>

//read in the locations that need to be visited and that the Genetic Algorithm will be applied on 
std::vector<Location> readLocations(std::string inputFile) {
	std::string loc;
	std::ifstream file(inputFile);
	std::vector<Location> locationList;
	Location currentLoc;
	std::size_t indexS, indexE;
	if (file.is_open()) {
		while (static_cast<bool>(std::getline(file, loc))) {
			//Name of Location
			indexS = 0;
			indexE = loc.find(',', indexS);
			currentLoc.mName = loc.substr(indexS, indexE);
			indexS = indexE + 1;
			//Latitude
			indexE = loc.find(',', indexS);
			currentLoc.mLatitude = std::stod(loc.substr(indexS, indexE));
			indexS = indexE + 1;
			//Longitude
			indexE = loc.find(',', indexS);
			currentLoc.mLongitude = std::stod(loc.substr(indexS, indexE));
			//add to vec of locations
			locationList.push_back(currentLoc);

			//Debug
			//std::cout << currentLoc.mName << " " << currentLoc.mLatitude << " " << currentLoc.mLongitude << "\n";
		}
		file.close();
	}
	else {
		std::cout << "error\n";
	}
	return locationList;
}

//create a random sequence
std::vector<int> createSequence(int size, std::mt19937& g1) {
	int count = 0;
	std::vector<int> seq(size);
	std::generate(seq.begin(), seq.end(), [&count]() {
		return count++;
		});
	std::shuffle(seq.begin() + 1, seq.end(), g1);
	return seq;
}

//create a population of random sequences 
Population createPopulation(int popSize, int size, std::mt19937& g1) {
	Population v;
	v.mMembers.resize(popSize);
	std::generate(v.mMembers.begin(), v.mMembers.end(), [&size, &g1]() {
		return createSequence(size, g1);
	});
	return v;
}

//calculate the fitness of the given population
std::vector<std::pair<int, double>> fitnessCalculation(const Population& v, const std::vector<Location>& locations) {
	std::vector<std::vector<double>> adjDistances(v.mMembers.size());
	std::vector<std::pair<int, double>> distances(v.mMembers.size());

	//calculate the distances between each two points in the trajectory using the Haversine Distance formula
	int count = 0;
	std::generate(adjDistances.begin(), adjDistances.end(), [&v, &count, &locations]() {
		std::vector<double> to;
		std::adjacent_difference(v.mMembers[count].begin(), v.mMembers[count].end(), std::back_inserter(to), [&locations](int a, int b) {
			double lon1 = 0.0174533 * locations[a].mLongitude;
			double lat1 = 0.0174533 * locations[a].mLatitude;
			double lon2 = 0.0174533 * locations[b].mLongitude;
			double lat2 = 0.0174533 * locations[b].mLatitude;
			double dlon = lon2 - lon1;
			double dlat = lat2 - lat1;
			double d = pow(sin(dlat / 2.00), 2) + cos(lat1) * cos(lat2) * pow(sin(dlon / 2.00), 2);
			double c = 2.00 * atan2(sqrt(d), sqrt(1 - d));
			double distance = 3961.00 * c;
			return distance;
			});
		double lon1 = 0.0174533 * locations[0].mLongitude;
		double lat1 = 0.0174533 * locations[0].mLatitude;
		double lon2 = 0.0174533 * locations[v.mMembers[count][v.mMembers[count].size() - 1]].mLongitude;
		double lat2 = 0.0174533 * locations[v.mMembers[count][v.mMembers[count].size() - 1]].mLatitude;
		double dlon = lon2 - lon1;
		double dlat = lat2 - lat1;
		double d = pow(sin(dlat / 2), 2) + cos(lat1) * cos(lat2) * pow(sin(dlon / 2), 2);
		double c = 2 * atan2(sqrt(d), sqrt(1 - d));
		double distance = 3961.00 * c;
		to.push_back(distance);
		count++;
		return to;
		});
	count = 0;
	//sum up the adjacent differences in every path to get the total distance
	std::generate(distances.begin(), distances.end(), [&count, &adjDistances](){
		std::pair<int, double> p;
		p.first = count;
		p.second = std::accumulate(adjDistances[count].begin(), adjDistances[count].end(), 0.00, [](const double& a, const double& b) {
			return a + b;
		});
		count++;
		return p;
	});
	return distances;
}

//sort the distances/fitness from the various trajectories in population in descending order
std::vector<std::pair<int, double>> sortFitness(std::vector<std::pair<int, double>>& fitness) {
	auto copyFitness = fitness;
	std::sort(copyFitness.begin(), copyFitness.end(), [](std::pair<int, double> a, std::pair<int, double> b) {
		bool c = false;
		if (a.second < b.second) {
			c = true;
		}
		return c;
	});
	return copyFitness; 
}

//create a probability vector for how likely an individual(trajectory) in population should be picked for 'crossover'
std::vector<double> probParent(std::vector<std::pair<int, double>> sortedFitness) {
	std::vector<double> probs(sortedFitness.size());
	auto popSize = sortedFitness.size();
	std::generate(probs.begin(), probs.end(), [&popSize](){
		return 1.00 / popSize;
	});
	probs[sortedFitness[0].first] *= 6.00;
	probs[sortedFitness[1].first] *= 6.00;
	for (auto i = 2; i < popSize / 2; i++) {
		probs[sortedFitness[i].first] *= 3.00;
	}
	double sum = std::accumulate(probs.begin(), probs.end(), 0.00, [](const double& a, const double& b) {
		return a + b;
	});
	std::transform(probs.begin(), probs.end(), probs.begin(), [&sum](double&a) {
		return a / sum;
	});
	return probs;
}

//for each individual of the population pick two random parents using a generated probability vector for crossover
std::vector<std::pair<int, int>> genCrossOverPairs(const std::vector<double>& probVec, int popSize, std::mt19937& g1) {
	std::uniform_real_distribution<double> uni (0.00, 1.00);
	std::vector<std::pair<int, int>> parentVec(popSize);
	std::generate(parentVec.begin(), parentVec.end(), [&probVec, &uni, &g1]() {
		auto firstR = uni(g1);
		int first;
		double sum = 0.00;
		for (int i = 0; i < probVec.size(); i++) {
			sum += probVec[i];
			if (sum >= firstR) {
				first = i;
				break;
			}
		}
		auto secondR = uni(g1);
		int second;
		sum = 0.00;
		for (int i = 0; i < probVec.size(); i++) {
			sum += probVec[i];
			if (sum >= secondR) {
				second = i;
				break;
			}
		}
		std::pair<int, int> a(first, second);
		return a;
	});
	return parentVec;
}

//Crossover and generation handling
Population genCrossover(std::vector<std::pair<int, int>> crossPairs, Population original, double mutationChance, std::mt19937& g1) {
	Population newGen;
	std::vector<std::vector<int>> members(original.mMembers.size());
	int count = 0;
	int numLocations = original.mMembers[0].size();
	std::generate(members.begin(), members.end(), [&original, &g1, &crossPairs, &count, &mutationChance, &numLocations]() {
		std::uniform_int_distribution<int> uniA(1, numLocations -2);
		int crossIndex = uniA(g1);
		std::uniform_int_distribution<int> uniB(0, 1);
		int parent = uniB(g1);
		std::vector<int> parent1;
		std::vector<int> parent2;
		if (parent == 1) {
			parent1 = original.mMembers[crossPairs[count].first];
			parent2 = original.mMembers[crossPairs[count].second];
		}
		else {
			parent1 = original.mMembers[crossPairs[count].second];
			parent2 = original.mMembers[crossPairs[count].first];
		}
		//create a cross over vector between the two parents
		std::vector<int> product(numLocations);
		std::copy_n(parent1.begin(), crossIndex+1, product.begin());
		std::copy_if(parent2.begin(), parent2.end(), product.end()-numLocations+crossIndex+1, [&product](int i) {
			bool c = true;
			if (std::find(product.begin(), product.end(), i) != product.end()) {
				c = false;
			}
			return c;
		});
		//create artifical mutations
		std::uniform_real_distribution<double> uni(0.00, 1.00);
		double mutation = uni(g1);
		if (mutation < mutationChance / 100) {
			std::uniform_int_distribution<int> mutateWhere(1, numLocations - 1);
			auto firstIndex = mutateWhere(g1);
			auto secondIndex = mutateWhere(g1);
			std::swap(product[firstIndex], product[secondIndex]);
		}
		count++;
		return product;
	});
	newGen.mMembers = members;
	return newGen;
}

//output the GA results into a log.txt file
void genOutputFile(const Population& v, std::vector<Location>& x, std::vector<std::pair<int, double>>& fitness, const std::vector<std::pair<int, int>>& crossOverPairs, int generation, bool last) {
	if (generation == 0) {
		std::ofstream file("log.txt");
		file << "INITIAL POPULATION:\n";
		file.close();
	}
	else {
		std::ofstream file("log.txt", std::ios_base::app);
		file << "GENERATION: " << generation << "\n";
		file.close();
	}
	std::ofstream file("log.txt", std::ios_base::app);
	if (file.is_open()) {
		for (auto i : v.mMembers) {
			for (auto j : i) {
				file << j;
				if (i[i.size() - 1] != j) {
					file << ',';
				}
			}
			file << "\n";
		}
		file << "FITNESS:\n";
		for (auto i : fitness) {
			file << i.first << ":" << i.second << "\n";
		}
		if (last) {
			file << "SOLUTION:\n";
			auto cities = v.mMembers[sortFitness(fitness)[0].first];
			for (auto i : cities) {
				file << x[i].mName << "\n";
			}
			file << x[cities[0]].mName << "\n";
			file << "DISTANCE: " << sortFitness(fitness)[0].second << " miles";
		}
		else {
			file << "SELECTED PAIRS:\n";
			for (auto i : crossOverPairs) {
				file << "(" << i.first << "," << i.second << ")" << "\n";
			}
		}
		file.close();
	}
	else {
		std::cout << "error\n";
	}
}