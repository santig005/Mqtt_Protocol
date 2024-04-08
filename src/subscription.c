#include "subscription.h"

void add_subscription(struct subscription * head, struct client_session * session, uint8_t qos, uint8_t * topic_name){
    struct subscription *s = (struct subscription *)malloc(sizeof(struct subscription));
    s->qos = qos;
    s->subscriber = (struct subscriber *)malloc(sizeof(struct subscriber));
    s->subscriber->session = session;
    s->topic = topic_name;
    struct subscription *current = head;
    while(current->next!=NULL){
        current = current->next;
    }
    current->next = s;
}