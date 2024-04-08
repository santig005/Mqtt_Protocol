#include "topic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "subscription.h"
struct topic *create_topic(uint8_t *name) {
  struct topic *t = (struct topic *)malloc(sizeof(struct topic));
  t->name = name;
  t->retained = false;
  t->next = NULL;
  return t;
}
/*
struct topic *add_topic(struct topic **head, uint8_t *name) {
  struct topic *t = create_topic(name);
  struct topic *current = *head;
  if (current != NULL) {
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = t;
  } else {
    *head = t;
  }
  return t;
}*/
struct topic *add_subtopic(struct topic **head, uint8_t *name) {
  struct topic *t = create_topic(name);
  struct topic *current = (*head)->subtopics;
  if (current != NULL) {
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = t;
  } else {
    (*head)->subtopics = t;
  }

  return t;
}

void print_topic(int level, struct topic *head) {
  struct topic *current = head;
  while (current != NULL) {
    for (int i = 0; i < level; i++) {
      printf("  ");
    }
    printf("Topic: %s\n", current->name);
    print_topic(level + 1, current->subtopics);
    current = current->next;
  }
}
void Topic_add_subscription(struct topic * t, struct subscription * s) {
  struct subscription * current = t->subscriptions;
  if (current != NULL) {
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = s;
  } else {
    t->subscriptions = s;
  }
}
struct topic *search_topic(struct topic *t, uint8_t *name) {
  uint8_t *token = (uint8_t *)strtok((char *)name, "/");
  struct topic *current = (t);
  struct topic *level=current;
  while (token != NULL) {
    struct topic * found=NULL;
    if(current->subtopics!=NULL){
      current=current->subtopics;
      struct topic *found = NULL;
      while (current != NULL) {
        if (strcmp((const char *)current->name, (const char *)token) == 0) {
          found = current;
          break;
        }
        current = current->next;
      }
      if(found==NULL){
        add_subtopic(&level, token);
      }
    }
    else{
      current=add_subtopic(&current, token);
    }
    level=current;
    token = strtok(NULL, "/");
  }
}


