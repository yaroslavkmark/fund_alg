#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <errno.h>

#define MAX_BASE 36
#define MAX_STR_LEN 100

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
            digit = c - 'A' + 10;
        }

        if (digit >= base) {
            errno = ERANGE;
            return LLONG_MAX;
        }
        if (flag){
            if (res < LLONG_MIN + llabs(digit * power)) {
                errno = ERANGE;
                //printf("%d \n", res > LLONG_MAX + llabs(digit * power));

                return LLONG_MIN;
            }
        }
        else{
            if (res > LLONG_MAX  - llabs(digit * power) || (res == LLONG_MAX / base && digit > LLONG_MAX % base)) {
                errno = ERANGE;
                //printf("%d \n", res > LLONG_MAX + llabs(digit * power));

                return LLONG_MAX;
            }
        }


        res += digit * power;
//        printf("%lld\n", res);

        if ((flag && i > 1) || (!flag && i)){
            if (power > LLONG_MAX / base || power < LLONG_MIN / base) {
                // printf("%d %d z\n", len, i);
                errno = ERANGE;
                return LLONG_MAX;
            }
            power *= base;
        }
//        power *= base;
    }

    return res;
}

char *ll_to_str(long long num, int base) {
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

    char *res = (char *)malloc(sizeof(char) * (i + 1));
    if (!res) {
        return NULL;
    }
    strcpy(res, buffer);
    return res;
}

void remove_leading_zeros(char *str) {
    int i = 0;
    int j = 0;

    while (str[i] == '0') {
        i++;
    }

    if (str[i] == '\0') {
        str[1] = '\0';
        return;
    }

    while (str[i] != '\0') {
        str[j++] = str[i++];
    }
    str[j] = '\0';
}

int main() {
    int base;

    printf("Введите основание системы счисления (2-36): ");
    if (scanf("%d", &base) != 1 || base < 2 || base > MAX_BASE) {
        printf("Ошибка: некорректное основание системы счисления\n");
        return 1;
    }

    char str[MAX_STR_LEN];
    long long max_abs = 0;
    long long max_num = 0;

    printf("Введите числа (для завершения введите Stop):\n");

    while (1) {
        scanf("%99s", str);

        if (str[0] == 'S' && str[1] == 't' && str[2] == 'o' && str[3] == 'p') {
            break;
        }

        long long num = str_to_ll(str, base);

        if (num == LLONG_MAX && errno == ERANGE) {
            printf("Ошибка: переполнение или некорректная цифра в числе\n");
            errno = 0;
            continue;
        }
        //printf("%lld\n", num);
        if (llabs(num) > max_abs) {
            max_abs = llabs(num);
            max_num = num;
        }
    }

    if (max_abs == 0) {
        printf("Не было введено ни одного числа\n");
        return 0;
    }

    char *max_str = ll_to_str(max_num, base);
    if (!max_str) {
        printf("Ошибка выделения памяти\n");
        return 1;
    }
    remove_leading_zeros(max_str);
    printf("Максимальное по модулю число: %s\n", max_str);
    free(max_str);


    int bases[] = {9, 18, 27, 36};
    int num_bases = sizeof(bases) / sizeof(bases[0]);

    for (int i = 0; i < num_bases; i++) {
        char *str_in_base = ll_to_str(max_num, bases[i]);
        if (!str_in_base) {
            printf("Ошибка выделения памяти\n");
            return 1;
        }
        remove_leading_zeros(str_in_base);
        printf("В системе счисления с основанием %d: %s\n", bases[i], str_in_base);
        free(str_in_base);
    }

    return 0;
}
