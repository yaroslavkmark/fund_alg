#include "functions_for_2_5.h"

char* Ro(int x){
    const char *roman[] = {
            "M", "CM", "D", "CD", "C", "XC", "L",
            "XL", "X", "IX", "V","IV", "I"
    };
    const int value[] = {1000, 900, 500, 400, 100, 90,
                          50, 40, 10, 9, 5, 4, 1
    };

    int len_str, size_str = 5;
    char *str, *tmp_for_realloc;
    str = (char *)malloc(size_str * sizeof(char));
    if (!str){
        return NULL;
    }
    if (!x){
        strcpy(str, "null");
        return str;
    }
    else if (x < 0){
        x = abs(x);
        str[0] = '-';
        str[1] = '\0';
        len_str = 1;
    } else{
        str[0] = '\0';
    }

    for (int i = 0; i < 13; ++i){
        int len_rom = strlen(roman[i]);
        while (x >= value[i]){
            if (size_str < len_str + len_rom + 1){
                size_str = len_str + len_rom + 1;
                tmp_for_realloc = (char *) realloc(str, size_str * sizeof(char));
                if (!tmp_for_realloc){
                    free(str);
                    return NULL;
                }
                str = tmp_for_realloc;
            }
            strcat(str, roman[i]);
            x -= value[i];
            len_str += len_rom;
        }
    }

    return str;
}

