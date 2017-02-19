/*
* File: CommandManager.h
* Copyright: 2017 Caner Korkmaz (info@canerkorkmaz.com)
* Description:
*/

#pragma once

#include <map>

#include "Command.h"

class command_manager
{
private:
	struct command_detail
	{
		command_type type;
		std::string info;
		std::string short_arg, long_arg;

		explicit command_detail(command_type type, const std::string& info, const std::string& short_arg, const std::string& long_arg);
	};
	struct section
	{
		std::string name;
		std::vector<command_detail> commands;

		explicit section(const std::string& name);
	};
public:
	explicit command_manager(const std::string& program_name);

	void run(const std::vector<std::string>& command_line_args);

	const std::string& help_message() const;
private:
	void init_modes_();

	void add_section(const section& section_);

	std::string generate_help_message_() const;

	std::string program_name_;
	
	std::vector<section> mode_sections_;
	std::map<std::string, command_type> modes_;

	std::string help_message_;
};
