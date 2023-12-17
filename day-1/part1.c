#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char buffer[1024];
    unsigned int calibrationVal = 0;
    FILE *pFile = fopen(argv[1], "r");
    if (pFile == NULL) {
        printf("Failed to open file\n");
        return 1;
    }

    while (fgets(buffer, sizeof(buffer), pFile) != NULL) {
        unsigned int first = 0;
        unsigned int last = 0;
        char *linePtr;
        for (linePtr = buffer; *linePtr != '\0'; linePtr++) {
            if (*linePtr >= '0' && *linePtr <= '9') {
                if (!first) {
                    first = *linePtr - '0';
                }
                last = *linePtr - '0';
            }
        }
        printf("%d, %d\n", first, last);
        calibrationVal += first * 10 + last;
    }
    fclose(pFile);
    printf("%d\n", calibrationVal);
    return 0;
}