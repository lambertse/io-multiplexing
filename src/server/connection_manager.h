#pragma once

#include "internal_define.h"

class ConnectionManager {
 public:
  explicit ConnectionManager();
  virtual ~ConnectionManager() = default;

  void remove_connection(int fd);
  void handle_connection_io(ConnectionSharedPtr conn);
  void register_processreq_funct(const ProcessRequestFunction& cb);

 private:
  bool try_one_request(ConnectionSharedPtr conn);
  void state_request(ConnectionSharedPtr conn);
  void state_response(ConnectionSharedPtr conn);

 private:
  ProcessRequestFunction _procrq_cb;
};
