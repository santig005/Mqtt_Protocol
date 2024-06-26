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
  // print the firstbyte of the packet
  uint8_t connack_response;
  switch (first_byte) {
  case B_CONNACK:
    connack_response = process_connack(buff);
    switch (connack_response) {
    case CONNACK_ACCEPTED:
      printf("Connection accepted\n");
      break;
    case CONNACK_UNNACCEPTABLE_PROTOCOL_VERSION:
      printf("Connection UNNACCEPTABLE_PROTOCOL_VERSION\n");
      break;
    case CONNACK_IDENTIFIER_REJECTED:
      printf("Connection IDENTIFIER_REJECTED\n");
      break;
    case CONNACK_SERVER_UNAVAILABLE:
      printf("Connection SERVER_UNAVAILABLE\n");
      break;
    case CONNACK_BAD_USERNAME_OR_PASSWORD:
      printf("Connection BAD_USERNAME_OR_PASSWORD\n");
      break;
    case CONNACK_NOT_AUTHORIZED:
      printf("Connection NOT_AUTHORIZED\n");
      break;
    }
  }
  return 0x01;
}
void send_connect(int connfd, struct connect *connect_messg) {
  uint16_t variable_header_length =
      2 + connect_messg->variable_header.protocol_name.length + 1 + 1 + 2;
  uint64_t payload_length =
      2 + strlen((const char *)connect_messg->payload.client_id);
  if (connect_messg->variable_header.connect_flags.bits.will_flag) {
    payload_length +=
        2 + strlen((const char *)connect_messg->payload.will_topic) + 2 +
        strlen((const char *)connect_messg->payload.will_message);
  }
  if (connect_messg->variable_header.connect_flags.bits.username) {
    payload_length += 2 + strlen((const char *)connect_messg->payload.username);
  }
  if (connect_messg->variable_header.connect_flags.bits.password) {
    payload_length += 2 + strlen((const char *)connect_messg->payload.password);
  }
  uint32_t remaining_length = variable_header_length + payload_length;

  uint8_t fixed_header_length = 1 + nbytes_remaining_length(remaining_length);
  uint64_t total_length =
      fixed_header_length + variable_header_length + payload_length;
  uint8_t connect_packet[total_length];
  uint8_t *ptr = &connect_packet[0];
  pack_byte(&ptr, B_CONNECT);
  pack_remaining_length(&ptr, remaining_length);
  write_string16(&ptr, connect_messg->variable_header.protocol_name.name);
  pack_byte(&ptr, connect_messg->variable_header.protocol_level);
  pack_byte(&ptr, connect_messg->variable_header.connect_flags.byte);
  pack_16b(&ptr, connect_messg->variable_header.keep_alive);

  write_string16(&ptr, connect_messg->payload.client_id);
  if (connect_messg->variable_header.connect_flags.bits.will_flag) {
    write_string16(&ptr, connect_messg->payload.will_topic);
    write_string16(&ptr, connect_messg->payload.will_message);
  }
  if (connect_messg->variable_header.connect_flags.bits.username) {
    write_string16(&ptr, connect_messg->payload.username);
  }
  if (connect_messg->variable_header.connect_flags.bits.password) {
    write_string16(&ptr, connect_messg->payload.password);
  }
  bytes_rw = write(connfd, connect_packet, total_length);
}

void send_subscribe(int connfd, struct subscribe *subscribe_messg) {
  uint16_t variable_header_length = 2;
  uint64_t payload_length=0;
  for (int i = 0; i < subscribe_messg->payload.tuples_len; i++) {
    payload_length +=
        2 + strlen((const char *)subscribe_messg->payload.tuples[i].topic) + 1;
  }
  uint32_t remaining_length = variable_header_length + payload_length;
  uint8_t fixed_header_length = 1 + nbytes_remaining_length(remaining_length);
  uint64_t total_length =
      fixed_header_length + variable_header_length + payload_length;
  uint8_t subscribe_packet[total_length];
  uint8_t *ptr = &subscribe_packet[0];
  pack_byte(&ptr, B_SUBSCRIBE);
  pack_remaining_length(&ptr, remaining_length);
  pack_16b(&ptr, subscribe_messg->variable_header.packet_id);
  for (int i = 0; i < subscribe_messg->payload.tuples_len; i++) {
    write_string16(&ptr, subscribe_messg->payload.tuples[i].topic);
    pack_byte(&ptr, subscribe_messg->payload.tuples[i].qos);
  }
  bytes_rw = write(connfd, subscribe_packet, total_length);
}

void send_publish(int connfd, struct publish *publish_messg){
  uint16_t variable_header_length = 2 + strlen((const char *)publish_messg->variable_header.topic);
  uint64_t payload_length = strlen((const char *)publish_messg->payload.message);

  uint32_t remaining_length = variable_header_length + payload_length;
  uint8_t fixed_header_length = 1 + nbytes_remaining_length(remaining_length);

  uint64_t total_length = fixed_header_length + variable_header_length + payload_length;
  uint8_t publish_packet[total_length];

  uint8_t *ptr = &publish_packet[0];
  pack_byte(&ptr, B_PUBLISH);
  pack_remaining_length(&ptr, remaining_length);
  write_string16(&ptr, publish_messg->variable_header.topic);

  bytes_rw = write(connfd, publish_packet, total_length);

}