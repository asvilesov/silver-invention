#include "RleFile.h"
#include "RleData.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <cstring>

void RleFile::CreateArchive(const std::string& source)
{
	std::ifstream::pos_type size;
	char* memblock;
	// Open the file for input, in binary mode, and start ATE (at the end)
	std::ifstream file(source, std::ios::in | std::ios::binary | std::ios::ate);
	if (file.is_open())
	{
		size = file.tellg(); // Save the size of the file
		memblock = new char[static_cast<unsigned int>(size)];
		file.seekg(0, std::ios::beg); // Seek back to start of file
		file.read(memblock, size);
		file.close();

		mRleData.Compress(memblock, static_cast<unsigned int>(size));
		mHeader.mFileSize = static_cast<unsigned int>(size);
		delete[] memblock;
	}

	std::string fileSuffix = ".rl1";
	std::string filename = source + fileSuffix;
	// Open the file for output, in binary mode, and overwrite an existing file
	std::ofstream out(filename, std::ios::out | std::ios::binary | std::ios::trunc);
	if (out.is_open())
	{
		//writing file header
		out.write(mHeader.mSig, 4);
		out.write(reinterpret_cast<char*>(&mHeader.mFileSize), 4);
		const char* filenameC = source.c_str();
		char fileNameLength = static_cast<char>(std::strlen(filenameC));
		out.write(&fileNameLength, 1);
		out.write(filenameC,std::strlen(filenameC));
		out.write(mRleData.mData, mRleData.mSize);
		out.close();
	}
}

void RleFile::ExtractArchive(const std::string& source)
{
	std::ifstream::pos_type size;
	char* memblock;
	// Open the file for input, in binary mode, and start ATE (at the end)
	std::ifstream file(source, std::ios::in | std::ios::binary | std::ios::ate);
	if (file.is_open())
	{
		size = file.tellg(); // Save the size of the file
		memblock = new char[static_cast<unsigned int>(size)];
		file.seekg(0, std::ios::beg); // Seek back to start of file
		file.read(memblock, size);
		file.close();
	
		//read and decompress given file
		mHeader.mFileSize = *(reinterpret_cast<int*>(&memblock[4]));
		int fileNameLength = static_cast<int>(memblock[8]);
		std::string fileName(&memblock[9], fileNameLength);
		mRleData.Decompress(&memblock[9 + fileNameLength], static_cast<size_t>(size) - 9 - static_cast<size_t>(fileNameLength) , static_cast<size_t>(mHeader.mFileSize));

		//rewrite original file with decompressed data
		std::ofstream out(fileName, std::ios::out | std::ios::binary | std::ios::trunc);
		if (out.is_open())
		{
			//writing data
			out.write(mRleData.mData, mRleData.mSize);
			out.close();
		}

		delete[] memblock;
	}
}
