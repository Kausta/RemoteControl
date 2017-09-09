// Program.cpp
// Created by Caner Korkmaz on 9/9/2017.
// Copyright 2017 Caner Korkmaz
//

#include <iostream>
#include "CommandManager.h"
#include "Program.h"

Program::Program(int argc, char **argv)
    : arguments(argv, argv+argc) {
  if(argv == nullptr || argc <= 0){
    throw std::logic_error("No arguments is not possible!");
  }
  CommandManager::instance().set_program_name(arguments[0]);
}

void Program::run() {
  if (arguments.size() < 2) {
    throw std::runtime_error("Mode not specified.");
  }
  auto mode = arguments[1];
  std::vector<std::string> mode_arguments{arguments.begin() + 2, arguments.end()};
  CommandManager::instance().run_from_mode(mode, mode_arguments);
}

