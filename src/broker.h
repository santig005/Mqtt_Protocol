#include "clientslist.h"
#include "topic.h"
#include <stddef.h>

#ifndef BROKER_H
#define BROKER_H
extern Clients * clist;
extern struct topic *root;
extern ssize_t bytes_rw;
void network_connection(int conffd);
#endif