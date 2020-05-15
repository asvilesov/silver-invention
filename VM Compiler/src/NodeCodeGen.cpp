#include "Node.h"
#include "parser.hpp"
#include <utility>
#include <iostream>

void CodeContext::virtRegsToReal() {
	std::size_t regLoc = 0;
	int i = 0;
	int virtReg = 0;
	int comma = 0;
	std::string begin;
	std::string end;
	for (auto& command : ops) {
		regLoc = command.find("%");
		comma = command.find(",");
		while (regLoc != std::string::npos) {
			if (comma == std::string::npos) {
				comma = command.size();
			}
			if (regLoc < comma) {
				virtReg = std::stoi(command.substr(regLoc + 1, comma - regLoc - 1));
				begin = command.substr(0, regLoc);
				end = command.substr(comma, command.size() - comma);
				command = begin + "r" + std::to_string(registerAlloc[virtReg] + 1) + end;
				std::cout << command << "\n";
			}
			else if (regLoc > comma) {
				virtReg = std::stoi(command.substr(regLoc + 1, 4));
				begin = command.substr(0, regLoc);
				command = begin + "r" + std::to_string(registerAlloc[virtReg] + 1);
				std::cout << command << "\n";
			}


			regLoc = command.find("%", regLoc + 1);
			comma = command.find(",", comma + 1);

			if (comma == command.size()) {
				break;
			}

			/*++i;
			if (i == 10) {
				i = 0;
				break;
			}*/
		}
		//i = 0;
	}
}

void CodeContext::registerAllocation() {
	std::vector<bool> regFree{ true, true, true, true, true, true, true, true }; //8 regs avaliable
	if (!registerIntervals.empty()) {
		for (auto i = 0; i < ops.size(); ++i) { // iterate through instructions
			std::cout << "instruction " << i << "\n";
			for (auto j = 0; j < registerIntervals.size(); ++j) { // iterate through intervals for matching instruction numbers of virtual registers
				if (registerIntervals[j].second == i) {// end of virt reg interval
					std::cout << "free register" << registerAlloc[j] << "\n";
					regFree[registerAlloc[j]] = true;
				}
				if (registerIntervals[j].first == i) { // start of virt reg interval
					for (auto k = 0; k < regFree.size(); ++k) { //iterate through register states
						if (regFree[k]) {
							std::cout << "register " << k << " is using virt reg "  << j << "\n";
							regFree[k] = false;
							registerAlloc.emplace_back(k);
							break;
						}
					}
				}
			}
		}
	}

}

void CodeContext::generateIntervals() {
	//create register intervals for allocation
	std::size_t regLoc = 0;
	for (auto j = 0; j < currVirtReg; ++j) {
		registerIntervals.emplace_back(std::make_pair(-1, -1));
	}
	int i = 0;
	int virtReg = 0;
	int comma = 0;
	for (auto command : ops) {
		regLoc = command.find("%");
		comma = command.find(",");
		while (regLoc != std::string::npos) {
			if (comma == std::string::npos) {
				comma = command.size();
			}
			virtReg = std::stoi(command.substr(regLoc + 1, comma-regLoc-1));
			if (registerIntervals[virtReg].first == -1) {
				registerIntervals[virtReg].first = i;
			}
			else {
				registerIntervals[virtReg].second = i;
			}
			regLoc = command.find("%", regLoc+1);
			comma = command.find(",");
		}
		++i;
	}
}

void NBlock::CodeGen(CodeContext& context)
{
	for (auto i : mStatements) {
		i->CodeGen(context);
	}
}

void NData::CodeGen(CodeContext& context)
{
	for (auto i : mDecls) {
		i->CodeGen(context);
	}
}

void NProgram::CodeGen(CodeContext& context)
{
	mData->CodeGen(context);
	mMain->CodeGen(context);
	context.ops.emplace_back(std::string("exit"));
}

void NNumeric::CodeGen(CodeContext& context)
{
	/*context.ops.push_back(std::string("movi $" + std::to_string(context.currVirtReg) + "," + std::to_string(mValue)));
	++context.currVirtReg;*/
}

void NVarDecl::CodeGen(CodeContext& context)
{
	context.ops.emplace_back(std::string("push r0"));
	context.stackAddress.insert(std::pair<std::string, int>(mName, context.currentStack));
	context.currentStack += 1;
}

