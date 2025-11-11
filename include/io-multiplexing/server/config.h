#ifndef IO_MUX_CONFIG_H__
#define IO_MUX_CONFIG_H__

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  int port;
  int max_connections;
  int max_message_size;
} ServerConfig;

static inline ServerConfig server_config_default() {
  static ServerConfig config = {8100, 100, 4096};
  
  if (config.port == 0) {
    config.port = 8100;
    config.max_connections = 100;
    config.max_message_size = 4096;
  }
  
  return config;
}

#ifdef __cplusplus
}
#endif

#endif
