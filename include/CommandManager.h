/*
* File: CommandManager.h
* Copyright: 2017 Caner Korkmaz (info@canerkorkmaz.com)
* Description:
*/

#pragma once

#include <map>

#include "Command.h"

class CommandManager {
 public:
  struct command_detail {
    CommandType type;
    std::string info;
    std::string short_arg, long_arg;
    std::optional<std::string> extra_description;

    explicit command_detail(CommandType type,
                            std::string info,
                            std::string short_arg,
                            std::string long_arg);
    explicit command_detail(CommandType type,
                            std::string info,
                            std::string short_arg,
                            std::string long_arg,
                            std::string extra_description);
  };
  struct section {
    std::string name;
    std::vector<command_detail> commands;

    explicit section(std::string name);
  };
 public:
  static CommandManager &instance() {
    static CommandManager command_manager;
    return command_manager;
  }

  void run_from_mode(const std::string &mode, const std::vector<std::string> &args) const;

  bool utility_modes_enabled() const { return utility_modes_enabled_; }
  void utility_modes_enabled(bool value) { utility_modes_enabled_ = value; }

  const std::string &program_name() const { return program_name_; }
  const std::string &help_message() const;

  void set_program_name(std::string name) {
    program_name_ = std::move(name);
    generate_help_message_();
  }
 private:
  CommandManager();

  void init_modes_();

  void add_section(const section &section_);
  void add_utility_section(const section &section_);
  static void add_section_to(const section &section_,
                             std::vector<section> &section_holder_,
                             std::map<std::string, CommandType> &mode_holder_);

  std::string generate_help_message_() const;
  static void insert_help_for_section_type(std::stringstream &ss, const std::vector<section> &section_holder_);

  CommandType get_mode(std::string mode_arg) const;

  std::string program_name_;

  bool utility_modes_enabled_;

  std::vector<section> sections_;
  std::vector<section> utility_sections_;

  std::map<std::string, CommandType> modes_;
  std::map<std::string, CommandType> utility_modes_;

  std::string help_message_;
};
