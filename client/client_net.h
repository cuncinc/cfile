#ifndef _CLIENT_NET_H
#define _CLIENT_NET_H

#include "../method.h"

#define MAXLINE 4096

void *send_message(struct method *p);

#endif