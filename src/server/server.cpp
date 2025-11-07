#pragma once

#include "io-multiplexing/server/server.h"
#include "server_impl.h"
#include <memory>

Server::Server(ServerConfig config) {
    _impl = std::make_shared<ServerImpl>(config);
}
Server::~Server() {}

bool Server::init(const ProcessRequestFunction &cb) {
    return _impl->init(cb);
}
bool Server::start() {
    return _impl->start();
}
bool Server::stop() {
    return _impl->stop();
}
bool Server::deinit() {
    return _impl->deinit();
}
ServerConfig Server::config() const {
    return _impl->config();
}
