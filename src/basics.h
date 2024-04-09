#ifndef BASICS_H
#define BASICS_H
#include <stdint.h>
#include <stdio.h>


#define B_CONNECT 0x10
#define B_CONNACK 0x20
#define B_PUBLISH 0x30
#define B_DISCONNECT 0xe0

enum connack_response{
  CONNACK_ACCEPTED = 0x00,
  CONNACK_UNNACCEPTABLE_PROTOCOL_VERSION = 0x01,
  CONNACK_IDENTIFIER_REJECTED = 0x02,
  CONNACK_SERVER_UNAVAILABLE = 0x03,
  CONNACK_BAD_USERNAME_OR_PASSWORD = 0x04,
  CONNACK_NOT_AUTHORIZED = 0x05
};



enum packet_type {
  CONNECT = 1,
  CONNACK = 2,
  PUBLISH = 3,
  SUBSCRIBE = 8,
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
      uint8_t *name;
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

struct subscribe {

    struct fixed_header header;

    unsigned short packet_id;

    unsigned short tuples_len;

    struct {
        unsigned short topic_len;
        unsigned char *topic;
        unsigned qos;
    } *tuples;
};

struct publish {
  struct fixed_header header;
  unsigned short packet_id;
  unsigned short topic_length;
  unsigned char *topic;
  unsigned short payload_length;
  unsigned char *payload;
};

struct packet {
  struct connect CONNECT;
  struct connack CONNACK;
  struct publish PUBLISH;
};


#endif