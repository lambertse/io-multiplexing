#ifndef IO_MUX_CLIENT_H__
#define IO_MUX_CLIENT_H__

#include <stdbool.h>
#include "io-multiplexing/server/config.h"

#ifdef __cplusplus
extern "C" {
#endif
/* Opaque pointer - forward declaration to hide implementation */
typedef struct Client Client;

Client* client_create(void);
void client_destroy(Client* client);

/* Member function equivalents */
bool client_connect(Client* client, ServerConfig* server_conf);
char* client_send_request(Client* client, const char* request, size_t req_size, size_t* res_size);
bool client_disconnect(Client* client);
#ifdef __cplusplus
}
#endif

#endif
