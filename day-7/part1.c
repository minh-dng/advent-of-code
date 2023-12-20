#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char card[14] = {'A', 'K', 'Q', 'J', 'T', '9', '8',
                 '7', '6', '5', '4', '3', '2', '\0'};
enum type {
    FiveOfAKind,
    FourOfAKind,
    FullHouse,
    ThreeOfAKind,
    TwoPair,
    Pair,
    HighCard
};
// Doubly linked list
struct hand {
    char cards[6];
    int point;
    struct hand *next;
    struct hand *prev;
};

enum type findType(char handStr[6]) {
    struct charCount {
        char card;
        int count;
    };
    int uniqueCount = 0;
    struct charCount *uniqueCards = NULL;
    for (char *pChar = handStr; *pChar != '\0'; pChar++) {
        int found = 0;
        for (int i = 0; i < uniqueCount; i++) {
            if (uniqueCards[i].card == *pChar) {
                found = 1;
                uniqueCards[i].count++;
                break;
            }
        }
        if (found) {
            continue;
        }
        struct charCount *tmp =
            realloc(uniqueCards, (uniqueCount + 1) * sizeof(struct charCount));
        if (tmp == NULL) {
            printf("%s\n", "Memory Allocation Failed");
            exit(1);
        }
        uniqueCards = tmp;
        uniqueCards[uniqueCount].card = *pChar;
        uniqueCards[uniqueCount].count = 1;
        uniqueCount++;
    }
    enum type type;
    switch (uniqueCount) {
        case 1:
            type = FiveOfAKind;
            break;
        case 4:
            type = Pair;
            break;
        case 5:
            type = HighCard;
            break;
        case 2:
            for (int i = 0; i < uniqueCount; i++) {
                if (uniqueCards[i].count == 4 || uniqueCards[i].count == 1) {
                    type = FourOfAKind;
                    break;
                }
                type = FullHouse;
                break;
            }
            break;
        case 3:
            for (int i = 0; i < uniqueCount; i++) {
                if (uniqueCards[i].count == 3) {
                    type = ThreeOfAKind;
                    break;
                } else if (uniqueCards[i].count == 2) {
                    type = TwoPair;
                    break;
                }
            }
            break;
        default:
            type = -1;
            printf("%s\n", "Invalid Hand");
    }
    free(uniqueCards);
    return type;
}

void addHand(struct hand ***pType, struct hand *newHand) {
    struct hand **pHandLinkedList = *pType;
    if (pHandLinkedList == NULL) {
        // First element
        pHandLinkedList = malloc(sizeof(struct hand *));
        *pHandLinkedList = newHand;
        *pType = pHandLinkedList;
        return;
    }
    struct hand *tmp = *pHandLinkedList;
    while (tmp != NULL) {
        for (int i = 0; i < 6; i++) {
            printf("%c, %c  ", tmp->cards[i], newHand->cards[i]);
            printf("%ld, %ld\n", strchr(card, tmp->cards[i]) - card,
                   strchr(card, newHand->cards[i]) - card);
            if (strchr(card, tmp->cards[i]) - card >
                strchr(card, newHand->cards[i]) - card) {
                    printf("%s\n", "Inserting");
                // Insert when larger strength
                newHand->next = tmp;
                newHand->prev = tmp->prev;
                if (tmp->prev != NULL) {
                    tmp->prev->next = newHand;
                    tmp->prev = newHand;
                }
                // Change the head pointer of linked list
                while ((*pHandLinkedList)->prev != NULL) {
                    (*pHandLinkedList) = (*pHandLinkedList)->prev;
                }
                // printf("%sdaf", tmp->cards);
                *pType = pHandLinkedList;
                return;
            } else if (strchr(card, tmp->cards[i]) - card <
                       strchr(card, newHand->cards[i]) - card) {
                break;
            }
        }
        tmp = tmp->next;
    }
    // Insert when smallest strength
    tmp = *pHandLinkedList;
    while (tmp->next != NULL) {
        tmp = tmp->next;
    }
    tmp->next = newHand;
    newHand->prev = tmp;
}

