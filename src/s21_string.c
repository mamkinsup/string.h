#include "s21_string.h"
size_t s21_strlen(const char *string) {
    size_t len = 0;
    for (; *(string + len) != '\0'; len++) {
    }
    return len;
}
int s21_strcmp(const char *string1, const char *string2) {
    for (; *string1 != '\0'; string1++, string2++) {
        if (*string1 != *string2) {
            break;
        }
    }
    return *string1 - *string2;
}

char *s21_strcpy(char *restrict string_one, const char *restrict string_two) {
    if (*string_two == '\0') {
        *string_one = '\0';
    }
    for (; *string_two != '\0'; string_two++, string_one++) {
        *string_one = *string_two;
    }
    return string_one;
}

char *s21_strcat(char *restrict string_origin, const char *restrict string_dop) {
    for (; *string_dop != '\0'; string_origin++, string_dop++) {
        *(string_origin + s21_strlen(string_origin)) = *string_dop;
    }
    return string_origin;
}

char *s21_strchr(const char *string, int simbol) {
    for (; *string != '\0'; string++) {
        if (*string && ((int)(*string)) == simbol) {
            break;
        }
    }
    return (*string && ((int)(*string)) == simbol) ? (char *)string : NULL;
}

char *s21_strstr(const char *origin_string, const char *search_string) {
    for (; *origin_string != '\0'; origin_string++) {
        if (*origin_string == *search_string) {
            break;
        }
    }
    return (*origin_string == *search_string) ? (char *)origin_string : NULL;
}

char *s21_strtok(char *string_origin, const char *string_token) {
    static int count = 0;
    static char *new_point;
    static char *new_point_double;
    static char *end_point;
    if (count == 0) {
        new_point = string_origin;
        new_point_double = string_origin;
        end_point = string_origin + s21_strlen(string_origin);
    }
    if (new_point_double < end_point && new_point < end_point) {
        for (; *new_point != *string_token; new_point++, string_origin++) {
        }
        while (*new_point == *string_token && *new_point != '\0') {
            new_point++;
        }
        new_point_double = new_point;
        while (*new_point_double != *string_token) {
            if (new_point_double > end_point) {
                count = -2;
                break;
            }
            new_point_double++;
        }
        *new_point_double = '\0';
        count++;
    } else {
        count = -1;
    }
    return (count < 1) ? NULL : new_point;
}