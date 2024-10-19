#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

enum ret_type_t{
    SUCCESS,    //Успех
    ERROR_NO_VALUES, //Недостаточно аргументов
    ERROR_OPEN,  //Не удалось открыть файл
    ERROR_SAME,  //Одинаковые файлы
    ERROR_MALLOC,    //Ошибка памяти
    ERROR_THE_SAME_FILE
};

//Сравнение имен двух файлов по данным путям
int cmpFiles(char* pathOrig, char* pathRes){
    char buf1[PATH_MAX];
    char *res1 = realpath(pathOrig, buf1);
    char buf2[PATH_MAX];
    char *res2 = realpath(pathRes, buf2);
    if (res1 != NULL && res2 != NULL){
        if (!strcmp(res1, res2)){
            return ERROR_THE_SAME_FILE;
        }
    }
    return SUCCESS;
}

//Проверка флага
int findFlag(char* qarg, const char** flags, int size){
    for (int i = 0; i < size; ++i) {
        if (!(strcmp(qarg, flags[i]))) {
            return i;
        }
    }
    return -1;
}

//Переписывает лексемы в результирующий файл в порядке четности
int funcForR(char* path_lex1, char* path_lex2, char* path_res){

    FILE* fileLex1, *fileLex2, *fileRes;
    if (!fopen(path_lex1, "r") || !fopen(path_res, "w")){
        return ERROR_OPEN;
    }

    fileLex1 = fopen(path_lex1, "r");

    fileRes = fopen(path_res, "w");

    char c;
    int flag = 1;
    int iter = -1;
    int flagEOF1 = 0, flagEOF2 = 0;

    while ((flagEOF1 != 1) || (flagEOF2 != 1)){
        if ((flag > 0) && (flagEOF1 == 0)){
            c = fgetc(fileLex1);
            while ((c == ' ') || (c == '\t') || (c == '\n')){
                c = fgetc(fileLex1);
            }
            if (c == EOF){
                flagEOF1 = 1;
            }
            while ((c != ' ') && (c != '\t') && (c != '\n') && (c != EOF)){
                fputc(c, fileRes);
                c = fgetc(fileLex1);
            }
            fputc(' ', fileRes);
            if (c == EOF){
                flagEOF1 = 1;
            }
            if (flagEOF2 != 1){
                flag *= -1;
            }
        }

        if ((flag < 0) && (flagEOF2 == 0)){
            c = fgetc(fileLex2);
            while ((c == ' ') || (c == '\t') || (c == '\n')){
                c = fgetc(fileLex2);
            }
            if (c == EOF){
                flagEOF2 = 1;
            }
            while ((c != ' ') && (c != '\t') && (c != '\n') && (c != EOF)){
                fputc(c, fileRes);
                c = fgetc(fileLex2);
            }
            fputc(' ', fileRes);
            if (c == EOF){
                flagEOF2 = 1;
            }
            if (flagEOF1 != 1){
                flag *= -1;
            }
        }
    }

    fclose(fileLex1);
    fclose(fileLex2);
    fclose(fileRes);

    return SUCCESS;
}

//Пишет в файл код символа в нужной системе счисления
void printFound(char c, int foundation, FILE* dest){
    char number[100];

    int codeASCII = c;
    int i = 0;
    while(codeASCII != 0){
        int rem = codeASCII % foundation;
        if (rem <= 9){
            number[i] = (char)(rem + '0');
            i++;
        }
        else{
            number[i] = (char)(rem - 10 + 'A');
            i++;
        }
        codeASCII /= foundation;
    }
    number[i] = '\0';
    for (int j = i - 1; j >= 0; j--){
        fputc(number[j], dest);
    }

    return;
}

//Переписывает оригинальный файл в результирующий с кучей странных условий...
int funcForA(char* path_orig, char* path_res){

    FILE* fileOrig, *fileRes;
    if (!fopen(path_orig, "r") || !fopen(path_res, "w")){
        return ERROR_OPEN;
    }
    fileOrig = fopen(path_orig, "r");
    fileRes = fopen(path_res, "w");

    char c;
    int counter = 0;

    while ((c = fgetc(fileOrig)) != EOF){
        while ((c == ' ') || (c == '\t') || (c == '\n')){
            c = fgetc(fileOrig);
        }
        if (c == EOF){
            break;
        }

        counter++;
        while ((c != ' ') && (c != '\t') && (c != '\n') && (c != EOF)){


            if (counter % 10 == 0){
                if ((c >= 'A') && (c <= 'Z')){
                    c -= ('A' - 'a');
                    printFound(c, 4, fileRes);
                }
                else{
                    if ((c >= '0') && (c <= '9')){
                        fputc(c, fileRes);
                    }
                    else{
                        printFound(c, 4, fileRes);
                    }
                }
            }

            if ((counter % 2 == 0) && (counter % 10 != 0)){
                if ((c >= 'A') && (c <= 'Z')){
                    c = c - ('A' - 'a');
                }
                fputc(c, fileRes);
            }

            if ((counter % 5 == 0) && (counter % 10 != 0)){
                if ((c >= '0') && (c <= '9')){
                    fputc(c, fileRes);
                }
                else{
                    printFound(c, 8, fileRes);
                }
            }

            if ((counter % 2 != 0) && (counter % 5 != 0) && (counter % 10 != 0)){
                fputc(c, fileRes);
            }
            c = fgetc(fileOrig);
        }
        fputc(' ', fileRes);
        if (c == EOF){
            break;
        }
    }

    fclose(fileOrig);
    fclose(fileRes);

    return SUCCESS;
}

//Находит последнее вхождение символа
int lastPos(char* origStr, char ch){
    int res = -1;
    for (int i = 0; i < (int)strlen(origStr); i++){
        if (origStr[i] == ch){
            res = i;
        }
    }
    return res;
}

int main(int argc, char *argv[]){

    if (argc != 5 && argc != 4){
        printf("Wrong args count");
        return ERROR_NO_VALUES;
    }

    const char* flags[] = {"-r", "/r", "-a", "/a"};
    for (int i = 1; i < argc; ++i) {
        int ret = findFlag(argv[i], flags, sizeof(flags)/sizeof(char*));
        if (ret == -1) {
            printf("This flag doesn't exist: %s\n", argv[i]);
            return -1;
        }
        else {
            int findCbsInt = ret / 2;

            if (findCbsInt == 0){
                char* pathLex1, *pathLex2, *pathRes;

                pathLex1 = argv[i + 1];
                pathLex2 = argv[i + 2];
                pathRes = argv[i + 3];

                if ((cmpFiles(pathLex1, pathRes)) || (cmpFiles(pathLex2, pathRes))){
                    printf("You're trying to overwrite the file. You can't do it");
                    return ERROR_SAME;
                }

                //Обработка ошибок
                switch(funcForR(pathLex1, pathLex2, pathRes)){
                    case ERROR_OPEN:
                        printf("Can't open a file with this path");
                        break;
                }
            }
            else{
                char *pathRes;
                char *pathOrig;
                pathRes = argv[i+2];
                pathOrig = argv[i+1];

                if ((cmpFiles(pathOrig, pathRes))){
                    printf("You're trying to overwrite the file. You can't do it");
                    return ERROR_SAME;
                }

                //Обработка ошибок
                switch(funcForA(pathOrig, pathRes)){
                    case ERROR_OPEN:
                        printf("Can't open a file with this path");
                        break;
                }
            }

            return SUCCESS;
        }

    }


    return 0;
}