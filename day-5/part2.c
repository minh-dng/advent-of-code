#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct range {
    long min;
    long max;
};
// Linked list of ranges
struct node {
    struct range data;
    struct node *next;
};
// 2 Linked lists of ranges
struct type {
    struct node *headSrc;
    struct node *headDest;
};

long findDest(long src, struct type *map) {
    struct node *pDestNode = map->headDest;
    struct node *pSrcNode = map->headSrc;
    long dest = -1;
    while (pSrcNode != NULL) {
        if (src >= pSrcNode->data.min && src <= pSrcNode->data.max) {
            dest = pDestNode->data.min + (src - pSrcNode->data.min);
            break;
        }
        pSrcNode = pSrcNode->next;
        pDestNode = pDestNode->next;
    }
    // No mapping found
    if (dest == -1) {
        dest = src;
    }
    return dest;
}

void findArrayOfSmallest(long min, long max, long *res,
                         struct type **typeArr[7], int typeIdx) {
    printf("\nMin: %ld Max: %ld\n", min, max);
    if (typeIdx == 7) {
        if (min < *res || *res == -1) {
            *res = min;
        }
        printf("\nEND SEARCH MIN: %ld\n", min);
        return;
    }
    struct node *pSrcNode = (*(typeArr[typeIdx]))->headSrc;
    struct node *pDestNode = (*(typeArr[typeIdx]))->headDest;
    while (pSrcNode != NULL) {
        printf("%ld  ", pSrcNode->data.min);
        if (min >= pSrcNode->data.min && max <= pSrcNode->data.max) {
            // In range
            findArrayOfSmallest(findDest(min, (*(typeArr[typeIdx]))),
                                findDest(max, (*(typeArr[typeIdx]))), res,
                                typeArr, typeIdx + 1);
            return;
        } else if (min >= pSrcNode->data.min && min <= pSrcNode->data.max &&
                   max > pSrcNode->data.max) {
            printf("\n%s", "🪓 Min in range, max out of range");
            findArrayOfSmallest(
                findDest(min, (*(typeArr[typeIdx]))),
                findDest(pSrcNode->data.max, (*(typeArr[typeIdx]))), res,
                typeArr, typeIdx + 1);
            findArrayOfSmallest(pSrcNode->data.max + 1, max, res, typeArr,
                                typeIdx);
            return;
        } else if (min < pSrcNode->data.min && max <= pSrcNode->data.max &&
                   max >= pSrcNode->data.min) {
            printf("\n%s", "🪓 Min out of range, max in range");
            findArrayOfSmallest(min, pSrcNode->data.min - 1, res, typeArr,
                                typeIdx);
            findArrayOfSmallest(
                findDest(pSrcNode->data.min, (*(typeArr[typeIdx]))),
                findDest(max, (*(typeArr[typeIdx]))), res, typeArr,
                typeIdx + 1);
            return;
        } else if (min < pSrcNode->data.min && max > pSrcNode->data.max) {
            printf("\n%s", "Min and max out of range, range in the middle🪓");
            findArrayOfSmallest(min, pSrcNode->data.min - 1, res, typeArr,
                                typeIdx);
            findArrayOfSmallest(
                findDest(pSrcNode->data.min, (*(typeArr[typeIdx]))),
                findDest(pSrcNode->data.max, (*(typeArr[typeIdx]))), res,
                typeArr, typeIdx + 1);
            findArrayOfSmallest(pSrcNode->data.max + 1, max, res, typeArr,
                                typeIdx);
            return;
        } else {
            pDestNode = pDestNode->next;
            pSrcNode = pSrcNode->next;
        }
    }
    findArrayOfSmallest(findDest(min, (*(typeArr[typeIdx]))),
                        findDest(max, (*(typeArr[typeIdx]))), res, typeArr,
                        typeIdx + 1);
    return;
}

void freeType(struct type *type) {
    struct node *pDestNode = type->headDest;
    struct node *pSrcNode = type->headSrc;
    while (pDestNode != NULL) {
        struct node *tmp = pDestNode;
        pDestNode = pDestNode->next;
        free(tmp);
    }
    while (pSrcNode != NULL) {
        struct node *tmp = pSrcNode;
        pSrcNode = pSrcNode->next;
        free(tmp);
    }
    free(type);
}

