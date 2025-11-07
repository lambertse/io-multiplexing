#include <iostream>

#include "io-multiplexing/server/server.h"
#include "io-multiplexing/define.h"

int main(){
    ServerConfig server_conf{.port = 8091};
    Server server(server_conf);
    ProcessRequestFunction cb = [](const Buffer& request) -> Buffer {
        std::cout << "Request: " << request << std::endl;
        return "Hello, I'm responsing for \"" + request + "\"\n";
    };
    if(!server.init(cb)){
        std::cout << "Init failed\n";
        return 0;
    }
    server.start();
    return 1;
}
