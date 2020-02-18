#include "SrcMain.h"
#include <string>
#include "RleFile.h"
#include <iostream>
#include <cstring>

void ProcessCommandArgs(int argc, const char* argv[])
{
	if (argc == 2) {
		RleFile r;
		int length = std::strlen(argv[1]);
		if (argv[1][length - 4] == '.' && argv[1][length - 3] == 'r' && argv[1][length - 2] == 'l' && argv[1][length - 1] == '1') {
			r.ExtractArchive(argv[1]);
		}
		else {
			r.CreateArchive(argv[1]);
		}

	}
}
