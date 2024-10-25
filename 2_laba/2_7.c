#include <stdio.h>

#define ITERATION_LIMIT 10000000

typedef enum Callback{
    SUCCESS,
    ERROR_ITERATIONS_LIMIT,
    ERROR_NO_SOLVE
} callback;

callback Dichotomy(double a, double b, double (*func)(double), double eps, double *res) {
    if (func(a) * func(b) >= 0.0) {
        *res = 0.0;
        return ERROR_NO_SOLVE;
    }
    double c;
    int n = 0;
    while ((b - a) / 2 > eps) {
        if (n > ITERATION_LIMIT) {
            *res = 0.0;
            return ERROR_ITERATIONS_LIMIT;
        }
        c = (a + b) / 2;
        if (func(a) * func(c) > 0) {
            a = c;
        } else {
            b = c;
        }
        n++;
    }
    *res = c;
    return SUCCESS;
}

double func1(double x) {
    return (x-1)*(x-1)/(x-1);
}

int main() {
    double res;
    int code = Dichotomy(-5.0, 5.0, func1, 0.00000000001, &res);
    if (code != SUCCESS) {
        switch (code) {
            case ERROR_ITERATIONS_LIMIT:
                printf("Too many iterations\n");
                break;

            case ERROR_NO_SOLVE:
                printf("There is no solvement in your interval\n");
                break;

        }
    }
    printf("%lf\n", res);
    return SUCCESS;
}