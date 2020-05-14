#include "Ops.h"
#include "Machine.h"

void Exit::Execute(Machine& machine)
{
	machine.setFlag("exit", true);
}
//////////////////////////////Register and Flag Executes///////////////////////////
void MovI::Execute(Machine& machine)
{
	if (std::get<0>(mParameters) == "tc"){
		if (std::get<1>(mParameters) > 7 || std::get<1>(mParameters) < 0) {
			throw NonFatal(14);
		}
		else {
			machine.setReg(std::get<0>(mParameters), std::get<1>(mParameters));
		}
	}
	else if (std::get<0>(mParameters) == "r0"){
		throw NonFatal(11);
	}
	else if (std::get<0>(mParameters) == "sc") {
		throw NonFatal(10);
	}
	else {
		machine.setReg(std::get<0>(mParameters), std::get<1>(mParameters));
	}
}

void Add::Execute(Machine& machine)
{
	int val = machine.getReg(std::get<1>(mParameters)) + machine.getReg(std::get<2>(mParameters));
	machine.setReg(std::get<0>(mParameters), val);

	if (static_cast<int64_t>(machine.getReg(std::get<1>(mParameters))) + static_cast<int64_t>(machine.getReg(std::get<2>(mParameters))) > INT32_MAX) {
		throw NonFatal(12);
	}
	if (static_cast<int64_t>(machine.getReg(std::get<1>(mParameters))) + static_cast<int64_t>(machine.getReg(std::get<2>(mParameters))) < INT32_MIN) {
		throw NonFatal(13);
	}
}

void Mov::Execute(Machine& machine)
{
	machine.setReg(std::get<0>(mParameters), machine.getReg(std::get<1>(mParameters)));
}

void Sub::Execute(Machine& machine)
{
	int val = machine.getReg(std::get<1>(mParameters)) - machine.getReg(std::get<2>(mParameters));
	machine.setReg(std::get<0>(mParameters), val);

	if (static_cast<int64_t>(machine.getReg(std::get<1>(mParameters))) - static_cast<int64_t>(machine.getReg(std::get<2>(mParameters))) > INT32_MAX) {
		throw NonFatal(12);
	}
	if (static_cast<int64_t>(machine.getReg(std::get<1>(mParameters))) - static_cast<int64_t>(machine.getReg(std::get<2>(mParameters))) < INT32_MIN) {
		throw NonFatal(13);
	}
}

void Mul::Execute(Machine& machine)
{
	int val = machine.getReg(std::get<1>(mParameters)) * machine.getReg(std::get<2>(mParameters));
	machine.setReg(std::get<0>(mParameters), val);

	if (static_cast<int64_t>(machine.getReg(std::get<1>(mParameters))) * static_cast<int64_t>(machine.getReg(std::get<2>(mParameters))) > INT32_MAX) {
		throw NonFatal(12);
	}
	if (static_cast<int64_t>(machine.getReg(std::get<1>(mParameters))) * static_cast<int64_t>(machine.getReg(std::get<2>(mParameters))) < INT32_MIN) {
		throw NonFatal(13);
	}
}

void Div::Execute(Machine& machine)
{
	if (machine.getReg(std::get<2>(mParameters)) == 0) {
		throw Fatal(102);
	}
	else {
		int val = machine.getReg(std::get<1>(mParameters)) / machine.getReg(std::get<2>(mParameters));
		machine.setReg(std::get<0>(mParameters), val);
	}

	if (static_cast<int64_t>(machine.getReg(std::get<1>(mParameters))) / static_cast<int64_t>(machine.getReg(std::get<2>(mParameters))) > INT32_MAX) {
		throw NonFatal(12);
	}
	if (static_cast<int64_t>(machine.getReg(std::get<1>(mParameters))) / static_cast<int64_t>(machine.getReg(std::get<2>(mParameters))) < INT32_MIN) {
		throw NonFatal(13);
	}
}

void Inc::Execute(Machine& machine)
{
	int val = machine.getReg(std::get<0>(mParameters)) + 1;
	machine.setReg(std::get<0>(mParameters), val);
}

