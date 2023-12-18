#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    (void)argc;
    FILE *pFile = fopen(argv[1], "r");
    int res = 1;
    char buffer[500];

    fgets(buffer, sizeof(buffer), pFile);
    strchr(buffer, '\n');

    printf("Res: %d\n", res);
    return 0;
}