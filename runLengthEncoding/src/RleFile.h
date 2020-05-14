#pragma once
#include "RleData.h"
#include <string>
#include <list>

class RleFile
{
	struct Header
	{
		char mSig[4] = {'R', 'L', 'E', '\x01'};
		int mFileSize = 0;
		unsigned char mFileNameLength = 0;
		std::string mFileName;
	} mHeader;

	RleData mRleData;

public:
	void CreateArchive(const std::string& source);

	void ExtractArchive(const std::string& source);
};
