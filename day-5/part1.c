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
    struct node *headDest;
    struct node *headSrc;
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
        while (*pEnd != ' ') {
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

    for (int i = 0; i < seedCount; i++) {
        printf("Seed: %ld ", seeds[i]);
        long soil = findDest(seeds[i], seedSoil);
        long fert = findDest(soil, soilFert);
        long water = findDest(fert, fertWater);
        long light = findDest(water, waterLight);
        long temp = findDest(light, lightTemp);
        long humid = findDest(temp, tempHumid);
        long dest = findDest(humid, humidLoc);
        if (res == -1 || res > dest) {
            res = dest;
        }
        printf("Dest: %ld\n", dest);
    }

    printf("Res: %ld\n", res);
    return 0;
}