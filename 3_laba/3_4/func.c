#include "func.h"

void menu(){
    printf("Choose one\n1. Add mail\n2. Delete mail\n3. Search mail\n4. Search for delivered packages\n"
           "5. Search for sent overdue packages\n6. Change delivered\n0. End\n");
}

callback post_init(Post *post) {
    post->addr = (Address *)malloc(sizeof(Address));
    if (!post->addr)
        return MEMORY_ERROR;
    post->size_mail = 2;
    post->mail_data = (Mail *)malloc(sizeof(Mail) * post->size_mail);
    if (!post->mail_data) {
        free(post->addr);
        return MEMORY_ERROR;
    }

    return SUCCESS;
}

bool is_number(const char *str) {
    if (*str == '-') {
        str++;
    }

    bool hasDot = false;
    if (*str == '\0') return false;

    while (*str) {
        if (*str == '.') {
            if (hasDot) return false;
            hasDot = true;
        } else if (!isdigit(*str)) {
            return false;
        }
        str++;
    }
    return true;
}

bool is_natural_number(const char *str) {
    if (*str == '-') {
        str++;
    }

    bool hasDot = false;
    if (*str == '\0') return false;

    while (*str) {
        if (*str == '.' || !isdigit(*str))
            return false;
        str++;
    }
    return true;
}

void get_string_with_space(char *str, int len){
    char c;
    int size = 0;
    while (isspace(c = getchar()));

    str[size++] = c;


    while ((c = getchar()) != '\n'){
        str[size++] = c;
        if (size >= len - 1){
            break;
        }
    }

    str[size] = '\0';
}

callback get_address(Address *address){
    if (!address)
        return MEMORY_ERROR;

    char house_number[11], apartment_number[11], city[128], street[128], building[128], index[8];

    printf("Enter city: ");
    get_string_with_space(city, 128);
    //printf("city = %s\n", city);

    printf("Enter street: ");
    get_string_with_space(street, 128);
   // printf("street = %s\n", street);

    printf("Enter building: ");
    scanf("%127s", building);
   // printf("building = %s\n", building);

    printf("Enter index:(format: XXXXXX) ");
    scanf("%7s", index);
    for (int i = 0; i < 6; ++i){
        if (!isdigit(index[i])){
            //printf("aboba = %c\n", index[i]);
            return INPUT_ERROR;
        }
    }

        if (index[6] != '\0'){
            while (getchar() != '\n');
            return INPUT_ERROR;
    }

    printf("Enter house number: ");
    scanf("%10s", house_number);
    //printf("house number: %s\n", house_number);

    printf("Enter apartment number: ");
    scanf("%10s", apartment_number);
   // printf("apartment number: %s\n", apartment_number);

    if (!is_natural_number(house_number) || !is_natural_number(apartment_number))
        return INPUT_ERROR;

    unsigned long tmp1 = strtoul(house_number, NULL, 10);
    unsigned long tmp2 = strtoul(apartment_number, NULL, 10);
    if (errno == ERANGE)
        return OVERFLOW;
    if (tmp1 <= 0 || tmp2 <= 0)
        return INPUT_ERROR;

    address->apartment_number = tmp2;
    address->house_number = tmp1;

    address->city = create(city);
    //printf("city1 = %s city2 = %s\n", city, address->city.data);
    if (!address->city.data)
        return MEMORY_ERROR;

    address->street = create(street);
    if (!address->street.data){
        destroy(&address->city);
        return MEMORY_ERROR;
    }

    address->index = create(index);
    if (!address->index.data){
        destroy(&address->city);
        destroy(&address->street);
        return MEMORY_ERROR;
    }

    address->building = create(building);
    if (!address->building.data){
        destroy(&address->city);
        destroy(&address->street);
        destroy(&address->index);
        return MEMORY_ERROR;
    }

    return SUCCESS;
}

callback get_weight(double *weight){
    if (!weight)
        return MEMORY_ERROR;

    char res[128];
    printf("Enter mail weight: ");
    scanf("%127s", res);
   // printf("Weight: %s\n", res);
    if (!is_number(res))
        return INPUT_ERROR;
    double tmp = strtod(res, NULL);

    if (errno == ERANGE)
        return OVERFLOW;

    if (tmp < 0)
        return INPUT_ERROR;

    *weight = tmp;

    return SUCCESS;
}

