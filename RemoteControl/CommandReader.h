#pragma once
#include <string>
#include <sstream>

#include "CommandManager.h"
#include <vector>

class command_reader
{
public:

	/**
	 * \brief Constructs a command reader using the manager given and no break modes\n
	 * If constructed with this mode, execute_line will <b>always</b> return true
	 * \param manager Command Manager to execute read line
	 */
	explicit command_reader(command_manager& manager);
	/**
	 * \brief Constructs a command reader using given manager and break modes
	 * \param manager Command Manager to execute read line
	 * \param break_modes Break modes to allow user to break, useful when user is entering from stdin
	 */
	explicit command_reader(command_manager& manager, const std::vector<std::string>& break_modes);

	/**
	 * \brief Parses mode and arguments from line and executes them using the manager given
	 * \param line Line to parse mode and arguments from
	 * \return True if executed mode, false if a break mode entered
	 */
	bool execute_line(const std::string& line) const;
private:
	command_manager& manager_;
	std::vector<std::string> break_modes_;
};
