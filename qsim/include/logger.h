#ifndef LOGGER_H
#define LOGGER_H

void logResultsCSV(const char *filename,
                   int numCustomers, int numTellers, int mode,
                   double avgTime, double stdDev, double maxTime);

#endif
