#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>

enum ret_type_t {
    SUCCESS,
    MUST_NOT_EQUAL_ERROR,
    ERROR
};

typedef enum ret_type_t (*callback)(double, double);

enum ret_type_t summa_a(double eps, double x){
    double sn = 1, sum = sn;
    int n = 0;
    do {
        if (fabs(x - 0) >= 0.00001){
            sn = sn * x / (n + 1);
            sum += sn;
            ++n;
        }
        else{
            return MUST_NOT_EQUAL_ERROR;
        }
    } while (fabs(sn * x / (n + 1)) > eps && n < 100);
    printf("%f\n", sum);
    return SUCCESS;
}

enum ret_type_t summa_b(double eps, double x){
    double sn = 1, sum = sn;
    int n = 0;
    do {
        if (fabs(x - 0) >= 0.00001){
            sn = -sn * x * x / (2 * n + 2) / (2 * n + 1);
            sum += sn;
            ++n;
        }
        else{
            return MUST_NOT_EQUAL_ERROR;
        }
    } while (fabs(-sn * x * x / (2 * n + 2) / (2 * n + 1)) > eps && n < 100);
    printf("%f\n", sum);
    return SUCCESS;
}

enum ret_type_t summa_c(double eps, double x){
    double sn = 1, sum = sn;
    int n = 0;
    do {
        if (fabs(x - 0) >= 0.00001){
            sn = sn * 27  * x * x * (n + 1) * (n + 1) * (n + 1) / (3 * n + 1) / (3 * n + 2) / (3 * n + 3);
            sum += sn;
            ++n;
        }
        else{
            return MUST_NOT_EQUAL_ERROR;
        }
    } while (fabs(sn * 27 / (3 * n + 1) / (3 * n + 2) / (3 * n + 3) * x * x * (n + 1) * (n + 1) * (n + 1)) > eps && n < 100);
    printf("%f\n", sum);
    return SUCCESS;
}

enum ret_type_t summa_d(double eps, double x){
    double sn = -2, sum = sn;
    int n = 1;
    do {
        if (abs(x - 0) >= 0.00001){
            sn = -sn * x * x * (2 * n + 1) / (2 * n + 2);
            sum += sn;
            ++n;
        }
        else{
            return MUST_NOT_EQUAL_ERROR;
        }
    } while (fabs(-sn * x * x * (2 * n + 1) / (2 * n + 2)) > eps && n < 100);
    printf("%f\n", sum);
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

int findFlag(char* qarg, const char** flags, int size) {
    for (int i = 0; i < size; ++i) {
        if (!(strcmp(qarg, flags[i]))) {
            return i;
        }
    }

    return -1;
}

int main(int argc, char *argv[]){
    if (argc != 3){
        printf("Неверное кол-во аргументов. Ожидалось <число eps> <число x>\n");
        return 0;
    }
    callback cbs[] = {summa_a, summa_b, summa_c, summa_d};
    double x, eps;
    if (is_number(argv[1])){
        eps = atof(argv[1]);
        if (fabs(eps - 0.0) < 0.00001){
            printf("eps не должно быть равно нулю\n");
            return 0;
        }
        if (eps < 0.0000001){
            eps = 0.0000001;
        }
    }
    else{
        printf("Не является числом.\n");
        return 0;
    }

    if (is_number(argv[2])){
        x = atof(argv[2]);
        if (x > 1){
            printf("Слишком большое x\n");
            return 0;
        }
    }
    else{
        printf("Не является числом.\n");
        return 0;
    }

    for (int i = 0; i < sizeof(cbs)/sizeof(callback); i++) {
        callback func = cbs[i];
        int res = func(eps, x);
        if (res == MUST_NOT_EQUAL_ERROR){
            printf("Число х не должно быть равно нулю\n");
            break;
        }
    }
    return 0;
}