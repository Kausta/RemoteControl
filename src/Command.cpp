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
#include "CommandReader.h"

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

mouse_click_command::mouse_click_command(const std::vector<std::string>& arguments)
	: command_base(arguments)
{
	type = mouse_control::click_type::Left;
	click_time_ms = 100ul;

	if(arguments.empty())
	{
		return;
	}
	auto type_opt = try_parse_type(arguments[0]);
	if(type_opt.has_value())
	{
		type = type_opt.value();
		if (arguments.size() >= 2)
		{
			auto time_opt = try_parse_time(arguments[1]);
			if (time_opt.has_value())
			{
				click_time_ms = time_opt.value();
			}
		}
	}
	else
	{
		auto time_opt = try_parse_time(arguments[0]);
		if(time_opt.has_value())
		{
			click_time_ms = time_opt.value();
			if(arguments.size() >= 2)
			{
				type_opt = try_parse_type(arguments[1]);
				if(type_opt.has_value())
				{
					type = type_opt.value();
				}
			}
		}
	}
}

void mouse_click_command::execute() const
{
	mouse_control::click(type, click_time_ms);
}


std::optional<mouse_control::click_type> mouse_click_command::try_parse_type(const std::string& arg)
{
	if(arg == "-l" || arg == "--left")
	{
		return mouse_control::click_type::Left;
	}
	if(arg == "-r" || arg == "--right")
	{
		return mouse_control::click_type::Right;
	}
	if(arg == "-m" || arg == "--middle")
	{
		return mouse_control::click_type::Middle;
	}
	return {};
}

std::optional<unsigned long> mouse_click_command::try_parse_time(const std::string& arg)
{
	try
	{
		return std::stoul(arg);
	}
	catch(...)
	{
		return {};
	}
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
	std::vector<std::string> break_modes{ "exit", "-e", "--exit" };

	command_reader reader(manager, break_modes);
	std::cout << "Input exit\\-e\\--exit to exit.\n";
	while (true)
	{
		try
		{
			std::string input;
			std::getline(std::cin, input);
			
			auto executed = reader.execute_line(input);
			if(!executed)
			{
				// User entered a break mode
				break;
			}
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
	case command_type::MouseClick:
		return make_command<mouse_click_command>(arguments);
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
