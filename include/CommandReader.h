// CommandReader.h
// Created by Caner Korkmaz on 9/9/2017.
// Copyright 2017 Caner Korkmaz
//

#pragma once

#include <string>
#include <vector>
#include "CommandManager.h"

class CommandReader {
 public:

  /**
   * @brief Constructs a command reader using the manager given and no break modes\n
   * If constructed with this mode, execute_line will <b>always</b> return true
   * @param manager Command Manager to execute read line
   */
  explicit CommandReader(CommandManager &manager);
  /**
   * @brief Constructs a command reader using given manager and break modes
   * @param manager Command Manager to execute read line
   * @param break_modes Break modes to allow user to break, useful when user is entering from stdin
   */
  explicit CommandReader(CommandManager &manager, const std::vector<std::string> &break_modes);

  /**
   * @brief Parses mode and arguments from line and executes them using the manager given
   * @param line Line to parse mode and arguments from
   * @return True if executed mode, false if a break mode entered
   */
  bool execute_line(const std::string &line) const;
 private:
  void init_() const;

  CommandManager &manager_;
  std::vector<std::string> break_modes_;
};
