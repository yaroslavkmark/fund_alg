#include <stdio.h>
#include <stdarg.h>
#include <math.h>

typedef enum Callback{
    ERROR = -1,
    SUCCESS,
    INPUT_ERROR,
    MEMORY_ERROR,
    OVERFLOW_ERROR
} callback;

callback is_overflow(double x) {
    if (isnan(x) || isinf(x))
        return OVERFLOW_ERROR;
    return SUCCESS;
}

callback geometric_mean(double *res, int count, ...) {
    if (count < 1)
        return INPUT_ERROR;

    va_list args;
    va_start(args, count);
    *res = 1;
    double cur;
    for (int i = 0; i < count; ++i) {

        cur = va_arg(args, double);
        if (cur < 0){
            va_end(args);
            return INPUT_ERROR;
        }

        *res *= cur;
        if (is_overflow(*res)) {
            va_end(args);
            return OVERFLOW_ERROR;
        }
    }
    *res = pow(*res, (double)1.0 / count);
    va_end(args);
    return SUCCESS;
}

callback fast_double_pow(double base, int degree, double *res) {
    callback state = SUCCESS;
    if (degree < 0)
        return fast_double_pow(1.0 / base, -degree, res);

    else if (!degree) {
        *res = 1.0;
        state = SUCCESS;
    }

    else if (degree & 1) {
        state = fast_double_pow(base, degree - 1, res);
        *res *= base;
        return is_overflow(*res);
    } else{
        state = fast_double_pow(base, degree / 2, res);
        *res *= *res;
        return is_overflow(*res);
    }
    return state;
}

int main() {

    double result;
    int geom_mean = geometric_mean(&result, 4, 0.0, 1.3, 1.8, 5.5);
    switch (geom_mean) {
        case INPUT_ERROR:
            printf("Input error\n");
            return INPUT_ERROR;

        case OVERFLOW_ERROR:
            printf("Overflow error\n");
            return OVERFLOW_ERROR;

        case SUCCESS:
            printf("Geometric mean: %lf\n", result);
            break;
    }

    int fdp = fast_double_pow(3.0, -2, &result);
    switch (fdp) {
        case OVERFLOW_ERROR:
            printf("Overflow error\n");
            return OVERFLOW_ERROR;

        case SUCCESS:
            printf("Fast pow: %lf\n", result);
            break;
    }

    return SUCCESS;
}