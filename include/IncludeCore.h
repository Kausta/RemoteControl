// IncludeCore.h
// Created by Caner Korkmaz on 9/9/2017.
// Copyright 2017 Caner Korkmaz
//

#pragma once

#ifdef _WIN32

#define RC_WINDOWS 1

#ifdef _WIN64
#define RC_WINDOWS_64 1
#else
#define RC_WINDOWS_32 1
#endif

#elif __linux__

#define RC_LINUX 1

#if __has_include("X11/X.h")
#define RC_LINUX_X11 1
#endif

#endif

#ifdef RC_WINDOWS

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif

#ifndef NOMINMAX
#define NOMINMAX 1
#endif

#include <windows.h>
#include "WinUtility.h"

#elif RC_LINUX

#if RC_LINUX_X11
#endif

#endif