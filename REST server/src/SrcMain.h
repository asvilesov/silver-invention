#pragma once

#include "imdb.h"

void ProcessCommandArgs(int argc, const char* argv[]);

void on_initialize(const string_t& address, std::string fileName);

void on_shutdown();
