/*
* File: CommandManager.cpp
* Copyright: 2017 Caner Korkmaz (info@canerkorkmaz.com)
* Description:
*/

#include <sstream>

#include "CommandManager.h"

void CommandManager::init_modes_() {
  section movement("Movement");
  movement.commands.emplace_back(CommandType::Move, "Moves cursor to coords", "-m", "--move");
  movement.commands.emplace_back(CommandType::MoveDelta,
                                 "Moves cursor to current coords + coords", "-md", "--move-delta");
  add_section(movement);

  std::stringstream click_help;
  click_help << "\tOptional arguments : \n"
             << "\t[button] =\n\t  -l\\--left: left click\n\t  -r\\--right: right click\n\t  -m\\--middle: middle click \n"
             << "\t[click_time] = click time in milliseconds";

  section click("Click");
  click.commands.emplace_back(CommandType::MouseClick, "Mouse click", "-mc", "--mouse-click", click_help.str());
  add_section(click);

  section network("Network");
  network.commands.emplace_back(CommandType::Network,
                                "Opens Connection at ip, port and gets movement modes from there",
                                "-n",
                                "--network");
  network.commands.emplace_back(CommandType::SecureNetwork,
                                "Network mode with ssl/tls, look to wiki for setting up authentication",
                                "-sn",
                                "--secure-network");
  add_utility_section(network);

  section other("Other");
  other.commands.emplace_back(CommandType::MultipleInput,
                              "Allows multiple commands from stdin, seperated with newlines.",
                              "-mi",
                              "--multi-input");
  other.commands.emplace_back(CommandType::Help,
                              "Shows this help message.",
                              "-h",
                              "--help");
  other.commands.emplace_back(CommandType::Version,
                              "Shows version info.",
                              "-v",
                              "--version");
  add_utility_section(other);
}

std::string CommandManager::generate_help_message_() const {
  std::stringstream ss;
  ss << "Remote Control Utility\n\n"
     << "Usage: " << program_name_ << " movement-mode x-pos y-pos\n"
     << "Usage: " << program_name_ << " click-mode [button] [click_time]\n"
     << "Usage: " << program_name_ << " network-mode port\n"
     << "Usage: " << program_name_ << " other-mode\n"
     << "=====================\n";
  insert_help_for_section_type(ss, sections_);
  insert_help_for_section_type(ss, utility_sections_);
  return ss.str();
}

void CommandManager::insert_help_for_section_type(std::stringstream &ss, const std::vector<section> &section_holder_) {
  for (auto const &section : section_holder_) {
    ss << section.name << " Modes: \n";
    for (auto const &command : section.commands) {
      ss << "  " << command.short_arg << " ," << command.long_arg << ":\n";
      ss << "\t" << command.info << "\n";
      if (command.extra_description.has_value()) {
        ss << command.extra_description.value() << "\n";
      }
    }
  }
}

CommandManager::command_detail::command_detail(CommandType type,
                                               std::string info,
                                               std::string short_arg,
                                               std::string long_arg)
    : type(type),
      info(std::move(info)),
      short_arg(std::move(short_arg)),
      long_arg(std::move(long_arg)),
      extra_description() {
}

CommandManager::command_detail::command_detail(CommandType type,
                                               std::string info,
                                               std::string short_arg,
                                               std::string long_arg,
                                               std::string extra_description)
    : command_detail(type, std::move(info), std::move(short_arg), std::move(long_arg)) {
  this->extra_description = std::move(extra_description);
}

CommandManager::section::section(std::string name)
    : name(std::move(name)) {
}

CommandManager::CommandManager()
    : program_name_("RemoteControl")
    , utility_modes_enabled_(true) {
  init_modes_();
  help_message_ = generate_help_message_();
}

void CommandManager::run_from_mode(const std::string &mode, const std::vector<std::string> &arguments) const {
  auto command_type = get_mode(mode);
  auto command = make_command(command_type, arguments);
  command->execute();
}

CommandType CommandManager::get_mode(std::string mode_arg) const {
  auto it = modes_.find(mode_arg);
  if (it!=modes_.end()) {
    return it->second;
  }
  if (utility_modes_enabled()) {
    auto it_u = utility_modes_.find(mode_arg);
    if (it_u!=utility_modes_.end()) {
      return it_u->second;
    }
  }
  throw std::runtime_error("Unknown mode!");
}

const std::string &CommandManager::help_message() const {
  return help_message_;
}

void CommandManager::add_section(const section &section_) {
  add_section_to(section_, sections_, modes_);
}

void CommandManager::add_utility_section(const section &section_) {
  add_section_to(section_, utility_sections_, utility_modes_);
}

void CommandManager::add_section_to(const section &section_,
                                    std::vector<section> &section_holder_,
                                    std::map<std::string, CommandType> &mode_holder_) {
  section_holder_.push_back(section_);
  for (auto const &command : section_.commands) {
    mode_holder_.emplace(command.short_arg, command.type);
    mode_holder_.emplace(command.long_arg, command.type);
  }
}

