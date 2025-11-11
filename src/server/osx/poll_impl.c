#include <sys/event.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#include "io-multiplexing/server/server.h"
#include "server/poll.h"
#include "shared/logging.h"
#include "shared/utils.h"

Poll *poll_create(Server *server) {
  Poll *poll = (Poll *)malloc(sizeof(Poll));
  poll->server = server;
  return poll;
}

bool poll_init(Poll *poll, int server_fd) {
  poll->_fd = kqueue();
  if (poll->_fd == 0) {
    LOG_INFO("kqueue failed: %s", strerror(errno));
    return false;
  }

  struct kevent ev;
  EV_SET(&ev, server_fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
  if (kevent(poll->_fd, &ev, 1, NULL, 0, NULL) == -1) {
    LOG_INFO("kevent: listen_sock failed: %s", strerror(errno));
    return false;
  }
  return true;
}

void poll_destroy(Poll* poll){
    // TBD
}


bool poll_start(Poll *poll) {
  struct kevent events[MAX_EVENTS];
  while (!poll->_is_stopped) {
    int nfds = kevent(poll->_fd, NULL, 0, events, MAX_EVENTS, NULL);
    if (nfds == 0) {
      LOG_INFO("kevent failed: %d", errno);
      break;
    }

    for (int n = 0; n < nfds; ++n) {
      int fd = (int)events[n].ident;

      if (poll->_conn_io_cb) {
        poll->_conn_io_cb(poll->server, fd);
      }
    }
  }
  return true;
}

bool poll_stop(Poll* poll){
if (poll->_fd >= 0) {
    close(poll->_fd);
    poll->_fd = -1;
  }
  poll->_is_stopped = true;
  return true;
}

void poll_add_connection(Poll* poll, int fd){
 struct kevent ev;
  EV_SET(&ev, fd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);
  if (kevent(poll->_fd, &ev, 1, NULL, 0, NULL) == -1) {
    LOG_INFO("kevent: conn_sock failed: %d", errno);
    exit(EXIT_FAILURE);
  }
}

void poll_remove_connection(Poll* poll, int fd){
    // TASK
}
void poll_set_connection_io_callback(Poll* poll, ConnectionIOCallback cb){
    poll->_conn_io_cb = cb;
}

