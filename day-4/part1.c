#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int extract_int(char *p_start) {
    char* p_end = p_start;
    size_t length = p_end - p_start;
    char tmp[length + 1];
    tmp[length] = '\0';
    memcpy(tmp, p_start, length);
    return (int) strtol(tmp, (char **)NULL, 10);
}

int main(int argc, char *argv[]) {
    FILE *file = fopen(argv[1], "r");
    char buffer[500];
    int* win;
    int* own;

    while(fgets(buffer, sizeof(buffer), file)) {
        char *p_line = buffer;
        while (*p_line != ':') p_line++;
        for (;*p_line != '|'; p_line++) {

        }
    }
}