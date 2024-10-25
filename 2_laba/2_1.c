#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

typedef enum Callback{
    ERROR = -1,
    SUCCESS,
    INPUT_ERROR,
    MEMORY_ERROR
} callback;

callback validate_input(int argc, char *argv[]) {
    if (argc < 3 || argv[1][0] != '-' || argv[1][2] != '\0')
        return INPUT_ERROR;
    return SUCCESS;
}

size_t my_strlen(const char* str) {
    if (str != NULL){
        size_t length = 0;
        while (*str++) {
            length++;
        }
        return length;
    }
    return ERROR;
}

char* my_strcat(char* str1, const char* str2) {
    size_t len1 = my_strlen(str1);
    size_t len2 = my_strlen(str2);

    for (size_t j = 0; j < len2; j++) {
        str1[len1 + j] = str2[j];
    }

    str1[len1 + len2] = '\0';

    return str1;
}

void count_length(const char* str) {
    printf("Длина строки: %lu\n", my_strlen(str));
}

callback reverse_string(char **reversed, const char* str) {
    *reversed = (char*)malloc((my_strlen(str) + 1) * sizeof(char));
    size_t len = my_strlen(str);

    for (size_t i = 0; i < len; i++) {
        (*reversed)[i] = str[len - 1 - i];
    }
    (*reversed)[len] = '\0';
    return SUCCESS;
}

callback upper_odd_positions(char** new_str, const char* str) {

    *new_str = (char*)malloc((my_strlen(str) + 1) * sizeof(char));
    if (*new_str == NULL) {
        perror("Ошибка выделения памяти");
        return MEMORY_ERROR;
    }

    for (size_t i = 0; i < my_strlen(str); i++) {
        if (i % 2 != 0) {
            (*new_str)[i] = toupper((unsigned char)str[i]);
        } else {
            (*new_str)[i] = str[i];
        }
    }
    (*new_str)[my_strlen(str)] = '\0';
    return SUCCESS;
}

callback rearrange_string(char** res, const char* str) {
    *res = (char*)malloc((my_strlen(str) + 1) * sizeof(char));
    if (*res == NULL) {
        perror("Ошибка выделения памяти");
        return MEMORY_ERROR;
    }
    char* digits = (char*)malloc((my_strlen(str) + 1) * sizeof(char));
    char* letters = (char*)malloc((my_strlen(str) + 1) * sizeof(char));
    char* others = (char*)malloc((my_strlen(str) + 1) * sizeof(char));

    if (!digits) {
        perror("Ошибка выделения памяти");
        free(*res);
        return MEMORY_ERROR;
    }
    else if (!letters) {
        perror("Ошибка выделения памяти");
        free(*res);
        free(digits);
        return MEMORY_ERROR;
    }
    else if (!others) {
        perror("Ошибка выделения памяти");
        free(*res);
        free(digits);
        free(letters);
        return MEMORY_ERROR;
    }

    size_t d = 0, l = 0, o = 0;
    for (size_t i = 0; i < my_strlen(str); i++) {
        if (isdigit(str[i])) {
            digits[d++] = str[i];
        } else if (isalpha(str[i])) {
            letters[l++] = str[i];
        } else {
            others[o++] = str[i];
        }
    }

    my_strcat(*res, digits);
    my_strcat(*res, letters);
    my_strcat(*res, others);

    digits[d] = '\0';
    letters[l] = '\0';
    others[o] = '\0';
    (*res)[my_strlen(str)] = '\0';

    free(digits);
    free(letters);
    free(others);
    return SUCCESS;
}

callback concatenate_randomly(int argc, char* argv[], char** result) {

    int allLen = 0;
    for (int i = 3; i < argc; i++){
        allLen += my_strlen(argv[i]);
    }
    *result = (char*)malloc((allLen + 1) * sizeof(char));

    if (*result == NULL) {
        perror("Ошибка выделения памяти");
        return MEMORY_ERROR;
    }
    (*result)[0] = '\0';

    unsigned int seed = (unsigned int)atoi(argv[2]);
    srand(seed);
    char** strings = (char**)malloc((argc - 3) * sizeof(char*));

    if (strings == NULL) {
        perror("Ошибка выделения памяти");
        free(*result);
        return MEMORY_ERROR;
    }

    for (int i = 3; i < argc; i++) {
        strings[i - 3] = argv[i];
    }

    for (int i = argc - 4; i > 0; i--) {
        int j = rand() % (i + 1);
        char* temp = strings[i];
        strings[i] = strings[j];
        strings[j] = temp;
    }

    for (int i = 0; i < argc - 3; i++) {
        my_strcat(*result, strings[i]);
    }

    free(strings);
    return SUCCESS;
}

bool is_number(const char *str) {
    if (*str == '-') {
        str++;
    }

    bool hasDot = false;
    if (*str == '\0') return false;

    while (*str) {
        if (*str == '.') {
            if (hasDot) return false;
            hasDot = true;
        } else if (!isdigit(*str)) {
            return false;
        }
        str++;
    }
    return true;
}

int main(int argc, char *argv[]) {
//
//    int argc = 3;
//    char *argv[] = {"./21", "-u", "qwert"};

    if (validate_input(argc, argv) == INPUT_ERROR) {
        printf("Input error\n");
        return INPUT_ERROR;
    }

    switch (argv[1][1]) {
        case 'l':
            if (argc != 3) {
                printf("Input error\n");
                return INPUT_ERROR;
            }

            count_length(argv[2]);
            break;

        case 'r':
            if (argc != 3){
                printf("Input error\n");
                return INPUT_ERROR;
            }

            char *reversed_string = NULL;
            reverse_string(&reversed_string, argv[2]);
            printf("Перевёрнутая строка: %s\n", reversed_string);
            free(reversed_string);
            break;

        case 'u':
            if (argc != 3){
                printf("Input error\n");
                return INPUT_ERROR;
            }

            char *new_str = NULL;
            upper_odd_positions(&new_str, argv[2]);
            printf("Строка с заглавными символами на нечётных позициях: %s\n", new_str);
            free(new_str);
            break;

        case 'n':
            if (argc != 3){
                printf("Input error\n");
                return INPUT_ERROR;
            }

            char *res = NULL;
            rearrange_string(&res, argv[2]);
            printf("Результирующая строка: %s\n", res);
            free(res);
            break;

        case 'c':
            if (argc < 4 || !is_number(argv[2]) || atoi(argv[2]) < 0) {
                printf("Input error\n");
                return INPUT_ERROR;
            }

            char* result = NULL;
            concatenate_randomly(argc, argv, &result);
            printf("Конкатенированная строка: %s\n", result);
            free(result);
            break;

        default:
            printf("NOT EXISTING FLAG\n");
            return INPUT_ERROR;
    }
    return SUCCESS;
}



