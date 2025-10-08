#include <stdio.h>
#include "logger.h"

void logResultsCSV(const char *filename,
                   int numCustomers, int numTellers, int mode,
                   double avgTime, double stdDev, double maxTime)
{
    FILE *fp = fopen(filename, "a");
    if (!fp)
    {
        perror("Error opening CSV file");
        return;
    }

    // CSV format: Customers, Tellers, Mode, AvgTime, StdDev, MaxTime
    fprintf(fp, "%d,%d,%s,%.2f,%.2f,%.2f\n",
            numCustomers, numTellers,
            mode == 1 ? "SINGLE" : "MULTIPLE",
            avgTime, stdDev, maxTime);

    fclose(fp);
}