void Dec::Execute(Machine& machine)
{
	int val = machine.getReg(std::get<0>(mParameters)) - 1;
	machine.setReg(std::get<0>(mParameters), val);
}

void Jmp::Execute(Machine& machine)
{
	machine.setReg("pc", machine.getReg(std::get<0>(mParameters)));
	machine.setJumpFlag(true);
}

void Cmpeq::Execute(Machine& machine)
{
	machine.setFlag("test", machine.getReg(std::get<0>(mParameters)) == machine.getReg(std::get<1>(mParameters)));
}

void Cmplt::Execute(Machine& machine)
{
	machine.setFlag("test", machine.getReg(std::get<0>(mParameters)) < machine.getReg(std::get<1>(mParameters)));
}

void Jt::Execute(Machine& machine)
{
	if (static_cast<bool>(machine.getFlag("test"))) {
		machine.setReg("pc", machine.getReg(std::get<0>(mParameters)));
		machine.setJumpFlag(true);
	}
}

void Jnt::Execute(Machine& machine)
{
	if (!static_cast<bool>(machine.getFlag("test"))) {
		machine.setReg("pc", machine.getReg(std::get<0>(mParameters)));
		machine.setJumpFlag(true);
	}
}
////////////////////////Stack Executes///////////////////////////////
void Push::Execute(Machine& machine)
{
	if (machine.sizeStack() == 256) {
		throw Fatal(101);
	}
	else {
		machine.pushStack(machine.getReg(std::get<0>(mParameters)));
	}
}

void Pop::Execute(Machine& machine)
{
	if (machine.sizeStack() == 0) {
		throw Fatal(103);
	}
	else {
		machine.setReg(std::get<0>(mParameters), machine.popStack());
	}
}

void Load::Execute(Machine& machine)
{
	machine.setReg(std::get<0>(mParameters), static_cast<int>(machine.indexStack(machine.getReg(std::get<1>(mParameters)))));
}

void Store::Execute(Machine& machine)
{
	machine.indexStack(machine.getReg(std::get<0>(mParameters))) = machine.getReg(std::get<1>(mParameters));
}

void Loadi::Execute(Machine& machine)
{
	if (std::get<1>(mParameters) > machine.sizeStack() - 1 || std::get<1>(mParameters) < 0) {
		throw Fatal(100);
	}
	else {
		machine.setReg(std::get<0>(mParameters), machine.indexStack(std::get<1>(mParameters)));
	}
}

void Storei::Execute(Machine& machine)
{
	if (std::get<0>(mParameters) > machine.sizeStack() - 1 || std::get<0>(mParameters) < 0) {
		throw Fatal(100);
	}
	else {
		machine.indexStack(std::get<0>(mParameters)) = machine.getReg(std::get<1>(mParameters));
	}
}

void Loadsc::Execute(Machine& machine)
{
	machine.setReg(std::get<0>(mParameters), static_cast<int>(machine.indexStack(machine.getReg("sc") - machine.getReg(std::get<1>(mParameters)) - static_cast<int>(1))));
}

void Storesc::Execute(Machine& machine)
{
	if (machine.getReg("sc") - machine.getReg(std::get<0>(mParameters)) - static_cast<int>(1) - 1 > machine.sizeStack() - 1 || machine.getReg("sc") - machine.getReg(std::get<0>(mParameters)) - static_cast<int>(1) < 0) {
		throw Fatal(100);
	}
	else {
		machine.indexStack(machine.getReg("sc") - machine.getReg(std::get<0>(mParameters)) - static_cast<int>(1)) = machine.getReg(std::get<1>(mParameters));
	}
}
///////////////////////////Draw Executes////////////////////////////////
void Pendown::Execute(Machine& machine)
{
	machine.setFlag("pen", true);
}

void Penup::Execute(Machine& machine)
{
	machine.setFlag("pen", false);
}

void Fwd::Execute(Machine& machine)
{
	machine.fwdDraw(machine.getReg(std::get<0>(mParameters)));
}

void Back::Execute(Machine& machine)
{
	machine.backDraw(machine.getReg(std::get<0>(mParameters)));
}