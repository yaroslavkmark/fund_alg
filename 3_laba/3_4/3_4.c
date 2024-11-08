#include "func.h"

int main(){
    // Part 1
    printf("\t\t Part 1\n");

    string s1, s2 = {0, NULL};
    s1 = create("123123");
    if (s1.len == -1){
        destroy(&s2);
        destroy(&s1);
        printf("NULL\n");
        return MEMORY_ERROR;
    }
    int res = copy(&s2, s1);
    if (!res){
        printf("%s %d\n", s2.data, s2.len);
        printf("%s %d\n", s1.data, s1.len);
    } else {
        destroy(&s2);
        destroy(&s1);
        printf("Failed to copy to new string\n");
        return MEMORY_ERROR;
    }

    int tmp = concatenate(&s1, s2);
    if (!tmp){
        printf("%s %d\n", s2.data, s2.len);
        printf("%s %d\n", s1.data, s1.len);
    } else{
        destroy(&s2);
        destroy(&s1);
        printf("Failed to concatenate\n");
        return MEMORY_ERROR;
    }

    string s3 = copy_to_new(s1);
    if (s3.data) {
        printf("Copied to new string: %s, Length: %d\n", s3.data, s3.len);
    } else {
        destroy(&s2);
        destroy(&s1);
        destroy(&s3);
        printf("Failed to copy to new string\n");
        return MEMORY_ERROR;
    }

    printf("%s %d\n", s2.data, s2.len);
    printf("%s %d\n", s1.data, s1.len);
    printf("%s %d\n", s3.data, s3.len);
    if (!string_compare(&s2, &s3))
        printf("Строки равны\n");
    else
        printf("%d <- Если отриц то первая меньше чем вторая если положит то вторая больше первой\n", string_compare(&s3, &s2));


    if (string_equivalence(&s1, &s3)){
        printf("<%s> == <%s> Равны\n", s1.data, s3.data);
    } else{
        printf("<%s> != <%s> Не равны\n", s1.data, s2.data);
    }

    destroy(&s2);
    destroy(&s1);
    destroy(&s3);
    // Part 2
    printf("\t\t Part 2\n");

    int overdue_count;
    int count_delivered;
    Mail *delivered = NULL;
    Mail *overdue = NULL;

    Post post;
    int mails_count = 0;
    char ans;
    char input[MAX_INPUT_SIZE];
    int callback;

    if (post_init(&post)){
        printf("MEMORY_ERROR\n");
        return MEMORY_ERROR;
    }

    printf("Enter the address of the current post office:\n");
    callback = get_address(post.addr);
    while (callback) {
        if (callback == MEMORY_ERROR) {
            printf("Address: Memory error\n");;
            return MEMORY_ERROR;
        }
        printf("\nThe data is incorrect!\n");
        printf("Enter the address of the current post office:\n");
        callback = get_address(post.addr);
    }

    menu();

    while(1){
        if (fgets(input, sizeof(input), stdin) != NULL){
            input[strcspn(input, "\n")] = 0;
            if (strlen(input) == 1){
                ans = input[0];

                if (ans == '0')break;

                switch (ans) {

                    case '1':
                        callback = add_mail(&post, &mails_count);
                        if (callback == MEMORY_ERROR)
                            printf("MEMORY_ERROR\n");
                        else if (callback == INPUT_ERROR)
                            printf("INPUT_ERROR\n");
                        else
                            printf("Goooooal\n");
                        qsort(post.mail_data, mails_count, sizeof(Mail), compare_mails);
                        break;

                    case '2':
                        callback = del_mail(&post, &mails_count);
                        if (callback == MEMORY_ERROR)
                            printf("MEMORY_ERROR\n");
                        else if (callback == INPUT_ERROR)
                            printf("INPUT_ERROR\n");
                        else if (callback == DONT_FIND){
                            printf("the specified flag is not in the data\n");
                        }
                        else
                            printf("Goooooal\n");
                        break;

                    case '3':
                        callback = search_mail(&post, &mails_count);
                        if (callback == MEMORY_ERROR)
                            printf("MEMORY_ERROR\n");
                        else if (callback == INPUT_ERROR)
                            printf("INPUT_ERROR\n");
                        else if (callback == DONT_FIND)
                            printf("the specified flag is not in the data\n");

                        else
                            printf("Goooooal\n");
                        break;

                    case '4':
                        count_delivered = 0;
                        callback = search_delivered(&post, mails_count, &count_delivered, &delivered);
                        if (callback == MEMORY_ERROR)
                            printf("MEMORY_ERROR\n");
                        else if (callback == INPUT_ERROR)
                            printf("INPUT_ERROR\n");
                        else{
                            if (!count_delivered)
                                printf("No delivered\n");
                            else{
                                for (int i = 0; i < count_delivered; ++i) {
                                    print_mail(&delivered[i]);
                                    printf("\n");
                                }

                                free(delivered);
                            }
                        }
                        break;

                    case '5':
                        overdue_count = 0;
                        callback = search_overdue(&post, mails_count, &overdue_count, &overdue);
                        if (callback == MEMORY_ERROR)
                            printf("MEMORY_ERROR\n");
                        else if (callback == INPUT_ERROR)
                            printf("INPUT_ERROR\n");
                        else{
                            if (!overdue_count)
                                printf("No delivered\n");
                            else{
                                for (int i = 0; i < overdue_count; ++i) {
                                    print_mail(&overdue[i]);
                                    printf("\n");
                                }
                                free(overdue);
                            }
                        }
                        break;

                    case '6':
                        callback = change(&post, mails_count, &count_delivered);
                        if (callback == MEMORY_ERROR)
                            printf("MEMORY_ERROR\n");
                        else if (callback == INPUT_ERROR)
                            printf("INPUT_ERROR\n");
                        else if (callback == DONT_FIND)
                            printf("the specified flag is not in the data\n");
                        break;
                    default:
                        printf("Incorrect input. Try again\n");
                        break;
                }
            } else if (strlen(input) != 0){
                printf("Некорректный ввод. Пожалуйста, попробуйте снова.\n");
            }
        }
        if (strlen(input) != 0)
            menu();
    }

    clear_post(&post, mails_count);
    return SUCCESS;
}
