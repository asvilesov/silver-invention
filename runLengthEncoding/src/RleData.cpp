#include "RleData.h"
#include <iostream>

void RleData::Compress(const char* input, size_t inSize)
{
	delete[] mData;
	mSize = 0;
	int tempMSize = 0;
	mData = new char[2 * inSize];
	
	int prun (0);
	int nrun (0);
	bool longPrun = false; //checks if there was a long prun the previous iteration 

	for (int i = 0; i < inSize; ++i) {
		if (prun == 0 && nrun == 0){
			if (input[i] == input[i + 1]) {
				prun++;
				if (i > 0) {
					if (input[i] == input[i - 1] && !longPrun) {
						prun++;
					}
					if (i == inSize - 1) {
						mData[mSize] = prun;
						mData[mSize + 2] = input[i];
					}
				}
			}
			else {
				nrun--;
				tempMSize = mSize;
				tempMSize++;
				mData[tempMSize] = input[i];
			}
			if (i == inSize - 1 && input[i] == input[i - 1]) {
				prun = 2;
				mData[mSize] = prun;
				mData[mSize + 2] = input[i];
			}
			if (i == inSize - 1 && nrun == -1) {
				mData[mSize] = 1;
				mSize++;
			}
		}
		else if (prun > 0){
			if (input[i] == input[i + 1] && i != inSize -1) { //last conditional to avoid null terminator error
				prun++;
			}
			else {
				prun++;
				mData[mSize] = prun;
				mData[mSize + 1] = input[i];
				mSize = mSize + 2;
				prun = 0;
			}
		}
		else if (nrun < 0) {
			if (input[i] != input[i + 1] && i != inSize - 1) { //change this conditional into seperate ones and do different stuff within.
				nrun--;
				tempMSize++;
				mData[tempMSize] = input[i];
			}
			else {
				if (nrun == -1) {
					nrun = 0;
					if (i == inSize - 1) {
						mData[mSize] = -2;
						tempMSize++;
						mData[tempMSize] = input[i];
					}
					else {
						mData[mSize] = 1;
					}
					mSize = mSize + 2;
				}
				if (nrun < -1) {
					if (i == inSize - 1) {
						nrun--;
					}
					mData[mSize] = nrun;
					if (i == inSize - 1) {
						tempMSize++;
						mData[tempMSize] = input[i];
					}
					nrun = 0;
					mSize = tempMSize + 1;
				}
				prun++;
			}
		}
		if (nrun == -int(MaxRunSize())) {
			mData[mSize] = nrun;
			mSize = tempMSize + 1;
			nrun = 0;
		}
		longPrun = true;
		if (prun == MaxRunSize()) {
			mData[mSize] = prun;
			mData[mSize + 1] = input[i];
			mSize = mSize + 2;
			prun = 0;
		}
		else {
			longPrun = false;
		}
	}
	//debugging
	/*std::cout << input << std::endl;
	std::cout << mData << std::endl;
	std::cout << inSize << std::endl;*/
}

void RleData::Decompress(const char* input, size_t inSize, size_t outSize)
{
	delete[] mData;
	mSize = 0;
	mData = new char[outSize];

	for (int i = 0; i < inSize; i++) {
		if (int(input[i]) > 0) {
			for (int j = 0; j < int(input[i]); j++) {
				mData[mSize + j] = input[i + 1];
			}
			mSize += int(input[i]);
			i = i + 1;
		}
		else if (int(input[i]) < 0) {
			for (int j = 0; j < -int(input[i]); j++) {
				mData[mSize + j] = input[i + j + 1];
			}
			mSize += -int(input[i]);
			i += -int(input[i]);
		}
		
	}
	//debugging
	//std::cout << input << std::endl;
	//std::cout << mData << std::endl;
	
}

std::ostream& operator<< (std::ostream& stream, const RleData& rhs)
{
	for (int i = 0; i < rhs.mSize; i++)
	{
		stream << rhs.mData[i];
	}
	return stream;
}
