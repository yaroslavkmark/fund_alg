#include "third_lesson.h"

int main(int argc, char* argv[]){
    const char* flags[] = {"-q", "/q", "-m", "/m", "-t", "/t"};

    for (int i = 1; i < argc; i++)
    {
        int ret = findflag(argv[i], flags, sizeof(flags)/sizeof(char*));
        if(ret == FLAG_ERROR){
            printf("NOT EXISTING FLAG - %s\n", argv[i]);
            return -1;
        }
        else{
            switch ((char)ret)
            {
                case 'q':
                {
                    int rt = SUCCESS;
                    double* argq = checking_for_right(flags, argv, argc, &i, 4, &rt, sizeof(flags)/sizeof(char*));
                    if(rt == BREAK){
                        free(argq);
                        break;
                    }
                    else if(rt == MUST_BE_A_NUMBER_ERROR_BREAK){
                        printf("The argument must be a number!\n");
                        break;
                    }
                    else if(rt == CONTINUE){
                        free(argq);
                        continue;
                    }
                    else if(rt == ERROR){
                        printf("Memory error!\n");
                    }
                    func_for_q(argq[0], argq[1], argq[2], argq[3]);
                    free(argq);
                    break;
                }

                case 'm':
                {
                    if (is_int(argv[2]) == MUST_BE_A_NON_ZERO_INT || is_int(argv[3]) == MUST_BE_A_NON_ZERO_INT){
                        printf("Must be a non_zero integer\n");
                    }
                    else{
                        int x1 = atoi(argv[2]), x2 = atoi(argv[3]);
                        int r = is_miltiple(x1, x2);
                        if(r == MULTIPLE){
                            printf("The first number is a multiple of the second.\n");
                        }
                        else if(r == N_MULTIPLE){
                            printf("The first numbers is not multiple of the second.\n");
                        }
                        break;
                    }
                }

                    break;
                case 't':
                {
                    int rt = SUCCESS;
                    double* argq = checking_for_right(flags, argv, argc, &i, 4, &rt, sizeof(flags)/sizeof(char*));
                    if(rt == BREAK){
                        free(argq);
                        break;
                    }
                    else if(rt == MUST_BE_A_NUMBER_ERROR_BREAK){
                        printf("The argument must be a number!\n");
                        break;
                    }
                    else if(rt == CONTINUE){
                        free(argq);
                        continue;
                    }
                    else if(rt == ERROR){
                        printf("Memory error!\n");
                    }
                    int r = is_rectangular(argq[0], argq[1], argq[2], argq[3]);
                    if(r == RECTANGLE){
                        printf("It can be a rectangular.\n");
                    }
                    else if(r == N_RECTANGLE){
                        printf("It can't be a rectangular.\n");
                    }
                    else if(r == MUST_BE_BIGGER_THAN_ZERO){
                        printf("Sides must be bigger than 0\n");
                    }
                    free(argq);
                    break;
                }
                default:
                    break;
            }
        }
    }

    return 0;
}