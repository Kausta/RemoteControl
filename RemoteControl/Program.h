/*
* File: Program.h
* Copyright: 2017 Caner Korkmaz (info@canerkorkmaz.com)
* Description:
*/

#pragma once
#include <string>
#include <vector>

class program
{
private:
	std::vector<std::string> arguments;
public:
	program(int argc, char* argv[]);
	
	int run();

	size_t argument_count() const { return arguments.size(); }
};
