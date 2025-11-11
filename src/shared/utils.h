#ifndef IO_MUX_UTILS_H
#define IO_MUX_UTILS_H 

#include "shared/logging.h"
#include <errno.h>
#include <fcntl.h>

static void set_fb_nonblocking(int fd) {
  errno = 0;
  int flags = fcntl(fd, F_GETFL, 0);
  if (errno) {
    LOG_ERROR("fcntl get error\n");
    return;
  }

  fcntl(fd, F_SETFL, flags | O_NONBLOCK);
  if (errno) {
    LOG_ERROR("fcntl set error\n");
    return;
  }
}

#endif
