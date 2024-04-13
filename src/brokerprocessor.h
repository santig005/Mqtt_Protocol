#ifndef BROKERPROCESSOR_H
#define BROKERPROCESSOR_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern Clients * clist;
void send_connack(int connfd,uint8_t session_present,uint8_t response);
uint8_t process_connect(struct connect * sc,uint8_t *buff);
uint8_t process_disconnect(uint8_t *buff, uint8_t *client_id);
uint8_t process_subscribe(uint8_t *buff, uint8_t *client_id);
void send_publish(int connfd, struct publish * pub)
uint8_t process_publish(uint8_t *buff,uint8_t * client_id, struct publish * pub);
uint8_t process_packet(int connfd, uint8_t *buff, uint8_t **client_id);

#endif // BROKERPROCESSOR_H
