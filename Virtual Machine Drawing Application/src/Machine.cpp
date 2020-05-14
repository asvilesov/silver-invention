#include "Machine.h"

Machine::Machine()
	: regIndex{"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "pc", "sc", "ex", "tx", "ty", "tr", "tc"}
	, flagIndex{"exit", "test", "pen"}
	, jumpFlag(false)
	, img(320,240)
	, drawer(img)
	, colorMap{ 
		{255, 255, 255},
		{255, 255, 0},
		{255, 0, 255},
		{255, 0, 0},
		{0, 255, 255},
		{0, 255, 0},
		{0, 0, 255},
		{0, 0, 0}}
{
	opMap.emplace("movi", &CreateOp<MovI>);
	opMap.emplace("add", &CreateOp<Add>);
	opMap.emplace("mov", &CreateOp<Mov>);
	opMap.emplace("sub", &CreateOp<Sub>);
	opMap.emplace("mul", &CreateOp<Mul>);
	opMap.emplace("div", &CreateOp<Div>);
	opMap.emplace("inc", &CreateOp<Inc>);
	opMap.emplace("dec", &CreateOp<Dec>);
	opMap.emplace("jmp", &CreateOp<Jmp>);
	opMap.emplace("cmpeq", &CreateOp<Cmpeq>);
	opMap.emplace("cmplt", &CreateOp<Cmplt>);
	opMap.emplace("jt", &CreateOp<Jt>);
	opMap.emplace("jnt", &CreateOp<Jnt>);
	opMap.emplace("exit", &CreateOp<Exit>);

	opMap.emplace("push", &CreateOp<Push>);
	opMap.emplace("pop", &CreateOp<Pop>);
	opMap.emplace("load", &CreateOp<Load>);
	opMap.emplace("store", &CreateOp<Store>);
	opMap.emplace("loadi", &CreateOp<Loadi>);
	opMap.emplace("storei", &CreateOp<Storei>);
	opMap.emplace("loadsc", &CreateOp<Loadsc>);
	opMap.emplace("storesc", &CreateOp<Storesc>);

	opMap.emplace("pendown", &CreateOp<Pendown>);
	opMap.emplace("penup", &CreateOp<Penup>);
	opMap.emplace("fwd", &CreateOp<Fwd>);
	opMap.emplace("back", &CreateOp<Back>);

	registers.emplace("r0", 0);
	registers.emplace("r1", 0);
	registers.emplace("r2", 0);
	registers.emplace("r3", 0);
	registers.emplace("r4", 0);
	registers.emplace("r5", 0);
	registers.emplace("r6", 0);
	registers.emplace("r7", 0);
	registers.emplace("pc", 0); // program counter
	registers.emplace("sc", 0); // stack count
	registers.emplace("ex", 0); // exception code
	registers.emplace("tx", 0);
	registers.emplace("ty", 0);
	registers.emplace("tr", 0);
	registers.emplace("tc", 0);

	flags.emplace("exit", false);
	flags.emplace("test", false); //conditional jumps
	flags.emplace("pen", false);

	img.set_all_channels(0, 0, 0);
	drawer.pen_width(1);
	drawer.pen_color(255, 255, 255);
}

void Machine::input(std::string fileName) {
	std::ifstream file(fileName);
	std::string line;
	std::string opName;
	std::string opParams;
	size_t start = 0;
	size_t end;

	if (file.is_open()) {
		while (!file.eof()) {
			std::getline(file, line);
			size_t end = line.find_first_of(" ", start);
			opName = line.substr(start, end - start);
			opParams = line.substr(end + 1);

			std::shared_ptr<Op> ptr = opMap.at(opName)();
			ptr->Parse(opParams);
			mOps.emplace_back(ptr);
		}
	}
}
///////////////////////////////////////////////////////
int Machine::getReg(std::string reg) {
	return registers.at(reg);
}
void Machine::setReg(std::string reg, int val) {
	registers.at(reg) = val;
}
bool Machine::getFlag(std::string flag) {
	return flags.at(flag);
}
void Machine::setFlag(std::string flag, bool val) {
	flags.at(flag) = val;
}
void Machine::setJumpFlag(bool val) {
	jumpFlag = val;
}
/////////////////////////////////////////////////////////
void Machine::pushStack(int val) {
	stack.push_back(val);
	int val2 = getReg("sc") + 1;
	setReg("sc", val2);
}
int Machine::popStack() {
	int end = stack.back();
	stack.pop_back();
	int val2 = getReg("sc") - 1;
	setReg("sc", val2);
	return end;
}
int& Machine::indexStack(size_t index) {
	return stack.at(index);
}
int Machine::sizeStack() {
	return stack.size();
}
///////////////////////////////////////////////////////////
void Machine::fwdDraw(int units) {
	//check pen color
	drawer.pen_color(colorMap[registers.at("tc")][0], colorMap[registers.at("tc")][1], colorMap[registers.at("tc")][2]);
	//draw
	int tempTx = registers.at("tx");
	int tempTy = registers.at("ty");
	float tempTr = static_cast<float>(registers.at("tr")) * 0.0174533f; //convert to rad
	int endX = tempTx + static_cast<int>(std::cos(tempTr) * static_cast<float>(units));
	int endY = tempTy + static_cast<int>(std::sin(tempTr) * static_cast<float>(units));
	if (flags.at("pen")) {
		drawer.line_segment(tempTx, tempTy, endX, endY);
	}
	registers.at("tx") = endX;
	registers.at("ty") = endY;
}

void Machine::backDraw(int units) {
	//check pen color
	drawer.pen_color(colorMap[registers.at("tc")][0], colorMap[registers.at("tc")][1], colorMap[registers.at("tc")][2]);
	//draw
	units = 0-units;
	int tempTx = registers.at("tx");
	int tempTy = registers.at("ty");
	float tempTr = static_cast<float>(registers.at("tr")) * 0.0174533f; //convert to rad
	int endX = tempTx + static_cast<int>(std::cos(tempTr) * static_cast<float>(units));
	int endY = tempTy + static_cast<int>(std::sin(tempTr) * static_cast<float>(units));
	if (flags.at("pen")) {
		drawer.line_segment(tempTx, tempTy, endX, endY);
	}
	registers.at("tx") = endX;
	registers.at("ty") = endY;
}


///////////////////////////////////////////////////////////
void Machine::execute() {
	//init logging and stack files
	std::ofstream registerFile("log.txt");
	registerFile << "********************" << "\n";
	for  (auto i : regIndex) {
		registerFile << i << "=" << registers.at(i) << "\n";
	}
	for (auto i : flagIndex) {
		registerFile << i << "=" << flags.at(i) << "\n";
	}
	std::ofstream stackFile("stack.txt");
	stackFile << "pc: " << getReg("pc") << " Stack:";
	for (auto i : stack) {
		stackFile << " " << i;
	}
	stackFile << "\n";

	//execute commands
	int pcTemp;
	while (!flags.at("exit")) {
		
		pcTemp = registers.at("pc");

		try {
			mOps[registers.at("pc")]->Execute(*this);
		}
		catch (NonFatal& e) {
			registers.at("ex") = e.errorCode;
		}
		catch (Fatal & e) {
			registers.at("ex") = e.errorCode;
			flags.at("exit") = true;
		}

		if (!jumpFlag) {
			registers.at("pc") += 1;
		}
		else {
			jumpFlag = false;
		}
		
		//log.txt***
		registerFile << "Executing: " << mOps[pcTemp]->GetName() << "\n";
		registerFile << "********************" << "\n";
		for (auto i : regIndex) {
			registerFile << i << "=" << registers.at(i) << "\n";
		}
		for (auto i : flagIndex) {
			registerFile << i << "=" << flags.at(i) << "\n";
		}
		//stack.txt***
		stackFile << "pc: " << getReg("pc") << " Stack:";
		for (auto i : stack) {
			stackFile << " " << i;
		}
		stackFile << "\n";
	}
	
	//save resulting image
	img.save_image("output.bmp");
}
