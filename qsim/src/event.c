#include <stdlib.h>
#include "event.h"

void addEvent(Event **head, Event *newEvent) {
    if (*head == NULL || (*head)->time > newEvent->time) {
        newEvent->next = *head;
        *head = newEvent;
        return;
    }
    Event *curr = *head;
    while (curr->next != NULL && curr->next->time <= newEvent->time) {
        curr = curr->next;
    }
    newEvent->next = curr->next;
    curr->next = newEvent;
}

Event *popEvent(Event **head) {
    if (*head == NULL) return NULL;
    Event *first = *head;
    *head = (*head)->next;
    return first;
}