struct hand *createHand(char handStr[6], int point) {
    struct hand *newHand = malloc(sizeof(struct hand));
    memcpy(newHand->cards, handStr, 6);
    newHand->point = point;
    newHand->next = NULL;
    newHand->prev = NULL;
    return newHand;
}

void freeHands(struct hand **hands) {
    if (hands == NULL) {
        return;
    }
    struct hand *tmp = *hands;
    while (tmp != NULL) {
        struct hand *tmpCur = tmp;
        tmp = tmp->next;
        free(tmpCur);
    }
    free(hands);
}

void calculateWinnings(int *res, int *multiplier, struct hand **hands) {
    if (hands == NULL) {
        return;
    }
    struct hand *tmp = *hands;
    while (tmp->next != NULL) {
        tmp = tmp->next;
    }
    while (tmp != NULL) {
        *res += (*multiplier)++ * tmp->point;
        tmp = tmp->prev;
    }
}

int main(int argc, char *argv[]) {
    (void)argc;
    FILE *pFile = fopen(argv[1], "r");
    char buffer[500];

    // Point the first hand
    struct hand **fiveOfAKind = NULL;
    struct hand **fourOfAKind = NULL;
    struct hand **fullHouse = NULL;
    struct hand **threeOfAKind = NULL;
    struct hand **twoPair = NULL;
    struct hand **pair = NULL;
    struct hand **highCard = NULL;

    while (fgets(buffer, sizeof(buffer), pFile)) {
        char handStr[6];
        int point;
        sscanf(buffer, "%5s %d", handStr, &point);
        printf("%s %d\n", handStr, point);
        struct hand *newHand = createHand(handStr, point);
        // enum type type = findType(handStr);
        switch (findType(handStr)) {
            case FiveOfAKind:
                addHand(&fiveOfAKind, newHand);
                break;
            case FourOfAKind:
                addHand(&fourOfAKind, newHand);
                break;
            case FullHouse:
                addHand(&fullHouse, newHand);
                break;
            case ThreeOfAKind:
                addHand(&threeOfAKind, newHand);
                break;
            case TwoPair:
                addHand(&twoPair, newHand);
                break;
            case Pair:
                addHand(&pair, newHand);
                break;
            case HighCard:
                addHand(&highCard, newHand);
                break;
            default:
                printf("%s\n", "Invalid Hand");
                break;
        }
    }
    fclose(pFile);

    // Print the first hand
    struct hand *tmp = *threeOfAKind;
    while (tmp != NULL) {
        printf("%s %d\n", tmp->cards, tmp->point);
        tmp = tmp->prev;
    }

    printf("\n");

    tmp = *threeOfAKind;
    while (tmp != NULL) {
        printf("%s %d\n", tmp->cards, tmp->point);
        tmp = tmp->next;
    }

    // Calculating the points
    int res = 0;
    int multiplier = 1;

    calculateWinnings(&res, &multiplier, highCard);
    printf("Res: %d, Mul: %d\n", res, multiplier);
    calculateWinnings(&res, &multiplier, pair);
    printf("Res: %d, Mul: %d\n", res, multiplier);
    calculateWinnings(&res, &multiplier, twoPair);
    printf("Res: %d, Mul: %d\n", res, multiplier);
    calculateWinnings(&res, &multiplier, threeOfAKind);
    printf("Res: %d, Mul: %d\n", res, multiplier);
    calculateWinnings(&res, &multiplier, fullHouse);
    // printf("Res: %d, Mul: %d\n", res, multiplier);
    calculateWinnings(&res, &multiplier, fourOfAKind);
    // printf("Res: %d, Mul: %d\n", res, multiplier);
    calculateWinnings(&res, &multiplier, fiveOfAKind);
    // printf("Res: %d, Mul: %d\n", res, multiplier);

    // Freeing hands
    freeHands(fiveOfAKind);
    freeHands(fourOfAKind);
    freeHands(fullHouse);
    freeHands(threeOfAKind);
    freeHands(twoPair);
    freeHands(pair);
    freeHands(highCard);

    printf("Res: %d\n", res);
}