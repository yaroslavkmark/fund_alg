#ifndef UNTITLED1_FUNC_H
#define UNTITLED1_FUNC_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <ctype.h>

enum ret_type_t {
    DIVISION_BY_ZERO_ERROR,
    MUST_NOT_BE_FLOAT_ERROR,
    SUCCESS,
    IS_NOT_SIMPLE_SUCCESS,
    IS_SIMPLE_SUCCESS,
    NO_DIVISIBLE_NUMBERS,
    ERROR,
    MEMORY_ERROR,
    TOO_BIG_ERROR,
    LENGTH_ERROR,
    TOO_SMALL_ERROR,
    NOT_A_NUMBER_ERROR,
    NEGATIVE_NUMBER_ERROR,
    CANT_BE_SIMPLE_ERROR,
    MUST_NOT_BE_NEGATIVE_ERROR,
    FACTORIAL_ARG_ERROR
};

typedef enum ret_type_t (*callback)(char*);

int is_divisible(double a, double b) {

    if (b == 0.0) {
        return 0;
    }

    double quotient = a / b;
    double rounded_quotient = round(quotient);
    double product = rounded_quotient * b;
    return fabs(product - a) < 1e-9;
}

enum ret_type_t funcForH(char* argv) {
    double x = atof(argv);
    bool hasOutput = false;
    if (fabs(x - 0) >= 0.00001){
        for (int i = 1; i <= 100; ++i) {
            if (is_divisible((double)i, x)) {
                printf("%d ", i);
                hasOutput = true;
            }
        }
        if (!hasOutput) {
            return NO_DIVISIBLE_NUMBERS;
        }
        printf("\n");
        return SUCCESS;
    }
    return DIVISION_BY_ZERO_ERROR;
}

enum ret_type_t funcForP(char* argv) {
    if(strlen(argv) >= 10){
        return LENGTH_ERROR;
    }
    int x = atoi(argv);
    if(x < 0){
        return NEGATIVE_NUMBER_ERROR;
    }
    else if(x == 0 || x == 1){
        return CANT_BE_SIMPLE_ERROR;
    }

    for (int i = 2; i * i <= x; ++i) {
        if (x % i == 0) {
            return IS_NOT_SIMPLE_SUCCESS;
        }
    }
    return IS_SIMPLE_SUCCESS;
}

enum ret_type_t funcForS(char* argv) {
    if(strlen(argv) >= 10){
        return LENGTH_ERROR;
    }
    int x = atoi(argv);
    if (x == 0) {
        printf("0\n");
        return SUCCESS;
    }

    char hexStr[20];
    sprintf(hexStr, "%X", x);
    for (int i = 0; hexStr[i] != '\0'; i++) {
        printf("%c ", hexStr[i]);
    }
    printf("\n");

    return SUCCESS;
}

enum ret_type_t funcForE(char* argv) {
    if(strlen(argv) >= 10){
        return LENGTH_ERROR;
    }
    int x = atoi(argv);
    if(x > 10){
        return TOO_BIG_ERROR;
    }
    else if(x <= 0){
        return TOO_SMALL_ERROR;
    }

    printf("Таблица степеней для чисел от 1 до 10:\n");
    for (int base = 1; base <= 10; ++base) {
        for (int exponent = 1; exponent <= x; ++exponent) {
            printf("%d ", (int)pow(base, exponent));
        }
        printf("\n");
    }

    return SUCCESS;
}

enum ret_type_t funcForA(char* argv) {
    if(strlen(argv) >= 10){
        return LENGTH_ERROR;
    }
    long long x = atoi(argv);
    if(x <= 0){
        return NEGATIVE_NUMBER_ERROR;
    }
    long long sum = (x * (x + 1)) / 2;
    printf("Сумма натуральных чисел от 1 до %lli = %lli\n", x, sum);
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

enum ret_type_t funcForF(char* argv) {
    if(strlen(argv) >= 10){
        return LENGTH_ERROR;
    }
    char *tmp = argv;
    if (*tmp == '-'){
        return MUST_NOT_BE_NEGATIVE_ERROR;
    }
    while (*tmp) {
        if (*tmp == '.') {
            printf("Онли для неотрицательных целых чисел\n");
            return MUST_NOT_BE_FLOAT_ERROR;
        }
        tmp++;
    }
    int x = atoi(argv);
    if (x > 20){
        return FACTORIAL_ARG_ERROR;
    }
    long long factorial = 1;

    for (int i = 1; i <= x; ++i) {
        factorial *= i;
    }

    printf("Факториал %d = %llu\n", x, factorial);
    return SUCCESS;
}

int findFlag(char* qarg, const char** flags, int size) {
    for (int i = 0; i < size; ++i) {
        if (!(strcmp(qarg, flags[i]))) {
            return i;
        }
    }

    return -1;
}

#endif
