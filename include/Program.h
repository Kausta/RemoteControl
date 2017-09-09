/*
* File: Program.h
* Copyright: 2017 Caner Korkmaz (info@canerkorkmaz.com)
* Description:
*/

#pragma once

#include <memory>
#include <vector>
#include "CommandManager.h"

class program
{
public:
	static program& instance();
private:
	std::vector<std::string> arguments;
	std::unique_ptr<command_manager> command_manager_;

	program();
public:
	void init_args(int argc, char *argv[]);
	
	int run();

	size_t argument_count() const { return arguments.size(); }

	command_manager& command_manager() const;
};
