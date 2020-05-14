#include "SrcMain.h"
#include <iostream>
#include "Machine.h"

void ProcessCommandArgs(int argc, const char* argv[])
{
	Machine myPC;
	myPC.input(argv[1]);
	myPC.execute();
}
