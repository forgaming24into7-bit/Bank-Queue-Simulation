#include <stdlib.h>
#include "queue.h"

TellerQueue *createQueue() {
    TellerQueue *q = (TellerQueue *)malloc(sizeof(TellerQueue));
    q->front = q->rear = NULL;
    q->length = 0;
    printf("[QUEUE] Created new queue at %p\n", q);
    return q;
}

void enqueue(TellerQueue *q, Customer *c) {
    QueueNode *node = (QueueNode *)malloc(sizeof(QueueNode));
    node->cust = c;
    node->next = NULL;
    if (q->rear == NULL) {
        q->front = q->rear = node;
    } else {
        q->rear->next = node;
        q->rear = node;
    }
    q->length++;
    printf("[QUEUE] Enqueued customer %d to queue %p (length=%d)\n", c->id, q, q->length);
}

Customer *dequeue(TellerQueue *q) {
    if (q->front == NULL) return NULL;
    QueueNode *temp = q->front;
    Customer *c = temp->cust;
    q->front = q->front->next;
    if (q->front == NULL) q->rear = NULL;
    free(temp);
    q->length--;
    printf("[QUEUE] Dequeued customer %d from queue %p (length=%d)\n", c->id, q, q->length);
    return c;
}
