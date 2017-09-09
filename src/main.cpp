// test_main.cpp
// Created by Caner Korkmaz on 9/9/2017.
// Copyright 2017 Caner Korkmaz
//

#include <iostream>
#include "Program.h"

int main(int argc, char *argv[]) {
  try {
    Program program{argc, argv};
    program.run();

    return EXIT_SUCCESS;
  } catch (std::runtime_error const &err){
    std::cerr << "Error: " << err.what() << '\n';
  } catch (std::logic_error const &err){
    std::cerr << "Unexpected error: " << err.what() << '\n';
  }

  return EXIT_FAILURE;
}