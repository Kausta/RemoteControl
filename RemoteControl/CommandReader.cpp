#include "CommandReader.h"

command_reader::command_reader(command_manager& manager)
	: manager_(manager)
	, break_modes_()
{
}

command_reader::command_reader(command_manager& manager, const std::vector<std::string>& break_modes)
	: manager_(manager)
	, break_modes_(break_modes)
{
}

bool command_reader::execute_line(const std::string& line) const
{
	std::istringstream iss(line);

	std::string mode;
	iss >> mode;
	for (auto const& break_mode : break_modes_)
	{
		if (mode == break_mode)
		{
			return false;
		}
	}

	std::string arg;
	std::vector<std::string> arguments;
	while (iss >> arg)
	{
		arguments.push_back(arg);
	}

	manager_.run_from_mode(mode, arguments);

	return true;
}
