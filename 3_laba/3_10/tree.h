#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

enum ret_type_t{
    SUCCESS,
    ERROR_ARGS,
    ERROR_MALLOC,
    ERROR_OPEN_FILE,
    ERROR_THE_SAME_FILE
};

typedef struct Node {
    char data;
    struct Node* sibling;
    struct Node* child;
} Node;

Node* createNode(char data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode)
        return NULL;
    newNode->data = data;
    newNode->sibling = NULL;
    newNode->child = NULL;
    return newNode;
}

Node* buildTree(char* expression, int* index) {
    Node* root = createNode(expression[*index]);
    if (!root)
        return NULL;

    (*index)++;

    if (expression[*index] == '(') {
        (*index)++;
        root->child = buildTree(expression, index);
    }

    if (expression[*index] == ',') {
        (*index)++;
        root->sibling = buildTree(expression, index);
    }

    if (expression[*index] == ')')
        (*index)++;

    return root;
}

void printTree(FILE* file, Node* root, int level) {
    if (root == NULL) return;

    for (int i = 0; i < level; i++) {
        fprintf(file, "  ");
    }

    fprintf(file, "%c\n", root->data);

    if (root->child != NULL)
        printTree(file, root->child, level + 1);

    if (root->sibling != NULL)
        printTree(file, root->sibling, level);

}

void freeTree(Node* root) {
    if (root == NULL)
        return;

    if (root->child != NULL)
        freeTree(root->child);

    if (root->sibling != NULL)
        freeTree(root->sibling);

    free(root);
}