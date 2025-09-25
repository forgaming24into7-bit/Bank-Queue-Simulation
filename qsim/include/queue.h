#ifndef QUEUE_H
#define QUEUE_H

#include "customer.h"

typedef struct QueueNode {
    Customer *cust;
    struct QueueNode *next;
} QueueNode;

typedef struct TellerQueue {
    QueueNode *front;
    QueueNode *rear;
    int length;
} TellerQueue;

TellerQueue *createQueue();
void enqueue(TellerQueue *q, Customer *c);
Customer *dequeue(TellerQueue *q);

#endif
