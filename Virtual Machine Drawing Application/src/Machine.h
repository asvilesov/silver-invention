#pragma once
#include "Ops.h"
#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <map>
#include <functional>
#include <../img/bitmap_image.hpp>
#include <Exceptions.h>

class Machine
{
public:
	Machine();
	//instruction input
	void input(std::string fileName);
	//register/flag helper functions
	int getReg(std::string reg);
	void setReg(std::string reg, int val);
	bool getFlag(std::string flag);
	void setFlag(std::string flag, bool val);
	void setJumpFlag(bool val);
	//stack helper functions
	void pushStack(int val);
	int popStack();
	int& indexStack(size_t index);
	int sizeStack();
	//drawing helper functions
	void fwdDraw(int units);
	void backDraw(int units);

	void execute();
private:
	std::vector<std::shared_ptr<Op>> mOps;
	std::map<std::string, std::function<std::shared_ptr<Op>()>> opMap;
	
	// map for holding registers
	std::map<std::string, int> registers;
	// map for holding register flags
	std::map<std::string, bool> flags;
	// vec for reg index
	std::vector<std::string> regIndex;
	// vec for flag index
	std::vector<std::string> flagIndex;
	// flag for if a jump has occured
	bool jumpFlag;
	// Stack
	std::vector<int> stack;

	//bitmap
	bitmap_image img;
	image_drawer drawer;

	//color map
	std::vector<std::vector<int>> colorMap;
};

template <typename T>
std::shared_ptr<Op> CreateOp() {
	return std::make_shared<T>();
}



