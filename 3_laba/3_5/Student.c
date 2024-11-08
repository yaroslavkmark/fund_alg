#include "Student.h"

int is_same_file(char *input_file, char *output_file) {
    char buf1[PATH_MAX];
    char *res1 = realpath(input_file, buf1);
    char buf2[PATH_MAX];
    char *res2 = realpath(output_file, buf2);
    if (res1 != NULL && res2 != NULL){
        if (!strcmp(res1, res2)){
            return ERROR_THE_SAME_FILE;
        }
    }
    return SUCCESS;
}


void menu(){
    printf("Choose one\n");
    printf("1. Search by ID\n2. Search by surname\n3. Search by name\n4. Search by group\n5. Sort by ID\n"
           "6. Sort by surname\n7. Sort by name\n8. Sort by group\n9. Move student information to a file\n"
           "s. move information about an above-average student to a file\nq. print all students\n0. End\n");
}

bool is_number(const char *str) {
    if (*str == '\0') return false;
    if (*str == '-') return false;

    while (*str) {
        if (*str == '.' || !isdigit(*str))
            return false;

        str++;
    }

    return true;
}

callback validate_data(char tmp[9][128], Student *data, int index) {
    if (!is_number(tmp[0]) || !is_number(tmp[8]) || !is_number(tmp[7]) || !is_number(tmp[6])
    || !is_number(tmp[5])|| !is_number(tmp[4]) || strlen(tmp[1]) < 1|| strlen(tmp[2]) < 1
    || strlen(tmp[3]) < 1)
        return INPUT_ERROR;

    for (int i = 4; i < 9; ++i){
        if (strlen(tmp[i]) != 1 || tmp[i][0] < '2' || tmp[i][0] > '5')
            return INPUT_ERROR;
    }

    unsigned long tmp2 = strtoul(tmp[0], NULL, 10);
    if (errno == ERANGE)
        return OVERFLOW;

    for (int i = 0; i < strlen(tmp[1]); ++i) {
        if (!isalpha(tmp[1][i]))
            return INPUT_ERROR;
    }

    for (int i = 0; i < strlen(tmp[2]); ++i) {
        if (!isalpha(tmp[2][i]))
            return INPUT_ERROR;
    }

    data[index].id = tmp2;
    strcpy(data[index].name, tmp[1]);
    strcpy(data[index].surname, tmp[2]);
    strcpy(data[index].group, tmp[3]);

    data[index].marks = (unsigned char *)malloc(5 * sizeof(unsigned char));
    if (!data[index].marks)
        return MEMORY_ERROR;

    for (int i = 0; i < 5; ++i){
        data[index].marks[i] = tmp[4 + i][0];
    }

    return SUCCESS;
}

callback read_student(Student **student_data, FILE *in, int *size_data, double *avg){
    int capacity = 3;
    *size_data = 0;
    *avg = 0;
    *student_data = malloc(capacity * sizeof(Student));
    if (!(*student_data))
        return MEMORY_ERROR;


    char tmp[9][128];

    while ((fscanf(in, "%127s %127s %127s %127s %127s %127s %127s %127s %127s", tmp[0], tmp[1], tmp[2], tmp[3], tmp[4], tmp[5], tmp[6], tmp[7], tmp[8])) == 9){
        if (validate_data(tmp, *student_data, *size_data)){
            free((*student_data)->marks);
            free(*student_data);
            return INPUT_ERROR;
        }

        (*size_data)++;
        if (*size_data >= capacity){
            capacity *= 2;
            Student *tmp_for_realloc = realloc(*student_data, capacity * sizeof(Student));
            if (!tmp_for_realloc){
                free((*student_data)->marks);
                free(*student_data);
                return MEMORY_ERROR;
            }
            *student_data = tmp_for_realloc;
        }
    }

    if (!(*size_data)){
        free((*student_data)->marks);
        free(*student_data);
        return INPUT_ERROR;
    }

    return SUCCESS;
}

void print_stud(Student stud){
    printf("ID: %lu\n", stud.id);
    printf("Name: %s\n", stud.name);
    printf("surname: %s\n", stud.surname);
    printf("Group: %s\n", stud.group);
    printf("Marks: %c %c %c %c %c\n", stud.marks[0], stud.marks[1], stud.marks[2], stud.marks[3], stud.marks[4]);
//    for (int i = 0; i < 5; ++i){
//        printf("%c ", stud.marks[i]);
//    }
//    printf("\n");
}

callback search_by_ID(Student *student, int size){
    if (!student)
        return MEMORY_ERROR;

    char ID[128];
    printf("Enter ID: ");
    if (scanf("%127s", ID) != 1 || !is_number(ID))
        return INPUT_ERROR;

    unsigned long tmp = strtoul(ID, NULL, 10);
    if (errno == ERANGE)
        return OVERFLOW;

    for (int i = 0; i < size; ++i){
        if (tmp == student[i].id){
            print_stud(student[i]);
            return SUCCESS;
        }
    }

    return DONT_FIND;
}

