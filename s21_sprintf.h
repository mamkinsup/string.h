#ifndef S21_SPRINTF_H_
#define S21_SPRINTF_H_

#include <stdint.h>

#include "s21_string.h"

#define BUFF_SIZE 1024

typedef struct {
  int minus;
  int plus;
  int space;
  int zero;
  int hash;
  int precision;
  int dot;
  int width;
  char length;
  char specifier;
} flag;

int s21_sprintf(char* str, const char* format, ...);
char* s21_strtol(int number);
const char* fill_flags(const char* format, flag* my_flag);
int s21_atoi(const char* str);
int check_number(char simbol);
const char* fill_width(const char* format, flag* my_flag,
                       va_list* pointer_value);
const char* fill_precision(const char* format, flag* my_flag,
                           va_list* pointer_value);
const char* fill_length(const char* format, flag* my_flag);
void conversion_int(char* my_buf, va_list* pointer_value, flag my_flag);
void format_precision(flag my_flag, char* my_buf);
void format_flags(flag my_flag, char* my_buf);
void adding_zeros(int position, char* buff, char* tmp, char signer_number);
void conversion_int_u(flag my_flag, char* my_buf, va_list* list_pointer);
void conversion_char(flag my_flag, char* my_buf, va_list* list_pointer);
void conversion_char_w(flag my_flag, char* my_buf, int wide_character);
void conversion_string(flag my_flag, char* my_buf, va_list* list_pointer);
void conversion_non_wide_string(flag my_flag, char* my_buf, char* my_string);
void double_converter(long double number, char* result, flag my_flag);
void conversion_double(flag my_flag, char* my_buf, va_list* list_pointer);
void translation_scientific_format(flag my_flag, char* buffer,
                                   va_list* list_pointer);
void conversion_flag_g(flag my_flag, char* my_buffer, va_list* list_pointer);
void convert_flag_g(flag my_flag, char* buffer, va_list* pointer_value);
char* s21_itoa(long int number, char* buffer);
void octal_number(flag my_flag, char* buffer, va_list* pointer_value);
void hexadecimal_number(flag my_flag, char* buffer, va_list* pointer_value);
void enter_pointer(char* buffer, va_list* pointer_value, flag my_flag);

#endif  // S21_SPRINTF_H_
