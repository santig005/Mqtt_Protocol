// This is only to test the program

#include "basics.h"
#include "convertion.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "clientslist.h"
#include "brokerprocessor.h"
#include "broker.h"
int main() {
  // a 1010
  // b 1011
  // c 1100
  // d 1101
  // e 1110
  // f 1111
  // 0001 0000
  // 0001 0011
  // 0000 0000
  // 0000 0100 longitud mqtt
  // 0100 1101  M
  // 0101 0001  Q
  // 0101 0100  T
  // 0101 0100  T
  // 0000 0100  Protocol level
  // 1100 1010  connect flags
  // 0000 0000  keepalive
  // 0000 1010
  // 0000 0000  client id length
  // 0000 0001
  // 0000 0111 client id
  // 0000 0000 // Username l
  // 0000 0001
  // 0100 0001 // A
  // 0000 0000 // Password l
  // 0000 0001
  // 0100 0010 // B
  // 1012044d51545404ca000a010007010041010042
  //Clients* clientslist;
  clist=Clients_newList();
  char mensaje[] = "101300044d51545404ca000a000107000141000142";
   char mensaje2[] = "101300044d51545404ca000a000108000143000142";
  uint8_t *buff = hextobytes(mensaje);
  uint8_t *buff2 = hextobytes(mensaje2);
  for (int i = 0; i < 10; i++) {
    printf("The %d is 0x%x\n", i, *(buff + i));
    //fflush(stdout);//:::::::::::
  }
  //Clients * clientslist=Clients_newList();
  uint8_t response = process_packet(2, buff);
  printf("The response was 0x%x", response);
  uint8_t response2 = process_packet(2, buff2);
  printf("The response was 0x%x", response2);
}