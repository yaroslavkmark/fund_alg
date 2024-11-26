#include "tree.h"

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