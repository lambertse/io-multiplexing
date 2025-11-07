#pragma once

#include <map>
#include "io-multiplexing/define.h"

enum ConnectionState {
  REQUEST = 0,
  RESPONSE,
  END,
};

struct Connection {
  int fd;
  ConnectionState state;
  // Buffer for reading
  Buffer rbuf;
  // Buffer for writing
  size_t wbuf_sent = 0;
  Buffer wbuf;
};

using ConnectionSharedPtr = std::shared_ptr<Connection>;
using FDConnectionMap = std::map<int, ConnectionSharedPtr>;

constexpr int k_max_msg = 1024;