char *Zr(unsigned int x){
    if (!x)
        return NULL;

    unsigned int *fib = (unsigned int *)malloc(52 * sizeof(unsigned int));
    if (!fib)
        return NULL;

    char *str = (char *) malloc(52 * sizeof(char ));
    if (!str){
        free(fib);
        return NULL;
    }

    fib[0] = 0;
    fib[1] = 1;
    int i = 1;
    while (x > fib[i]){
        ++i;
        fib[i] = fib[i - 1] + fib[i - 2];
    }

    for (int j = i; j > 0; --j){
        if (fib[j] <= x && fib[j] != 0){
            x -= fib[j];
            fib[j] = 0;
            --j;
            if (!x){
                break;
            }
        }
    }
    for (int j = 1; j <= i; ++j){
        if (!fib[j])
            str[j - 1] = '1';
        else str[j - 1] = '0';

    }
    str[i - 1] = '\0';

    char *tmp = strrchr(str, '1');
    *(++tmp) = '1';
    *(++tmp) = '\0';
    free(fib);

    return str;
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

char *Cv(int x, int base){
    if (base < 2 || base > 36){
        base = 10;
    }
    char *res;
    if (ll_to_str(x, base, &res)){
        return NULL;
    }
    return res;
}

long long str_to_ll(const char *str, int base) {
    long long res = 0;
    long long power = 1;
    int len = strlen(str);
    int flag = 0;

    if (str[0] == '-') {
        power = -1;
        flag = 1;
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
        if (flag){
            if (res < LLONG_MIN + llabs(digit * power)) {
                errno = ERANGE;
                return LLONG_MIN;
            }
        }
        else{
            if (res > LLONG_MAX  - llabs(digit * power) || (res == LLONG_MAX / base && digit > LLONG_MAX % base)) {
                errno = ERANGE;
                return LLONG_MAX;
            }
        }

        res += digit * power;

        if ((flag && i > 1) || (!flag && i)){
            if (power > LLONG_MAX / base || power < LLONG_MIN / base) {
                errno = ERANGE;
                return LLONG_MAX;
            }
            power *= base;
        }
    }

    return res;
}

long long to(char *x, int base){
    if (base < 2 || base > 36){
        base = 10;
    }

    long long res = str_to_ll(x, base);
    return res;
}

char *memory_dump(void *data, int size) {

    char *res = (char *)malloc((9 * size) * sizeof(char));
    // 9 = 8 бит + пробел
    if (!res){
        return NULL;
    }

    char *ptr = (char *)data;
    int ind = 0;
    for (int i = 0; i < size; ++i) {

        for (int j = 7; j >= 0; --j) {
            res[ind++] = (ptr[i] & (1 << j)) ? '1' : '0';
        }

        res[ind++] = ' ';
    }
    res[--ind] = '\0';

    return res;
}

int oversprintf(char *str, const char *format, ...){
    if (!str && !format){
        return -1;
    }

    va_list args;
    va_start(args, format);
    int len_f = strlen(format), count = 0;

    for (int i = 0; i < len_f; ++i){

        if (format[i] == '%' && i + 2 < len_f && format[i + 1] == 'R' && format[i + 2] == 'o'){
            int x = va_arg(args, int);
            char *res;
            res = Ro(x);
            if (res){
                count += sprintf(str + count, "%s", res);
            }
            i += 2;
            free(res);
        }

        else if (format[i] == '%' && i + 2 < len_f && format[i + 1] == 'Z' && format[i + 2] == 'r'){
            int x = va_arg(args, int);
            char *res;
            res = Zr(x);
            if (res){
                count += sprintf(str + count, "%s", res);
            }
            i += 2;
            free(res);
        }

        else if (format[i] == '%' && i + 2 < len_f && format[i + 1] == 'C' && format[i + 2] == 'v'){
            int x = va_arg(args, int);
            int base = va_arg(args, int);
            char *res;
            res = Cv(x, base);
            if (res){
                for (int j = 0; j < strlen(res); ++j){
                    res[j] = tolower(res[j]);
                }
                count += sprintf(str + count, "%s", res);
            }
            i += 2;
            free(res);
        }

        else if (format[i] == '%' && i + 2 < len_f && format[i + 1] == 'C' && format[i + 2] == 'V'){
            int x = va_arg(args, int);
            int base = va_arg(args, int);
            char *res;
            res = Cv(x, base);
            if (res){
                count += sprintf(str + count, "%s", res);
            }
            i += 2;
            free(res);
        }

        else if (format[i] == '%' && i + 2 < len_f && format[i + 1] == 't' && format[i + 2] == 'o'){
            char* x = va_arg(args, char *);
            int base = va_arg(args, int);
            long long res;
            int flag = 0;
            res = to(x, base);

            for (int j = 0; j < strlen(x); ++j) {
                if (isupper(x[j])) {
                    flag = 1;
                }
            }

            if (res != LLONG_MAX && res != LLONG_MIN && !flag) {
                count += sprintf(str + count, "%lld", res);
            }

            i += 2;
        }

        else if (format[i] == '%' && i + 2 < len_f && format[i + 1] == 'T' && format[i + 2] == 'O'){
            char* x = va_arg(args, char *);
            int base = va_arg(args, int);
            long long res;
            int flag = 0;

            for (int j = 0; j < strlen(x); ++j) {
                if (islower(x[j])) {
                    flag = 1;
                }
            }

            res = to(x, base);
            if (res != LLONG_MAX && res != LLONG_MIN && !flag) {
                count += sprintf(str + count, "%lld", res);
            }

            i += 2;
        }

        else if (format[i] == '%' && i + 2 < len_f && format[i + 1] == 'm' && format[i + 2] == 'i'){
            int x = va_arg(args, int);
            char *res;
            res = memory_dump(&x, sizeof(int));
            if (res){
                count += sprintf(str + count, "%s", res);
            }
            i += 2;
            free(res);
        }

        else if (format[i] == '%' && i + 2 < len_f && format[i + 1] == 'm' && format[i + 2] == 'u'){
            unsigned int x = va_arg(args, unsigned int);
            char *res;
            res = memory_dump(&x, sizeof(unsigned int));
            if (res){
                count += sprintf(str + count, "%s", res);
            }
            i += 2;
            free(res);
        }

        else if (format[i] == '%' && i + 2 < len_f && format[i + 1] == 'm' && format[i + 2] == 'd'){
            double x = va_arg(args, double );
            char *res;
            res = memory_dump(&x, sizeof(double));
            if (res){
                count += sprintf(str + count, "%s", res);
            }
            i += 2;
            free(res);
        }

        else if (format[i] == '%' && i + 2 < len_f && format[i + 1] == 'm' && format[i + 2] == 'f'){
            float x = va_arg(args, double);
            char *res;
            res = memory_dump(&x, sizeof(float));
            if (res){
                count += sprintf(str + count, "%s", res);
            }
            i += 2;
            free(res);
        }

        else if (format[i] == '%' && i + 1 < len_f){
            char tmp[10];
            tmp[0] = '%';

            int j;
            for (j = i + 1; j < i + 9 && j < len_f && format[j] != '%'; ++j){
                tmp[j - i] = format[j];
            }

            tmp[j - i] = '\0';
            i = j - 1;

            count += vsprintf(str + count, tmp, args);
        }

        else {
            str[count++] = format[i];
        }
    }
    str[count] = '\0';
    va_end(args);
    return count;
}

int overfprintf(FILE *file, const char *format, ...){
    if (!file && !format){
        return -1;
    }

    va_list args;
    va_start(args, format);
    int len_f = strlen(format), count = 0;

    for (int i = 0; i < len_f; ++i){

        if (format[i] == '%' && i + 2 < len_f && format[i + 1] == 'R' && format[i + 2] == 'o'){
            int x = va_arg(args, int);
            char *res;
            res = Ro(x);
            if (res){
                count += fprintf(file, "%s", res);
            }
            i += 2;
            free(res);
        }

        else if (format[i] == '%' && i + 2 < len_f && format[i + 1] == 'Z' && format[i + 2] == 'r'){
            int x = va_arg(args, int);
            char *res;
            res = Zr(x);
            if (res){
                count += fprintf(file, "%s", res);
            }
            i += 2;
            free(res);
        }

        else if (format[i] == '%' && i + 2 < len_f && format[i + 1] == 'C' && format[i + 2] == 'v'){
            int x = va_arg(args, int);
            int base = va_arg(args, int);
            char *res;
            res = Cv(x, base);
            if (res){
                for (int j = 0; j < strlen(res); ++j){
                    res[j] = tolower(res[j]);
                }
                count += fprintf(file, "%s", res);
            }
            i += 2;
            free(res);
        }

        else if (format[i] == '%' && i + 2 < len_f && format[i + 1] == 'C' && format[i + 2] == 'V'){
            int x = va_arg(args, int);
            int base = va_arg(args, int);
            char *res;
            res = Cv(x, base);
            if (res){
                count += fprintf(file, "%s", res);
            }
            i += 2;
            free(res);
        }

        else if (format[i] == '%' && i + 2 < len_f && format[i + 1] == 't' && format[i + 2] == 'o'){
            char* x = va_arg(args, char *);
            int base = va_arg(args, int);
            long long res;
            int flag = 0;
            res = to(x, base);

            for (int j = 0; j < strlen(x); ++j) {
                if (isupper(x[j])) {
                    flag = 1;
                }
            }

            if (res != LLONG_MAX && res != LLONG_MIN && !flag) {
                count += fprintf(file, "%lld", res);
            }

            i += 2;
        }

        else if (format[i] == '%' && i + 2 < len_f && format[i + 1] == 'T' && format[i + 2] == 'O'){
            char* x = va_arg(args, char *);
            int base = va_arg(args, int);
            long long res;
            int flag = 0;

            for (int j = 0; j < strlen(x); ++j) {
                if (islower(x[j])) {
                    flag = 1;
                }
            }

            res = to(x, base);
            if (res != LLONG_MAX && res != LLONG_MIN && !flag) {
                count += fprintf(file, "%lld", res);
            }

            i += 2;
        }

        else if (format[i] == '%' && i + 2 < len_f && format[i + 1] == 'm' && format[i + 2] == 'i'){
            int x = va_arg(args, int);
            char *res;
            res = memory_dump(&x, sizeof(int));
            if (res){
                count += fprintf(file, "%s", res);
            }
            i += 2;
            free(res);
        }

        else if (format[i] == '%' && i + 2 < len_f && format[i + 1] == 'm' && format[i + 2] == 'u'){
            unsigned int x = va_arg(args, unsigned int);
            char *res;
            res = memory_dump(&x, sizeof(unsigned int));
            if (res){
                count += fprintf(file, "%s", res);
            }
            i += 2;
            free(res);
        }

        else if (format[i] == '%' && i + 2 < len_f && format[i + 1] == 'm' && format[i + 2] == 'd'){
            double x = va_arg(args, double );
            char *res;
            res = memory_dump(&x, sizeof(double));
            if (res){
                count += fprintf(file, "%s", res);
            }
            i += 2;
            free(res);
        }

        else if (format[i] == '%' && i + 2 < len_f && format[i + 1] == 'm' && format[i + 2] == 'f'){
            float x = va_arg(args, double);
            char *res;
            res = memory_dump(&x, sizeof(float));
            if (res){
                count += fprintf(file, "%s", res);
            }
            i += 2;
            free(res);
        }

        else if (format[i] == '%' && i + 1 < len_f){
            char tmp[10];
            tmp[0] = '%';

            int j;
            for (j = i + 1; j < i + 9 && j < len_f && format[j] != '%'; ++j){
                tmp[j - i] = format[j];
            }

            tmp[j - i] = '\0';
            i = j - 1;

            count += vfprintf(file, tmp, args);
        }

        else {
            fputc(format[i], file);
            count++;
        }
    }

    va_end(args);
    return count;
}
