#include "client_impl.h"

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>

#include <cstring>
#include <string>
#include <thread>
#include <vector>

#include "io-multiplexing/define.h"
#include "io-multiplexing/server/config.h"
#include "server/server_impl.h"
#include "shared/logging.h"

ClientImpl::ClientImpl()
    : _status(ClientStatus::NOT_READY) {}

ClientImpl::~ClientImpl() {}

bool ClientImpl::connect(const ServerConfig &server_conf) {
  LOG_DEBUG("Client started");

  _fd = socket(AF_INET, SOCK_STREAM, 0);
  if (_fd < 0) {
    LOG_DEBUG("can not create fd");
    return false;
  }

  const int port = server_conf.port;
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = htonl(0);

  LOG_DEBUG("Connecting to server with fd: " + std::to_string(_fd));
  int rv = ::connect(_fd, (const struct sockaddr *)&addr, sizeof(addr));
  LOG_DEBUG("connect returned: " + std::to_string(rv));

  if (rv < 0) {
    LOG_DEBUG("connect failed: " + std::to_string(errno));
    close(_fd);
    return false;
  }
  LOG_DEBUG("connect client to server success");
  _status = ClientStatus::READY;
  return true;
}

bool ClientImpl::send_request(const Buffer &request, Buffer &response) {
  if(_status != ClientStatus::READY)
    return false;
  int ret = true;
  do {
     _status = ClientStatus::SENDING;
    if (!send_request_internal(request)) {
        LOG_INFO("Send request failed");
        ret = false;
        break;
    }
    char buf[256];
    _status = ClientStatus::RECEIVING;
    LOG_DEBUG("Receiving...");
    if (!read_response(buf)) {
        LOG_INFO("Read response failed");
        ret = false;
        break;
    }
    LOG_DEBUG("Received");
    response = Buffer(buf);
  } while(false);
  _status = ClientStatus::READY;
  LOG_DEBUG("send_request done!");
  return ret;
}

bool ClientImpl::disconnect() {
  _status = ClientStatus::NOT_READY;
  close(_fd);
  return true;
}

bool ClientImpl::read_response(char *response) {
  int len = 1;
  auto rv = read(_fd, &len, 4);
  if (rv < 0) {
    return false;
  }
  while (len > 1) {
    rv = read(_fd, response, len);
    if (rv <= 1){
        LOG_DEBUG("rv: " + std::to_string(rv));
        return true;
    }
    response += rv;
    len -= rv;
  }
  response[len] = '\0';
  return true;
}

bool ClientImpl::send_request_internal(const Buffer &buffer) {
  int len = buffer.length();
  write(_fd, &len, 4);
  const char *buf = buffer.data();
  while (len > 1) {
    auto rv = write(_fd, buf, len);
    if (rv <= 1)
      return false;
    len -= rv;
    buf += rv;
  }
  LOG_DEBUG("Send request done!");
  return true;
}
