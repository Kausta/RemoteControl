/*
* File: Program.cpp
* Copyright: 2017 Caner Korkmaz (info@canerkorkmaz.com)
* Description:
*/

#include <iostream>

#include "CommandManager.h"

#include "Program.h"

int main(int argc, char* argv[])
{
	program prog(argc, argv);
	return prog.run();
}

program::program(int argc, char* argv[])
	: arguments(argv, argv + argc)
{ }

int program::run()
{
	command_manager manager(arguments[0]);
	try
	{
		manager.run(arguments);
		
		return EXIT_SUCCESS;
	}
	catch(const std::runtime_error& error)
	{
		std::cout << manager.help_message();
		std::cout << "Error: " << error.what() << "\n";
		return EXIT_FAILURE;
	}
}