void NArrayDecl::CodeGen(CodeContext& context)
{
	int arrSize = mSize->GetValue();
	for (auto i = 0; i < arrSize; ++i) {
		context.ops.emplace_back(std::string("push r0"));
	}
	context.stackAddress.insert(std::pair<std::string, int>(mName, context.currentStack));
	context.currentStack += arrSize;
}

void NNumericExpr::CodeGen(CodeContext& context)
{
	context.ops.emplace_back(std::string("movi %" + std::to_string(context.currVirtReg) + "," + std::to_string(mNumeric->GetValue())));
	mResultRegister = std::to_string(context.currVirtReg);
	++context.currVirtReg;
}

void NVarExpr::CodeGen(CodeContext& context)
{
	context.ops.emplace_back(std::string("loadi %" + std::to_string(context.currVirtReg) + "," + std::to_string(context.stackAddress.at(mName))));
	mResultRegister = std::to_string(context.currVirtReg);
	++context.currVirtReg;
}

void NBinaryExpr::CodeGen(CodeContext& context)
{
	mLhs->CodeGen(context);
	mRhs->CodeGen(context);
	std::string operation;
	if (mType == 270) {
		operation = "add";
	}
	else if (mType == 271) {
		operation = "sub";
	}
	else if (mType == 272) {
		operation = "mul";
	}
	else if (mType == 273) {
		operation = "div";
	}
	mResultRegister = std::to_string(context.currVirtReg);
	context.ops.emplace_back(std::string(operation + " %" + mResultRegister + ",%" + mLhs->GetResultRegister() + ",%" + mRhs->GetResultRegister())); 
	++context.currVirtReg;
}

void NArrayExpr::CodeGen(CodeContext& context)
{
	mSubscript->CodeGen(context);
	std::string baseReg = std::to_string(context.currVirtReg);
	context.ops.emplace_back(std::string("movi %" + baseReg + "," + std::to_string(context.stackAddress.at(mName))));
	++context.currVirtReg;
	std::string addReg = std::to_string(context.currVirtReg);
	context.ops.emplace_back(std::string("add %" + addReg + ",%" + baseReg + ",%" + mSubscript->GetResultRegister()));
	++context.currVirtReg;
	mResultRegister = std::to_string(context.currVirtReg); // stores the result of the addition
	context.ops.emplace_back(std::string("load %" + mResultRegister + ",%" + addReg));
	++context.currVirtReg;
}

void NAssignVarStmt::CodeGen(CodeContext& context)
{
	mRhs->CodeGen(context);
	context.ops.emplace_back("storei " + std::to_string(context.stackAddress.at(mName)) + ",%" + mRhs->GetResultRegister());
}

void NAssignArrayStmt::CodeGen(CodeContext& context)
{
	//process right hand side of statement
	mRhs->CodeGen(context);
	//process the array subscript
	mSubscript->CodeGen(context);
	//find stack position
	std::string baseReg = std::to_string(context.currVirtReg);
	context.ops.emplace_back(std::string("movi %" + baseReg + "," + std::to_string(context.stackAddress.at(mName))));
	++context.currVirtReg;
	std::string addReg = std::to_string(context.currVirtReg);
	context.ops.emplace_back(std::string("add %" + addReg + ",%" + baseReg + ",%" + mSubscript->GetResultRegister()));
	++context.currVirtReg;
	context.ops.emplace_back(std::string("store %" + addReg + ",%" + mRhs->GetResultRegister()));


}

void NIncStmt::CodeGen(CodeContext& context)
{
	std::string varReg = std::to_string(context.currVirtReg);
	context.ops.emplace_back(std::string("loadi %" + varReg + "," + std::to_string(context.stackAddress.at(mName))));
	++context.currVirtReg;
	context.ops.emplace_back(std::string("inc %" + varReg));
	context.ops.emplace_back(std::string("storei " + std::to_string(context.stackAddress.at(mName)) + ",%" + varReg));
}

void NDecStmt::CodeGen(CodeContext& context)
{
	std::string varReg = std::to_string(context.currVirtReg);
	context.ops.emplace_back(std::string("loadi %" + varReg + "," + std::to_string(context.stackAddress.at(mName))));
	++context.currVirtReg;
	context.ops.emplace_back(std::string("dec %" + varReg));
	context.ops.emplace_back(std::string("storei " + std::to_string(context.stackAddress.at(mName)) + ",%" + varReg));
}

