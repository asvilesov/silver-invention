#pragma once
#include <iostream>
#include <string>
#include <fstream>

struct FastaParser {
	//header string
	std::string header;
	std::string data;

	FastaParser(const std::string& file_name) {
		std::ifstream::pos_type size;
		std::ifstream file(file_name, std::ios::ate);
		std::string temp;
		if (file.is_open()) {
			size = file.tellg();
			data.reserve(size);
			file.seekg(0, std::ios::beg);
			std::getline(file, header);
			header = header.substr(1);
			while (!file.eof()) {
				std::getline(file, temp);
				data += temp; 
			}
			file.close();
			/*std::cout << size << "\n";
			std::cout << "header" << header << "\n";
			std::cout << "data" << data << "\n";*/
		}
	}
};