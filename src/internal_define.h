#ifndef IO_MUX_INTERNAL_DEFINE_H__
#define IO_MUX_INTERNAL_DEFINE_H__

#include "io-multiplexing/define.h"
#include <unistd.h>
#include <stdlib.h>

typedef enum {
  REQUEST = 0,
  RESPONSE,
  END,
} ConnectionState;

#define K_MAX_MSG 1024
typedef struct {
  int fd;
  ConnectionState state;
  // char* for reading
  char rbuf[4 + K_MAX_MSG];
  size_t rbuf_size;
  // char* for writing
  size_t wbuf_sent;
  char wbuf[4 + K_MAX_MSG];
  size_t wbuf_size;
} Connection;

static inline Connection* create_connection(int fd) {
  Connection* conn = (Connection*)malloc(sizeof(Connection));
  if (conn == NULL) {
    return NULL;  // Handle allocation failure
  }
  conn->fd = fd;
  conn->state = REQUEST;
  conn->rbuf_size = 0;
  conn->wbuf_sent = 0;
  conn->wbuf_size = 0;
  return conn;
}

#endif
