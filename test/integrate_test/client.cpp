#include <cstdlib>
#include <iostream>
#include <thread>

#include "io-multiplexing/client/client.h"
#include "io-multiplexing/define.h"
#include "io-multiplexing/server/config.h"

int main() {
  ServerConfig server_conf{.port = 8091};

  std::vector<std::thread> threads;
  // Random vector string of 20 random string:
  std::vector<std::string> random_strings = {"Hello, World!",
                                             "Protobuf is great!",
                                             "Multithreading in C++",
                                             "Network programming",
                                             "Socket communication",
                                             "Concurrency",
                                             "Thread safety",
                                             "C++ best practices",
                                             "Error handling",
                                             "Performance optimization",
                                             "Data serialization",
                                             "Protocol buffers",
                                             "Asynchronous programming",
                                             "Event-driven architecture",
                                             "Client-server model",
                                             "Network protocols",
                                             "TCP/IP stack",
                                             "Socket programming in C++",
                                             "C++ networking libraries",
                                             "Cross-platform development"};
  const int numClients = random_strings.size();
  for (int i = 0; i < numClients; i++) {
    threads
        .emplace_back([&]() {
          Client client;
          client.connect(server_conf);
          Buffer response;
          client.send_request(random_strings[i], response);
          std::cout << "Got response: " << response << std::endl;
        })
        .join();
  }
  for (auto &t : threads) {
    if (t.joinable()) {
      t.join();
    }
  }
}
