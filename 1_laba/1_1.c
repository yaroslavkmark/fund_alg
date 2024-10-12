#include <stdio.h>
#include "func.h"

int f(int ret, char* str){
    switch (ret)
    {
        case MEMORY_ERROR:
            printf("Memory error, something went wrong!\n");
            break;
        case TOO_BIG_ERROR:
            printf("The number mustn't be bigger than 10!\n");
            break;
        case TOO_SMALL_ERROR:
            printf("The number must be bigger than 0!\n");
            break;
        case NO_DIVISIBLE_NUMBERS:
            printf("There are no numbers that are divisible to %s\n", str);
            break;
        case NEGATIVE_NUMBER_ERROR:
            printf("The number must be positive!\n");
            break;
        case CANT_BE_SIMPLE_ERROR:
            printf("Number %s can't be simple or not simple.\n", str);
            break;
        case MUST_NOT_BE_NEGATIVE_ERROR:
            printf("The number must not be negative!\n");
            break;
        case FACTORIAL_ARG_ERROR:
            printf("The number mustn't be bigger than 10.\n");
            break;
        case IS_NOT_SIMPLE_SUCCESS:
            printf("Number %s is not simple.\n", str);
            break;
        case IS_SIMPLE_SUCCESS:
            printf("Number %s is simple.\n", str);
            break;
        case LENGTH_ERROR:
            printf("The length of the number must be lower than 10 to avoid errors!\n");
            break;
        case MUST_NOT_BE_FLOAT_ERROR:
            printf("Only for non-negative integers\n");
        case DIVISION_BY_ZERO_ERROR:
            printf("It cannot be divided by zero\n");
        default:
            break;
    }
    return SUCCESS;
}

int main(int argc, char *argv[]) {
//    char *argv[] = {"./number_program", "2.5", "-h"};
//    int argc = 3;
    const char* flags[] = {"-h", "/h", "-p", "/p", "-s", "/s", "-e", "/e", "-a", "/a", "-f", "/f"};
    callback cbs[] = {&funcForH, &funcForP, &funcForS, &funcForE, &funcForA, &funcForF};
//    printf("%s\n", argv[1]);
//    printf("%s\n", argv[2]);
//    printf("%d\n", argc);
    if (argc < 3) {
        printf("Неверное количество аргументов. Ожидалось: <число> <флаг>\n");
        return -1;
    }

    for (int i = 1; i < argc; i += 2) {
        if (i + 1 >= argc) {
            printf("Пропущен флаг для числа: %s\n", argv[i]);
            return -1;
        }

        char* numberStr = argv[i];
        char* flagStr = argv[i + 1];

        if (!is_number(numberStr)) {
            printf("Ошибка: '%s' не является числом. Ожидалось <число> <флаг>.\n", numberStr);
            return -1;
        }

        int ret = findFlag(flagStr, flags, sizeof(flags) / sizeof(char*));
        if (ret == -1) {
            printf("THIS FLAG DOES NOT EXIST: %s\n", flagStr);
            return -1;
        } else {
            int findCbsInt = ret / 2;
            callback find = cbs[findCbsInt];
            int a = find(numberStr);
            f(a, numberStr);
        }
    }

    return 0;
}


