#ifndef CLIENTPROCESSOR_H
#define CLIENTPROCESSOR_H

#include <stdint.h>
#include "basics.h"
uint8_t process_packet(int connfd,uint8_t * buff);
uint8_t process_connack(uint8_t * buff);
void send_disconnect(int connfd);
void send_connect( int connfd,struct connect * conn);

#endif




