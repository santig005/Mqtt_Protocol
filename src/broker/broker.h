#include "clientslist.h"
#include "topic.h"
#include <stddef.h>
#include <pthread.h>
#ifndef BROKER_H
#define BROKER_H

//extern Clients * clist;
//extern pthread_mutex_t clist_mutex;
extern struct topic *root;
extern ssize_t bytes_rw;
void* network_connection(void * arg);
#endif
