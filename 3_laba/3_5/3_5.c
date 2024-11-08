#include "Student.h"


int main(int argc, char *argv[]){
    if (argc != 3){
        printf("Expected <program file> <input file> <output file>\n");
        return INPUT_ERROR;
    }

    FILE *in = fopen(argv[1], "r");
    if (!in)
        return FILE_OPEN_ERROR;

    Student *student_data = NULL;
    int size_data;
    double avg;

    int res = read_student(&student_data, in, &size_data, &avg);
    if (res != SUCCESS){
        if (res == INPUT_ERROR)
            printf("INPUT ERROR\n");

        if (res == MEMORY_ERROR)
            printf("MEMORY_ERROR\n");

        return res;
    }
//    for (int i = 0; i < size_data; ++i){
//        printf("%lu %s %s %s\n", student_data->id, student_data->name, student_data->surname, student_data->group);
//        for (int j = 0; j < 5; ++j){
//            printf("%c ", student_data->marks[j]);
//        }
//        printf("\n");
//    }
    fclose(in);

    char input[MAX_INPUT_SIZE];
    char ans;
    int callback;
    Student *arr;
    int size_arr;
    menu();
    while (1){
        if (fgets(input, sizeof(input), stdin) != NULL){
            input[strcspn(input, "\n")] = 0;
            if (strlen(input) == 1){
                ans = input[0];

                if (ans == '0')break;

                switch (ans) {
                    case '1':
                        callback = search_by_ID(student_data, size_data);
                        if (callback == MEMORY_ERROR)
                            printf("MEMORY_ERROR\n");
                        else if (callback == INPUT_ERROR)
                            printf("INPUT_ERROR\n");
                        else if (callback == DONT_FIND)
                            printf("the specified flag is not in the data\n");

                        break;

                    case '3':
                        callback = search_by_name(student_data, size_data, &arr, &size_arr);
                        if (callback == MEMORY_ERROR)
                            printf("MEMORY_ERROR\n");
                        else if (callback == INPUT_ERROR)
                            printf("INPUT_ERROR\n");
                        else{
                            if (!size_arr)
                                printf("there is no such name in the data\n");
                            else{
                                for (int i = 0; i < size_arr; ++i){
                                    printf("Stud %d:\n", i + 1);
                                    print_stud(arr[i]);
                                    printf("\n");
                                }
                                free(arr);
                            }
                        }
                        break;

                    case '2':
                        callback = search_by_surname(student_data, size_data, &arr, &size_arr);
                        if (callback == MEMORY_ERROR)
                            printf("MEMORY_ERROR\n");
                        else if (callback == INPUT_ERROR)
                            printf("INPUT_ERROR\n");
                        else{
                            if (!size_arr)
                                printf("there is no such surname in the data\n");
                            else{
                                for (int i = 0; i < size_arr; ++i){
                                    printf("Stud %d:\n", i + 1);
                                    print_stud(arr[i]);
                                    printf("\n");
                                }
                                free(arr);
                            }
                        }
                        break;

                    case '4':
                        callback = search_by_group(student_data, size_data, &arr, &size_arr);
                        if (callback == MEMORY_ERROR)
                            printf("MEMORY_ERROR\n");
                        else if (callback == INPUT_ERROR)
                            printf("INPUT_ERROR\n");
                        else{
                            if (!size_arr)
                                printf("there is no such surname in the data\n");
                            else{
                                for (int i = 0; i < size_arr; ++i){
                                    printf("Stud %d:\n", i + 1);
                                    print_stud(arr[i]);
                                    printf("\n");
                                }
                                free(arr);

                            }
                        }
                        break;

                    case '5':
                        qsort(student_data, size_data, sizeof(Student), id_cmp);
                        printf("SUCCESS\n");
                        break;


                    case '6':
                        qsort(student_data, size_data, sizeof(Student), surname_cmp);
                        printf("SUCCESS\n");
                        break;

                    case '7':
                        qsort(student_data, size_data, sizeof(Student), name_cmp);
                        printf("SUCCESS\n");
                        break;

                    case '8':
                        qsort(student_data, size_data, sizeof(Student), group_cmp);
                        printf("SUCCESS\n");
                        break;

                    case '9':
                        if (is_same_file(argv[1], argv[2])){
                            printf("is same file\n");
                            break;
                        }
                        callback = inf_file(student_data, size_data, argv[2]);
                        if (callback == MEMORY_ERROR)
                            printf("MEMORY_ERROR\n");
                        else if (callback == INPUT_ERROR)
                            printf("INPUT_ERROR\n");
                        else if (callback == DONT_FIND)
                            printf("the specified flag is not in the data\n");
                        else if (callback == FILE_OPEN_ERROR)
                            printf("FILE_OPEN_ERROR\n");
                        else
                            printf("SUCCESS\n");
                        break;

                    case 's':
                        callback = above_average(student_data, size_data, argv[1]);
                        if (callback == MEMORY_ERROR)
                            printf("MEMORY_ERROR\n");
                        else if (callback == INPUT_ERROR)
                            printf("INPUT_ERROR\n");
                        else if (callback == DONT_FIND)
                            printf("the specified flag is not in the data\n");
                        else if (callback == FILE_OPEN_ERROR)
                            printf("FILE_OPEN_ERROR\n");
                        else if (callback == ERROR_THE_SAME_FILE)
                            printf("IS SAME FILE\n");
                        else
                            printf("SUCCESS\n");
                        break;

                    case 'q':
                        for (int i = 0; i < size_data; ++i){
                            printf("%lu ", student_data[i].id);
                            printf("%s ", student_data[i].name);
                            printf("%s ", student_data[i].surname);
                            printf("%s ", student_data[i].group);
                            printf("%c %c %c %c %c\n", student_data[i].marks[0], student_data[i].marks[1], student_data[i].marks[2], student_data[i].marks[3], student_data[i].marks[4]);

                        }
                        break;

                    default:
                        printf("Некорректный ввод. Пожалуйста, попробуйте снова.\n");
                        break;
                }
            } else if (strlen(input) != 0){
                printf("Некорректный ввод. Пожалуйста, попробуйте снова.\n");
            }
            //while (getchar() != '\n');
        }
        if (strlen(input) != 0)
            menu();
    }
    del_stud_data(student_data, size_data);
    return SUCCESS;
}