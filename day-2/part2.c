#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *extract_str(char *startSearch, char *endSearch) {
    size_t length = endSearch - startSearch + 1;
    char *temp = malloc(length + 1);
    if (temp == NULL) {
        printf("Failed to allocate memory\n");
        return NULL;
    }
    temp[length] = '\0';
    memcpy(temp, startSearch, length);
    return temp;
}

int main(int argc, char *argv[]) {
    char buffer[1024];
    int result = 0;
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Failed to open file\n");
        return 1;
    }

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        char *startSearchPtr = buffer;
        char *linePtr = buffer;

        while (*linePtr != ':') linePtr++;

        int count;
        int red = 0;
        int blue = 0;
        int green = 0;
        startSearchPtr = ++linePtr;
        for (; *linePtr != '\0'; linePtr++) {
            if (!(*linePtr == ',' || *linePtr == ';' || *linePtr == '\n' ||
                  *(linePtr + 1) == '\0')) {
                continue;
            }
            char *color = malloc(6);
            char *substr = extract_str(
                startSearchPtr, *(linePtr + 1) == '\0' && *linePtr != '\n'
                                    ? linePtr
                                    : linePtr - 1);
            sscanf(substr, " %d %s", &count, color);
            free(substr);
            if (strcmp("red", color) == 0 && red < count) {
                red = count;
            } else if (strcmp("green", color) == 0 && green < count) {
                green = count;
            } else if (strcmp("blue", color) == 0 && blue < count) {
                blue = count;
            }
            free(color);
            startSearchPtr = linePtr + 1;
        }
        result += red * green * blue;
    }
    printf("%d\n", result);
    return 0;
}