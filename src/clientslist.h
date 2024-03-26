#ifndef CLIENTLIST_H
#define CLIENTLIST_H
#include <stdint.h>
#include <stdio.h>

typedef struct client{
  uint8_t *client_id;
  uint8_t *username;
  uint8_t *password;
  uint8_t *will_topic;
  uint8_t *will_message;
  client *next;
}Client;

typedef struct clients {
    Client *head;
}Clients;

Clients * Clients_newList();

void Clients_add(Clients * list,uint8_t *client_id, uint8_t *username, uint8_t *password, uint8_t *will_topic, uint8_t *will_message);

Client * Clients_find(Clients *list, uint8_t *client_id);

void Clients_print(Clients *list);




#endif