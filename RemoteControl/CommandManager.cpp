/*
* File: CommandManager.cpp
* Copyright: 2017 Caner Korkmaz (info@canerkorkmaz.com)
* Description:
*/

#include <sstream>

#include "CommandManager.h"

void command_manager::init_modes_()
{
	sections_.reserve(1);
	utility_sections_.reserve(2);

	section movement("Movement");
	movement.commands.emplace_back(command_type::Move, "Moves cursor to coords", "-m", "--move");
	movement.commands.emplace_back(command_type::MoveDelta, "Moves cursor to current coords + coords", "-md", "--move-delta");
	add_section(movement);

	
	section network("Network");
	network.commands.emplace_back(command_type::Network, "Opens connection at ip, port and gets movement modes from there", "-n", "--network");
	network.commands.emplace_back(command_type::SecureNetwork, "Network mode with ssl/tls, look to wiki for setting up authentication", "-sn", "--secure-network");
	add_utility_section(network);
	
	section other("Other");
	other.commands.emplace_back(command_type::MultipleInput, "Allows multiple commands from stdin, seperated with newlines.", "-mi", "--multi-input");
	add_utility_section(other);
}

std::string command_manager::generate_help_message_() const
{
	std::stringstream ss;
	ss << "Remote Control Utility\n\n";
	ss << "Usage: " << program_name_ << " movement-mode x-pos y-pos\n";
	ss << "Usage: " << program_name_ << " network-mode port\n";
	ss << "Usage: " << program_name_ << " other-mode\n";
	ss << "=====================\n";
	insert_help_for_section_type(ss, sections_);
	insert_help_for_section_type(ss, utility_sections_);
	return ss.str();
}

void command_manager::insert_help_for_section_type(std::stringstream& ss, const std::vector<section>& section_holder_)
{
	for (auto const& section : section_holder_)
	{
		ss << section.name << " Modes: \n";
		for (auto const& command : section.commands)
		{
			ss << "  " << command.short_arg << " ," << command.long_arg << ":\n";
			ss << "\t" << command.info << "\n";
		}
	}
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

command_manager::command_manager()
	: command_manager("RemoteControl")
{
}

command_manager::command_manager(const std::string& program_name)
  : program_name_(program_name)
{
	init_modes_();
	help_message_ = generate_help_message_();
}

void command_manager::run_from_mode(const std::string& mode, const std::vector<std::string>& arguments)
{
	auto command_type = get_mode(mode);
	auto command = make_command(command_type, arguments);
	command->execute();
}

command_type command_manager::get_mode(std::string mode_arg) const
{
	auto it = modes_.find(mode_arg);
	if (it != modes_.end())
	{
		return it->second;
	}
	if (utility_modes_enabled())
	{
		auto it_u = utility_modes_.find(mode_arg);
		if (it_u != utility_modes_.end())
		{
			return it_u->second;
		}
	}
	throw std::runtime_error("Unknown mode!");
}


const std::string& command_manager::help_message() const
{
	return help_message_;
}

void command_manager::add_section(const section& section_)
{
	add_section_to(section_, sections_, modes_);
}

void command_manager::add_utility_section(const section& section_)
{
	add_section_to(section_, utility_sections_, utility_modes_);
}

void command_manager::add_section_to(const section& section_, std::vector<section>& section_holder_, std::map<std::string, command_type>& mode_holder_)
{
	section_holder_.push_back(section_);
	for (auto const& command : section_.commands)
	{
		mode_holder_.emplace(command.short_arg, command.type);
		mode_holder_.emplace(command.long_arg, command.type);
	}
}

