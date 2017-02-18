/*
* File: MouseControl.cpp
* Copyright: 2017 Caner Korkmaz (info@canerkorkmaz.com)
* Description:
*/

#include <iostream>

#include "IncludeCore.h"
#include "WinUtility.h"

#include "MouseControl.h"


using mouse_control::point_type;

point_type mouse_control::get_pos()
{
	point_type pos;
	if(!GetCursorPos(&pos))
	{
		throw get_cursor_pos_exception("GetCursorPos failed: " + GetLastErrorAsString() + "!");
	}
	return pos;
}

point_type mouse_control::screen_to_absolute(const point_type& point)
{
#ifdef RC_WINDOWS
	static auto screenX = ::GetSystemMetrics(SM_CXVIRTUALSCREEN);
	static auto screenY = ::GetSystemMetrics(SM_CYVIRTUALSCREEN);

	point_type newP;
	newP.x = static_cast<long>(std::round(65536.0 / screenX * point.x));
	newP.y = static_cast<long>(std::round(65536.0 / screenY * point.y));

	return newP;
#endif
}

#ifdef RC_WINDOWS
void mouse_control::send_mouse_input(const point_type& point, DWORD flags, DWORD mouse_data, DWORD time, ULONG_PTR extra_info)
{
	INPUT input;
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

void mouse_control::move_to(const point_type& point)
{
#ifdef RC_WINDOWS
	mouse_control::send_mouse_input(screen_to_absolute(point), MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE);
#endif
}

void mouse_control::move_to_relative(const point_type& point)
{
	// Relative move by itself is very inaccurate so even if we are moving with delta it is better to use absolute and add current coords manually
	// However, if get cursor pos fails, than it is better to try send inputs default relative movement
	// https://msdn.microsoft.com/en-us/library/windows/desktop/ms646273(v=vs.85).aspx
	// Pointer speed and threshold settings affect relative movement
	try
	{
		auto cursorPos = mouse_control::get_pos();
		cursorPos.x += point.x;
		cursorPos.y += point.y;
#ifdef RC_WINDOWS
		mouse_control::send_mouse_input(screen_to_absolute(cursorPos), MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE);
#endif
	}
	catch(const get_cursor_pos_exception& ex)
	{
		std::cout << ex.what() << '\n';
		std::cout << "Using default relative movement of SendInput\n";
#ifdef RC_WINDOWS
		mouse_control::send_mouse_input(point, MOUSEEVENTF_MOVE);
#endif
	}


	
}
