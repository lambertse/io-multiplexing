#include <unistd.h>

#include <assert.h>

#include "connection_manager.h"
#include "internal_define.h"
#include "io-multiplexing/server/server.h"
#include "poll.h"
#include "shared/logging.h"
#include "shared/rb_tree.h"
#include "shared/utils.h"
#include <netinet/ip.h>

struct Server {
  int _fd;
  ServerConfig _config;
  struct sockaddr_in _addr;
  RBTree *_fd_2_conn;
  Poll *_poll;
  ConnectionManager *_connection_manager;
};

static void accept_new_connection(Server *server) {
  socklen_t addrlen = sizeof(server->_addr);
  int connFD = accept(server->_fd, (struct sockaddr *)&server->_addr, &addrlen);
  if (connFD == -1) {
    LOG_INFO("accept failed: %d", errno);
    exit(EXIT_FAILURE);
  }
  set_fb_nonblocking(connFD);
  poll_add_connection(server->_poll, connFD);

  LOG_DEBUG("accept_new_conn with fd %d", connFD);
  Connection *newConn = create_connection(connFD);
  newConn->state = REQUEST;

  rbtree_insert(server->_fd_2_conn, connFD, newConn);
}

void handle_connection_io(Server *server, int fd) {
  Connection *conn = NULL;
  if ((conn = rbtree_get(server->_fd_2_conn, fd)) && !conn) {
    LOG_ERROR("Connection not found for fd: %d", fd);
    return;
  }

  if (conn->state == END) {
    LOG_DEBUG("Connection ended for fd: %d", fd);
    close(fd);
    rbtree_delete(server->_fd_2_conn, fd);
    return;
  }
  cm_handle_connection_io(server->_connection_manager, conn);
}

static void add_connection(void *server_void, int fd) {
  Server *server = (Server *)server_void;
  if (fd == server->_fd) {
    LOG_DEBUG("Accept a connection");
    accept_new_connection(server);
  } else {
    LOG_DEBUG("Handle a connection");
    handle_connection_io(server, fd);
  }
}

static bool setup_socket(Server *server) {
  server->_fd = socket(AF_INET, SOCK_STREAM, 0);
  int val = false;
  setsockopt(server->_fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
  if (server->_fd < 0) {
    LOG_DEBUG("fd is not valid ");
    return false;
  }
  set_fb_nonblocking(server->_fd);

  server->_addr.sin_family = AF_INET;
  server->_addr.sin_port = htons(server->_config.port);
  server->_addr.sin_addr.s_addr = htonl(0);

  if (bind(server->_fd, (struct sockaddr *)&server->_addr,
           sizeof(server->_addr)) < 0) {
    LOG_DEBUG("bind failed");
    return false;
  }
  if (listen(server->_fd, 100) < 0) {
    LOG_DEBUG("listen failed");
    return false;
  }
  return true;
}

/* Constructor/Destructor equivalents */
Server *server_create(ServerConfig config) {
  Server *server = (Server *)malloc(sizeof(Server));
  server->_fd = -1;
  server->_config = config;
  server->_poll = poll_create(server);
  server->_connection_manager =
      (ConnectionManager *)malloc(sizeof(ConnectionManager));
  server->_fd_2_conn = rbtree_create(sizeof(Connection));
  return server;
}
void server_destroy(Server *server) {
  if (!server)
    return;
  close(server->_fd);
  if (server->_poll) {
    poll_destroy(server->_poll);
    free(server->_poll);
  }
  // TBD
}

/* Member function equivalents */
bool server_init(Server *server, ProcessRequestFunction cb) {
  if (!setup_socket(server)) {
    LOG_INFO("Failed to setup socket");
    return false;
  }
  cm_register_processreq_funct(server->_connection_manager, cb);

  LOG_INFO("Server is listening on port %d", server->_config.port);
  poll_set_connection_io_callback(server->_poll, add_connection);
  return poll_init(server->_poll, server->_fd);
}

bool server_start(Server *server) {
  if (server->_poll && !poll_start(server->_poll)) {
    LOG_INFO("Failed to start the poll");
    return false;
  }
  return true;
}

bool server_stop(Server *server) {
  if (server->_poll) {
    poll_stop(server->_poll);
  }
  
  // TODO
  // Consider better close fd mechanism
  RBTreeIterator *iter = rbtree_iterator_create();
  rbtree_iterator_init(iter, server->_fd_2_conn->root);

  while (rbtree_iterator_has_next(iter)) {
    RBNode *node = rbtree_iterator_next(iter);

    int fd = node->key;
    void *conn = node->value;

    if (conn) {
      close(fd);
    }
  }
  rbtree_iterator_destroy(iter);
  rbtree_destroy(server->_fd_2_conn);
  //

  if (server->_fd >= 0) {
    close(server->_fd);
    server->_fd = -1;
  }
  return true;
}

bool server_deinit(Server *server) { return true; }
