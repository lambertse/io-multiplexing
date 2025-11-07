#pragma once

#include <memory>

#include "io-multiplexing/define.h"
#include "io-multiplexing/server/config.h"

class Client {
 public:
  Client();
  ~Client();

  bool connect(const ServerConfig& server_conf);
  bool send_request(const Buffer &request, Buffer& response);
  bool disconnect();

 private:
  std::shared_ptr<class ClientImpl> _impl;
};

