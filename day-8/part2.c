#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
    char name[4];
    struct node *left;
    struct node *right;
};

struct nodeContainer {
    struct node **nodesArray;
    int nodeCount;
};

struct node *createNode(char *value) {
    struct node *newNode = malloc(sizeof(struct node));
    strcpy(newNode->name, value);
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

void replaceLeft(struct node *root, struct node *replacement) {
    free(root->left);
    root->left = replacement;
}

void replaceRight(struct node *root, struct node *replacement) {
    free(root->right);
    root->right = replacement;
}

int findEnd(struct node *root, struct node *end, char *directions) {
    int res = 0;
    struct node **pNode = &root;
    while (*pNode != end) {
        for (char *pDir = directions; *pDir != '\0'; pDir++, res++) {
            if (*pNode == end) {
                break;
            }
            printf("%s -> %c: ", (*pNode)->name, *pDir);
            if (*pDir == 'L') {
                pNode = &(*pNode)->left;
            } else if (*pDir == 'R') {
                pNode = &(*pNode)->right;
            }
            printf("%s\n", (*pNode)->name);
        }
    }
    return res;
}

int main(int argc, char *argv[]) {
    (void)argc;
    FILE *pFile = fopen(argv[1], "r");
    int res = 0;
    char buffer[500];

    // Get directions
    fgets(buffer, sizeof(buffer), pFile);
    char *pChar = strchr(buffer, '\n');
    size_t length = pChar - buffer;
    char directions[length + 1];
    directions[length] = '\0';
    memcpy(directions, buffer, length);
    printf("%s\n", directions);

    // Skip a line
    fgets(buffer, sizeof(buffer), pFile);

    struct nodeContainer nodes = {.nodesArray = NULL, .nodeCount = 0};
    struct nodeContainer nodesA = {.nodesArray = NULL, .nodeCount = 0};
    struct nodeContainer nodesZ = {.nodesArray = NULL, .nodeCount = 0};
    while (fgets(buffer, sizeof(buffer), pFile)) {
        struct node **tmp = realloc(
            nodes.nodesArray, (nodes.nodeCount + 1) * sizeof(struct node *));
        if (tmp == NULL) {
            printf("%s\n", "Memory Allocation Failed");
            return 1;
        }
        nodes.nodesArray = tmp;
        char src[4], left[4], right[4];
        sscanf(buffer, "%3s = (%3s, %3s)", src, left, right);
        printf("%s %s %s\n", src, left, right);
        struct node *tmpNode = createNode(src);
        tmpNode->left = createNode(left);
        tmpNode->right = createNode(right);
        nodes.nodesArray[nodes.nodeCount++] = tmpNode;
        if (src[3] == 'A') {
            tmp = realloc(nodesA.nodesArray,
                          (nodesA.nodeCount + 1) * sizeof(struct node *));
            if (tmp == NULL) {
                printf("%s\n", "Memory Allocation Failed");
                return 1;
            }
            nodesA.nodesArray = tmp;
            nodesA.nodesArray[nodesZ.nodeCount++] = tmpNode;
        } else if (src[3] == 'Z') {
            tmp = realloc(nodesZ.nodesArray,
                          (nodesZ.nodeCount + 1) * sizeof(struct node *));
            if (tmp == NULL) {
                printf("%s\n", "Memory Allocation Failed");
                return 1;
            }
            nodesZ.nodesArray = tmp;
            nodesZ.nodesArray[nodesZ.nodeCount++] = tmpNode;
        }
    }
    int *stepLog = malloc(sizeof(int) * nodesA.nodeCount);

    fclose(pFile);

    // Matching nodes with mem address
    for (int i = 0; i < nodes.nodeCount; i++) {
        struct node *tmpNode = nodes.nodesArray[i];
        for (int j = 0; j < nodes.nodeCount; j++) {
            if (strcmp(tmpNode->left->name, nodes.nodesArray[j]->name) == 0) {
                replaceLeft(tmpNode, nodes.nodesArray[j]);
            }
            if (strcmp(tmpNode->right->name, nodes.nodesArray[j]->name) == 0) {
                replaceRight(tmpNode, nodes.nodesArray[j]);
            }
        }
    }

    for (int i = 0; i < nodesA.nodeCount; i++) {
        for (int j = 0; j < nodesZ.nodeCount; j++) {
            int tmp = findEnd(nodesA.nodesArray[i], nodesZ.nodesArray[j],
                              directions);
            if (tmp > 0) {
                stepLog[i] = tmp;
            }
        }
    }

    // Free nodes
    for (int i = 0; i < nodes.nodeCount; i++) {
        free(nodes.nodesArray[i]);
    }
    free(nodes.nodesArray);
    free(nodesA.nodesArray);
    free(nodesZ.nodesArray);

    printf("Res: %d\n", res);
    return 0;
}
