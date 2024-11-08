#ifndef UNTITLED1_STUDENT_H
#define UNTITLED1_STUDENT_H

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

#define MAX_INPUT_SIZE 100

typedef struct Student{
    unsigned long id;
    char name[128];
    char surname[128];
    char group[128];
    unsigned char *marks;
} Student;

typedef enum Callback {
    SUCCESS,
    MEMORY_ERROR,
    INPUT_ERROR,
    OVERFLOW,
    DONT_FIND,
    FILE_OPEN_ERROR,
    ERROR_THE_SAME_FILE
} callback;


callback read_student(Student **student_data, FILE *in, int *size_data, double *avg);
bool is_number(const char *str);
callback validate_data(char tmp[9][128], Student *data, int index);
void menu();
callback search_by_ID(Student *student, int size);
callback search_by_name(Student *student, int size, Student **arr, int *arr_size);
callback search_by_surname(Student *student, int size, Student **arr, int *arr_size);
callback search_by_group(Student *student, int size, Student **arr, int *arr_size);
void print_stud(Student stud);
int id_cmp(const void *a, const void *b);
int surname_cmp(const void *a, const void *b);
int name_cmp(const void *a, const void *b);
int group_cmp(const void *a, const void *b);
callback inf_file(Student *data, int size, char *output);
callback above_average(Student *data, int size, char *file);
void del_stud_data(Student *student, int size);
int is_same_file(char *input_file, char *output_file);

#endif
