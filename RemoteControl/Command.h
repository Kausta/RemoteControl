/*
* File: Command.h
* Copyright: 2017 Caner Korkmaz (info@canerkorkmaz.com)
* Description:
*/

#pragma once
#include <vector>
#include <memory>

#include "MouseControl.h"

enum class command_type
{
	Move,
	MoveDelta,
	Network,
	SecureNetwork,
	Unknown
};

class command_base
{
protected:
	std::vector<std::string> arguments_;

public:
	explicit command_base(const std::vector<std::string>& arguments);
	virtual ~command_base() = default;

	virtual void execute() const = 0;
	void operator()() const { execute(); }
};

class move_command : public command_base
{
protected:
	mouse_control::point_type point_;
public:
	explicit move_command(const std::vector<std::string>& arguments);

	void execute() const override;
};

class move_delta_command : public move_command
{
public:
	explicit move_delta_command(const std::vector<std::string>& arguments);

	void execute() const override;
};

class network_command : public command_base
{
private:
	unsigned short port_;
public:
	explicit network_command(const std::vector<std::string>& arguments);

	void execute() const override;
};

template<typename T> /* where T extends command_base */
std::enable_if_t<std::is_base_of<command_base, T>::value,
	std::unique_ptr<command_base>> make_command(const std::vector<std::string>& arguments)
{
	return std::make_unique<T>(arguments);
}

std::unique_ptr<command_base> make_command(command_type type, const std::vector<std::string>& arguments);