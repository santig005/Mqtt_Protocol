#include "clientslist.h"
#include <stddef.h>

#ifndef BROKER_H
#define BROKER_H
extern Clients * clist;
extern ssize_t bytes_rw;
void network_connection(int conffd);
#endif