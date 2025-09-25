#include <stdlib.h>
#include "teller.h"

Teller *createTeller(int id) {
    Teller *t = (Teller *)malloc(sizeof(Teller));
    t->id = id;
    t->busyUntil = 0;
    t->totalServiceTime = 0;
    t->totalIdleTime = 0;
    t->line = createQueue();
    return t;
}

Customer *getNextCustomer(Teller *t) {
    return dequeue(t->line);
}
