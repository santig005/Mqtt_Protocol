#ifndef BASICS_H
#define BASICS_H
#include <stdint.h>
#include <stdio.h>

enum packet_type {
  CONNECT = 1,
  CONNACK = 2,
  PUBLISH = 3,
  DISCONNECT = 14
};

enum qos_level { 
  AT_MOST_ONCE,
  AT_LEAST_ONCE,
  EXACTLY_ONCE
};

union basic_header {
  uint8_t byte;
  struct {
    unsigned retain : 1;
    unsigned qos : 2;
    unsigned dup : 1;
    unsigned packet_type : 4;
  } bits;
};

struct fixed_header {
  union basic_header basic_header;
  uint32_t remaining_length;
};
struct connect {
  struct fixed_header header;
  struct {
    struct {
      uint16_t length;
      char *name;
    } protocol_name;

    uint8_t protocol_level;

    union {
      uint8_t byte;
      struct {
        unsigned reserverd : 1;
        unsigned clean_session : 1;
        unsigned will_flag : 1;
        unsigned will_qos : 2;
        unsigned will_retain : 1;
        unsigned password : 1;
        unsigned username : 1;
      } bits;

    } connect_flags;

    uint16_t keep_alive;

  } variable_header;

  struct {
    uint8_t *client_id;
    uint8_t *username;
    uint8_t *password;
    uint8_t *will_topic;
    uint8_t *will_message;
  } payload;
};
struct connack {
  struct fixed_header header;
  struct {
    union {
      uint8_t byte;
      struct {
        unsigned session_present : 1;
        unsigned reserved : 7;
      } bits;
    } ack_flags;
    uint8_t return_code;
  } variable_header;
};

struct publish {
  struct fixed_header header;
  unsigned short paket_id;
  unsigned short topic_length;
  unsigned char *topic;
  unsigned short payload_length;
  unsigned char *payload;
};

struct disconnect {
  struct fixed_header header;
  uint8_t remaining_length;
};

struct packet {
  struct connect CONNECT;
  struct connack CONNACK;
  struct publish PUBLISH;
  struct disconnect DISCONNECT;
};

int process_connect(uint8_t *buff);
void process_packet(int connfd, uint8_t *buff);

#endif