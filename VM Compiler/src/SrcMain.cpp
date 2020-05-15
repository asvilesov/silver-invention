#include "SrcMain.h"
#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include "Node.h"
#include <fstream>
#include <string>

extern int proccparse(); // NOLINT
struct yy_buffer_state; // NOLINT
extern void procc_flush_buffer(yy_buffer_state* b); // NOLINT
extern int procclex_destroy(); // NOLINT
extern FILE* proccin; // NOLINT

extern int gLineNumber;
extern NProgram* gProgram;
bool gSyntaxError = false;

int ProcessCommandArgs(int argc, const char* argv[])
{
	gLineNumber = 1;
	gSyntaxError = false;
	if (argc < 2)
	{
		std::cout << "You must pass the input file as a command line parameter." << std::endl;
		return 1;
	}

	// Read input from the first command line parameter
	proccin = fopen(argv[1], "r");
	if (proccin == nullptr)
	{
		std::cout << "File not found: " << argv[1] << std::endl;
		return 1;
	}
	procc_flush_buffer(nullptr);

	// Start the parse
	proccparse();

	if (gProgram != nullptr && argc == 3)
	{
		//Create Abstract Synatx Tree
		std::ofstream ast;
		ast.open("ast.txt", std::ofstream::out);
		gProgram->OutputAST(ast, 0);
		std::string emitCheck(argv[2]);
	
		//Create Emits
		if (emitCheck == "emit" || emitCheck == "reg") {
			std::ofstream emit;
			emit.open("emit.txt", std::ofstream::out);
			CodeContext c;
			gProgram->CodeGen(c);
			std::cout << "Code Generation: \n";
			for (auto i : c.ops) {
				emit << i << "\n";
				std::cout << i << "\n";
			}
			emit.close();

			//Create register allocation
			if (emitCheck == "reg") {
				c.generateIntervals();
				c.registerAllocation();

				std::ofstream reg;
				reg.open("reg.txt", std::ofstream::out);
				reg << "INTERVALS:\n";
				for (auto j = 0; j < c.registerIntervals.size(); ++j) {
					std::cout << c.registerIntervals[j].first << " " << c.registerIntervals[j].second << " \n";
					reg << "%" << j << ":" << c.registerIntervals[j].first << "," << c.registerIntervals[j].second << "\n";
				}
				reg << "ALLOCATION:\n";
				for (auto j = 0; j < c.registerAlloc.size(); ++j) {
					std::cout << j << " " << c.registerAlloc[j]+1 << " \n";
					reg << "%" << j << ":r" << c.registerAlloc[j] + 1 << "\n";
				}
				emit.open("emit.txt", std::ofstream::out);
				c.virtRegsToReal();
				std::cout << "@@@@\n";
				for (auto i : c.ops) {
					emit << i << "\n";
					std::cout << i << "\n";
				}

			}
		}
	}
	else
	{
		// (Just a useful separator for debug cout statements in grammar)
		std::cout << "**********************************************\n";
	}

	// Close the file stream
	fclose(proccin);
	// Destroy lexer so it reinitializes
	procclex_destroy();
	// Return 1 if syntax error, 0 otherwise
	return static_cast<int>(gSyntaxError);
}

void proccerror(const char* s) // NOLINT
{
	std::cout << s << " on line " << gLineNumber << std::endl;
	gSyntaxError = true;
}
