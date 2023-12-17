#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    (void)argc;
    FILE *pFile = fopen(argv[1], "r");
    int res = 1;
    char buffer[500];

    int raceCount = 0;
    int *time = NULL;
    int *distance = NULL;

    fgets(buffer, sizeof(buffer), pFile);
    char *pEnd = buffer;
    while (1) {
        while (*pEnd != ' ' && *pEnd != '\0') {
            pEnd++;
        }
        int tmpNum = strtol(pEnd, &pEnd, 10);
        if (tmpNum == 0) {
            break;
        }
        int *tmp = realloc(time, sizeof(int) * (raceCount + 1));
        if (tmp == NULL) {
            printf("%s\n", "Memory Allocation Failed");
            return 1;
        }
        time = tmp;
        time[raceCount++] = tmpNum;
    }

    fgets(buffer, sizeof(buffer), pFile);
    pEnd = buffer;
    distance = malloc(sizeof(int) * raceCount);
    while (*pEnd != ' ' && *pEnd != '\0') {
        pEnd++;
    }
    for (int i = 0; i < raceCount; i++) {
        distance[i] = strtol(pEnd, &pEnd, 10);
    }
    fclose(pFile);

    for (int i = 0; i < raceCount; i++) {
        printf("%d   %d     ", time[i], distance[i]);
        double lowerBound =
            ((-time[i] + sqrt(pow(time[i], 2) - 4.0 * distance[i])) / -2);
        double upperBound =
            ((-time[i] - sqrt(pow(time[i], 2) - 4.0 * distance[i])) / -2);
        printf("%f, %f\n", lowerBound, upperBound);
        res *= (int)(ceil(upperBound) - floor(lowerBound)) - 1;
        printf("%d\n", (int)(ceil(upperBound) - floor(lowerBound)) - 1);
    }
    free(distance);
    free(time);

    printf("Res: %d\n", res);
    return 0;
}