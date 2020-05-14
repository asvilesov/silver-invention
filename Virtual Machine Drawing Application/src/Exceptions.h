#pragma once
#include <exception>

class NonFatal: public std::exception{
public:
	NonFatal(int err) : errorCode(err) {};
	int errorCode;
};

struct Fatal : public std::exception {
	Fatal(int err) : errorCode(err) {};
	int errorCode;
};