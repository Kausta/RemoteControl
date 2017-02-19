/*
 * File: Command.cpp
 * Copyright: 2017 Caner Korkmaz (info@canerkorkmaz.com)
 * Description:
 */

#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include <sstream>

#include "Server.h"

#include "Command.h"
#include "Program.h"

command_base::command_base(const std::vector<std::string>& arguments)
	: arguments_(arguments)
{
}

move_command::move_command(const std::vector<std::string>& arguments)
	: command_base(arguments)
{
	if (arguments.size() < 2)
	{
		throw std::runtime_error("Not enough arguments for move command.");
	}

	auto x = std::stol(arguments[0]),
		y = std::stol(arguments[1]);
	point_ = {x,y};
}

void move_command::execute() const
{
	mouse_control::move_to(point_);
}

move_delta_command::move_delta_command(const std::vector<std::string>& arguments)
	: move_command(arguments)
{
}

void move_delta_command::execute() const
{
	mouse_control::move_to_relative(point_);
}

network_command::network_command(const std::vector<std::string>& arguments)
	: command_base(arguments)
{
	if(arguments.empty())
	{
		throw std::runtime_error("Port arguments not specified for network mode.");
	}
	port_ = static_cast<unsigned short>(std::stoul(arguments_[0]));
}

void network_command::execute() const
{
	boost::asio::io_service io_service;
	network::server server(io_service, port_);
	io_service.run();
}

multiple_input_command::multiple_input_command(const std::vector<std::string>& arguments)
	: command_base(arguments)
{
}

void multiple_input_command::execute() const
{
	auto& manager = program::instance().command_manager();
	std::cout << "Input exit\\-e\\--exit to exit.\n";
	while (true)
	{
		try
		{
			std::string input;
			std::getline(std::cin, input);
			std::istringstream iss(input);
			
			std::string mode;
			iss >> mode;
			if(mode == "exit" || mode == "-e" || mode == "--exit")
			{
				break;
			}

			std::string arg;
			std::vector<std::string> arguments;
			while(iss >> arg)
			{
				arguments.push_back(arg);
			}

			manager.run_from_mode(mode, arguments);
		}
		catch (const std::runtime_error& error)
		{
			std::cout << "Error: " << error.what() << "\n";
		}
	}
}

std::unique_ptr<command_base> make_command(command_type type, const std::vector<std::string>& arguments)
{
	switch (type)
	{
	case command_type::Move:
		return make_command<move_command>(arguments);
	case command_type::MoveDelta:
		return make_command<move_delta_command>(arguments);
	case command_type::Network:
	case command_type::SecureNetwork:
		return make_command<network_command>(arguments);
	case command_type::MultipleInput:
		return make_command<multiple_input_command>(arguments);
	case command_type::Unknown:
	default:
		throw std::runtime_error("Unknown mode argument entered!");
	}
}