callback get_postal_index(string *str){
    if(!str)
        return MEMORY_ERROR;

    char s[16];
    printf("Enter postal index:(14 digits) ");
    scanf("%15s", s);
    //printf("postal index 14: %s", s);
    if (s[14] != '\0')
        return INPUT_ERROR;

    for (int i = 0; i < 14; ++i){
        if (!isdigit(s[i]))
            return INPUT_ERROR;
    }

    *str = create(s);
    if (str->len == -1)
        return MEMORY_ERROR;

    return SUCCESS;
}

callback validate_time(const char *time){
    if (time[8] != '\0' || time[2] != ':' || time[5] != ':' || !isdigit(time[0]) || !isdigit(time[1]) || !isdigit(time[3]) || !isdigit(time[4]) || !isdigit(time[6]) || !isdigit(time[7]))
        return INPUT_ERROR;

    int h, m, s;
    h = atoi(time);
    m = atoi(time + 3);
    s = atoi(time + 6);

    if (h > 23 || m > 59 || s > 59)
        return INPUT_ERROR;

    return SUCCESS;
}

callback validate_date(const char *date){
    if (date[2] != ':' || date[5] != ':' || !isdigit(date[0]) || !isdigit(date[1]) || !isdigit(date[3])
    || !isdigit(date[4]) || !isdigit(date[6]) || !isdigit(date[7]) || !isdigit(date[8])
    || !isdigit(date[9]) || date[10] != '\0'){
        return INPUT_ERROR;
    }

    int y, m, d;
    d = atoi(date);
    m = atoi(date + 3);
    y = atoi(date + 6);
    if ((m == 4 || m == 6 || m == 9 || m == 11) && d > 30)
        return INPUT_ERROR;

    if (m == 2 && d > 28)
        return INPUT_ERROR;

    if (d > 31)
        return INPUT_ERROR;

    if (m > 12 || d == 0 || y < 1900 || y > 2025)
        return INPUT_ERROR;

    return SUCCESS;
}

callback get_create_time(string *str){
    if (!str)
        return MEMORY_ERROR;

    char date[24], time[10];
    printf("Enter create time(format: dd:MM:yyyy hh:mm:ss): ");
    scanf("%23s", date);
    scanf("%9s", time);

    if (validate_time(time) || validate_date(date))
        return INPUT_ERROR;

    strcat(date, " ");
    strcat(date, time);

    *str = create(date);
    if (str->len == -1)
        return MEMORY_ERROR;

    return SUCCESS;
}

callback get_getting_time(string *str){
    if (!str)
        return MEMORY_ERROR;

    char date[24], time[10];
    printf("Enter getting time(format: dd:MM:yyyy hh:mm:ss): ");
    scanf("%23s", date);
    scanf("%9s", time);

    if (validate_time(time) || validate_date(date))
        return INPUT_ERROR;

    strcat(date, " ");
    strcat(date, time);

    *str = create(date);
    if (str->len == -1)
        return MEMORY_ERROR;

    return SUCCESS;
}

int date_comparison(const char *s1, const char *s2){
    //format: dd:MM:yyyy hh:mm:ss
    //        0123456789012345678
    int tmp1, tmp2;

    tmp1 = atoi(s1 + 6);
    tmp2 = atoi(s2 + 6);
    if (tmp1 != tmp2)
        return tmp1 - tmp2;

    tmp1 = atoi(s1 + 3);
    tmp2 = atoi(s2 + 3);
    if (tmp1 != tmp2)
        return tmp1 - tmp2;

    tmp1 = atoi(s1);
    tmp2 = atoi(s2);
    if (tmp1 != tmp2)
        return tmp1 - tmp2;

    return strcmp(s1 + 11, s2 + 11);
}

callback clear_address(Address *address){
    destroy(&address->city);
    destroy(&address->street);
    destroy(&address->index);
    destroy(&address->building);

    return SUCCESS;
}

