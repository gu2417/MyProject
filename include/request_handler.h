#ifndef REQUEST_HANDLER_H
#define REQUEST_HANDLER_H

#include "protocol.h"
#include "server_state.h"

int handle_request(ClientContext *client, const Message *request, Message *response);

#endif
