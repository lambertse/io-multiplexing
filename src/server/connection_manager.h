#ifndef IO_MUX_CONNECTION_MANAGER_H__
#define IO_MUX_CONNECTION_MANAGER_H__

#include "internal_define.h"

typedef struct {
  ProcessRequestFunction _procreq_cb;
} ConnectionManager;

void cm_remove_connection(ConnectionManager *cm, int fd);
void cm_handle_connection_io(ConnectionManager *cm, Connection *conn);
void cm_register_processreq_funct(ConnectionManager *cm,
                                  ProcessRequestFunction cb);

#endif