void NComparison::CodeGen(CodeContext& context)
{
	mLhs->CodeGen(context);
	mRhs->CodeGen(context);
	std::string compOp;
	if (mType == 274) {
		compOp = "cmplt";
	}
	else if (mType == 275) {
		compOp = "cmpeq";
	}
	context.ops.emplace_back(std::string(compOp + " %" + mLhs->GetResultRegister() + ",%" + mRhs->GetResultRegister()));
}

void NIfStmt::CodeGen(CodeContext& context)
{
	//evaluate comparison
	mComp->CodeGen(context);
	//evaluate if/else statement
	std::string jumpAdd = std::to_string(context.currVirtReg);
	size_t tempPC = context.ops.size();
	context.ops.emplace_back(std::string("movi %" + jumpAdd + ",")); //at this point we have not added the resulting address to conditional jump to
	context.ops.emplace_back(std::string("jnt %" + jumpAdd));
	++context.currVirtReg;
	//see how many statements are in the block + evaluate block
	mIfBlock->CodeGen(context);
	size_t endPC = context.ops.size();
	//check if an else statement exists
	if (mElseBlock != nullptr) {
		context.ops[tempPC] += std::to_string(endPC+2); // the + 2? Because we are going two steps past the conditional
		jumpAdd = std::to_string(context.currVirtReg);
		tempPC = context.ops.size();
		context.ops.emplace_back(std::string("movi %" + jumpAdd + ",")); //at this point we have not added the resulting address to jump to
		context.ops.emplace_back(std::string("jmp %" + jumpAdd));
		++context.currVirtReg;
		//see how many statements are in the else block + evaluate block
		mElseBlock->CodeGen(context);
		endPC = context.ops.size();
		context.ops[tempPC] += std::to_string(endPC);
	}
	else {
		context.ops[tempPC] += std::to_string(endPC);
	}

}

void NWhileStmt::CodeGen(CodeContext& context)
{
	//evaluate comparison
	mComp->CodeGen(context);
	//evaluate jnt statement
	std::string jumpAdd = std::to_string(context.currVirtReg);
	size_t tempPC = context.ops.size();
	context.ops.emplace_back(std::string("movi %" + jumpAdd + ",")); //at this point we have not added the resulting address to conditional jump to
	context.ops.emplace_back(std::string("jnt %" + jumpAdd));
	++context.currVirtReg;
	//see how many statements are in the block + eval
	mBlock->CodeGen(context);
	size_t endPC = context.ops.size();
	context.ops[tempPC] += std::to_string(endPC+2);
	//jmp back to the conditional at the beginning of the loop
	std::string jumpBack = std::to_string(context.currVirtReg);
	context.ops.emplace_back(std::string("movi %" + jumpBack + "," + std::to_string(tempPC-3))); // the + 3? Because we want to execute the conditional again
	context.ops.emplace_back(std::string("jmp %" + jumpBack));
	++context.currVirtReg;

}

void NPenUpStmt::CodeGen(CodeContext& context)
{
	context.ops.emplace_back(std::string("penup"));
}

void NPenDownStmt::CodeGen(CodeContext& context)
{
	context.ops.emplace_back(std::string("pendown"));
}

void NSetPosStmt::CodeGen(CodeContext& context)
{
	mXExpr->CodeGen(context);
	mYExpr->CodeGen(context);
	context.ops.emplace_back(std::string("mov tx,%" + mXExpr->GetResultRegister()));
	context.ops.emplace_back(std::string("mov ty,%" + mYExpr->GetResultRegister()));
}

void NSetColorStmt::CodeGen(CodeContext& context)
{
	mColor->CodeGen(context);
	context.ops.emplace_back(std::string("mov tc,%" + mColor->GetResultRegister()));
}

void NFwdStmt::CodeGen(CodeContext& context)
{
	mParam->CodeGen(context);
	context.ops.emplace_back(std::string("fwd %" + mParam->GetResultRegister()));
}

void NBackStmt::CodeGen(CodeContext& context)
{
	mParam->CodeGen(context);
	context.ops.emplace_back(std::string("back %" + mParam->GetResultRegister()));
}

void NRotStmt::CodeGen(CodeContext& context)
{
	mParam->CodeGen(context);
	context.ops.emplace_back(std::string("add tr,tr,%" + mParam->GetResultRegister()));
}
