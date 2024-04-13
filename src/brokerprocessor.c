#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "basics.h"
#include "broker.h"
#include "brokerprocessor.h"
#include "clientslist.h"
#include "packer.h"
Clients * clist;
pthread_mutex_t clist_mutex=PTHREAD_MUTEX_INITIALIZER;
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
	pthread_mutex_lock(&clist_mutex);
    Clients_add(clist, connect_messg);
pthread_mutex_unlock(&clist_mutex);
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
  Client *c=Clients_find(clist,client_id);
  struct subscribe *subscribe_messg =
      (struct subscribe *)malloc(sizeof(struct subscribe));
      uint32_t local_remaining=0;
  subscribe_messg->header.remaining_length = remaining_length(&buff);
  local_remaining=subscribe_messg->header.remaining_length;
  subscribe_messg->variable_header.packet_id = next_16b(&buff);
  local_remaining-=2;
  while (local_remaining > 0) {
    struct packet_topic *packet = (struct packet_topic *)malloc(sizeof(struct packet_topic));
    packet->topic_len = next_16b(&buff);
    packet->topic = next_nbytes(&buff, packet->topic_len);
    packet->qos = next_byte(&buff);
    local_remaining=local_remaining-2-1-packet->topic_len;
    struct subscription * subscription_ob=new_subscription(c->session,packet->qos,packet->topic);
    add_subscription(c->session->subscriptions,subscription_ob);
    struct topic * topic_in_root = search_topic(root,subscription_ob->topic);
    Topic_add_subscription(&topic_in_root,subscription_ob);
  }
  print_topic(0,root);
}

void send_publish(int connfd, struct publish * publish_messg){
//hacemos una copia en memoria de el packete publish
  struct publish * copia=(struct publish *)malloc(sizeof(struct publish));
  memcpy((void*)copia,(void*)publish_messg,sizeof(struct publish));
  uint16_t variable_header_length = 2 + copia->variable_header.topic_length + 2;
  uint16_t payload_length = copia->payload.payload_len;
  uint32_t remaining_length = variable_header_length + payload_length;
  uint8_t fixed_header_length = 1 + nbytes_remaining_length(remaining_length);
  uint64_t total_length= fixed_header_length + remaining_length;
  uint8_t publish_packet[total_length];
  uint8_t *ptr = &publish_packet[0];
  copia->header.basic_header.byte = 0x30;
  pack_byte(&ptr, copia->header.basic_header.byte);
  pack_remaining_length(&ptr, remaining_length);
  pack_16b(&ptr, copia->variable_header.topic_len);
  pack_nbytes(&ptr, copia->variable_header.topic, copia->variable_header.topic_len);
  pack_16b(&ptr, copia->variable_header.packet_id);
  pack_nbytes(&ptr, copia->payload.message, copia->payload.payload_len);
  bytes_rw = write(connfd, publish_packet, total_length);
}

uint8_t process_publish(uint8_t *buff, uint8_t *client_id,struct publish * publish_messg) {
  Client *c=Clients_find(clist,client_id);
  publish_messg->header.remaining_length = remaining_length(&buff);
  publish_messg->variable_header.topic_len = next_16b(&buff);
  publish_messg->variable_header.topic = next_nbytes(&buff, publish_messg->variable_header.topic_len);
  publish_messg->variable_header.packet_id = next_16b(&buff);
  publish_messg->payload.payload_len = publish_messg->header.remaining_length - publish_messg->variable_header.topic_len - 2;
  publish_messg->payload.message = next_nbytes(&buff, publish_messg->payload.payload_len);
  if(publish_messg->header.basic_header.bits.retain){
    struct topic *topic_in_root = search_topic(root,publish_messg->variable_header.topic);
    topic_in_root->retained = true;
    topic_in_root->retained_message = publish_messg->payload.message;
  }
  struct subscription *current_sub = topic_in_root->subscriptions;
  while(current_sub!=NULL){
    send_publish(current_sub->subscriber->session->connfd,publish_messg);
    current_sub=current_sub->next;
  }
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
      printf("========================\n");
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
    response = process_disconnect(buff, *client_id);
    return response;
  case B_SUBSCRIBE:
    response = process_subscribe(buff, *client_id);
    return 0x01;
    case default:
    //sacamos los primeros 4 bits y vemos si es publish, tomamos dup, qos y retain los mandamos a la funcion 
    //process_publish
    uint8_t byte = first_byte;;
    struct publish *publish_messg = (struct publish *)malloc(sizeof(struct publish));
    publish_messg->header.basic_header.byte = byte;
    response = process_publish(buff, *client_id,publish_messg);
  }
  return 0x01;
}
