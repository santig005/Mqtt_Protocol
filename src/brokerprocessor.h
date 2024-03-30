#ifndef BROKERPROCESSOR_H
#define BROKERPROCESSOR_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint8_t process_connect(uint8_t *buff);
uint8_t process_disconnect(uint8_t *buff, uint8_t *first_byte);
uint8_t process_publish(uint8_t *buff);
void process_packet(int connfd, uint8_t *buff);

#endif // BROKERPROCESSOR_H