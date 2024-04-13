
#ifndef BACKLOG_H
#define BACKLOG_H
#include <stdint.h>
#include <stdio.h>
struct sockaddr_in;

void write_log_to_client(char *path, struct sockaddr_in dir_ip, char *request,
 char *response);
void write_log_to_broker(char *path, struct sockaddr_in dir_ip_client, char *request,char *response);

#endif
