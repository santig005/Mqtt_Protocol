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
  if (connect_messg->variable_header.protocol_level != 0x04) {
    return 0x01;
  }
  connect_messg->variable_header.connect_flags.byte = next_byte(&buff);
  connect_messg->variable_header.keep_alive = next_16b(&buff);
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
  uint8_t connack[4] = {B_CONNACK, 0x02, session_present, response};
  bytes_rw = write(connfd, connack, sizeof(connack));
}
uint8_t process_disconnect(uint8_t *buff, uint8_t *client_id) {
  Client *c = Clients_find(clist, client_id);
  printf("The client addres is %p\n", c);
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
uint8_t process_subscribe(uint8_t *buff, uint8_t *client_id) {
  struct subscribe *subscribe_messg =
      (struct subscribe *)malloc(sizeof(struct subscribe));
  subscribe_messg->header.remaining_length = remaining_length(&buff);
  subscribe_messg->variable_header.packet_id = next_16b(&buff);
  uint8_t *topic;
  uint8_t qos;
  while (buff != NULL) {
    read_string16(&buff, &topic);
    qos = next_byte(&buff);
    printf("Topic: %s\n", topic);
    printf("Qos: %d\n", qos);
  }
  printf("now we print the topics\n");
  print_topic(0, root);
  return 0x00;
}

uint8_t process_publish(uint8_t *buff) {
  struct publish *publish_messg =
      (struct publish *)malloc(sizeof(struct publish));
  publish_messg->header.remaining_length = remaining_length(&buff);
}

uint8_t process_packet(int connfd, uint8_t *buff, uint8_t **client_id) {
  uint8_t first_byte = next_byte(&buff);
  uint8_t response;
  struct connect *connect_messg;
  switch (first_byte) {
  case B_CONNECT:
    connect_messg = (struct connect *)malloc(sizeof(struct connect));
    response = process_connect(connect_messg, buff);
    if (response == 0x00) {
      *client_id = connect_messg->payload.client_id;
      Client *c = Clients_find(clist, *client_id);
      printf("==================\n");
      printf("Client: %p\n", c);
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
  case B_DISCONNECT:
    printf("This id gonna disconnect %s\n", *client_id);
    response = process_disconnect(buff, *client_id);
    return response;
  case B_SUBSCRIBE:
    response = process_subscribe(buff, *client_id);
    return 0x01;
  }
  return 0x01;
}