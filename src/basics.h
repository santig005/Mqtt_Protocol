#ifndef BASICS_H
#define BASICS_H
#include <stdio.h>
#include <stdint.h>
union basic_header {
    uint8_t byte;
    struct {
        unsigned retain : 1;
        unsigned qos : 2;
        unsigned dup : 1;
        unsigned packet_type : 4;
    } bits;
};

struct fixed_header{
  union basic_header basic_header;
  uint64_t remaining_length;
};
struct connect{
  struct fixed_header header;

  struct{

    struct{
      uint16_t length;
      char name[16];
    }protocol_name;

    uint8_t protocol_level;

    union{
        uint8_t byte;
        struct {
            int reserverd : 1;
            unsigned clean_session : 1;
            unsigned will_flag : 1;
            unsigned will_qos : 2;
            unsigned will_retain : 1;
            unsigned password : 1;
            unsigned username : 1;
        } bits;

    }connnect_flags;

    uint16_t keep_alive;

  }variable_header;

  struct{
  uint8_t *client_id;
  uint8_t *username;
  uint8_t *password;
  uint8_t *will_topic;
  uint8_t *will_message;
  }payload;
  
};

struct packet{
  struct connect CONNECT;
};

#endif