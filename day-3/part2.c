#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char buffer[200];
    char *input = NULL;
    int result = 0;
    // COL_COUNT does include '\n'
    int COL_COUNT;
    int row;

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Failed to open file\n");
        return 1;
    }

    for (row = 0; fgets(buffer, sizeof(buffer), file) != NULL; row++) {
        if (!COL_COUNT) COL_COUNT = (int) strlen(buffer);
        printf("%ld\n", (size_t) COL_COUNT * (row + 1));
        char *tmp = realloc(input, (size_t) COL_COUNT * (row + 1));
        if (tmp == NULL) {
            return 1;
        } else {
            input = tmp;
        }
        char *p_inputIdx = input + row * COL_COUNT;

        for (char *linePtr = buffer; *linePtr != '\0'; linePtr++) {
            *(p_inputIdx++) = *linePtr;
        }
    }

    for (int r = 0; r < row; r++) {
        for (int c = 0; c < COL_COUNT; c++) {
            printf("%c", *(input + (r * COL_COUNT + c)));
        }
    }
    free(input);
    return 0;
}