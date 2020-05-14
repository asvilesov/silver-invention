#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

//translator thats sequences DNA
struct Translator{
	/*(A) Alanine
	(C) Cysteine
	(D) Aspartic acid
	(E) Glutamic acid
	(F) Phenylalanine
	(G) Glycine
	(H) Histidine
	(I) Isoleucine
	(K) Lysine
	(L) Leucine 
	(M) Methionine
	(N) Asparagine
	(P) Proline
	(Q) Glutamine
	(R) Arginine
	(S) Serine
	(T) Threonine
	(V) Valine
	(W) Tryptophan
	(Y) Tyrosine
	*/
	std::string mHeader;
	std::string mCodonList;
	std::vector<std::string> mCodonNames;
	std::unordered_map<char, unsigned char> mNucleotides;
	std::unordered_map<unsigned char, int> mCodonHistogram;
	int mAminoAcidCount;

	//state machine for determining amino acid of a codon
	const unsigned char mCodonTable[24][4] = {
		//State = | T | C | A | G |
		{0, 0, 1, 0},
		{2, 0, 1, 0},
		{0, 0, 1, 'M'}, //Starting Sequence ATG
		{4, 9, 14, 19}, //First Letter in Amino Acid
		{5, 6, 7, 8}, //T**
		{'F', 'F', 'L', 'L'}, //TT*
		{'S', 'S', 'S', 'S'}, //TC*
		{'Y', 'Y', 0, 0}, //TA*
		{'C', 'C', 0, 'W'}, //TG*
		{10, 11, 12, 13}, //C**
		{'L', 'L', 'L', 'L'}, //CT*
		{'P', 'P', 'P', 'P'}, //CC*
		{'H', 'H', 'Q', 'Q'}, //CA*
		{'R', 'R', 'R', 'R'}, //CG*
		{15, 16, 17, 18}, //A**
		{'I', 'I', 'I', 'M'}, //AT* //Can you form Methionine in the middle of a sequence?
		{'T', 'T', 'T', 'T'}, //AC*
		{'N', 'N', 'K', 'K'}, //AA*
		{'S', 'S', 'R', 'R'}, //AG*
		{20, 21, 22, 23}, //G**
		{'V', 'V', 'V', 'V'}, //GT*
		{'A', 'A', 'A', 'A'}, //GC*
		{'D', 'D', 'E', 'E'}, //GA*
		{'G', 'G', 'G', 'G'}, //GG*
	};

	Translator(const std::string& header,const std::string& sequence)
		: mCodonList("ACDEFGHIKLMNPQRSTVWY")
		, mCodonNames({ "(A) Alanine","(C) Cysteine", "(D) Aspartic acid", "(E) Glutamic acid",
		"(F) Phenylalanine", "(G) Glycine", "(H) Histidine", "(I) Isoleucine", "(K) Lysine",
		"(L) Leucine", "(M) Methionine", "(N) Asparagine", "(P) Proline", "(Q) Glutamine",
		"(R) Arginine", "(S) Serine", "(T) Threonine", "(V) Valine", "(W) Tryptophan", "(Y) Tyrosine"})
		, mHeader(header)
		, mAminoAcidCount(0)
	{
		//setup codon histogram hashtable
		for (char const& codon : mCodonList) {
			mCodonHistogram[static_cast<unsigned char>(codon)] = 0;
		}
		//setup nucleotide table to convert letter to int
		mNucleotides['T'] = 0;
		mNucleotides['C'] = 1;
		mNucleotides['A'] = 2;
		mNucleotides['G'] = 3;
		//sequence the DNA strand
		unsigned char state = 0;
		for (auto& nucleotide : sequence) {
			//determine state
			if (state < 24) {
				state = mCodonTable[state][mNucleotides[nucleotide]];
			}
			//act on state
			if (state == 'M') {
				mCodonHistogram[state] += 1;
				state = 3;
			}
			else if (state > 23) {
				mCodonHistogram[state] += 1;
				state = 3;
			}
		}

		for (auto& codon : mCodonList) {
			mAminoAcidCount += mCodonHistogram[codon];
		}
	}

	void write(std::string file_name = "amino.txt") {
		std::ofstream file(file_name);
		if (file.is_open()) {
			file << mHeader << "\n";
			file << "Total amino acids produced: " << mAminoAcidCount << "\n";
			for (size_t i = 0; i < mCodonNames.size(); ++i) {
				file << mCodonNames[i] << ": " << mCodonHistogram[mCodonList[i]] << "\n";
			}


		}

	}
};
