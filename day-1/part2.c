#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char buffer[1024];
    unsigned int calibrationVal = 0;
    const char *numStr[9] = {"one", "two",   "three", "four", "five",
                             "six", "seven", "eight", "nine"};

    FILE *pFile = fopen(argv[1], "r");
    if (pFile == NULL) {
        printf("Failed to open file\n");
        return 1;
    }

    while (fgets(buffer, sizeof(buffer), pFile) != NULL) {
        unsigned int first = 0;
        unsigned int last = 0;
        char *startSearchPtr = buffer;
        char *linePtr;
        for (linePtr = buffer; *linePtr != '\0'; linePtr++) {
            if (*linePtr >= '0' && *linePtr <= '9') {
                if (!first) {
                    first = *linePtr - '0';
                }
                last = *linePtr - '0';
                startSearchPtr = linePtr + 1;
            } else {
                // https://stackoverflow.com/q/69735716/14046889
                // Get the length of the sub-string
                size_t length = linePtr - startSearchPtr + 1;
                if (length < 3) {
                    continue;
                }

                // Create an array for the sub-string
                char temp[length + 1];

                // Terminate it
                temp[length] = '\0';

                // Copy the sub-string
                memcpy(temp, startSearchPtr, length);

                for (int i = 0; i < 9; i++) {
                    char *chrPtr = strstr(temp, numStr[i]);
                    if (chrPtr != NULL) {
                        if (!first) {
                            first = i + 1;
                        }
                        last = i + 1;
                        startSearchPtr = linePtr;
                        break;
                    }
                }
            }
        }
        printf("%d, %d\n", first, last);
        calibrationVal += first * 10 + last;
    }
    fclose(pFile);
    printf("%d\n", calibrationVal);
    return 0;
}