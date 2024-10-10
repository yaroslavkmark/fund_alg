#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

enum ret_type_t{
    SUCCESS,    //Успех
    ERROR_OPEN,  //Не удалось открыть файл
};
typedef int (*callback)(char*, char*);

bool find_element(const char *db_output_file[], const char *element_to_check, int argc){

    for (int i = 0; i < argc; i++) {
        if (strcmp((db_output_file[i]), element_to_check) == 0) {
            return true;
        }
    }
    return false;
}

int process_file(const char *input_file, const char *output_file, int exclude_numbers, int count_letters, int count_specials, int substitute_specials, const char *db_output_file[], int argc) {
    FILE *in = fopen(input_file, "r");
    if (!in) {
        return ERROR_OPEN;
    }
    FILE *out;
    if (find_element(db_output_file, output_file, argc)){
        out = fopen(output_file, "a+");
        if (!out) {
            fclose(in);
            return ERROR_OPEN;
        }
    }
    else{
        out = fopen(output_file, "w+");
        if (!out) {
            fclose(in);
            return ERROR_OPEN;
        }
    }


    char line[256];
    while (fgets(line, sizeof(line), in)) {
        char processed_line[256] = "";
        int letter_count = 0;
        int special_count = 0;
        for (int i = 0; line[i] != '\0'; i++) {
            char c = line[i];
            if (exclude_numbers && isdigit(c)) {
                continue;
            }
            if (isalpha(c)) {
                letter_count++;
            } else if (!isalnum(c) && c != ' '){
                special_count++;
            }
            if (substitute_specials && !isdigit(c)) {
                char buffer[10];
                sprintf(buffer, "\\x%02X", (unsigned char)c);
                strcat(processed_line, buffer);
            } else {
                strncat(processed_line, &c, 1);
            }
        }

        if (exclude_numbers){
            fprintf(out, "%s", processed_line);
        }

        if (substitute_specials){
            fprintf(out, "%s\n", processed_line);
        }

        if (count_letters) {
            fprintf(out, "Latin letters count: %d\n", letter_count);
        }

        if (count_specials) {
            fprintf(out, "Special characters count: %d\n", special_count);
        }

    }

    fclose(in);
    fclose(out);
    return SUCCESS;
}

int findFlag(char* qarg, const char** flags, int size) {
    for (int i = 0; i < size; ++i) {
        if (!(strcmp(qarg, flags[i]))) {
            return i;
        }
    }

    return -1;
}

int main(int argc, char *argv[]) {
//    char *argv[] = {"./number_program", "-nd", "/home/myar/Документы/123", "11"};
//    int argc = 4;
    if (argc < 3) {
        printf("Неверное количество аргументов.\n");
        return 1;
    }
    const char* flags[] = {"-d", "/d", "-i", "/i", "-s", "/s", "-a", "/a", "-nd", "/nd", "-ni", "/ni", "-ns", "/ns", "-na", "/na"};
    const char *db_output_file[argc];
    int len = 0;
    for (int i = 1; i < argc; ++i) {
        int exclude_numbers = 0;
        int count_letters = 0;
        int count_specials = 0;
        int substitute_specials = 0;
        int ret = findFlag(argv[i], flags, sizeof(flags) / sizeof(char*));
        if (ret == -1) {
            printf("THIS FLAG DOES NOT EXIST: %s\n", argv[i]);
            return 0;
        } else {
            char *input_file;
            char *output_file;
            int findCbsInt = ret / 2;
            if (findCbsInt == 0) {
                exclude_numbers = 1;
            } else if (findCbsInt == 1) {
                count_letters = 1;
            } else if (findCbsInt == 2) {
                count_specials = 1;
            } else if (findCbsInt == 3) {
                substitute_specials = 1;
            } else if (findCbsInt == 4) {
                exclude_numbers = 1;
                if (i + 2 < argc){
                    input_file = argv[i + 1];
                }
                else{
                    printf("Не хватает аргумента\n");
                    return 0;
                }
                if (i + 1 < argc) {
                     output_file = argv[i + 2];
                }
                else{
                    printf("Не хватает аргумента\n");
                    return 0;
                }
            } else if (findCbsInt == 5) {
                count_letters = 1;
                if (i + 1 < argc) {
                    output_file = argv[i + 2];
                }
                else{
                    printf("Не хватает аргумента\n");
                    return 0;
                }
                if (i + 2 < argc){
                    input_file = argv[i + 1];
                }
                else{
                    printf("Не хватает аргумента\n");
                    return 0;
                }
            } else if (findCbsInt == 6) {
                count_specials = 1;
                if (i + 1 < argc) {
                    output_file = argv[i + 2];
                }
                else{
                    printf("Не хватает аргумента\n");
                    return 0;
                }
                if (i + 2 < argc){
                    input_file = argv[i + 1];
                }
                else{
                    printf("Не хватает аргумента\n");
                    return 0;
                }
            } else if (findCbsInt == 7) {
                substitute_specials = 1;
                if (i + 1 < argc) {
                    output_file = argv[i + 2];
                }
                else{
                    printf("Не хватает аргумента\n");
                    return 0;
                }
                if (i + 2 < argc){
                    input_file = argv[i + 1];
                }
                else{
                    printf("Не хватает аргумента\n");
                    return 0;
                }
            }
            int h;
            if (findCbsInt < 4){
                if (i + 1 < argc){
                    input_file = argv[i + 1];
                    char *token = strrchr(input_file, '/');
                    token += 1;
                    char output_path[256];

                    char *last_slash = strrchr(input_file, '/');

                    if (last_slash != NULL) {
                        size_t length = last_slash - input_file + 1;
                        strncpy(output_path, input_file, length);
                        output_path[length] = '\0';
                    }
                    char output_file1[256];
                    if (token != NULL){
                        int u = snprintf(output_file1, sizeof(output_file1), "%sout_%s", output_path,token);
                        if (u < 0){
                            abort();
                        }
                    }
                    else{
                        snprintf(output_file1, sizeof(output_file1), "out_%s", input_file);
                    }
                    if (input_file == output_file){
                        printf("Имя файла для ввода и вывода совпадает\n");
                        return 0;
                    }
                    h = process_file(input_file, output_file1, exclude_numbers, count_letters, count_specials, substitute_specials, db_output_file, len);
                    db_output_file[len] = output_file1;
                }
                else{
                    printf("Не хватает аргумента\n");
                    return 0;
                }
                ++i;
            }
            else{
                if (input_file == output_file){
                    printf("Имя файла для ввода и вывода совпадает\n");
                    return 0;
                }
                h = process_file(input_file, output_file, exclude_numbers, count_letters, count_specials, substitute_specials, db_output_file, len);
                db_output_file[len] = output_file;
                i += 2;
            }
            ++len;
            switch (h) {
                case ERROR_OPEN:
                    perror("Unable to open file");
                    break;
            }
        }

    }

    return 0;
}
