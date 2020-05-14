#pragma once
#include "Parser.hpp"
#include <string>
#include <tuple>
#include <vector>
#include <algorithm>


struct Op // NOLINT
{
	virtual const char* GetName() const = 0;
	virtual void Parse(const std::string& params) = 0;
	virtual void Execute(class Machine& machine) = 0;
	virtual ~Op() = default;
};

template <typename... Args>
struct OpBase : Op
{
	void Parse(const std::string& params) override {
		//split into vect
		std::vector<std::string> paramV = Split(params);
		// reverse vec (due to recursive template function)
		std::reverse(paramV.begin(), paramV.end());
		//generate tuple
		mParameters = ParseStr<Args...>(paramV);
	}

	// tuple for holding operation instructions
	std::tuple<Args...> mParameters;
};

struct MovI : OpBase<std::string, int> {
	const char* GetName() const override { 
		return "movi";
	}
	void Execute(class Machine& machine) override;
};

///////////////////////////Register and Flag Ops///////////////////////////////
struct Exit : OpBase<> {
	const char* GetName() const override { return "exit"; }
	void Execute(class Machine& machine) override;
};

struct Add : OpBase<std::string, std::string, std::string> {
	const char* GetName() const override { return "add"; }
	void Execute(class Machine& machine) override;
};

struct Mov : OpBase<std::string, std::string> {
	const char* GetName() const override { return "mov"; }
	void Execute(class Machine& machine) override;
};

struct Sub : OpBase<std::string, std::string, std::string> {
	const char* GetName() const override { return "sub"; }
	void Execute(class Machine& machine) override;
};

struct Mul : OpBase<std::string, std::string, std::string> {
	const char* GetName() const override { return "mul"; }
	void Execute(class Machine& machine) override;
};

struct Div : OpBase<std::string, std::string, std::string> {
	const char* GetName() const override { return "div"; }
	void Execute(class Machine& machine) override;
};

struct Inc : OpBase<std::string> {
	const char* GetName() const override { return "inc"; }
	void Execute(class Machine& machine) override;
};

struct Dec : OpBase<std::string> {
	const char* GetName() const override { return "dec"; }
	void Execute(class Machine& machine) override;
};

struct Jmp : OpBase<std::string> {
	const char* GetName() const override { return "jmp"; }
	void Execute(class Machine& machine) override;
};

struct Cmpeq : OpBase<std::string, std::string> {
	const char* GetName() const override { return "cmpeq"; }
	void Execute(class Machine& machine) override;
};

struct Cmplt : OpBase<std::string, std::string> {
	const char* GetName() const override { return "cmplt"; }
	void Execute(class Machine& machine) override;
};

struct Jt : OpBase<std::string> {
	const char* GetName() const override { return "jt"; }
	void Execute(class Machine& machine) override;
};

struct Jnt : OpBase<std::string> {
	const char* GetName() const override { return "jnt"; }
	void Execute(class Machine& machine) override;
};
/////////////////////////Stack Ops///////////////////////////////////
struct Push : OpBase<std::string> {
	const char* GetName() const override { return "push"; }
	void Execute(class Machine& machine) override;
};

struct Pop : OpBase<std::string> {
	const char* GetName() const override { return "pop"; }
	void Execute(class Machine& machine) override;
};

struct Load : OpBase<std::string, std::string> {
	const char* GetName() const override { return "load"; }
	void Execute(class Machine& machine) override;
};

struct Store : OpBase<std::string, std::string> {
	const char* GetName() const override { return "store"; }
	void Execute(class Machine& machine) override;
};

struct Loadi : OpBase<std::string, int> {
	const char* GetName() const override { return "loadi"; }
	void Execute(class Machine& machine) override;
};

struct Storei : OpBase<int, std::string> {
	const char* GetName() const override { return "storei"; }
	void Execute(class Machine& machine) override;
};

struct Loadsc : OpBase<std::string, std::string> {
	const char* GetName() const override { return "loadsc"; }
	void Execute(class Machine& machine) override;
};

struct Storesc : OpBase<std::string, std::string> {
	const char* GetName() const override { return "storesc"; }
	void Execute(class Machine& machine) override;
};
/////////////////////////Drawing Ops///////////////////////////////////////
struct Pendown : OpBase<> {
	const char* GetName() const override { return "pendown"; }
	void Execute(class Machine& machine) override;
};

struct Penup : OpBase<> {
	const char* GetName() const override { return "penup"; }
	void Execute(class Machine& machine) override;
};

struct Fwd : OpBase<std::string> {
	const char* GetName() const override { return "fwd"; }
	void Execute(class Machine& machine) override;
};

struct Back : OpBase<std::string> {
	const char* GetName() const override { return "back"; }
	void Execute(class Machine& machine) override;
};



