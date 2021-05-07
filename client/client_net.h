#ifndef _CLIENT_NET_H
#define _CLIENT_NET_H

#include "../protocal.h"

void *send_method(struct method *method);

void *send_file_method(struct method *method);

#endif