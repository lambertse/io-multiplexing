#include <assert.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>

#include "connection_manager.h"
#include "shared/logging.h"

static void state_response(Connection *conn) {
  LOG_DEBUG("state_response: %d", conn->fd);
  do {
    ssize_t rv = 0;
    do {
      ssize_t cap = conn->wbuf_size - conn->wbuf_sent;
      rv = write(conn->fd, &conn->wbuf[conn->wbuf_sent], cap);
    } while (rv < 0 && errno == EINTR);

    if (rv < 0) {
      if (errno == EAGAIN) {
        // Got EAGAIN, stop it
        break;
      } else {
        LOG_DEBUG("write() error: %d", errno);
        conn->state = END;
        break;
      }
    }
    conn->wbuf_sent += (size_t)rv;
    if (conn->wbuf_sent == conn->wbuf_size) {
      conn->state = END;
      conn->wbuf_sent = 0;
      break;
    }
    // Still got some data in wbuf, could try to continue writing
  } while (true);
}

static bool try_one_request(ConnectionManager *cm, Connection *conn) {
  if (conn->rbuf_size < 4) {
    // Not enough data in the buffer, will retry in the next iterator
    return false;
  }
  u_int32_t len = 0;
  memcpy(&len, &conn->rbuf[0], 4);
  LOG_DEBUG("Request length: %d", len);
  if (len > K_MAX_MSG) {
    LOG_DEBUG("Too long");
    conn->state = END;
    return false;
  }
  if (conn->rbuf_size < 4 + len) {
    // Not enough data in the buffer
    return false;
  }

  size_t res_size = 0;
  // char* req_str = (char*)malloc(len);
  // strcpy(req_str, &conn->rbuf[4]);
  char *res_str = cm->_procreq_cb(&conn->rbuf[4], len, &res_size);

  if (res_size > K_MAX_MSG - 4) {
    LOG_INFO("Response too large");
    conn->state = END;
    return false;
  }

  memcpy(&conn->wbuf[0], &res_size, 4);
  memcpy(&conn->wbuf[4], res_str, res_size);
  conn->wbuf_size = 4 + res_size;

  // Remove the processed data from the read buffer
  size_t remaining = conn->rbuf_size - (4 + len);
  if (remaining > 0) {
    memmove(conn->rbuf, &conn->rbuf[4 + len], len);
  }
  conn->rbuf_size = remaining;
  //

  state_response(conn);
  return conn->state == REQUEST;
}

static void state_request(ConnectionManager *cm, Connection *conn) {
  LOG_DEBUG("state_request: %d", conn->fd);
  do {
    ssize_t rv = 0;
    do {
      size_t cap = K_MAX_MSG - conn->rbuf_size;
      rv = read(conn->fd, &conn->rbuf[conn->rbuf_size], cap);
    } while (rv < 0 && errno == EINTR);

    if (rv < 0) {
      if (errno == EAGAIN) {
        // Got EAGAIN, stop it
        break;
      } else {
        LOG_DEBUG("read() error %d", errno);
        break;
      }
    }

    if (rv == 0) {
      if (conn->rbuf_size > 0) {
        LOG_DEBUG("Unexpected EOF");
      } else {
        LOG_DEBUG("EOF");
      }
      conn->state = END;
      break;
    }

    conn->rbuf_size += (size_t)(rv);
    while (try_one_request(cm, conn)) {
    }
  } while (conn->state == REQUEST);
}

void cm_remove_connection(ConnectionManager *cm, int fd) {
  // TBD
}

void cm_handle_connection_io(ConnectionManager *cm, Connection *conn) {
  if (conn->state == REQUEST) {
    state_request(cm, conn);
  } else if (conn->state == RESPONSE) {
    state_response(conn);
  }
}
void cm_register_processreq_funct(ConnectionManager *cm,
                                  ProcessRequestFunction cb) {
  cm->_procreq_cb = cb;
}

