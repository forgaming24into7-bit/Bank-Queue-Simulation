#ifndef EVENT_H
#define EVENT_H

#include <stdio.h>

typedef enum { CUSTOMER_ARRIVAL, CUSTOMER_DEPARTURE, TELLER_EVENT } EventType;

struct Event;
typedef void (*ActionFunc)(struct Event *);  // Function pointer for actions

typedef struct Event {
    double time;            // Event time
    EventType type;         // Event type
    void *actor;            // Pointer to Customer or Teller
    ActionFunc action;      // Function pointer for event action
    struct Event *next;     // For event queue (linked list)
} Event;

// Event action prototypes
void customerArrival(Event *e);
void customerDeparture(Event *e);
Event *createEvent(double time, EventType type, void *actor, ActionFunc action);
void tellerEvent(Event *e);

void addEvent(Event **head, Event *newEvent);   // Insert into queue sorted by time
Event *popEvent(Event **head);                  // Pop earliest event
// --- Statistics functions ---
int getTotalCustomers();
double getAvgTime();
double getStdDev();
double getMaxTime();

#endif
