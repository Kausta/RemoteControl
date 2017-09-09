// CommandReader.cpp
// Created by Caner Korkmaz on 9/9/2017.
// Copyright 2017 Caner Korkmaz
//

#include <sstream>
#include "CommandReader.h"

CommandReader::CommandReader(CommandManager &manager)
    : CommandReader(manager, {}) {
}

CommandReader::CommandReader(CommandManager &manager, const std::vector<std::string> &break_modes)
    : manager_(manager), break_modes_(break_modes) {
  init_();
}

bool CommandReader::execute_line(const std::string &line) const {
  std::istringstream iss(line);

  std::string mode;
  iss >> mode;
  for (auto const &break_mode : break_modes_) {
    if (mode==break_mode) {
      return false;
    }
  }

  std::string arg;
  std::vector<std::string> arguments;
  while (iss >> arg) {
    arguments.push_back(arg);
  }

  manager_.run_from_mode(mode, arguments);

  return true;
}

void CommandReader::init_() const {
  manager_.utility_modes_enabled(false);
}
