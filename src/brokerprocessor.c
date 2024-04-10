#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "basics.h"
#include "broker.h"
#include "brokerprocessor.h"
#include "clientslist.h"
#include "packer.h"



uint8_t process_connect(struct connect *connect_messg, uint8_t *buff) {
  connect_messg->header.remaining_length = remaining_length(&buff);
  connect_messg->variable_header.protocol_name.length = next_16b(&buff);
  connect_messg->variable_header.protocol_name.name =
      next_nbytes(&buff, connect_messg->variable_header.protocol_name.length);

  if (strcmp((const char *)connect_messg->variable_header.protocol_name.name,
             "MQTT") != 0) {
    return 0x01;
  }
  connect_messg->variable_header.protocol_level = next_byte(&buff);
  // Verificamos que sea el protocolo 4
  if (connect_messg->variable_header.protocol_level != 0x04) {
    return 0x01;
  }
  connect_messg->variable_header.connect_flags.byte = next_byte(&buff);
  connect_messg->variable_header.keep_alive = next_16b(&buff);
  // payload
  read_string16(&buff, &connect_messg->payload.client_id);
  if (connect_messg->variable_header.connect_flags.bits.will_flag) {
    read_string16(&buff, &connect_messg->payload.will_topic);
    read_string16(&buff, &connect_messg->payload.will_message);
  }

  if (connect_messg->variable_header.connect_flags.bits.username) {
    read_string16(&buff, &connect_messg->payload.username);
  }

  if (connect_messg->variable_header.connect_flags.bits.password) {
    read_string16(&buff, &connect_messg->payload.password);
  }
  Client *exist = Clients_find(clist, connect_messg->payload.client_id);
  if (exist == NULL) {
    Clients_print(clist);
    Clients_add(clist, connect_messg);
  }
  Clients_print(clist);
  return 0x00;
}
void send_connack(int connfd, uint8_t session_present, uint8_t response) {
  uint8_t connack[4] = {B_CONNACK, 0x02, session_present,response};
  bytes_rw=write(connfd, connack, sizeof(connack));
}
uint8_t process_disconnect(uint8_t *buff, uint8_t *client_id) {
  Client *c = Clients_find(clist, client_id);
  printf("Client id: %s was disconnected\n", c->client_id);
  if (c == NULL) {
    return 0x01;
  } else {
    c->session->connected = false;
  }
  if (remaining_length(&buff) != 0x00) {
    return 0x01;
  }
  return 0x00;
}
/*
uint8_t process_subscribe(uint8_t * buff, uint8_t first_byte){

  struct subscribe * subscribe_messg=(struct subscribe *)malloc(sizeof(struct
subscribe)); uint8_t reserved_bytes = (first_byte & 0x0F);

  if(reserved_bytes!=0x02){
    return 0x01;
  }

  subscribe_messg->header.remaining_length=remaining_length(&buff);
  subscribe_messg->packet_id=next_16b(&buff);
  subscribe_messg->header.remaining_length -= sizeof(uint16_t);

  int i = 0;
  while (subscribe_messg->header.remaining_length > 0) {

       Read length bytes of the first topic filter
      uint16_t topic_len = next_16b(&buff);
      subscribe_messg->header.remaining_length -= sizeof(uint16_t);

      subscribe_messg->tuples[i].topic_len = topic_len;
      unpack_bytes(&buff, topic_len, subscribe_messg->tuples[i].topic);
      subscribe_messg->header.remaining_length -= topic_len;
      subscribe_messg->tuples[i].qos = next_byte(&buff);
      subscribe_messg->header.remaining_length -= sizeof(uint8_t);
      i++;
  }

   subscribe_messg->tuples_len = i;

    return 0x00;
  }
*/

uint8_t process_publish(uint8_t *buff) {
  struct publish *publish_messg =
      (struct publish *)malloc(sizeof(struct publish));
  publish_messg->header.remaining_length = remaining_length(&buff);
}

uint8_t process_packet(int connfd, uint8_t *buff, uint8_t *client_id) {
  uint8_t first_byte = next_byte(&buff);
  uint8_t packet_type = (first_byte & 0xF0) >> 4;
  uint8_t response;
  struct connect *connect_messg;
  switch (packet_type) {
  case CONNECT:
    connect_messg = (struct connect *)malloc(sizeof(struct connect));
    response = process_connect(connect_messg, buff);
    if (response == 0x00) {
      client_id = connect_messg->payload.client_id;
      Client *c = Clients_find(clist, client_id);
      //lets print something of the new client
      printf("==================\n");
      // print the address of memory of c
      printf("Client: %p\n", c);
      printf("New connection from %s\n");
      printf("Client ID: %s\n", c->client_id);
      printf("Username: %s\n", c->username);
      printf("Password: %s\n", c->password);
      printf("======================== \n");
      c->session->connfd = connfd;
      send_connack(connfd, 0x01, CONNACK_ACCEPTED);
      return 0x01;
    } else {
      send_connack(connfd, 0x00, response);
      return 0x00;
    }
  case DISCONNECT:
    response = process_disconnect(buff, client_id);
    return response;
  }
  return 0x00;
}