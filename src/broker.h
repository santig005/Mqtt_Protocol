#include "clientslist.h"
#include "topic.h"
#include <stddef.h>
#include <pthread.h>
#ifndef BROKER_H
#define BROKER_H

 Clients * clist;
extern pthread_mutex_t list_mutex;
extern struct topic *root;
extern ssize_t bytes_rw;
void network_connection(int conffd);
#endif
