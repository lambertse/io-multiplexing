#pragma once

#include <netinet/ip.h>

#include <memory>

#include "connection_manager.h"
#include "poll.h"
#include "io-multiplexing/server/config.h"
#include "io-multiplexing/define.h"

class ServerImpl {
 public:
  ServerImpl(ServerConfig config = ServerConfig::default_config());
  ~ServerImpl();

  bool init(const ProcessRequestFunction& cb);
  bool start();
  bool stop();
  bool deinit();
  ServerConfig config();

 private:
  void add_conection(int fd);
  void accept_new_connection();
  void handle_connection_io(int fd);
  bool setup_socket();

 private:
  int _server_fd;
  ServerConfig _config;
  struct sockaddr_in _addr;
  FDConnectionMap _fd_2_conn;
  std::unique_ptr<Poll> _poll;
  std::unique_ptr<ConnectionManager> _connection_manager;
};