callback add_mail(Post *post, int *mails_count){

    Mail *tmp_for_realloc;
    if (*mails_count >= post->size_mail){
        post->size_mail *= 2;
        tmp_for_realloc = (Mail *)realloc(post->mail_data, post->size_mail * sizeof(Mail));

        if (!tmp_for_realloc)
            return MEMORY_ERROR;

        post->mail_data = tmp_for_realloc;
    }

    int callback;
    if ((callback = get_address(&post->mail_data[*mails_count].address)) != SUCCESS)
        return callback;

    if ((callback = get_weight(&post->mail_data[*mails_count].weight)) != SUCCESS){
        clear_address(&post->mail_data[*mails_count].address);
        return callback;
    }

    if ((callback = get_postal_index(&post->mail_data[*mails_count].postal_index)) != SUCCESS){
        clear_address(&post->mail_data[*mails_count].address);
        post->mail_data[*mails_count].weight = -1;
        return callback;
    }

    if ((callback = get_create_time(&post->mail_data[*mails_count].create_time)) != SUCCESS){
        clear_address(&post->mail_data[*mails_count].address);
        destroy(&post->mail_data[*mails_count].postal_index);
        post->mail_data[*mails_count].weight = -1;
        return callback;
    }

    if ((callback = get_getting_time(&post->mail_data[*mails_count].getting_time)) != SUCCESS){
        clear_address(&post->mail_data[*mails_count].address);
        destroy(&post->mail_data[*mails_count].postal_index);
        destroy(&post->mail_data[*mails_count].create_time);
        post->mail_data[*mails_count].weight = -1;
        return callback;
    }

    if (date_comparison(post->mail_data[*mails_count].getting_time.data, post->mail_data[*mails_count].create_time.data) <= 0){
        clear_address(&post->mail_data[*mails_count].address);
        destroy(&post->mail_data[*mails_count].postal_index);
        destroy(&post->mail_data[*mails_count].create_time);
        destroy(&post->mail_data[*mails_count].getting_time);
        post->mail_data[*mails_count].weight = -1;
        return INPUT_ERROR;
    }
    post->mail_data->delivered = '0';

    ++(*mails_count);
    return SUCCESS;
}

int compare_mails(const void *a, const void *b){
    const Mail *mail_1 = (const Mail *)a;
    const Mail *mail_2 = (const Mail *)b;
    int callback = string_compare(&mail_1->address.index, &mail_2->address.index);

    return !callback ? callback : string_compare(&mail_1->postal_index, &mail_2->postal_index);
}

void clear_mail(Mail *mail){
    clear_address(&mail->address);
    destroy(&mail->postal_index);
    destroy(&mail->create_time);
    destroy(&mail->getting_time);
}

callback del_mail(Post *post, int *mails_count){
    if (!post || !post->mail_data)
        return MEMORY_ERROR;

    string ind;
    int callback = get_postal_index(&ind);

    if (callback)
        return callback;

    for (int i = 0; i < *mails_count; ++i){
        if (string_equivalence(&post->mail_data[i].postal_index, &ind)){
            clear_mail(&post->mail_data[i]);
            destroy(&ind);
            for (int j = i; j < (*mails_count - 1); ++j) {
                post->mail_data[i] = post->mail_data[i + 1];
            }
            (*mails_count)--;
            return SUCCESS;
        }
    }
    destroy(&ind);
    return DONT_FIND;
}

callback search_mail(Post *post, int *mails_count){
    if (!post || !post->mail_data)
        return MEMORY_ERROR;

    string ind;
    int callback = get_postal_index(&ind);

    if (callback)
        return callback;

    for (int i = 0; i < *mails_count; ++i){
        if (string_equivalence(&post->mail_data[i].postal_index, &ind)){
            print_mail(&post->mail_data[i]);
            destroy(&ind);
            return SUCCESS;
        }
    }

    destroy(&ind);
    return DONT_FIND;
}

void print_address(Address *address){
    printf("City: %s\n", address->city.data);
    printf("Street: %s\n", address->street.data);
    printf("Building: %s\n", address->building.data);
    printf("Index: %s\n", address->index.data);
    printf("Apartment number: %lu\n", address->apartment_number);
    printf("House number: %lu\n", address->house_number);
}

