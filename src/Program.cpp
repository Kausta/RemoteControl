/*
* File: Program.cpp
* Copyright: 2017 Caner Korkmaz (info@canerkorkmaz.com)
* Description:
*/

#include <iostream>

#include "CommandManager.h"

#include "Program.h"

program& program::instance()
{
	static program instance_;
	return instance_;
}

program::program()
{
}

void program::init_args(int argc, char* argv[])
{
	arguments = std::vector<std::string>{ argv, argv + argc };
}

int program::run()
{
	command_manager_ = std::make_unique<::command_manager>(arguments[0]);
	try
	{
		if(arguments.size() < 2)
		{
			throw std::runtime_error("Mode not specified.");
		}
		auto mode = arguments[1];
		std::vector<std::string> mode_arguments{ arguments.begin() + 2, arguments.end() };
		command_manager_->run_from_mode(mode, mode_arguments);
		
		return EXIT_SUCCESS;
	}
	catch(const std::runtime_error& error)
	{
		std::cout << command_manager_->help_message();
		std::cout << "Error: " << error.what() << "\n";
		return EXIT_FAILURE;
	}
}

command_manager& program::command_manager() const
{
	return *command_manager_;
}
