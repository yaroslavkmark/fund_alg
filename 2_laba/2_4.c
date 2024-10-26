#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STR_LEN 100

typedef enum Callback {
    ERROR = -1,
    SUCCESS,
    INPUT_ERROR,
    MEMORY_ERROR,
    OVERFLOW_ERROR,
    NOT_A_NUM,
    EMPTY_LINE
} callback;

typedef struct {
    double x;
    double y;
} Point;

float cross_product(Point a, Point b, Point c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

callback is_convex(int *res, int count, ...) {
    if (count < 0) {
        return INPUT_ERROR;
    }
    if (count <= 3){
        *res = 1;
        return SUCCESS;
    }
    *res = 1;
    va_list args;
    va_start(args, count);

    Point cords[count];
    for (int i = 0; i < count; i++) {
        cords[i].x = (float)va_arg(args, double);
        cords[i].y = (float)va_arg(args, double);
    }
    va_end(args);

    if (cords[count - 1].x != cords[0].x || cords[count - 1].y != cords[0].y){
        *res = 0;
        return SUCCESS;
    }

    int sign = 0;

    for (int i = 0; i < count; i++) {
        float cp = cross_product(cords[i], cords[(i + 1) % count], cords[(i + 2) % count]);
        if (cp != 0) {
            if (sign == 0) {
                sign = (cp > 0) ? 1 : -1;
            } else if ((cp > 0 && sign < 0) || (cp < 0 && sign > 0)) {
                *res = 0;
                return SUCCESS;
            }
        }
    }
    return SUCCESS;
}

int ValidateNum(const char* argv, int base) {
    int cnt = 0;
    while (*argv != '\0') {
        if (!isalnum(*argv)) {
            return NOT_A_NUM;
        }
        argv++;
        cnt++;
    }
    if (cnt == 0) {
        return EMPTY_LINE;
    }
    return SUCCESS;
}

callback calculate_polynomial(double *result, double x, int n, ...) {
    va_list args;
    va_start(args, n);

    *result = 0;
    if (n < 0)
        return INPUT_ERROR;
    for (int i = n; i >= 0; --i) {
        double coeff = va_arg(args, double);
        *result += coeff * pow(x, i);
        if (isinf(*result) || isnan(*result)) {
            return OVERFLOW_ERROR;
        }
    }

    va_end(args);
    return SUCCESS;
}

long long str_to_ll(const char *str, int base) {
    long long res = 0;
    long long power = 1;
    int len = strlen(str);
    int flag = 0;

    if (str[0] == '-') {
        return LLONG_MAX;
    }

    for (int i = len - 1; i >= flag; i--) {
        char c = str[i];
        int digit;

        if (isdigit(c)) {
            digit = c - '0';
        } else {
            digit = toupper(c) - 'A' + 10;
        }

        if (digit >= base) {
            errno = ERANGE;
            return LLONG_MAX;
        }

        if (res > LLONG_MAX  - llabs(digit * power) || (res == LLONG_MAX / base && digit > LLONG_MAX % base)) {
            errno = ERANGE;
            return LLONG_MAX;
        }


        res += digit * power;

        if (i){
            if (power > LLONG_MAX / base || power < LLONG_MIN / base) {
                errno = ERANGE;
                return LLONG_MAX;
            }
            power *= base;
        }
    }

    return res;
}

callback ll_to_str(long long num, int base, char** res) {
    char buffer[MAX_STR_LEN];
    int i = 0;
    int is_negative = 0;

    if (num == 0) {
        buffer[i++] = '0';
    } else if (num < 0) {
        is_negative = 1;
        num = -num;
    }

    while (num > 0) {
        int digit = num % base;
        if (digit < 10) {
            buffer[i++] = digit + '0';
        } else {
            buffer[i++] = digit - 10 + 'A';
        }
        num /= base;
    }

    if (is_negative) {
        buffer[i++] = '-';
    }

    buffer[i] = '\0';

    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temp;
        start++;
        end--;
    }

    *res = (char *)malloc(sizeof(char) * (i + 1));
    if (!*res) {
        return MEMORY_ERROR;
    }
    strcpy(*res, buffer);
    return SUCCESS;
}

