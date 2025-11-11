#ifndef IO_MUX_POLL_H__
#define IO_MUX_POLL_H__

#include <stdbool.h>

typedef void (*ConnectionIOCallback)(void *server, int fd);
typedef struct Server Server;

#define MAX_EVENTS 10

typedef struct {
  ConnectionIOCallback _conn_io_cb;
  int _fd;
  bool _is_stopped;
  Server *server;
} Poll;

Poll *poll_create(Server *server);
void poll_destroy(Poll *poll);
bool poll_init(Poll *poll, int fd);
bool poll_start(Poll *poll);
bool poll_stop(Poll *poll);
void poll_add_connection(Poll *poll, int fd);
void poll_remove_connection(Poll *poll, int fd);
void poll_set_connection_io_callback(Poll *poll, ConnectionIOCallback cb);

#endif
