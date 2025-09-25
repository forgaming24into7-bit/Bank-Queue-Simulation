#ifndef CUSTOMER_H
#define CUSTOMER_H

typedef struct {
    int id;
    double arrivalTime;
    double serviceStartTime;
    double departureTime;
} Customer;

Customer *createCustomer(int id, double arrivalTime);

#endif
