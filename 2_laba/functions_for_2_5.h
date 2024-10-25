#ifndef UNTITLED1_FUNCTIONS_FOR_2_5_H
#define UNTITLED1_FUNCTIONS_FOR_2_5_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <errno.h>

#define MAX_STR_LEN 100

typedef enum Callback {
    SUCCESS,
    MEMORY_ERROR,
    INPUT_ERROR,
    FILE_OPEN_ERROR
} callback;

callback ll_to_str(long long num, int base, char** res);
long long str_to_ll(const char *str, int base);
char *memory_dump(void *data, int size);
char *Cv(int x, int base);
char* Ro(int x);
char* Zr(unsigned int x);
long long to(char *x, int base);
int oversprintf(char *str, const char *format, ...);
int overfprintf(FILE *file, const char *format, ...);

#endif //UNTITLED1_FUNCTIONS_FOR_2_5_H
