
#ifndef UNTITLED1_MY_STRING_H
#define UNTITLED1_MY_STRING_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct String{
    int len;
    char *data;
} string;

typedef enum Callback {
    SUCCESS,
    MEMORY_ERROR,
    INPUT_ERROR,
    OVERFLOW,
    DONT_FIND
} callback;

string create(char *str);
void destroy(string *str);
int string_compare(const string *str_1, const string *str_2);
int string_equivalence(string *str_1, string *str_2);
callback copy(string *s1, string s2);
callback concatenate(string *s1, string s2);
string copy_to_new(const string src);

#endif