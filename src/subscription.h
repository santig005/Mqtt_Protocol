#ifndef SUBSCRIPTION_H
#define SUBSCRIPTION_H
#include <stdint.h>
#include "basics.h"
struct subscription{
    uint8_t qos;
    uint8_t * topic;
    struct subscription * next;
    struct subscriber* subscriber;

};
struct subscriber{
    struct client_session * session;
};


void add_subscription(struct subscription * head, struct client_session * session, uint8_t qos, uint8_t * topic_name);



#endif // SUBSCRIPTION_H