#pragma once

#include "io-multiplexing/server/server.h"

enum ClientStatus { NOT_READY, READY, SENDING, RECEIVING };

class ClientImpl {
public:
  ClientImpl(ProcessResponseFunction &cb);
  ~ClientImpl();

  bool connect(const ServerConfig &server_conf);
  bool send_request(const Buffer &request, Buffer& response);
  bool disconnect();

private:
  bool read_response(char *response);
  bool send_request_internal(const Buffer &buffer);

private:
  ProcessResponseFunction _procres_cb;
  Server _server;
  int _fd;
  ClientStatus _status;
};
