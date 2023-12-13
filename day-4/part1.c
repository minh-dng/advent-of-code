#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int is_a_num_char(char *p_char) { return (*p_char >= '0' && *p_char <= '9'); }

int main(int argc, char *argv[]) {
    FILE *p_file = fopen(argv[1], "r");
    int res = 0;
    char buffer[500];
    int tmpNum;

    while (fgets(buffer, sizeof(buffer), p_file)) {
        int *win = NULL;
        int *own = NULL;
        int winIdx = 0;
        int ownIdx = 0;
        int matchCount = 0;

        char *p_line = buffer;
        while (*p_line != ':') p_line++;
        for (; *p_line != '|'; p_line++) {
            if (!is_a_num_char(p_line)) {
                continue;
            }
            tmpNum = strtol(p_line, &p_line, 10);
            int *tmp = realloc(win, sizeof(int) * (winIdx + 1));
            if (tmp == NULL) {
                printf("%s\n", "Memory Allocation Failed");
                return 1;
            }
            win = tmp;
            win[winIdx++] = tmpNum;
        }
        for (; *p_line != '\0'; p_line++) {
            if (!is_a_num_char(p_line)) {
                continue;
            }
            tmpNum = strtol(p_line, &p_line, 10);
            int *tmp = realloc(own, sizeof(int) * (ownIdx + 1));
            if (tmp == NULL) {
                printf("%s\n", "Memory Allocation Failed");
                return 1;
            }
            own = tmp;
            own[ownIdx++] = tmpNum;
        }
        for (int ownCheckIdx = 0; ownCheckIdx < ownIdx; ownCheckIdx++) {
            for (int winCheckIdx = 0; winCheckIdx < winIdx; winCheckIdx++) {
                if (own[ownCheckIdx] == win[winCheckIdx]) {
                    matchCount++;
                    break;
                }
            }
        }
        if (matchCount) {
            res += (int)pow(2, matchCount - 1);
        }
        free(win);
        free(own);
    }
    fclose(p_file);

    printf("Res: %d\n", res);
    return 0;
}