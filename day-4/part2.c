#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    FILE *p_file = fopen(argv[1], "r");
    int res = 0;
    char buffer[500];
    int tmpNum;
    int cardQuantity = 0;
    int *cardInstancesCount = NULL;

    for (int cardIdx = 0; fgets(buffer, sizeof(buffer), p_file); cardIdx++) {
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
            if (!isdigit(*p_line)) {
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

        // Load list of own numbers
        for (; *p_line != '\0'; p_line++) {
            if (!isdigit(*p_line)) {
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
            int *tmp =
                realloc(cardInstancesCount, sizeof(int) * (++cardQuantity));
            if (tmp == NULL) {
                printf("%s\n", "Memory Allocation Failed");
                return 1;
            }
            cardInstancesCount = tmp;
            cardInstancesCount[cardIdx] = 1;
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
    fclose(p_file);

    for (int i = 0; i < cardQuantity; i++) {
        res += cardInstancesCount[i];
    }
    free(cardInstancesCount);

    printf("Res: %d\n", res);
    return 0;
}