// Command.h
// Created by Caner Korkmaz on 9/9/2017.
// Copyright 2017 Caner Korkmaz
//

#pragma once

#include <vector>
#include <memory>
#include <optional>
#include "MouseControl.h"

enum class CommandType {
  Move,
  MoveDelta,
  MouseClick,
  Network,
  SecureNetwork,
  MultipleInput,
  Help,
  Version,
  Unknown
};

class CommandBase {
 protected:
  std::vector<std::string> arguments_;

 public:
  explicit CommandBase(const std::vector<std::string> &arguments);
  virtual ~CommandBase() = default;

  virtual void execute() const = 0;
  void operator()() const { execute(); }
};

class MoveCommand : public CommandBase {
 protected:
  mouse_control::point_type point_;
 public:
  explicit MoveCommand(const std::vector<std::string> &arguments);

  void execute() const override;
};

class MoveDeltaCommand : public MoveCommand {
 public:
  explicit MoveDeltaCommand(const std::vector<std::string> &arguments);

  void execute() const override;
};

class MouseClickCommand : public CommandBase {
 private:
  mouse_control::click_type type;

  unsigned long click_time_ms;

  static std::optional<mouse_control::click_type> try_parse_type(const std::string &arg);
  static std::optional<unsigned long> try_parse_time(const std::string &arg);
 public:
  explicit MouseClickCommand(const std::vector<std::string> &arguments);

  void execute() const override;
};

class NetworkCommand : public CommandBase {
 private:
  unsigned short port_;
 public:
  explicit NetworkCommand(const std::vector<std::string> &arguments);

  void execute() const override;
};

class MultipleInputCommand : public CommandBase {
 public:
  explicit MultipleInputCommand(const std::vector<std::string> &arguments);

  void execute() const override;
};

class HelpCommand : public CommandBase {
 public:
  explicit HelpCommand(const std::vector<std::string> &arguments);

  void execute() const override;
};


class VersionCommand : public CommandBase {
 public:
  explicit VersionCommand(const std::vector<std::string> &arguments);

  void execute() const override;
};

template<typename T>
/* where T extends CommandBase */
std::enable_if_t<std::is_base_of<CommandBase, T>::value,
                 std::unique_ptr<CommandBase>> make_command(const std::vector<std::string> &arguments) {
  return std::make_unique<T>(arguments);
}

std::unique_ptr<CommandBase> make_command(CommandType type, const std::vector<std::string> &arguments);