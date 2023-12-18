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
    struct node *root;
    struct node *end;
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
        if (strcmp(src, "AAA") == 0) {
            root = tmpNode;
        } else if (strcmp(src, "ZZZ") == 0) {
            end = tmpNode;
        }
        nodes.nodesArray[nodes.nodeCount++] = tmpNode;
    }
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

    printf("Root: %s %s %s\n", root->name, root->left->name, root->right->name);

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

    // Free nodes
    for (int i = 0; i < nodes.nodeCount; i++) {
        free(nodes.nodesArray[i]);
    }
    free(nodes.nodesArray);

    printf("Res: %d\n", res);
    return 0;
}
