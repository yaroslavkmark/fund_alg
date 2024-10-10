#include <stdio.h>
#include <ctype.h>
#include <limits.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define MAX_BASE 36
#define MAX_NUMBER_LEN 101

enum ret_type_t{
    SUCCESS,    //Успех
    ERROR_OPEN,  //Не удалось открыть файл
};

int find_min_base(const char *str) {
    int max_digit = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (i == 0 && str[i] == '-'){
            continue;
        }
        else if (isdigit(str[i])) {
            max_digit = (str[i] - '0' > max_digit) ? str[i] - '0' : max_digit;
        }
        else if (isalpha(str[i])) {
            max_digit = (tolower(str[i]) - 'a' + 10 > max_digit) ? tolower(str[i]) - 'a' + 10 : max_digit;
        } else{
            return -1;
        }
    }
    return (max_digit < 1) ? 2 : max_digit + 1;
}

long long convert_to_decimal(const char *str, int base) {
    long long decimal = 0;
    long long power = 1;
    int flag = 0;
    int length = 0;
    while (str[length] != '\0') {
        length++;
    }
    if (str[0] == '-'){
        flag = 1;
        power = -1;
    }
    for (int i = length - 1; i >= flag; i--) {
        int digit = 0;
        if (isdigit(str[i])) {
            digit = str[i] - '0';
        } else if((str[i] - '0' >= 49 && str[i] <= 74) || (str[i] - '0' >= 17 && str[i] <= 42)){
            digit = tolower(str[i]) - 'a' + 10;
        }
        if (flag){
            if (decimal < LLONG_MIN + llabs(digit * power)) {
                errno = ERANGE;
                printf("%d \n", decimal > LLONG_MAX + llabs(digit * power));

                return -1;
            }
        }
        else{
            if (decimal > LLONG_MAX  - llabs(digit * power) || (decimal == LLONG_MAX / base && digit > LLONG_MAX % base)) {
                errno = ERANGE;
                printf("%d \n", decimal > LLONG_MAX + llabs(digit * power));

                return -1;
            }
        }
        decimal += digit * power;

        if ((flag && i > 1) || (!flag && i)){
            if (power > LLONG_MAX / base || power < LLONG_MIN / base) {
                printf("%d %d z\n", length, i);
                errno = ERANGE;
                return -1;
            }
            power *= base;
        }

    }
    return decimal;
}

int f(const char *name_input_file, const char *name_output_file){
    FILE *input_file = fopen(name_input_file, "r");
    if (input_file == NULL) {
        return ERROR_OPEN;
    }

    FILE *output_file = fopen(name_output_file, "w");
    if (output_file == NULL) {
        fclose(input_file);
        return ERROR_OPEN;
    }

    char number_str[MAX_NUMBER_LEN];

    while (fscanf(input_file, "%100s", number_str) == 1) {
        int i = 0;
        while (number_str[i] == '0' && number_str[i + 1] != '\0') {
            i++;
        }

        if (number_str[i] == '\0') {
            fprintf(output_file, "0 2 0\n");
            continue;
        }

        int min_base = find_min_base(number_str + i);



        if (min_base == -1 || min_base > MAX_BASE || min_base < 2) {
            fprintf(stderr, "Invalid number: %s\n", number_str);
            continue;
        }

        long long decimal_value = convert_to_decimal(number_str + i, min_base);
        if (decimal_value == -1 && errno == ERANGE) {
            fprintf(stderr, "Overflow detected for number: %s\n", number_str);
            continue;
        }

        fprintf(output_file, "%s %d %lld\n", number_str + i, min_base, decimal_value);
    }

    fclose(input_file);
    fclose(output_file);
    return SUCCESS;
}

int strlen_custom(const char *s) {
    int length = 0;
    while (s[length] != '\0') {
        length++;
    }
    return length;
}


int f1(char *s1, char *s2) {
    const char *prefix = "./";
    int prefix_length = strlen_custom(prefix);
    int s1_length = strlen_custom(s1);


    if (strlen_custom(s2) != prefix_length + s1_length) {
        return 0;
    }

    for (int i = 0; i < prefix_length; i++) {
        if (s2[i] != prefix[i]) {
            return 0;
        }
    }

    for (int i = 0; i < s1_length; i++) {
        if (s2[i + prefix_length] != s1[i]) {
            return 0;
        }
    }

    return 1;
}

int main(int argc, char *argv[]) {
//    int argc = 3;
//    char *argv[] = {"./8_laba", "8.txt", "./8.txt"};
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }
    if (argv[1] == argv[2] || f1(argv[1], argv[2])) {
        fprintf(stderr, "Error: Input and output files must be different.\n");
        return 1;
    }


    int tmp = f(argv[1], argv[2]);

    switch (tmp) {
        case ERROR_OPEN:
            perror("Error opening file");
    }

    return SUCCESS;
}

