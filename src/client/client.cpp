#include "io-multiplexing/client/client.h"
#include "client_impl.h"
#include <memory>

Client::Client(ProcessResponseFunction &cb) {
  _impl = std::make_shared<ClientImpl>(cb);
}
Client::~Client() {}

bool Client::connect(const ServerConfig& server_conf) {
    return _impl->connect(server_conf);
}
bool Client::send_request(const Buffer& request, Buffer& response) {
    return _impl->send_request(request, response);
}
bool Client::disconnect() {
    return _impl->disconnect();
}
