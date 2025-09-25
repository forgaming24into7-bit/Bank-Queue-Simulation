#include "event.h"
#include "customer.h"
#include "teller.h"
#include "queue.h"

extern Event *eventQueue;
extern double avgService;
extern double simClock;
extern double simTime;
extern int numTellers;
extern Teller **tellers;
extern int singleQueueMode;
extern TellerQueue *globalQueue;
#include <stdlib.h>

// Stub for createEvent
Event *createEvent(double time, EventType type, void *actor, ActionFunc action) {
    Event *e = (Event *)malloc(sizeof(Event));
    if (!e) return NULL;
    e->time = time;
    e->type = type;
    e->actor = actor;
    e->action = action;
    e->next = NULL;
    return e;
}

#include "event.h"
#include "customer.h"
#include "teller.h"
#include "queue.h"

extern Event *eventQueue;
extern double avgService;
extern double simClock;
extern int numTellers;
extern Teller **tellers;
extern int singleQueueMode;
extern TellerQueue *globalQueue;
#include <stdlib.h>
// Stub for customerDeparture
void customerDeparture(Event *e) {
    // Implement departure logic here
        Customer *c = (Customer *)e->actor;
        printf("[%.2f] Customer %d DEPARTED (Time in bank: %.2f min)\n", simClock, c->id, c->departureTime - c->arrivalTime);
        double timeInBank = c->departureTime - c->arrivalTime;
        extern void recordCustomerTime(double timeInBank);
        recordCustomerTime(timeInBank);
}

// ... [statistics code stays the same] ...

// 1. Customer arrival
void customerArrival(Event *e) {
    Customer *c = (Customer *)e->actor;
    simClock = e->time;
    printf("[%.2f] Customer %d ARRIVED\n", simClock, c->id);

    if (singleQueueMode) {
        enqueue(globalQueue, c);

        // If any teller is free, start service immediately
        for (int i = 0; i < numTellers; i++) {
            if (tellers[i]->busyUntil <= simClock && globalQueue->length > 0) {
                Customer *next = dequeue(globalQueue);
                double serviceTime = 2 * avgService * rand() / (double)RAND_MAX;
                next->serviceStartTime = simClock;
                next->departureTime = simClock + serviceTime;
                tellers[i]->busyUntil = next->departureTime;
                tellers[i]->totalServiceTime += serviceTime;

                addEvent(&eventQueue,
                    createEvent(next->departureTime, CUSTOMER_DEPARTURE, next, customerDeparture));
                addEvent(&eventQueue,
                    createEvent(next->departureTime, TELLER_EVENT, tellers[i], tellerEvent));
            }
        }
    } else {
        // --- Multiple queue mode ---
        Teller *chosen = tellers[0];
        for (int i = 1; i < numTellers; i++) {
            if (tellers[i]->line->length < chosen->line->length) {
                chosen = tellers[i];
            }
        }

        enqueue(chosen->line, c);

        if (chosen->busyUntil <= simClock) {
            Customer *next = dequeue(chosen->line);
            if (next) {
                double serviceTime = 2 * avgService * rand() / (double)RAND_MAX;
                next->serviceStartTime = simClock;
                next->departureTime = simClock + serviceTime;
                chosen->busyUntil = next->departureTime;
                chosen->totalServiceTime += serviceTime;

                addEvent(&eventQueue,
                    createEvent(next->departureTime, CUSTOMER_DEPARTURE, next, customerDeparture));
                addEvent(&eventQueue,
                    createEvent(next->departureTime, TELLER_EVENT, chosen, tellerEvent));
            }
        }
    }
}

// 3. Teller event
void tellerEvent(Event *e) {
    Teller *t = (Teller *)e->actor;
    simClock = e->time;
    printf("[%.2f] Teller %d EVENT\n", simClock, t->id);

    Customer *next = NULL;

    if (singleQueueMode) {
        next = dequeue(globalQueue);
    } else {
        next = dequeue(t->line);
    }

    extern double simTime;
    if (simClock > simTime) {
        printf("[%.2f] Simulation time exceeded, teller %d will not schedule further events.\n", simClock, t->id);
        return;
    }

    if (next) {
        double serviceTime = 2 * avgService * rand() / (double)RAND_MAX;
        next->serviceStartTime = simClock;
        next->departureTime = simClock + serviceTime;
        t->busyUntil = next->departureTime;
        t->totalServiceTime += serviceTime;

        addEvent(&eventQueue,
            createEvent(next->departureTime, CUSTOMER_DEPARTURE, next, customerDeparture));
        addEvent(&eventQueue,
            createEvent(next->departureTime, TELLER_EVENT, t, tellerEvent));
    } else {
        double idle = 1 + (150.0 * rand() / (double)RAND_MAX);
        t->totalIdleTime += idle;
        addEvent(&eventQueue,
            createEvent(simClock + idle, TELLER_EVENT, t, tellerEvent));
    }
}
