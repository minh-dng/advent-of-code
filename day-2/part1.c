#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RED_LIMIT 12
#define GREEN_LIMIT 13
#define BLUE_LIMIT 14

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
        char *linePtr;

        int game_number;
        for (linePtr = buffer; *linePtr != ':'; linePtr++) {
            if (*linePtr >= '0' && *linePtr <= '9' &&
                startSearchPtr != buffer) {
                startSearchPtr = linePtr;
            }
        }

        char *substr = extract_str(startSearchPtr, linePtr);

        sscanf(substr, "Game %d:", &game_number);
        printf("Game %d\n", game_number);
        free(substr);

        int count;
        int red = 0;
        int blue = 0;
        int green = 0;
        bool valid_game = true;
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
            printf("%s\n", substr);
            sscanf(substr, " %d %s", &count, color);
            free(substr);
            if (strcmp("red", color) == 0) {
                red += count;
            } else if (strcmp("green", color) == 0) {
                green += count;
            } else if (strcmp("blue", color) == 0) {
                blue += count;
            }
            free(color);
            if (*linePtr == ';' || *(linePtr + 1) == '\0') {
                if (red > RED_LIMIT || blue > BLUE_LIMIT ||
                    green > GREEN_LIMIT) {
                    valid_game = false;
                    break;
                }
                red = 0;
                blue = 0;
                green = 0;
            }
            startSearchPtr = linePtr + 1;
        }
        if (valid_game) {
            result += game_number;
        }
    }
    printf("%d\n", result);
    return 0;
}