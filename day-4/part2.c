#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int is_a_num_char(char *p_char) { return (*p_char >= '0' && *p_char <= '9'); }

int extract_int(char **p_start) {
    char *p_end = *p_start;
    while (is_a_num_char(p_end)) p_end++;
    size_t length = p_end - *p_start;
    char tmp[length + 1];
    tmp[length] = '\0';
    memcpy(tmp, *p_start, length);
    *p_start = p_end;
    return (int)strtol(tmp, (char **)NULL, 10);
}

int main(int argc, char *argv[]) {
    FILE *file = fopen(argv[1], "r");
    int res = 0;
    char buffer[500];
    int tmpNum;
    int cardQuantity = 0;
    // Initialise first list elemenent as 1
    int *cardInstancesCount = NULL;

    for (int cardIdx = 0; fgets(buffer, sizeof(buffer), file); cardIdx++) {
        int *win = NULL;
        int *own = NULL;
        int winIdx = 0;
        int ownIdx = 0;
        int matchCount = 0;

        char *p_line = buffer;
        // Skip to ':'
        while (*p_line != ':') p_line++;

        // Load list of winning numbers
        for (; *p_line != '|'; p_line++) {
            if (!is_a_num_char(p_line)) {
                continue;
            }
            tmpNum = extract_int(&p_line);
            int *tmp = realloc(win, sizeof(int) * (winIdx + 1));
            if (tmp == NULL) {
                printf("%s\n", "Memory Allocation Failed");
                return 1;
            }
            win = tmp;
            tmp = NULL;
            win[winIdx++] = tmpNum;
        }

        // Load list of own numbers
        for (; *p_line != '\0'; p_line++) {
            if (!is_a_num_char(p_line)) {
                continue;
            }
            tmpNum = extract_int(&p_line);
            int *tmp = realloc(own, sizeof(int) * (ownIdx + 1));
            if (tmp == NULL) {
                printf("%s\n", "Memory Allocation Failed");
                return 1;
            }
            own = tmp;
            tmp = NULL;
            own[ownIdx++] = tmpNum;
        }

        // Check how many matchings
        for (int ownCheckIdx = 0; ownCheckIdx < ownIdx; ownCheckIdx++) {
            for (int winCheckIdx = 0; winCheckIdx < winIdx; winCheckIdx++) {
                if (own[ownCheckIdx] == win[winCheckIdx]) {
                    matchCount++;
                    break;
                }
            }
        }
        free(win);
        free(own);

        if (cardQuantity == cardIdx) {
            printf("%p\n", &cardInstancesCount);
            int *tmp =
                realloc(cardInstancesCount, sizeof(int) * (cardQuantity + 1));
            if (tmp = NULL) {
                printf("%s\n", "Memory Allocation Failed");
                return 1;
            }
            cardInstancesCount = tmp;
            printf("%p\n", &cardInstancesCount);
            cardInstancesCount[cardIdx] = 1;
            printf("%s\n", "no");
            printf("%d",*cardInstancesCount);
            tmp = NULL;
        }

        // If need to expand cardQuantity
        int newCardQuantity = cardIdx + matchCount + 1;
        if (newCardQuantity > cardQuantity) {
            int *tmp =
                realloc(cardInstancesCount, sizeof(int) * newCardQuantity);
            if (tmp == NULL) {
                printf("%s\n", "Memory Allocation Failed");
                return 1;
            } else {
                cardInstancesCount = tmp;
            }

            // Initialise all 1 for new card discovery
            for (int i = cardQuantity; i < newCardQuantity; i++) {
                cardInstancesCount[i] = 1;
            }
            cardQuantity = newCardQuantity;

            // Add current instance to the cards
            for (int i = cardIdx + 1; i < newCardQuantity; i++) {
                cardInstancesCount[i] += cardInstancesCount[cardIdx];
            }
            // Set newCardQuantity
            cardQuantity = newCardQuantity;
        }
        // If no cardQuantity expansion.
        else {
            for (int i = cardIdx + 1; i < newCardQuantity; i++) {
                cardInstancesCount[i] += cardInstancesCount[cardIdx];
            }
        }
    }
    for (int i = 0; i < cardQuantity; i++) {
        printf("%d", cardInstancesCount[i]);
    }
    free(cardInstancesCount);
    printf("Res: %d\n", res);
    return 0;
}