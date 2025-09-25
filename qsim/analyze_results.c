#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    FILE *fp = fopen("output/results.csv", "r");
    if (!fp) {
        perror("Error opening results.csv");
        return 1;
    }

    char line[256];
    int count = 0;
    double avgSum = 0, maxMax = 0, stdSum = 0;
    printf("Customers,Tellers,Mode,AvgTime,StdDev,MaxTime\n");
    while (fgets(line, sizeof(line), fp)) {
        int customers, tellers;
        char mode[16];
        double avg, std, max;
        if (sscanf(line, "%d,%d,%15[^,],%lf,%lf,%lf", &customers, &tellers, mode, &avg, &std, &max) == 6) {
            printf("%s", line);
            avgSum += avg;
            stdSum += std;
            if (max > maxMax) maxMax = max;
            count++;
        }
    }
    fclose(fp);
    if (count > 0) {
        printf("\nSummary over %d runs:\n", count);
        printf("Average of AvgTime: %.2f\n", avgSum / count);
        printf("Average of StdDev: %.2f\n", stdSum / count);
        printf("Maximum MaxTime: %.2f\n", maxMax);
    } else {
        printf("No valid data found.\n");
    }
    return 0;
}
