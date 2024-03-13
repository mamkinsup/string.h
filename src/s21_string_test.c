#include "s21_string_test.h"

#include <stdio.h>
#include <stdlib.h>

#include "s21_string.h"

int main() {
    s21_strlen_test();
    s21_strcmp_test();
    s21_strcpy_test();
    s21_strcat_test();
    s21_strchr_test();
    s21_strstr_test();
    s21_strtok_test();
    return 0;
}

void s21_strlen_test() {
    char *one = "Hello world!\0";
    if (s21_strlen(one) == 12) {
        printf("|Hello world!| %lu | SUCCES\n", s21_strlen(one));
    } else {
        printf("|Hello world!| %lu | FAIL\n", s21_strlen(one));
    }
    char *two = "";
    if (s21_strlen(two) == 0) {
        printf("|| %lu | SUCCES\n", s21_strlen(two));
    } else {
        printf("|| %lu | FAIL\n", s21_strlen(two));
    }
    char *three = "                           23                           ";
    if (s21_strlen(three) == 56) {
        printf(
            "|                           23                           | %lu | "
            "SUCCES\n",
            s21_strlen(three));
    } else {
        printf(
            "|                           23                           | %lu | "
            "FAIL\n",
            s21_strlen(three));
    }
}
void s21_strcmp_test() {
    char *x1 = "Hello world!\0";
    char *x2 = "Hi, how are you!\0";
    if (s21_strcmp(x1, x2) < 0) {
        printf("\n|Hello world!| |Hi, how are you | %d | SUCCES\n", s21_strcmp(x1, x2));
    } else {
        printf("\n|Hello world!| |Hi, how are you | %d | FAIL\n", s21_strcmp(x1, x2));
    }
    char *y1 = "               23                  \0";
    char *y2 = "               \0";
    if (s21_strcmp(y1, y2) > 0) {
        printf(
            "|               23                   | | Hi, how are you | %d | "
            "SUCCES\n",
            s21_strcmp(y1, y2));
    } else {
        printf(
            "|               23                   | | Hi, how are you | %d | "
            "FAIL\n",
            s21_strcmp(y1, y2));
    }
    char *z1 = "\0";
    char *z2 = "\0";
    if (s21_strcmp(z1, z2) == 0) {
        printf("|| || %d | SUCCES\n", s21_strcmp(z1, z2));
    } else {
        printf("|| || %d | FAIL\n", s21_strcmp(z1, z2));
    }
}

void s21_strcpy_test() {
    char *string_one = "Hello world!\0";
    char *string_one_copy = malloc(sizeof(char) * s21_strlen(string_one));
    s21_strcpy(string_one_copy, string_one);
    if (*string_one == *string_one_copy) {
        printf("\n%s|one| |one_copy| %s | SUCCES\n", string_one, string_one_copy);
    } else {
        printf("%s|one| |one_copy| %s | FAIL\n", string_one, string_one_copy);
    }
    free(string_one_copy);
    char *string_two = "\0";
    char *string_two_copy = malloc(sizeof(char) * s21_strlen(string_two));
    s21_strcpy(string_two_copy, string_two);
    if (*string_two == *string_two_copy) {
        printf("%s|two| |two_copy| %s | SUCCES\n", string_two, string_two_copy);
    } else {
        printf("%s|two| |two_copy| %s | FAIL\n", string_two, string_two_copy);
    }
    free(string_two_copy);
    char *string_three = "                           23                           \0";
    char *string_three_copy = malloc(sizeof(char) * s21_strlen(string_three));
    s21_strcpy(string_three_copy, string_three);
    if (*string_three == *string_three_copy) {
        printf("%s|three| |three_copy| %s | SUCCES\n", string_three, string_three_copy);
    } else {
        printf("%s|three| |three_copy| %s | FAIL\n", string_three, string_three_copy);
    }
    free(string_three_copy);
}

void s21_strcat_test() {
    char string_one[40] = "Hello world! \0";
    char string_one_copy[17] = "My name is Vadim\0";
    s21_strcat(string_one, string_one_copy);
    if (s21_strlen(string_one) == 29) {
        printf(
            "\nHello world! |string_one| |%s| string_one_copy | %s | string_one "
            "SUCCES\n",
            string_one_copy, string_one);
    } else {
        printf(
            "Hello world! |string_one| |%s| string_one_copy | %s | string_one "
            "FAIL\n",
            string_one_copy, string_one);
    }
    char string_two[41] = "1                  \0";
    char string_two_copy[21] = "                  20\0";
    s21_strcat(string_two, string_two_copy);
    if (s21_strlen(string_two) == 20) {
        printf(
            "Hello world! |string_one| |%s| string_one_copy | %s | string_one "
            "SUCCES\n",
            string_two_copy, string_two);
    } else {
        printf(
            "Hello world! |string_one| |%s| string_one_copy | %s | string_one "
            "FAIL\n",
            string_two_copy, string_two);
    }
    char string_three[2] = "\0";
    char string_three_copy[2] = "\0";
    s21_strcat(string_three, string_three_copy);
    size_t len = 0;
    if (s21_strlen(string_three) == len) {
        printf(
            "Hello world! |string_one| |%s| string_one_copy | %s | string_one "
            "SUCCES\n",
            string_three_copy, string_three);
    } else {
        printf(
            "Hello world! |string_one| |%s| string_one_copy | %s | string_one "
            "FAIL\n",
            string_three_copy, string_three);
    }
}

