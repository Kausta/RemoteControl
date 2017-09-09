// MouseControl.h
// Created by Caner Korkmaz on 9/9/2017.
// Copyright 2017 Caner Korkmaz
//

#pragma once

#include "IncludeCore.h"

#include <stdexcept>

class get_cursor_pos_exception : public std::runtime_error {
 public:
  explicit get_cursor_pos_exception(const std::string &message)
      : runtime_error(message) {
  }

  explicit get_cursor_pos_exception(const char *message)
      : runtime_error(message) {
  }
};

namespace mouse_control {
#ifdef RC_WINDOWS
using point_type = POINT;
#else
struct point_type{
    long x, y;
};
#endif

point_type get_pos();
point_type screen_to_absolute(const point_type &point);

#ifdef RC_WINDOWS
void send_mouse_input(const point_type &point, DWORD flags, DWORD mouse_data = 0, DWORD time = 0, ULONG_PTR extra_info = 0);
#endif

void move_to(const point_type &point);
void move_to_relative(const point_type &point);

enum class click_type {
  Left,
  Right,
  Middle
};
void click(click_type click, unsigned long click_time_ms = 100ul);
}
