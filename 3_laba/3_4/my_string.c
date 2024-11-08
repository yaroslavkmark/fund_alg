#include "my_string.h"

string create(char *str){
    string res;


    if (!str){
        res.len = -1;
        res.data = NULL;
        return res;
    }


    res.len = strlen(str);
    res.data = malloc((res.len + 1) * sizeof(char));

    if (!res.data){
        res.len = -1;
        res.data = NULL;
        return res;
    }

    strcpy(res.data, str);
    //printf("str = %s res = %s res = %d\n",str, res.data, res.len);
    return res;
}

void destroy(string *str){
    if (str->data)
        free(str->data);
    str->data = NULL;
    str->len = 0;
}

int string_compare(const string *str_1, const string *str_2){
    return (str_1->len != str_2->len) ? str_1->len - str_2->len : strcmp(str_1->data, str_2->data);
}

int string_equivalence(string *str_1, string *str_2) {
    return !strcmp(str_1->data, str_2->data);
}

callback copy(string *s1, string s2){
    if(!s1 || !s2.data || s2.len == -1){
        return MEMORY_ERROR;
    }

    if (s1->data){
        free(s1->data);
    }

    s1->len = s2.len;
    s1->data = malloc((s2.len + 1) * sizeof(char));
    if (!s1->data){
        return MEMORY_ERROR;
    }
    strcpy(s1->data, s2.data);
    return SUCCESS;
}

callback concatenate(string *s1, string s2){
    if (!s1 || !s1->data || s1->len == -1 || !s2.data || s2.len == -1){
        return MEMORY_ERROR;
    }
    char *tmp = realloc(s1->data, s1->len + s2.len + 1);

    if (!tmp){
        free(s1->data);
        return MEMORY_ERROR;
    }
    s1->data = tmp;

    strcat(s1->data, s2.data);
    s1->len += s2.len;
    return SUCCESS;
}

string copy_to_new(const string src) {
    string dest;
    if (src.len == -1) {
        dest.len = -1;
        dest.data = NULL;
        return dest;
    }

    dest.len = src.len;
    dest.data = malloc((dest.len + 1) * sizeof(char));

    if (!dest.data) {
        dest.len = -1;
        return dest;
    }

    strcpy(dest.data, src.data);
    return dest;
}