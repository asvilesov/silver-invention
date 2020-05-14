#pragma once

#include <vector>
#include <string>
#include <iostream>
#include "Parser.hpp"
#include <algorithm>

struct SequenceAligner {
	std::string mSeqOne;
	std::string mAlignedOne;
	std::string mSeqTwo;
	std::string mAlignedTwo;
	int length;
	FastaParser one;
	FastaParser two;
	
	std::vector<std::vector<short>> mWunschMag;
	std::vector<std::vector<char>> mWunschDir;
	enum Angle : char {
		aboveLeft,
		left, 
		above
	};

	//input is two FASTA files of differente genes
	SequenceAligner(const std::string x, const std::string y)
		:one(x)
		,two(y)
	{
		mSeqOne = std::move(one.data);
		mSeqTwo = std::move(two.data);

		mWunschMag.resize(mSeqOne.size()+1);
		mWunschDir.resize(mSeqOne.size()+1);
		for (auto i = 0; i < mWunschMag.size(); ++i) {
			mWunschMag[i].resize(mSeqTwo.size()+1);
			mWunschDir[i].resize(mSeqTwo.size()+1);
		}
		for (short i = 0; i < mWunschMag[0].size(); ++i) {
			mWunschMag[0][i] = -i;
			mWunschDir[0][i] = left;
		}
		for (short i = 0; i < mWunschMag.size(); ++i) {
			mWunschMag[i][0] = -i;
			mWunschDir[i][0] = above;
		}
		if (mSeqOne.size() > mSeqTwo.size()) {
			length = mSeqOne.size();
		}
		else {
			length = mSeqTwo.size();
		}

		mAlignedOne.reserve(length);
		mAlignedTwo.reserve(length);
	}

	//run the Needleman-Wunsch algorithm on the two gene sequence
	void align() {
		short max = 0;
		char angle;
		short temp = 0;
		for (auto i = 1; i < mWunschMag.size(); ++i) {
			for (auto j = 1; j < mWunschMag[0].size(); ++j) {
				//upper left
				angle = aboveLeft;
				if (mSeqOne[i - 1] == mSeqTwo[j - 1]) {
					max = 1;
				}
				else {
					max = -1;
				}
				max = mWunschMag[i - 1][j - 1] + max;
				
				//upper
				temp = mWunschMag[i - 1][j] - 1;
				if (temp > max) {
					max = temp;
					angle = above;
				}
				//left
				temp = mWunschMag[i][j - 1] - 1;
				if (temp > max) {
					max = temp;
					angle = left;
				}
				mWunschMag[i][j] = max;
				mWunschDir[i][j] = angle;
			}
		}
		short x = mWunschMag.size() - 1;
		short y = mWunschMag[0].size() - 1;

		while (x != 0 || y != 0) {
			if (mWunschDir[x][y] == aboveLeft) {
				mAlignedOne += mSeqOne[x - 1];
				mAlignedTwo += mSeqTwo[y - 1];
				x -= 1;
				y -= 1;
			}
			else if (mWunschDir[x][y] == left) {
				mAlignedTwo += mSeqTwo[y - 1];
				mAlignedOne += "_";
				y -= 1;
			}
			else {
				mAlignedTwo += "_";
				mAlignedOne += mSeqOne[x - 1];
				x -= 1;
			}
		}
		std::reverse(mAlignedOne.begin(), mAlignedOne.end());
		std::reverse(mAlignedTwo.begin(), mAlignedTwo.end());
	}

		void output(std::string fileName = "match.result") {
			std::ofstream file(fileName);
			std::string connecterStr;
			for (auto i = 0; i < mAlignedOne.size(); ++i) {
				if (mAlignedOne[i] == mAlignedTwo[i]) {
					connecterStr += "|";
				}
				else {
					connecterStr += " ";
				}
			}
			if (file.is_open()) {
				file << "A: " << one.header << "\n";
				file << "B: " << two.header << "\n";
				file << "Score: " << mWunschMag[mWunschMag.size()-1][mWunschMag[0].size()-1] << "\n\n";
				int count = 0;
				while (count < mAlignedOne.size()) {
					file << mAlignedOne.substr(count, 70) << "\n";
					file << connecterStr.substr(count, 70) << "\n";
					file << mAlignedTwo.substr(count, 70) << "\n\n";
					count += 70;
				}

				file.close();
			}
		}


};