callback search_by_name(Student *student, int size, Student **arr, int *arr_size){
    if (!student || !arr)
        return MEMORY_ERROR;

    char name[128];
    *arr_size = 0;
    printf("Enter name: ");
    if (scanf("%127s", name) != 1)
        return INPUT_ERROR;

    Student *tmp_arr = (Student *)malloc(sizeof(Student) * size);
    if (!tmp_arr)
        return MEMORY_ERROR;

    for (int i = 0; i < size; ++i){
        if (!strcmp(name, student[i].name)){
            tmp_arr[(*arr_size)++] = student[i];
        }
    }

    *arr = tmp_arr;
    return SUCCESS;
}

callback search_by_surname(Student *student, int size, Student **arr, int *arr_size){
    if (!student || !arr)
        return MEMORY_ERROR;

    char name[128];
    *arr_size = 0;
        printf("Enter surname: ");
        if (scanf("%127s", name) != 1)
            return INPUT_ERROR;

    Student *tmp_arr = (Student *)malloc(sizeof(Student) * size);
    if (!tmp_arr)
        return MEMORY_ERROR;

    for (int i = 0; i < size; ++i){
        if (!strcmp(name, student[i].surname)){
            tmp_arr[(*arr_size)++] = student[i];
        }
    }

    *arr = tmp_arr;
    return SUCCESS;
}

callback search_by_group(Student *student, int size, Student **arr, int *arr_size){
    if (!student || !arr)
        return MEMORY_ERROR;

    char name[128];
    *arr_size = 0;
    printf("Enter group: ");
    if (scanf("%127s", name) != 1)
        return INPUT_ERROR;

    Student *tmp_arr = (Student *)malloc(sizeof(Student) * size);
    if (!tmp_arr)
        return MEMORY_ERROR;

    for (int i = 0; i < size; ++i){
        if (!strcmp(name, student[i].group)){
            tmp_arr[(*arr_size)++] = student[i];
        }
    }

    *arr = tmp_arr;
    return SUCCESS;
}

int id_cmp(const void *a, const void *b){
    return ((const Student *)a)->id - ((const Student *)b)->id;
}

int surname_cmp(const void *a, const void *b){
    return strcmp(((const Student *)a)->surname, ((const Student *)b)->surname);
}

int name_cmp(const void *a, const void *b){
    return strcmp(((const Student *)a)->name, ((const Student *)b)->name);
}

int group_cmp(const void *a, const void *b){
    return strcmp(((const Student *)a)->group, ((const Student *)b)->group);
}
callback inf_file(Student *data, int size, char *output){
    if (!data)
        return MEMORY_ERROR;
//    char output[128];
    char ID[128];
    double sum = 0;
//
//    printf("Enter output file: ");
//    if (scanf("%127s", output) != 1){
//        return INPUT_ERROR;
//    }
    printf("Enter ID: ");
    if (scanf("%127s", ID) != 1 || !is_number(ID)){
        return INPUT_ERROR;
    }

    unsigned long tmp = strtoul(ID, NULL, 10);
    if (errno == ERANGE)
        return OVERFLOW;

    FILE* out = fopen(output, "w");
    if (!out)
        return FILE_OPEN_ERROR;

    for (int i = 0; i < size; ++i){
        if (tmp == data[i].id){
            for (int j = 0; j < 5; ++j){
                sum += (data[i].marks[j] - '0');
            }
            fprintf(out, "%s %s %s %lf\n", data[i].surname, data[i].name, data[i].group, sum/5);
            fclose(out);
            return SUCCESS;
        }
    }

    fclose(out);
    return DONT_FIND;
}

callback above_average(Student *data, int size, char *file){
    if (!data)
        return MEMORY_ERROR;

    char output[128];
    printf("Enter output file: ");
    if (scanf("%127s", output) != 1){
        return INPUT_ERROR;
    }
    if (is_same_file(file, output))
        return ERROR_THE_SAME_FILE;

    FILE* out = fopen(output, "w");
    if (!out)
        return FILE_OPEN_ERROR;

    double sum = 0;
    for (int i = 0; i < size; ++i){
        for (int j = 0; j < 5; ++j){
            sum += (data[i].marks[j] - '0');
        }
    }

    sum /= (5 * size);

    for (int i = 0; i < size; ++i){
        double tmp_sum = 0;
        for (int j = 0; j < 5; ++j){
            tmp_sum += (data[i].marks[j] - '0');
        }
        tmp_sum /= 5;
        if (tmp_sum > sum){
            fprintf(out, "%s %s\n", data[i].surname, data[i].name);
        }
    }

    fclose(out);
    return SUCCESS;
}

void del_stud_data(Student *student, int size){
    if (!student)
        return;

    free(student->marks);
    free(student);
}
