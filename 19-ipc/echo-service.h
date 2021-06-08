#pragma once

#include "memchannel.h"

#define ECHO_SERVICE_ID_BASE        "/isel.so.memecho"
#define ECHO_SERVICE_SERVER_ID      ECHO_SERVICE_ID_BASE ".server"
#define ECHO_SERVICE_CLIENT_ID_BASE ECHO_SERVICE_ID_BASE ".client."

#define PID_MAX_LEN 10

#define ECHO_MSG_MAX_LEN 256

typedef struct echomsg {
	int sender;
	char msg[ECHO_MSG_MAX_LEN];
} echomsg_t;
