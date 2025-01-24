#ifndef S21_SSCANF_H
#define S21_SSCANF_H

#include "s21_string.h"

#define BUFF_SIZE 1024

typedef enum widthType { WIDTH_NONE, WIDTH_STAR, WIDTH_NUMBER } widthType;

typedef enum lengthType {
  LENGTH_NONE,
  LENGTH_SHORT,
  LENGTH_LONG_INT,
  LENGTH_LONG_DOUBLE
} lengthType;

typedef struct token {
  char* str;
  int is_space;
  unsigned int width;
  int type;
  int is_percent;
  s21_size_t str_len;
  widthType widthType;
  lengthType lengthType;
} token;

void create_tokens(char*, token*, int*);
void parse_token(char* fstr, int* i, token* tokens, int* j);
void parse_str(char* fstr, int* i, token* tokens, int j, int len);
void skip_space(char*, int*, token*, int);
char* skip_int(char*);
char* skip_float(char*, int);
char* skip_exp(char*, int*, int*);
char* skip_point(char*, int*, int*, int*);
char* skip_digits(char*, int*, int*);
char* skip_sign(char*, int*, int*);

int check_format(char*, int, int*);
int check_float_format(char*, int);
int check_octal_format(char*, int);
int check_hex_format(char*, int);
int check_decimal_format(char*, int);

void write_unsigned_from_string(token, char**, va_list, int);
void write_number_from_string(token, char**, va_list, int);
void write_tokens(token*, int, char*, int*, va_list);
void process_token(token*, int, char**, va_list, int*, int*, s21_size_t);

void skip_num(char** str, int base, int width);
void skip_decimal(char** str, int width);
void skip_hex(char** str, int width);
void skip_octal(char** str, int width);

void write_type_d(token*, int, char**, va_list, int*, int*);
void write_type_i(token*, int, char**, va_list, int*, int*);
void write_type_u(token*, int, char**, va_list, int*, int*);
void write_type_x(token*, int, char**, va_list, int*, int*);
void write_type_o(token*, int, char**, va_list, int*, int*);
void write_type_c(token*, int, char**, va_list, int*);
void write_type_s(token*, int, char**, va_list, int*, int*);
void write_type_p(token*, int, char**, va_list, int*, int*);
void write_type_float(token*, int, char**, va_list, int*, int*);
void write_type_n(char**, va_list, s21_size_t);
int check_space(char);
long s21_strtol(const char*, int);
long double s21_atold(char*);

#endif  // S21_SSCANF_H
