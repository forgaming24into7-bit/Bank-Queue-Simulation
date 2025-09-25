#include "customer.h"
static Customer *customerList[MAX_CUSTOMERS];
static int customerListCount = 0;

Customer *createCustomer(int id, double arrivalTime) {
    Customer *c = (Customer *)malloc(sizeof(Customer));
    c->id = id;
    c->arrivalTime = arrivalTime;
    c->serviceStartTime = 0.0;
    c->departureTime = 0.0;
    if (customerListCount < MAX_CUSTOMERS) {
        customerList[customerListCount++] = c;
    }
    return c;
}

Customer *getCustomerByIndex(int idx) {
    if (idx < 0 || idx >= customerListCount) return NULL;
    return customerList[idx];
}
#include <stdlib.h>
#include "customer.h"

Customer *createCustomer(int id, double arrivalTime) {
    Customer *c = (Customer *)malloc(sizeof(Customer));
    c->id = id;
    c->arrivalTime = arrivalTime;
    c->serviceStartTime = 0;
    c->departureTime = 0;
    return c;
}