int main(int argc, char *argv[]) {
    FILE *p_file = fopen(argv[1], "r");
    long res = -1;
    char buffer[500];
    long *seeds = NULL;
    int seedCount = 0;

    struct type *seedSoil = NULL;
    struct type *soilFert = NULL;
    struct type *fertWater = NULL;
    struct type *waterLight = NULL;
    struct type *lightTemp = NULL;
    struct type *tempHumid = NULL;
    struct type *humidLoc = NULL;

    fgets(buffer, sizeof(buffer), p_file);
    char *pEnd = buffer;
    while (1) {
        while (*pEnd != ' ' && *pEnd != '\0') {
            pEnd++;
        }
        long tmpNum = strtol(pEnd, &pEnd, 10);
        if (tmpNum == 0) {
            break;
        }
        long *tmp = realloc(seeds, sizeof(long int) * (seedCount + 1));
        if (tmp == NULL) {
            printf("%s\n", "Memory Allocation Failed");
            return 1;
        }
        seeds = tmp;
        seeds[seedCount++] = tmpNum;
    }

    struct type **map;
    while (fgets(buffer, sizeof(buffer), p_file)) {
        char *pLine = buffer;
        if (*pLine == '\n') continue;
        if (strcmp("seed-to-soil map:\n", buffer) == 0) {
            map = &seedSoil;
        } else if (strcmp("soil-to-fertilizer map:\n", buffer) == 0) {
            map = &soilFert;
        } else if (strcmp("fertilizer-to-water map:\n", buffer) == 0) {
            map = &fertWater;
        } else if (strcmp("water-to-light map:\n", buffer) == 0) {
            map = &waterLight;
        } else if (strcmp("light-to-temperature map:\n", buffer) == 0) {
            map = &lightTemp;
        } else if (strcmp("temperature-to-humidity map:\n", buffer) == 0) {
            map = &tempHumid;
        } else if (strcmp("humidity-to-location map:\n", buffer) == 0) {
            map = &humidLoc;
        } else {
            // Start parsing
            long dest = strtol(pLine, &pLine, 10);
            long src = strtol(pLine, &pLine, 10);
            long range = strtol(pLine, &pLine, 10);
            if (*map == NULL) {
                (*map) = malloc(sizeof(struct type));
                (*map)->headDest = malloc(sizeof(struct node));
                (*map)->headDest->data.min = dest;
                (*map)->headDest->data.max = dest + range - 1;
                (*map)->headDest->next = NULL;

                (*map)->headSrc = malloc(sizeof(struct node));
                (*map)->headSrc->data.min = src;
                (*map)->headSrc->data.max = src + range - 1;
                (*map)->headSrc->next = NULL;
            } else {
                // Append to the end of the linked list
                struct node *pDestNode = (*map)->headDest;
                while (pDestNode->next != NULL) {
                    pDestNode = pDestNode->next;
                }
                pDestNode->next = malloc(sizeof(struct node));
                pDestNode->next->data.min = dest;
                pDestNode->next->data.max = dest + range - 1;
                pDestNode->next->next = NULL;
                struct node *pSrcNode = (*map)->headSrc;
                while (pSrcNode->next != NULL) {
                    pSrcNode = pSrcNode->next;
                }
                pSrcNode->next = malloc(sizeof(struct node));
                pSrcNode->next->data.min = src;
                pSrcNode->next->data.max = src + range - 1;
                pSrcNode->next->next = NULL;
            }
        }
    }
    fclose(p_file);

    struct type **typeArr[7] = {&seedSoil,  &soilFert,  &fertWater, &waterLight,
                                &lightTemp, &tempHumid, &humidLoc};

    for (int i = 0; i < seedCount - 1; i += 2) {
        printf("🌱Seed: %ld", seeds[i]);
        findArrayOfSmallest(seeds[i], seeds[i] + seeds[i + 1] - 1, &res,
                            typeArr, 0);
    }
    free(seeds);
    for (int i = 0; i < 7; i++) {
        freeType(*(typeArr[i]));
    }

    printf("Res: %ld\n", res);
    return 0;
}
