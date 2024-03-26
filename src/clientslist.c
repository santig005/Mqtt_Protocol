#include "clientslist.h"
#include <string.h>
#include <stdlib.h>

Clients * Clients_newList() {
    Clients *newList = (Clients*)malloc(sizeof(Clients));
    newList->head = NULL;
    return newList;
}

void Clients_add(Clients * list, uint8_t *client_id, uint8_t *username, uint8_t *password, uint8_t *will_topic, uint8_t *will_message) {
    Client *new_client = (Client*)malloc(sizeof(Client));
    new_client->client_id = client_id;
    new_client->username = username;
    new_client->password = password;
    new_client->will_topic = will_topic;
    new_client->will_message = will_message;
    new_client->next = NULL;
    
    if (list->head == NULL) {
        list->head = new_client;
    } else {
        Client *current = list->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = new_client;
    }
}

Client *Clients_find(Clients *list, uint8_t *client_id) {
  Client *current = list->head;
  while (current != NULL) {
    if (strcmp((const char *)current->client_id, (const char *)client_id) ==
        0) {
      return current;
    }

    current = current->next;
  }
  return NULL;
}

void Clients_print(Clients *list) {
  Client *current = list->head;
  printf("Clients List:\n");
  while (current != NULL) {
    printf("Client ID: %s\n", current->client_id);
    printf("Username: %s\n", current->username);
    printf("Password: %s\n", current->password);
    printf("Will Topic: %s\n", current->will_topic);
    printf("Will Message: %s\n", current->will_message);
    current = current->next;
  }
  printf("\n");
}