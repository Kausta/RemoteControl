// WinUtility.cpp
// Created by Caner Korkmaz on 9/9/2017.
// Copyright 2017 Caner Korkmaz
//
#include <stdexcept>
#include "IncludeCore.h"

//http://stackoverflow.com/questions/1387064/how-to-get-the-error-message-from-the-error-code-returned-by-getlasterror
//Returns the last Win32 error, in string format. Returns an empty string if there is no error.
std::string GetLastErrorAsString() {
#if RC_WINDOWS
  //Get the error message, if any.
  auto errorMessageID = ::GetLastError();
  if (errorMessageID==0)
    return std::string(); //No error message has been recorded

  LPWSTR messageBuffer = nullptr;
  auto size = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                             nullptr,
                             errorMessageID,
                             MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                             reinterpret_cast<LPWSTR>(&messageBuffer),
                             0,
                             nullptr);
  if(!size){
    throw std::runtime_error("Failed to format the error message!");
  }
  auto size_needed = WideCharToMultiByte(CP_UTF8, 0, messageBuffer, static_cast<int>(size), nullptr, 0, nullptr, nullptr);
  std::string message(size_needed, 0);
  WideCharToMultiByte(CP_UTF8, 0, messageBuffer, static_cast<int>(size), message.data(), size_needed, nullptr, nullptr);

  //Free the buffer.
  LocalFree(messageBuffer);

  return message;
#else
  return std::string{};
#endif
}

