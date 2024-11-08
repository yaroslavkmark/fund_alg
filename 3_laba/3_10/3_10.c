#include "tree.h"

int is_same_file(char *input_file, char *output_file) {
    char buf1[PATH_MAX];
    char *res1 = realpath(input_file, buf1);
    char buf2[PATH_MAX];
    char *res2 = realpath(output_file, buf2);
    if (res1 != NULL && res2 != NULL){
        if (!strcmp(res1, res2)){
            return ERROR_THE_SAME_FILE;
        }
    }
    return SUCCESS;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Wrong arguments\n");
        return ERROR_ARGS;
    }

    if (is_same_file(argv[1], argv[2])) {
        printf("Same files\n");
        return ERROR_THE_SAME_FILE;
    }

    FILE* inputFile = fopen(argv[1], "r");
    if (!inputFile) {
        printf("Failed to open file\n");
        return ERROR_OPEN_FILE;
    }

    FILE* outputFile = fopen(argv[2], "w");
    if (!outputFile) {
        printf("Failed to open file\n");
        fclose(inputFile);
        return ERROR_OPEN_FILE;
    }

    char* line;
    while (fscanf(inputFile, "%ms", &line) != EOF) {
        //printf("%s\n", line);
        line[strcspn(line, "\n")] = '\0';

        int index = 0;
        Node* root = buildTree(line, &index);

        if (!root){
            printf("Failed to malloc\n");
            return ERROR_MALLOC;
        }

        printTree(outputFile, root, 0);
        fprintf(outputFile, "\n");

        freeTree(root);
    }

    fclose(inputFile);
    fclose(outputFile);

    return SUCCESS;
}