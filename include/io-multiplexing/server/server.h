#ifndef IO_MUX_SERVER_H__
#define IO_MUX_SERVER_H__

#include "io-multiplexing/server/config.h"
#include "io-multiplexing/define.h"
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Opaque pointer - forward declaration to hide implementation */
typedef struct Server Server;

/* Constructor/Destructor equivalents */
Server* server_create(ServerConfig config);
void server_destroy(Server* server);

/* Member function equivalents */
bool server_init(Server* server, ProcessRequestFunction cb);
bool server_start(Server* server);
bool server_stop(Server* server);
bool server_deinit(Server* server);

#ifdef __cplusplus
}
#endif

#endif

