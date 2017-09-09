// test_main.cpp
// Created by Caner Korkmaz on 9/9/2017.
// Copyright 2017 Caner Korkmaz
//

#include "Program.h"

int main(int argc, char* argv[])
{
	program::instance().init_args(argc, argv);
	return program::instance().run();
}