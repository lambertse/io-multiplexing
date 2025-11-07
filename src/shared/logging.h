#pragma once
#include <iostream>
#include <string>

namespace logging {
enum LogLevel {
  DEBUG = 1 << 0,  // 0b0001 = 1
  INFO = 1 << 1,   // 0b0010 = 2
};

static int log_level = /* DEBUG | */ INFO;  // Use OR to enable both

static void debug(const std::string &str) {
  if (!(log_level & DEBUG)) {  // Use AND to check if bit is set
    return;
  }
  std::cout << "DEBUG: " << str << std::endl;
}

static void info(const std::string &str) {
  if (!(log_level & INFO)) {  // Use AND to check if bit is set
    return;
  }
  std::cout << "INFO: " << str << std::endl;
}

}  // namespace logging

#define LOG_DEBUG(str) logging::debug(str)
#define LOG_INFO(str) logging::info(str)
