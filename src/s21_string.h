#include <stdio.h>

size_t s21_strlen(const char* string);
int s21_strcmp(const char* string1, const char* string2);
char* s21_strcpy(char* restrict string_one, const char* restrict string_two);
char* s21_strcat(char* restrict string_origin, const char* restrict string_dop);
char* s21_strchr(const char* string, int simbol);
char* s21_strstr(const char* origin_string, const char* search_string);
char* s21_strtok(char* string_origin, const char* string_token);