int is_Kaprekar(char* num, int base) {

    long long number = str_to_ll(num, base); // 10cc
    if (number == LLONG_MAX) return 0;

    long long x_10_squared;
    char *x_base_squared;
    if (LLONG_MAX / number >= number){
        x_10_squared = number * number;
        int tmp = ll_to_str(x_10_squared, base, &x_base_squared);
        if (tmp) return 0;

        int len = strlen(x_base_squared);
        if (len == 1 && x_base_squared[0] == '1') return 1;

        char *left = (char*)malloc(len * sizeof(char));
        if (!left) return 0;

        for (int j = 1; j < len; ++j){
            left[j - 1] = x_base_squared[j - 1];
            left[j] = '\0';

            long long a_10, b_10;
            a_10 = str_to_ll(left, base);
            b_10 = str_to_ll(&(x_base_squared[j]), base);

            if (a_10 == LLONG_MAX || b_10 == LLONG_MAX) continue;

            if (a_10 == 0 || b_10 == 0) continue;

            if (LLONG_MAX - b_10 < a_10) continue;

            if (a_10 + b_10 == number){
                free(left);
                return 1;
            }

        }
        free(left);
    }

    return 0;
}

int func3(int base, int cnt, ...) {
    if (base < 2 || base > 36) {
        return INPUT_ERROR;
    }
    va_list args;
    va_start (args, cnt);
    for(int i = 0; i < cnt; i++) {
        char* tek_num_str = va_arg(args, char*);
        if (ValidateNum(tek_num_str, base) != SUCCESS) {
            va_end(args);
            return ValidateNum(tek_num_str, base);
        }
        if (is_Kaprekar(tek_num_str, base)) {
            printf("<%s> IT IS KAPREKAR'S NUMBER\n", tek_num_str);
        }
        else{
            printf("<%s> IT IS NOT KAPREKAR'S NUMBER\n", tek_num_str);
        }
    }

    va_end(args);
    return SUCCESS;
}

int main() {
    // IS CONVEX PART
    int res;
    int is_convex_check = is_convex(&res, 6, 0.0, 0.0, 0.0, 2.0, 1.0, 2.0, 1.0,
                                    1.0, 2.0, 1.0, 2.0, 0.0);

    switch (is_convex_check) {

        case INPUT_ERROR:
            printf("INPUT_ERROR\n");
            break;

        case SUCCESS:
            if (res){
                printf("is Convex\n");
            }
            else{
                printf("is not Convex\n");
            }
            break;
    }

    // calculate_polynomial PART
    double result;
    callback polynom = calculate_polynomial(&result, 1.0, 2, 2.0, 3.0, 4.0);

    switch (polynom) {
        case INPUT_ERROR:
            printf("INPUT_ERROR\n");
            break;

        case OVERFLOW_ERROR:
            printf("OVERFLOW_ERROR\n");

        case SUCCESS:
            printf("polynom: %f\n", result);
    }

    // Kaprekar Part
    int is_kaprekar = func3(10, 3, "9", "1", "100");
    switch (is_kaprekar) {
        case INPUT_ERROR:
            printf("INPUT_ERROR\n");
            break;
        case NOT_A_NUM:
            printf("NOT A NUMBER OR A LETTER\n");
            break;
        case EMPTY_LINE:
            printf("EMPTY LINE\n");
            break;
    }
    is_kaprekar = func3(16, 1, "AB");
    switch (is_kaprekar) {
        case INPUT_ERROR:
            printf("INPUT_ERROR\n");
            break;
        case NOT_A_NUM:
            printf("NOT A NUMBER OR A LETTER\n");
            break;
        case EMPTY_LINE:
            printf("EMPTY LINE\n");
            break;
    }
}