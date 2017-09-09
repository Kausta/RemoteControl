// Command.cpp
// Created by Caner Korkmaz on 9/9/2017.
// Copyright 2017 Caner Korkmaz
//

#include <vector>
#include <memory>
#include <iostream>
#include <sstream>
#include "Server.h"

CommandBase::CommandBase(const std::vector<std::string> &arguments)
    : arguments_(arguments) {
}

MoveCommand::MoveCommand(const std::vector<std::string> &arguments)
    : CommandBase(arguments) {
  if (arguments.size() < 2) {
    throw std::runtime_error("Not enough arguments for move command.");
  }

  auto x = std::stol(arguments[0]),
      y = std::stol(arguments[1]);
  point_ = {x, y};
}

void MoveCommand::execute() const {
  mouse_control::move_to(point_);
}

MoveDeltaCommand::MoveDeltaCommand(const std::vector<std::string> &arguments)
    : MoveCommand(arguments) {
}

void MoveDeltaCommand::execute() const {
  mouse_control::move_to_relative(point_);
}

MouseClickCommand::MouseClickCommand(const std::vector<std::string> &arguments)
    : CommandBase(arguments) {
  type = mouse_control::click_type::Left;
  click_time_ms = 100ul;

  if (arguments.empty()) {
    return;
  }
  auto type_opt = try_parse_type(arguments[0]);
  if (type_opt.has_value()) {
    type = type_opt.value();
    if (arguments.size() >= 2) {
      auto time_opt = try_parse_time(arguments[1]);
      if (time_opt.has_value()) {
        click_time_ms = time_opt.value();
      }
    }
  } else {
    auto time_opt = try_parse_time(arguments[0]);
    if (time_opt.has_value()) {
      click_time_ms = time_opt.value();
      if (arguments.size() >= 2) {
        type_opt = try_parse_type(arguments[1]);
        if (type_opt.has_value()) {
          type = type_opt.value();
        }
      }
    }
  }
}

void MouseClickCommand::execute() const {
  mouse_control::click(type, click_time_ms);
}

std::optional<mouse_control::click_type> MouseClickCommand::try_parse_type(const std::string &arg) {
  if (arg=="-l" || arg=="--left") {
    return mouse_control::click_type::Left;
  }
  if (arg=="-r" || arg=="--right") {
    return mouse_control::click_type::Right;
  }
  if (arg=="-m" || arg=="--middle") {
    return mouse_control::click_type::Middle;
  }
  return {};
}

std::optional<unsigned long> MouseClickCommand::try_parse_time(const std::string &arg) {
  try {
    return std::stoul(arg);
  }
  catch (...) {
    return {};
  }
}

NetworkCommand::NetworkCommand(const std::vector<std::string> &arguments)
    : CommandBase(arguments) {
  if (arguments.empty()) {
    throw std::runtime_error("Port arguments not specified for network mode.");
  }
  port_ = static_cast<unsigned short>(std::stoul(arguments_[0]));
}

void NetworkCommand::execute() const {
  boost::asio::io_service io_service;
  network::Server server(io_service, port_);
  io_service.run();
}

MultipleInputCommand::MultipleInputCommand(const std::vector<std::string> &arguments)
    : CommandBase(arguments) {
}

void MultipleInputCommand::execute() const {
  auto &manager = CommandManager::instance();
  std::vector<std::string> break_modes{"exit", "-e", "--exit"};

  CommandReader reader(manager, break_modes);
  std::cout << "Input exit\\-e\\--exit to exit.\n";
  while (true) {
    try {
      std::string input;
      std::getline(std::cin, input);

      auto executed = reader.execute_line(input);
      if (!executed) {
        // User entered a break mode
        break;
      }
    }
    catch (const std::runtime_error &error) {
      std::cout << "Error: " << error.what() << "\n";
    }
  }
}

HelpCommand::HelpCommand(const std::vector<std::string> &arguments)
    : CommandBase(arguments) {
}

void HelpCommand::execute() const {
  std::cout << CommandManager::instance().help_message() << '\n';
}

VersionCommand::VersionCommand(const std::vector<std::string> &arguments)
    : CommandBase(arguments) {
}

void VersionCommand::execute() const {
  std::cout << "Remote Control Version 0.1\n";
  std::cout << "https://github.com/Kausta/RemoteControl\n";
}


std::unique_ptr<CommandBase> make_command(CommandType type, const std::vector<std::string> &arguments) {
  switch (type) {
    case CommandType::Move: return make_command<MoveCommand>(arguments);
    case CommandType::MoveDelta: return make_command<MoveDeltaCommand>(arguments);
    case CommandType::MouseClick: return make_command<MouseClickCommand>(arguments);
    case CommandType::SecureNetwork:
      [[fallthrough]];
    case CommandType::Network: return make_command<NetworkCommand>(arguments);
    case CommandType::MultipleInput: return make_command<MultipleInputCommand>(arguments);
    case CommandType::Help: return make_command<HelpCommand>(arguments);
    case CommandType::Version: return make_command<VersionCommand>(arguments);
    case CommandType::Unknown: throw std::runtime_error("Unknown mode argument entered!");
  }
  throw std::logic_error("Implemented all command types, unknown error occured.");
}
