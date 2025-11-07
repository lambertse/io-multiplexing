#pragma once
#include "io-multiplexing/logging.h"
#include <iostream>
#include <string>

namespace logging {

static void debug(const std::string &str) {
  if (!(log_level & DEBUG)) { // Use AND to check if bit is set
    return;
  }
  if (write_log_funct) {
    write_log_funct(str);
  } else {
    std::cout << "DEBUG: " << str << std::endl;
  }
}

static void info(const std::string &str) {
  if (!(log_level & INFO)) { // Use AND to check if bit is set
    return;
  }
  if (write_log_funct) {
    write_log_funct(str);
  } else {
    std::cout << "INFO: " << str << std::endl;
  }
}

} // namespace logging

#define LOG_DEBUG(str) logging::debug(str)
#define LOG_INFO(str) logging::info(str)
