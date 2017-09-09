// Program.h
// Created by Caner Korkmaz on 9/9/2017.
// Copyright 2017 Caner Korkmaz
//

#pragma once

#include <memory>
#include <vector>
#include "CommandManager.h"

class Program {
 public:
  Program(int argc, char* argv[]);

  void run();
 private:
  std::vector<std::string> arguments;
};
