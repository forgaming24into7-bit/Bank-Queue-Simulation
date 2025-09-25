#ifndef TELLER_H
#define TELLER_H

#include "queue.h"

typedef struct {
    int id;
    double busyUntil;
    double totalServiceTime;
    double totalIdleTime;
    TellerQueue *line;   // Each teller has a line (linked list of customers)
} Teller;

Teller *createTeller(int id);
Customer *getNextCustomer(Teller *t);

#endif