void print_mail(Mail *mail){
    print_address(&mail->address);
    printf("Weight: %lf\n", mail->weight);
    printf("Postal index: %s\n", mail->postal_index.data);
    printf("Create time: %s\n", mail->create_time.data);
    printf("Getting time: %s\n", mail->getting_time.data);
}

callback search_delivered(Post *post, int mails_count, int *count_delivered, Mail **delivered){

    if (!post || !post->mail_data || !delivered)
        return MEMORY_ERROR;

    Mail *tmp_delivered = (Mail *)malloc(sizeof(Mail) * mails_count);
    if (!tmp_delivered)
        return MEMORY_ERROR;

    for (int i = 0; i < mails_count; ++i){
        if (post->mail_data[i].delivered == '1'){
            tmp_delivered[(*count_delivered)++] = post->mail_data[i];
        }
    }

    qsort(tmp_delivered, *count_delivered, sizeof(Mail), compare_time_mails);
    *delivered = tmp_delivered;
    return SUCCESS;
}

int compare_time_mails(const void *a, const void *b) {
    const Mail *mail_1 = (const Mail *)a;
    const Mail *mail_2 = (const Mail *)b;

    string time_1, time_2;
    time_1 = mail_1->create_time;
    time_2 = mail_2->create_time;

    int param_1, param_2;
    param_1 = atoi(time_1.data + 6);
    param_2 = atoi(time_2.data + 6);
    if (param_1 != param_2)
        return param_1 - param_2;

    param_1 = atoi(time_1.data + 3);
    param_2 = atoi(time_2.data + 3);
    if (param_1 != param_2)
        return param_1 - param_2;


    param_1 = atoi(time_1.data);
    param_2 = atoi(time_2.data);
    if (param_1 != param_2)
        return param_1 - param_2;

    return strcmp(time_1.data + 11, time_2.data + 11);
}

callback search_overdue(Post *post, int mails_count, int *overdue_count, Mail **overdue){
    if (!post || !post->mail_data || !overdue)
        return MEMORY_ERROR;

    Mail *tmp_overdue = (Mail *)malloc(sizeof(Mail) * mails_count);
    if (!tmp_overdue)
        return MEMORY_ERROR;

    time_t _cur_time = time(NULL);
    struct tm *now = localtime(&_cur_time);
    char cur_time[24];
    sprintf(cur_time, "%02d:%02d:%d %02d:%02d:%02d", now->tm_mday, now->tm_mon + 1, now->tm_year + 1900, now->tm_hour, now->tm_min, now->tm_sec);

    for(int i = 0; i < mails_count; ++i){
        if (post->mail_data[i].delivered == '0' && date_comparison(post->mail_data[i].getting_time.data, cur_time) < 0){
            tmp_overdue[(*overdue_count)++] = post->mail_data[i];
        }
    }

    qsort(tmp_overdue, *overdue_count, sizeof(Mail), compare_time_mails);
    *overdue = tmp_overdue;
    return SUCCESS;
}

void clear_post(Post *post, int mails_count){
    clear_address(post->addr);
    free(post->addr);
    for (int i = 0; i < mails_count; ++i){
        clear_mail(&post->mail_data[i]);
    }
    free(post->mail_data);
}

callback change(Post *post, int mails_count, int *count_delivered){
    if (!post || !post->mail_data)
        return MEMORY_ERROR;
    string ind;
    int callback = get_postal_index(&ind);

    if (callback)
        return callback;

    char c[3];
    printf("Enter 1 if delivered, or 0 if not delivered: ");
    scanf("%2s", c);
    if (strcmp(c, "1") != 0 && strcmp(c, "0") != 0)
        return INPUT_ERROR;

    for (int i = 0; i < mails_count; ++i){
        if (string_equivalence(&post->mail_data[i].postal_index, &ind)){
            if (!strcmp(c, "1"))
                post->mail_data[i].delivered = '1';
            else
                post->mail_data[i].delivered = '0';
            destroy(&ind);
            return SUCCESS;
        }
    }

    destroy(&ind);
    return DONT_FIND;
}