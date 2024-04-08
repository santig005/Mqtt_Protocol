#include "clientslist.h"
#include <string.h>
#include <stdlib.h>

Clients * Clients_newList() {
    Clients *newList = (Clients*)malloc(sizeof(Clients));
    newList->head = NULL;
    return newList;
}

void Clients_add(Clients *list, struct connect *cn) {
  Client *new_client = (Client *)malloc(sizeof(Client));
  new_client->client_id = cn->payload.client_id;
  new_client->username = cn->payload.username;
  new_client->password = cn->payload.password;
  struct client_session *session = (struct client_session *)malloc(sizeof(struct client_session));
  session->client_id = cn->payload.client_id;
  session->will_topic = cn->payload.will_topic;
  session->will_message = cn->payload.will_message;
  session->connected = true;
  new_client->session = session;
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
    printf("Will Topic: %s\n", current->session->will_topic);
    printf("Will Message: %s\n", current->session->will_message);
    current = current->next;
  }
  printf("\n");
}