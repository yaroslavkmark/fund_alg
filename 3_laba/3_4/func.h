#ifndef UNTITLED1_FUNC_H
#define UNTITLED1_FUNC_H

#include "my_string.h"
#include <ctype.h>
#include <stdbool.h>
#include <errno.h>
#include <time.h>

#define MAX_INPUT_SIZE 100

typedef struct Address{
    string city, street, building, index;
    unsigned long house_number, apartment_number;
} Address;

typedef struct Mail{
    Address address;
    double weight;
    string postal_index, create_time, getting_time;
    char delivered;
} Mail;

typedef struct Post{
    Address *addr;
    Mail *mail_data;
    int size_mail;
} Post;

int compare_mails(const void *a, const void *b);
void menu();
void get_string_with_space(char *str, int len);
callback post_init(Post *post);
callback validate_date(const char *date);
callback validate_time(const char *time);
callback add_mail(Post *post, int *mails_count);
callback get_weight(double *weight);
callback get_address(Address *address);
callback get_postal_index(string *str);
callback get_create_time(string *str);
callback get_getting_time(string *str);
int date_comparison(const char *s1, const char *s2);
callback clear_address(Address *address);
bool is_number(const char *str);
bool is_natural_number(const char *str);
callback del_mail(Post *post, int *mails_count);
void clear_mail(Mail *mail);
callback search_mail(Post *post, int *mails_count);
void print_mail(Mail *mail);
void print_address(Address *address);
callback search_delivered(Post *post, int mails_count, int *count_delivered, Mail **delivered);
int compare_time_mails(const void *a, const void *b);
callback search_overdue(Post *post, int mails_count, int *overdue_count, Mail **overdue);
void clear_post(Post *post, int mails_count);
callback change(Post *post, int mails_count, int *count_delivered);

#endif
