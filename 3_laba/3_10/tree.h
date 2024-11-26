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

Node* createNode(char data);
Node* buildTree(char* expression, int* index);
void printTree(FILE* file, Node* root, int level);
void freeTree(Node* root);