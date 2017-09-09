// MouseControl.cpp
// Created by Caner Korkmaz on 9/9/2017.
// Copyright 2017 Caner Korkmaz
//

#include <iostream>
#include <cmath>
#include "IncludeCore.h"

#include "MouseControl.h"

using mouse_control::point_type;

point_type mouse_control::get_pos() {
  point_type pos{0, 0};
#if RC_WINDOWS
  if (!GetCursorPos(&pos)) {
    throw get_cursor_pos_exception("GetCursorPos failed: " + GetLastErrorAsString() + "!");
  }
#endif
  return pos;
}

point_type mouse_control::screen_to_absolute(const point_type &point) {
  point_type new_pos{point};
#ifdef RC_WINDOWS
  static auto screenX = ::GetSystemMetrics(SM_CXVIRTUALSCREEN);
  static auto screenY = ::GetSystemMetrics(SM_CYVIRTUALSCREEN);

  new_pos.x = static_cast<long>(std::round(65536.0/screenX*point.x));
  new_pos.y = static_cast<long>(std::round(65536.0/screenY*point.y));
#endif
  return new_pos;
}

#ifdef RC_WINDOWS
void mouse_control::send_mouse_input(const point_type &point, DWORD flags, DWORD mouse_data, DWORD time, ULONG_PTR extra_info) {
  INPUT input {};
  input.type = INPUT_MOUSE;
  input.mi.dx = point.x;
  input.mi.dy = point.y;
  input.mi.mouseData = mouse_data;
  input.mi.dwFlags = flags;
  input.mi.time = time;
  input.mi.dwExtraInfo = extra_info;

  ::SendInput(1, &input, sizeof(INPUT));
}
#endif

void mouse_control::move_to(const point_type &point) {
#ifdef RC_WINDOWS
  mouse_control::send_mouse_input(screen_to_absolute(point), MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE);
#else
  screen_to_absolute(point); // Stop unused
#endif
}

void mouse_control::move_to_relative(const point_type &point) {
  // Relative move by itself is very inaccurate so even if we are moving with delta it is better to use absolute and add current coords manually
  // However, if get cursor pos fails, than it is better to try send inputs default relative movement
  // https://msdn.microsoft.com/en-us/library/windows/desktop/ms646273(v=vs.85).aspx
  // Pointer speed and threshold settings affect relative movement
  try {
    auto cursorPos = mouse_control::get_pos();
    cursorPos.x += point.x;
    cursorPos.y += point.y;
#ifdef RC_WINDOWS
    mouse_control::send_mouse_input(screen_to_absolute(cursorPos), MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE);
#endif
  }
  catch (const get_cursor_pos_exception &ex) {
    std::cout << ex.what() << '\n';
    std::cout << "Using default relative movement of SendInput\n";
#ifdef RC_WINDOWS
    mouse_control::send_mouse_input(point, MOUSEEVENTF_MOVE);
#endif
  }

}

void mouse_control::click(click_type click, unsigned long click_time_ms) {
#ifdef RC_WINDOWS
  DWORD down_flag, up_flag;
  switch (click) {
    case click_type::Left:
      down_flag = MOUSEEVENTF_LEFTDOWN;
      up_flag = MOUSEEVENTF_LEFTUP;
      break;
    case click_type::Right:
      down_flag = MOUSEEVENTF_RIGHTDOWN;
      up_flag = MOUSEEVENTF_RIGHTUP;
      break;
    case click_type::Middle:
      down_flag = MOUSEEVENTF_MIDDLEDOWN;
      up_flag = MOUSEEVENTF_MIDDLEDOWN;
      break;
  }

  send_mouse_input({0, 0}, down_flag);
  Sleep(click_time_ms);
  send_mouse_input({0, 0}, up_flag);
#else
  // Unused
  switch (click) {
    case click_type::Left:
      break;
    case click_type::Right:
      break;
    case click_type::Middle:
      break;
  }
  click_time_ms += 0;
#endif
}
