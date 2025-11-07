#pragma once

#include <functional>

enum LogLevel {
  DEBUG = 1 << 0,  // 0b0001 = 1
  INFO = 1 << 1,   // 0b0010 = 2
};
using WriteLogFunction = std::function<void(const std::string&)>;
static int log_level = /* DEBUG | */ INFO;  // Use OR to enable both
static WriteLogFunction write_log_funct = nullptr; 

static void register_write_log_funct(WriteLogFunction fnt){
    write_log_funct = std::move(fnt);
}
