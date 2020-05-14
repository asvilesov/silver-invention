#include "SrcMain.h"

void ProcessCommandArgs(int argc, const char* argv[])
{
	if (argc == 2) {
		std::string fileName(argv[1]);
		FastaParser parsedDNA(fileName);
		Translator histogram(parsedDNA.header, parsedDNA.data);
		histogram.write();
	}
	else if (argc == 3) {
		std::string fileOne(argv[1]);
		std::string fileTwo(argv[2]);
		SequenceAligner comp(fileOne, fileTwo);
		comp.align();
		comp.output();
	}
}
