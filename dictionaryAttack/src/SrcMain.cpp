#include "SrcMain.h"


void ProcessCommandArgs(int argc, const char* argv[])
{
	if (std::string(argv[1]) == "dictionary") {
		auto hashTableSize = std::stoi(argv[2]);
		std::string inputFile(argv[3]);
		std::string passwordFile(argv[4]);
		auto knownPasswords(processPossiblePasswords(inputFile, hashTableSize));
		dictionaryAttack(passwordFile, knownPasswords);
	}
	else if (std::string(argv[1]) == "phrases") {
		auto hashTableSize = std::stoi(argv[2]);
		std::string wordComboFile(argv[3]);
		std::string passwordFile(argv[4]);
		std::vector<std::string> wordPool;

		auto start = std::chrono::high_resolution_clock::now();
		auto unknownPasswords(processUnknownHashes(passwordFile, hashTableSize, wordPool));
		auto end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
		float secondsFirst = duration / 1000000000.0f;

		start = std::chrono::high_resolution_clock::now();
		bruteForceDictAttack(wordComboFile, unknownPasswords);
		end = std::chrono::high_resolution_clock::now();
		duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
		float secondsFourth = duration / 1000000000.0f;

		start = std::chrono::high_resolution_clock::now();
		outputFoundPasswords(unknownPasswords, wordPool);
		end = std::chrono::high_resolution_clock::now();
		duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
		float secondsFifth = duration / 1000000000.0f;

		//uncomment to see timing of functions
		//std::cout << "Time for various parts: " <<secondsFirst << " " << secondsFourth << " " << secondsFifth << "\n";
	}
	else {
		std::cout << "Incorrect parameters. Try using 'dictionary' or 'phrases'\n";
	}
}

HashTable<std::string> processPossiblePasswords(const std::string& inputFile, const size_t& tableCapacity) {
	std::ifstream file(inputFile);
	HashTable<std::string> passStorage(tableCapacity);
	std::string word;
	std::string hexStr;
	hexStr.resize(40);
	unsigned char hash[20];
	if (file.is_open()) {
		while (!file.eof()) {
			std::getline(file, word);
			sha1::Calc(word.c_str(), word.length(), hash);
			sha1::ToHexString(hash, &hexStr[0]);
			passStorage.Insert(hexStr, word);
		}
	}
	return passStorage;
}

void dictionaryAttack(const std::string& passwordHashDatabase, const HashTable<std::string>& knownPasswords) {
	std::ifstream file(passwordHashDatabase);
	std::string word;
	std::string* possiblePassword;
	std::ofstream fileOutput("solved.txt");
	if (file.is_open() && fileOutput.is_open()) {
		while (!file.eof()) {
			std::getline(file, word);
			possiblePassword = knownPasswords.Find(word);
			if (possiblePassword != nullptr) {
				fileOutput << word << "," << *possiblePassword << "\n";
			}
		}
	}
}

HashTable<std::string> processUnknownHashes(const std::string& inputFile, const size_t& tableCapacity, std::vector<std::string>& wordPool) {
	std::ifstream file(inputFile);
	HashTable<std::string> passStorage(tableCapacity);
	std::string word;
	if (file.is_open()) {
		while (!file.eof()) {
			std::getline(file, word);
			passStorage.Insert(word);
			wordPool.emplace_back(word);
		}
	}
	return passStorage;
}

std::vector<std::string> createPermutations(const std::string& inputFile) {
	std::vector<std::string> wordPool;
	std::ifstream file(inputFile);
	std::string word;
	if (file.is_open()) {
		while (!file.eof()) {
			std::getline(file, word);
			wordPool.emplace_back(word);
		}
	}
	std::vector<std::string> permutations;
	for (auto i = 0; i < wordPool.size(); ++i) {
		for (auto j = 0; j < wordPool.size(); ++j) {
			for (auto k = 0; k < wordPool.size(); ++k) {
				for (auto l = 0; l < wordPool.size(); ++l) {
					word.clear();
					word += wordPool[i];
					word += wordPool[j];
					word += wordPool[k];
					word += wordPool[l];
					permutations.emplace_back(word);
				}
			}
		}
	}
	return permutations;
}

void bruteForceDictAttack(const std::vector<std::string>& phrases, HashTable<std::string>& passwords) {
	tbb::parallel_for(tbb::blocked_range<size_t>(0, phrases.size()), [&passwords, &phrases](const tbb::blocked_range<size_t>& r) {
		std::string word;
		std::string hexStr;
		std::string* value;
		hexStr.resize(40);
		unsigned char hash[20];
		for (auto i = r.begin(); i != r.end(); ++i) {
			sha1::Calc(phrases[i].c_str(), phrases[i].length(), hash);
			sha1::ToHexString(hash, &hexStr[0]);
			value = passwords.Find(hexStr);
			if (value != nullptr) {
				*value = phrases[i];
			}
		}
		});
	//tbb::parallel_for_each(phrases.begin(), phrases.end(), [&passwords](const std::string& combo) { //tbb::parallel_for_each
	//	std::string word;
	//	std::string hexStr;
	//	std::string* value;
	//	hexStr.resize(40);
	//	unsigned char hash[20];
	//	sha1::Calc(combo.c_str(), combo.length(), hash);
	//	sha1::ToHexString(hash, &hexStr[0]);
	//	value = passwords.Find(hexStr);
	//	if (value != nullptr) {
	//		*value = combo;
	//	}
	//});
}

void outputFoundPasswords(const HashTable<std::string>& passwords, const std::vector<std::string>& wordPool) {
	std::string* value;
	std::ofstream fileOutput("solved.txt");
	std::for_each(wordPool.begin(), wordPool.end(), [&fileOutput, &passwords, &value](const std::string& key) {
		value = passwords.Find(key);
		if (value != nullptr) {
			fileOutput << key << "," << *value << "\n";
		}
	});
}

void bruteForceDictAttack(const std::string& inputFile, HashTable<std::string>& passwords) {
	//generate permutations
	std::vector<std::string> wordPool;
	std::ifstream file(inputFile);
	std::string word;
	if (file.is_open()) {
		while (!file.eof()) {
			std::getline(file, word);
			wordPool.emplace_back(word);
		}
	}
	tbb::parallel_for(tbb::blocked_range<size_t>(0, wordPool.size()), [&passwords, &wordPool](const tbb::blocked_range<size_t>& r) {
		std::string word;
		std::string temp;
		std::string hexStr;
		std::string* value;
		hexStr.resize(40);
		unsigned char hash[20];
		for (auto i = r.begin(); i != r.end(); ++i) {
			for (auto j = 0; j < wordPool.size(); ++j) {
				for (auto k = 0; k < wordPool.size(); ++k) {
					for (auto l = 0; l < wordPool.size(); ++l) {
						word += wordPool[i];
						word += wordPool[j];
						word += wordPool[k];
						word += wordPool[l];
						sha1::Calc(word.c_str(), word.length(), hash);
						sha1::ToHexString(hash, &hexStr[0]);
						value = passwords.Find(hexStr);
						if (value != nullptr) {
							*value = word;
						}
						word.clear();
					}
				}
			}
		}
		});
}












