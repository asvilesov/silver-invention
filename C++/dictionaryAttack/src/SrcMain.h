#pragma once

#include "HashTable.hpp"
#include <tbb/parallel_for_each.h>
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sha1.h>
#include <vector>
#include <algorithm>
#include <chrono>

//running a dictionary or brute force password cracking attack
void ProcessCommandArgs(int argc, const char* argv[]);

//input known passwords into a hashtable using SHA-1 hashing function
HashTable<std::string> processPossiblePasswords(const std::string& inputFile, const size_t& tableCapacity);

//runs a dictionary attack given a hashtable with known passwords and a file with password hashes that need to be cracked
void dictionaryAttack(const std::string& passwordHashDatabase, const HashTable<std::string>& knownPasswords);

HashTable<std::string> processUnknownHashes(const std::string& inputFile, const size_t& tableCapacity, std::vector<std::string>& wordPool);

std::vector<std::string> createPermutations(const std::string& inputFile);

void bruteForceDictAttack(const std::vector<std::string>& phrases, HashTable<std::string>& passwords);

void outputFoundPasswords(const HashTable<std::string>& passwords, const std::vector<std::string>& wordPool);

//more efficient implementation of dictionary attack that creates permutations of passwords in parallel in addition to looking through hashtable
void bruteForceDictAttack(const std::string& inputFile, HashTable<std::string>& passwords);
