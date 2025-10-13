// Helper to get wait time for a customer by index
#include "customer.h"
extern Customer *getCustomerByIndex(int idx);
double getCustomerWaitTime(int idx)
{
    Customer *c = getCustomerByIndex(idx);
    if (!c)
        return 0.0;
    return c->serviceStartTime - c->arrivalTime;
}
#include <math.h>
// Statistics tracking
#define MAX_CUSTOMERS 1000
static double customerTimes[MAX_CUSTOMERS];
static int customerCount = 0;
static double customerWaitTimes[MAX_CUSTOMERS];
static double maxWaitTime = 0.0;

void recordCustomerTime(double timeInBank)
{
    if (customerCount < MAX_CUSTOMERS)
    {
        customerTimes[customerCount] = timeInBank;
        // Wait time = serviceStartTime - arrivalTime
        extern double getCustomerWaitTime(int idx);
        double waitTime = getCustomerWaitTime(customerCount);
        customerWaitTimes[customerCount] = waitTime;
        if (waitTime > maxWaitTime)
            maxWaitTime = waitTime;
        customerCount++;
    }
}

int getTotalCustomers()
{
    return customerCount;
}

double getAvgTime()
{
    if (customerCount == 0)
        return 0.0;
    double sum = 0.0;
    for (int i = 0; i < customerCount; i++)
    {
        sum += customerTimes[i];
    }
    return sum / customerCount;
}

double getStdDev()
{
    if (customerCount == 0)
        return 0.0;
    double avg = getAvgTime();
    double sumSq = 0.0;
    for (int i = 0; i < customerCount; i++)
    {
        double diff = customerTimes[i] - avg;
        sumSq += diff * diff;
    }
    return sqrt(sumSq / customerCount);
}

double getMaxTime()
{
    if (customerCount == 0)
        return 0.0;
    double max = customerTimes[0];
    for (int i = 1; i < customerCount; i++)
    {
        if (customerTimes[i] > max)
            max = customerTimes[i];
    }
    return max;
}
double getMaxWaitTime()
{
    return maxWaitTime;
}
// Stub implementations for missing statistics functions
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "event.h"
#include "customer.h"
#include "teller.h"
#include "queue.h"
#include "logger.h"

void customerArrival(Event *e); // Forward declaration

Event *eventQueue = NULL;
double avgService;
double simClock = 0;
int numTellers;
Teller **tellers;
int singleQueueMode = 0;         // 0 = multiple queues, 1 = single queue
TellerQueue *globalQueue = NULL; // Used in single queue mode
double simTime;

int main(int argc, char *argv[])
{
    printf("Simulation starting...\n");
    if (argc != 6)
    {
        printf("Usage: %s #customers #tellers simulationTime avgServiceTime mode(0=multiQ,1=singleQ)\n", argv[0]);
        return 1;
    }

    int numCustomers = atoi(argv[1]);
    numTellers = atoi(argv[2]);
    simTime = atof(argv[3]);
    avgService = atof(argv[4]);
    singleQueueMode = atoi(argv[5]);

    srand(time(NULL));

    // Create tellers
    tellers = (Teller **)malloc(numTellers * sizeof(Teller *));
    for (int i = 0; i < numTellers; i++)
    {
        tellers[i] = createTeller(i);
    }

    // If single queue mode, create one global line
    if (singleQueueMode)
    {
        globalQueue = createQueue();
    }

    // Generate customer arrivals
    for (int i = 0; i < numCustomers; i++)
    {
        double arrTime = simTime * rand() / (double)RAND_MAX;
        Customer *c = createCustomer(i, arrTime);
        addEvent(&eventQueue, createEvent(arrTime, CUSTOMER_ARRIVAL, c, customerArrival));
    }

    // Schedule initial teller events
    for (int i = 0; i < numTellers; i++)
    {
        addEvent(&eventQueue, createEvent(0.0, TELLER_EVENT, tellers[i], tellerEvent));
    }

    // Debug: count events scheduled
    int eventCount = 0;
    Event *tmp = eventQueue;
    while (tmp)
    {
        eventCount++;
        tmp = tmp->next;
    }
    printf("Events scheduled after setup: %d\n", eventCount);

    if (!eventQueue)
    {
        printf("Event queue is empty before simulation loop!\n");
    }

    // Event loop
    Event *curr;
    while ((curr = popEvent(&eventQueue)) != NULL)
    {
        printf("Processing event: type=%d, time=%.2f, actor=%p\n", curr->type, curr->time, curr->actor);
        curr->action(curr);
        free(curr);
    }

    // --- Simulation summary ---
    printf("\n--- Simulation Results (%s queue mode) ---\n",
           singleQueueMode ? "SINGLE" : "MULTIPLE");
    printf("Total customers: %d\n", getTotalCustomers());
    printf("Average time in bank: %.2f\n", getAvgTime());
    printf("Std. deviation: %.2f\n", getStdDev());
    printf("Maximum time in bank: %.2f\n", getMaxTime());
    printf("Total simulation time: %.2f\n", simTime);
    printf("Maximum wait time: %.2f\n", getMaxWaitTime());

    for (int i = 0; i < numTellers; i++)
    {
        Teller *t = tellers[i];
        double util = t->totalServiceTime / (t->totalServiceTime + t->totalIdleTime);
        printf("Teller %d: Service=%.2f, Idle=%.2f, Utilization=%.2f%%\n",
               t->id, t->totalServiceTime, t->totalIdleTime, util * 100.0);
    }

    // --- Log results to CSV ---
    printf("Calling logResultsCSV...\n");
    logResultsCSV("output/results.csv",
                  numCustomers, numTellers, singleQueueMode,
                  getAvgTime(), getStdDev(), getMaxTime());

    return 0;
}
