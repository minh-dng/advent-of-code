#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

struct numChar {
    char *numChars;
    int length;
};

int main(int argc, char *argv[]) {
    (void)argc;
    FILE *pFile = fopen(argv[1], "r");
    char buffer[500];

    struct numChar timeStruct = {.numChars = NULL, .length = 0};
    struct numChar distStruct = {.numChars = NULL, .length = 0};

    fgets(buffer, sizeof(buffer), pFile);
    for (char *pEnd = buffer; *pEnd != '\0'; pEnd++) {
        if (isdigit(*pEnd)) {
            char *tmp = realloc(timeStruct.numChars, timeStruct.length + 1);
            if (tmp == NULL) {
                printf("%s\n", "Memory Allocation Failed");
                return 1;
            }
            timeStruct.numChars = tmp;
            timeStruct.numChars[timeStruct.length++] = *pEnd;
        }
    }

    fgets(buffer, sizeof(buffer), pFile);
    for (char *pEnd = buffer; *pEnd != '\0'; pEnd++) {
        if (isdigit(*pEnd)) {
            char *tmp = realloc(distStruct.numChars, distStruct.length + 1);
            if (tmp == NULL) {
                printf("%s\n", "Memory Allocation Failed");
                return 1;
            }
            distStruct.numChars = tmp;
            distStruct.numChars[distStruct.length++] = *pEnd;
        }
    }
    fclose(pFile);

    char *tmp = realloc(timeStruct.numChars, timeStruct.length + 1);
    if (tmp == NULL) {
        printf("%s\n", "Memory Allocation Failed");
        return 1;
    }
    timeStruct.numChars = tmp;
    timeStruct.numChars[timeStruct.length++] = '\0';
    tmp = realloc(distStruct.numChars, distStruct.length + 1);
    if (tmp == NULL) {
        printf("%s\n", "Memory Allocation Failed");
        return 1;
    }
    distStruct.numChars = tmp;
    distStruct.numChars[distStruct.length++] = '\0';
    tmp = NULL;

    long long time = strtoll(timeStruct.numChars, NULL, 10);
    long long distance = strtoll(distStruct.numChars, NULL, 10);

    printf("%lld   %lld     ", time, distance);
    double lowerBound = ((-time + sqrt(pow(time, 2) - 4.0 * distance)) / -2);
    double upperBound = ((-time - sqrt(pow(time, 2) - 4.0 * distance)) / -2);
    printf("%f, %f\n", lowerBound, upperBound);
    printf("Res %f\n", (ceil(upperBound) - floor(lowerBound)) - 1);
    free(timeStruct.numChars);
    free(distStruct.numChars);

    return 0;
}