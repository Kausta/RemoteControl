/*
* File: CommandManager.cpp
* Copyright: 2017 Caner Korkmaz (info@canerkorkmaz.com)
* Description:
*/

#include <sstream>

#include "CommandManager.h"

void command_manager::init_modes_()
{
	mode_sections_.reserve(2);

	section movement("Movement");
	movement.commands.emplace_back(command_type::Move, "Moves cursor to coords", "-m", "--move");
	movement.commands.emplace_back(command_type::MoveDelta, "Moves cursor to current coords + coords", "-md", "--move-delta");
	add_section(movement);
	section network("Network");
	network.commands.emplace_back(command_type::Network, "Opens connection at ip, port and gets movement modes from there", "-n", "--network");
	network.commands.emplace_back(command_type::SecureNetwork, "Network mode with ssl/tls, look to wiki for setting up authentication", "-sn", "--secure-network");
	add_section(network);
}

std::string command_manager::generate_help_message_() const
{
	std::stringstream ss;
	ss << "Remote Control Utility\n\n";
	ss << "Usage: " << program_name_ << " movement-mode x-pos y-pos\n";
	ss << "Usage: " << program_name_ << " network-mode port\n";
	ss << "=====================\n";
	for (auto const& section : mode_sections_)
	{
		ss << section.name << " Modes: \n";
		for (auto const& command : section.commands)
		{
			ss << "  " << command.short_arg << " ," << command.long_arg << ":\n";
			ss << "\t" << command.info << "\n";
		}
	}
	return ss.str();
}



command_manager::command_detail::command_detail(command_type type, const std::string& info, const std::string& short_arg, const std::string& long_arg)
  : type(type),
	info(info),
	short_arg(short_arg),
	long_arg(long_arg)
{
}

command_manager::section::section(const std::string& name)
  : name(name)
{
}

command_manager::command_manager(const std::string& program_name)
  : program_name_(program_name)
{
	init_modes_();
	help_message_ = generate_help_message_();
}

void command_manager::run(const std::vector<std::string>& command_line_args)
{
	// Expected that it will be given from programmer in constructor
	// Designed in this way to not have an extra copy, but also to make help_message available as early as possible
	// Also helps when trying to partially run
	//program_name_ = command_line_args[0];
	if (command_line_args.size() < 2)
	{
		throw std::runtime_error("Mode not specified!");
	}
	auto mode = command_line_args[1];
	std::vector<std::string> arguments{ command_line_args.begin() + 2, command_line_args.end() };

	auto it = modes_.find(mode);
	if (it == modes_.end())
	{
		throw std::runtime_error("Unknown mode!");
	}
	auto command = make_command(it->second, arguments);
	command->execute();
}

const std::string& command_manager::help_message() const
{
	return help_message_;
}

void command_manager::add_section(const section& section_)
{
	mode_sections_.push_back(section_);
	for (auto const& command : section_.commands)
	{
		modes_.emplace(command.short_arg, command.type);
		modes_.emplace(command.long_arg, command.type);
	}
}