void s21_strchr_test() {
    int string_one = 108;
    char string_one_copy[13] = "Hello world!\0";
    if (*(s21_strchr(string_one_copy, string_one)) == 108) {
        printf(
            "\nHello world! |string_one| l |%s| string_one_copy | %s | "
            "string_one SUCCES\n",
            string_one_copy, s21_strchr(string_one_copy, string_one));
    } else {
        printf(
            "\nHello world! |string_one| l |%s| string_one_copy | %s | "
            "string_one FAIL\n",
            string_one_copy, s21_strchr(string_one_copy, string_one));
    }
    int string_two = 32;
    char string_two_copy[12] = "12345 12345\0";
    if (*s21_strchr(string_two_copy, string_two) == string_two_copy[6]) {
        printf(
            "12345 12345 |string_one| |%s| string_one_copy | %s | string_one "
            "SUCCES\n",
            string_two_copy, s21_strchr(string_two_copy, string_two));
    } else {
        printf(
            "12345 12345 |string_one|  |%s| string_one_copy | %s | string_one "
            "SUCCES\n",
            string_two_copy, s21_strchr(string_two_copy, string_two));
    }
    int string_three = 47;
    char string_three_copy[13] = "12345 /12345\0";
    if (*s21_strchr(string_three_copy, string_three) == string_three_copy[7]) {
        printf(
            "12345 /12345 |string_one| / |%s| string_one_copy | %s | string_one "
            "SUCCES\n",
            string_three_copy, s21_strchr(string_three_copy, string_three));
    } else {
        printf(
            "12345 /12345 |string_one| / |%s| string_one_copy | %s | string_one "
            "SUCCES\n",
            string_three_copy, s21_strchr(string_three_copy, string_three));
    }
}

void s21_strstr_test() {
    char *string_one = "wor\0";
    char *string_one_copy = "Hello world!\0";
    char *string_check = "world!\0";
    if (*(s21_strstr(string_one_copy, string_one)) == *string_check) {
        printf(
            "\nHello world! |string_one| wor |%s| string_one_copy | %s | "
            "string_one SUCCES\n",
            string_one_copy, s21_strstr(string_one_copy, string_one));
    } else {
        printf(
            "\nHello world! |string_one| wor |%s| string_one_copy | %s | "
            "string_one FAIL\n",
            string_one_copy, s21_strstr(string_one_copy, string_one));
    }
    char *string_two = "/1\0";
    char *string_two_copy = "12345 /12345\0";
    char *string_check_two = "/12345\0";
    if (*(s21_strstr(string_two_copy, string_two)) == *string_check_two) {
        printf(
            "12345 /12345 |string_one| /1 |%s| string_one_copy | %s | "
            "string_one SUCCES\n",
            string_two_copy, s21_strstr(string_two_copy, string_two));
    } else {
        printf(
            "12345 /12345 |string_one| /1 |%s| string_one_copy | %s | "
            "string_one FAIL\n",
            string_two_copy, s21_strstr(string_two_copy, string_two));
    }
    char *string_three = "\0";
    char *string_three_copy = "\0";
    char *string_check_three = "\0";
    if (*(s21_strstr(string_three_copy, string_three)) == *string_check_three) {
        printf("|string_one| /1 |%s| string_one_copy | %s | string_one SUCCES\n", string_three_copy,
               s21_strstr(string_three_copy, string_three));
    } else {
        printf("|string_one||%s| string_one_copy | %s | string_one FAIL\n", string_three_copy,
               s21_strstr(string_three_copy, string_three));
    }
}

void s21_strtok_test() {
    char *string_one = "!";
    char string_one_copy[44] = "!!!!Hello world!!!! My name  is Vadim!";
    char *string_check_1 = "Hello world";
    char *new_string;
    new_string = s21_strtok(string_one_copy, string_one);
    if (*new_string == *string_check_1) {
        printf(
            "\n!!!!Hello world!!!! My name  is Vadim |string_one| name |%s| "
            "string_one_copy | %s | string_one SUCCES\n",
            string_one, new_string);
    } else {
        printf(
            "\nH!!!!Hello world!!!! My name  is Vadim |string_one| name |%s| "
            "string_one_copy | %s | string_one FAIL\n",
            string_one, new_string);
    }
    new_string = s21_strtok(string_one_copy, string_one);
    char *string_check_2 = " My name  is Vadim\0";
    if (*new_string == *string_check_2) {
        printf(
            "\n!!!!Hello world!!!! My name  is Vadim |string_one| name |%s| "
            "string_one_copy | %s | string_one SUCCES\n",
            string_one, new_string);
    } else {
        printf(
            "\n!!!!Hello world!!!! My name  is Vadim |string_one| name |%s| "
            "string_one_copy | %s | string_one FAIL\n",
            string_one, new_string);
    }
    new_string = s21_strtok(string_one_copy, string_one);
    if (new_string == NULL) {
        printf(
            "\n!!!!Hello world!!!! My name  is Vadim |string_one| name |%s| "
            "string_one_copy | NULL | string_one SUCCES\n",
            string_one);
    } else {
        printf(
            "\n!!!!Hello world!!!! My name  is Vadim |string_one| name |%s| "
            "string_one_copy | %s | string_one FAIL\n",
            string_one, new_string);
    }
}