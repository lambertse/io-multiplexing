#pragma once

#include <memory>

#include "io-multiplexing/server/config.h"
#include "io-multiplexing/define.h"

class Server {
 public:
  Server(ServerConfig config = ServerConfig::default_config());
  ~Server();

  bool init(const ProcessRequestFunction& cb);
  bool start();
  bool stop();
  bool deinit();

  ServerConfig config() const;

 private:
  std::shared_ptr<class ServerImpl> _impl;
};

