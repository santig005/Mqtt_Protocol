#include "backlog.h"
#include "basics.h"
#include "client.h"
#include "packer.h"
#include <arpa/inet.h> // inet_addr()
#include <netdb.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h> // bzero()
#include <sys/socket.h>
#include <unistd.h> // read(), write(), close()

uint8_t process_connack(uint8_t *buff) {
  printf("llego 0\n");
  uint32_t remaining_len = remaining_length(&buff);
  uint8_t session_present = next_byte(&buff);
  uint8_t response = next_byte(&buff);
  return response;
}
void send_disconnect(int connfd) {
  uint8_t disconnect[2] = {0xe0, 0x00};
  bytes_rw = write(connfd, disconnect, sizeof(disconnect));
}
uint8_t process_packet(int connfd, uint8_t *buff) {
  uint8_t first_byte = next_byte(&buff);
  uint8_t connack_response;
  switch (first_byte) {
  case B_CONNACK:
    printf("the packet is connack\n");
    connack_response = process_connack(buff);
    switch (connack_response) {
    case CONNACK_ACCEPTED:
      printf("Connection accepted");
      break;
    case CONNACK_UNNACCEPTABLE_PROTOCOL_VERSION:
      printf("Connection UNNACCEPTABLE_PROTOCOL_VERSION");
      break;
    case CONNACK_IDENTIFIER_REJECTED:
      printf("Connection IDENTIFIER_REJECTED");
      break;
    case CONNACK_SERVER_UNAVAILABLE:
      printf("Connection SERVER_UNAVAILABLE");
      break;
    case CONNACK_BAD_USERNAME_OR_PASSWORD:
      printf("Connection BAD_USERNAME_OR_PASSWORD");
      break;
    case CONNACK_NOT_AUTHORIZED:
      printf("Connection NOT_AUTHORIZED");
      break;
    }
  }
  return 0x01;
}
void send_connect(int connfd, struct connect *connect_messg) {
  //calculamos la longitud que tendran los campos de la variable header y del payload, de acuerdo a la longitud del cliente, y a si en las flags hay contraseña, usuario
  uint16_t variable_header_length = 2+ connect_messg->variable_header.protocol_name.length + 1 + 1 + 2; 
  uint16_t payload_length = 2 + strlen((const char *)connect_messg->payload.client_id);
  if (connect_messg->variable_header.connect_flags.bits.will_flag) {
    payload_length += 2 + strlen((const char *)connect_messg->payload.will_topic) + 2 + strlen((const char *)connect_messg->payload.will_message);
  }
  if (connect_messg->variable_header.connect_flags.bits.username) {
    payload_length += 2 + strlen((const char *)connect_messg->payload.username);
  }
  if (connect_messg->variable_header.connect_flags.bits.password) {
    payload_length += 2 + strlen((const char *)connect_messg->payload.password);
  }
  uint32_t remaining_length = variable_header_length + payload_length;
  

  
}