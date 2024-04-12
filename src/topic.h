#ifndef TOPIC_H
#define TOPIC_H
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "subscription.h"

struct topic {
  uint8_t *name;
  bool retained;
  uint8_t *retained_message;
  struct topic *next;
  struct topic *subtopics;
  struct subscription * subscriptions;
};

struct topic *create_topic(uint8_t *name);
//struct topic * add_topic(struct topic **t, uint8_t *name);
struct topic *add_subtopic(struct topic **t, uint8_t *name);
void print_topic(int level, struct topic *t);
void Topic_add_subscription(struct topic ** t, struct subscription * s);
void add_retained_message(struct topic *t, uint8_t *message);
struct topic *search_topic(struct topic *t, uint8_t *name);

#endif // TOPIC_H
