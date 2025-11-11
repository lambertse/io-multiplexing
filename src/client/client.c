#include "io-multiplexing/client/client.h"
#include "shared/logging.h"
#include <stdlib.h>

#include <sys/_types/_ssize_t.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

typedef enum {
    CLIENT_STATUS_NOT_READY,
    CLIENT_STATUS_READY,
    CLIENT_STATUS_SENDING,
    CLIENT_STATUS_RECEIVING
} ClientStatus;

/* Internal implementation structure */
struct Client {
    int fd;
    ClientStatus status;
};

/* Forward declarations of internal helper functions */
static bool send_request_internal(Client* client, const char* buffer, size_t buffer_size);
static char* read_response(Client* client, size_t* res_size);

/* Constructor */
Client* client_create(void) {
    Client* client = (Client*)malloc(sizeof(Client));
    if (client == NULL) {
        return NULL;
    }
    client->fd = -1;
    client->status = CLIENT_STATUS_NOT_READY;
    return client;
}

/* Destructor */
void client_destroy(Client* client) {
    if (client != NULL) {
        if (client->fd >= 0) {
            close(client->fd);
        }
        free(client);
    }
}

/* Connect to server */
bool client_connect(Client* client, ServerConfig* server_conf) {
    if (client == NULL) {
        return false;
    }

    LOG_DEBUG("Client started");
    client->fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client->fd < 0) {
        LOG_DEBUG("can not create fd");
        return false;
    }

    const int port = server_conf->port;
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(0);

    int rv = connect(client->fd, (const struct sockaddr *)&addr, sizeof(addr));
    if (rv < 0) {
  
        LOG_DEBUG("connect failed.");
        close(client->fd);
        client->fd = -1;
        return false;
    }

    LOG_DEBUG("connect client to server success");
    client->status = CLIENT_STATUS_READY;
    return true;
}

/* Send request and receive response */
char* client_send_request(Client* client, const char* request, size_t req_size, size_t* res_size) {
    if (client == NULL || request == NULL) {
        return NULL;
    }

    if (client->status != CLIENT_STATUS_READY) {
        return NULL;
    }
    char* res;

    do {
        client->status = CLIENT_STATUS_SENDING;
        if (!send_request_internal(client, request, req_size)) {
            LOG_INFO("Send request failed");
            break;
        }

        client->status = CLIENT_STATUS_RECEIVING;
        LOG_DEBUG("Receiving...");
        if ((res = read_response(client, res_size)) && !res) {
            LOG_INFO("Read response failed");
            break;
        }
        LOG_DEBUG("Received");
    } while(false);

    client->status = CLIENT_STATUS_READY;
    LOG_DEBUG("send_request done!");
    return res;
}

/* Disconnect from server */
bool client_disconnect(Client* client) {
    if (client == NULL) {
        return false;
    }

    client->status = CLIENT_STATUS_NOT_READY;
    if (client->fd >= 0) {
        close(client->fd);
        client->fd = -1;
    }
    return true;
}

/* Internal helper: read response from server */
static char *read_response(Client *client, size_t* res_size) {
  ssize_t rv = read(client->fd, res_size, 4);
  size_t cur_size = *res_size;
  if (rv < 0) {
    return NULL;
  }
  char *response = (char *)malloc(*res_size);
  char *res = response;

  while (cur_size > 1) {
    rv = read(client->fd, response, cur_size);
    if (rv <= 1) {
      return response;
    }
    response += rv;
    cur_size -= rv;
  }
  return res;
}

/* Internal helper: send request to server */
static bool send_request_internal(Client *client, const char *buffer,
                                  size_t buffer_size) {
  write(client->fd, &buffer_size, 4);

  while (buffer_size > 1) {
    ssize_t rv = write(client->fd, buffer, buffer_size);
    if (rv <= 1) {
      return false;
    }
    buffer_size -= rv;
    buffer += rv;
  }

  LOG_DEBUG("Send request done!");
  return true;
}
