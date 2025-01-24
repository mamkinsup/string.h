#include "s21_sscanf.h"

int s21_sscanf(const char* str, const char* format, ...) {
  int flag = 0;
  char* string = (char*)str;

  if (*str) {
    token tokens[BUFF_SIZE] = {{0}};
    int token_count = 0;

    char* format_str = (char*)format;
    create_tokens(format_str, tokens, &token_count);

    va_list ap;
    va_start(ap, format);

    write_tokens(tokens, token_count, string, &flag, ap);
    va_end(ap);
  } else {
    flag = -1;
  }
  return flag;
}

void write_tokens(token* tokens, int token_count, char* string, int* arg_count,
                  va_list ap) {
  int flag = 1;
  s21_size_t string_len = s21_strlen(string);

  for (int i = 0; i < token_count && flag; i++) {
    while (flag && tokens[i].str_len != 0 && *string != '\0') {
      if (!check_space(*string)) {
        if (*string == *(tokens[i].str)) {
          string++;
          tokens[i].str++;
          tokens[i].str_len--;
          while (check_space(*string)) {
            string++;
          }
        } else {
          flag = 0;
        }
      } else {
        string++;
      }
    }
    if (flag) {
      while ((check_space(*string) &&
              (tokens[i].is_space || !(tokens[i].type == 'c'))) ||
             (*string == '%' && tokens[i].is_percent)) {
        string++;
      }
      if (flag && tokens[i].type != 0) {
        process_token(tokens, i, &string, ap, arg_count, &flag, string_len);
      }
    }
  }
}

void process_token(token* tokens, int tokenIndex, char** string, va_list ap,
                   int* arg_count, int* flag, s21_size_t string_len) {
  switch (tokens[tokenIndex].type) {
    case 'd':
      write_type_d(tokens, tokenIndex, string, ap, arg_count, flag);
      break;
    case 'i':
      write_type_i(tokens, tokenIndex, string, ap, arg_count, flag);
      break;
    case 'u':
      write_type_u(tokens, tokenIndex, string, ap, arg_count, flag);
      break;
    case 'X':
    case 'x':
      write_type_x(tokens, tokenIndex, string, ap, arg_count, flag);
      break;
    case 'o':
      write_type_o(tokens, tokenIndex, string, ap, arg_count, flag);
      break;
    case 'c':
      write_type_c(tokens, tokenIndex, string, ap, arg_count);
      break;
    case 's':
      write_type_s(tokens, tokenIndex, string, ap, arg_count, flag);
      break;
    case 'p':
      write_type_p(tokens, tokenIndex, string, ap, arg_count, flag);
      break;
    case 'n':
      write_type_n(string, ap, string_len);
      break;
    case 'e':
    case 'E':
    case 'g':
    case 'G':
    case 'f':
      write_type_float(tokens, tokenIndex, string, ap, arg_count, flag);
      break;
    default:
      break;
  }
}

void write_type_c(token* tokens, int i, char** string, va_list ap,
                  int* arg_count) {
  if (tokens[i].widthType != WIDTH_STAR) (*arg_count)++;
  if (tokens[i].widthType == WIDTH_STAR) {
    (*string)++;
  } else {
    if (tokens[i].lengthType == LENGTH_NONE) {
      char* arg = va_arg(ap, char*);
      *arg = **string;
      (*string)++;
    }
  }
}

void write_type_s(token* tokens, int i, char** string, va_list ap,
                  int* arg_count, int* flag) {
  if (**string == '\0') *flag = 0;
  if (*flag) {
    if (tokens[i].widthType != WIDTH_STAR) (*arg_count)++;
    if (tokens[i].widthType == WIDTH_STAR) {
      while (**string != '\0' && !check_space(**string)) {
        (*string)++;
      }
    } else {
      if (tokens[i].lengthType == LENGTH_NONE) {
        char* arg = va_arg(ap, char*);
        unsigned int j = 0;
        while ((j < tokens[i].width || tokens[i].widthType == WIDTH_NONE) &&
               **string != '\0' && !check_space(**string)) {
          arg[j] = **string;
          j++;
          (*string)++;
        }
        arg[j] = '\0';
      }
    }
    while (**string == ' ') {
      (*string)++;
    }
  }
}

void write_type_p(token* tokens, int i, char** string, va_list ap,
                  int* arg_count, int* flag) {
  *flag = check_hex_format(*string, tokens[i].width);
  if (*flag) {
    (*arg_count)++;
    char* tmp = (char*)malloc((s21_strlen(*string) + 3) * sizeof(char));
    tmp[0] = '0';
    tmp[1] = 'x';
    tmp[2] = '\0';
    s21_strcpy(tmp + 2, *string);
    write_unsigned_from_string(tokens[i], string, ap, 16);
    free(tmp);
  }
}

void write_type_float(token* tokens, int i, char** string, va_list ap,
                      int* arg_count, int* flag) {
  *flag = check_float_format(*string, tokens[i].width);
  if (*flag) {
    if (tokens[i].widthType == WIDTH_STAR) {
      *string = skip_float(*string, tokens[i].width);
    } else {
      (*arg_count)++;
      void* arg = va_arg(ap, void*);
      char* tmp = S21_NULL;
      if (tokens[i].widthType == WIDTH_NUMBER) {
        tmp = (char*)malloc((tokens[i].width + 1) * sizeof(char));
        s21_strncpy(tmp, *string,
                    tokens[i].width > s21_strlen(*string) ? s21_strlen(*string)
                                                          : tokens[i].width);
        tmp[tokens[i].width > s21_strlen(*string) ? s21_strlen(*string)
                                                  : tokens[i].width] = '\0';
      } else {
        tmp = (char*)malloc((s21_strlen(*string) + 1) * sizeof(char));
        s21_strcpy(tmp, *string);
      }
      if (tokens[i].lengthType == LENGTH_LONG_DOUBLE)
        *((long double*)arg) = (long double)s21_atold(tmp);
      else if (tokens[i].lengthType == LENGTH_LONG_INT)
        *((double*)arg) = (double)s21_atold(tmp);
      else
        *((float*)arg) = (float)s21_atold(tmp);
      if (tmp) free(tmp);
      *string = skip_float(*string, tokens[i].width);
    }
  }
}

char* skip_float(char* str, int width) {
  int flag = 0, point_flag = 0;
  if (width == 0) width--;

  str = skip_sign(str, &width, &point_flag);
  str = skip_digits(str, &width, &flag);
  str = skip_point(str, &width, &flag, &point_flag);
  str = skip_digits(str, &width, &flag);
  str = skip_exp(str, &width, &flag);
  str = skip_digits(str, &width, &flag);

  return str;
}

void write_unsigned_from_string(token token, char** string, va_list ap,
                                int base) {
  if (token.widthType == WIDTH_STAR) {
    skip_num(string, base, BUFF_SIZE);
  } else {
    if (token.lengthType == LENGTH_NONE) {
      if (token.widthType == WIDTH_NUMBER) {
        unsigned int* arg = va_arg(ap, unsigned int*);
        char* tmp = (char*)malloc((token.width + 1) * sizeof(char));
        s21_strncpy(tmp, *string, token.width);
        tmp[token.width] = '\0';
        *arg = s21_strtol(tmp, base);
        free(tmp);
        skip_num(string, base, token.width);
      } else {
        unsigned int* arg = va_arg(ap, unsigned int*);
        *arg = s21_strtol(*string, base);
        skip_num(string, base, BUFF_SIZE);
      }
    } else if (token.lengthType == LENGTH_LONG_INT) {
      if (token.widthType == WIDTH_NUMBER) {
        unsigned long* arg = va_arg(ap, unsigned long*);
        char* tmp = (char*)malloc((token.width + 1) * sizeof(char));
        s21_strncpy(tmp, *string, token.width);
        tmp[token.width] = '\0';
        *arg = s21_strtol(tmp, base);
        free(tmp);
        skip_num(string, base, token.width);
      } else {
        unsigned long* arg = va_arg(ap, unsigned long*);
        *arg = s21_strtol(*string, base);
        skip_num(string, base, BUFF_SIZE);
      }
    } else if (token.lengthType == LENGTH_SHORT) {
      if (token.widthType == WIDTH_NUMBER) {
        unsigned short* arg = va_arg(ap, unsigned short*);
        char* tmp = (char*)malloc((token.width + 1) * sizeof(char));
        s21_strncpy(tmp, *string, token.width);
        tmp[token.width] = '\0';
        *arg = s21_strtol(tmp, base);
        free(tmp);
        skip_num(string, base, token.width);
      } else {
        unsigned short* arg = va_arg(ap, unsigned short*);
        *arg = s21_strtol(*string, base);
        skip_num(string, base, BUFF_SIZE);
      }
    }
  }
}

void write_number_from_string(token token, char** string, va_list ap,
                              int base) {
  if (token.widthType == WIDTH_STAR) {
    skip_num(string, base, BUFF_SIZE);
  } else {
    if (token.lengthType == LENGTH_NONE) {
      if (token.widthType == WIDTH_NUMBER) {
        int* arg = va_arg(ap, int*);
        char* tmp = (char*)malloc((token.width + 1) * sizeof(char));
        s21_strncpy(tmp, *string, token.width);
        tmp[token.width] = '\0';
        *arg = s21_strtol(tmp, base);
        free(tmp);
        skip_num(string, base, token.width);
      } else {
        int* arg = va_arg(ap, int*);
        *arg = s21_strtol(*string, base);
        skip_num(string, base, BUFF_SIZE);
      }
    } else if (token.lengthType == LENGTH_LONG_INT) {
      if (token.widthType == WIDTH_NUMBER) {
        long* arg = va_arg(ap, long*);
        char* tmp = (char*)malloc((token.width + 1) * sizeof(char));
        s21_strncpy(tmp, *string, token.width);
        tmp[token.width] = '\0';
        *arg = s21_strtol(tmp, base);
        free(tmp);
        skip_num(string, base, token.width);
      } else {
        long* arg = va_arg(ap, long*);
        *arg = s21_strtol(*string, base);
        skip_num(string, base, BUFF_SIZE);
      }
    } else if (token.lengthType == LENGTH_SHORT) {
      if (token.widthType == WIDTH_NUMBER) {
        short* arg = va_arg(ap, short*);
        char* tmp = (char*)malloc((token.width + 1) * sizeof(char));
        s21_strncpy(tmp, *string, token.width);
        tmp[token.width] = '\0';
        *arg = s21_strtol(tmp, base);
        free(tmp);
        skip_num(string, base, token.width);
      } else {
        short* arg = va_arg(ap, short*);
        *arg = s21_strtol(*string, base);
        skip_num(string, base, BUFF_SIZE);
      }
    }
  }
}

void skip_hex(char** string, int width) {
  int pre_flag = 0;

  if (width == 0) width--;

  if ((**string == '+' || **string == '-') && width != 1) {
    if ((*(*string + 1) >= '0' && *(*string + 1) <= '9') ||
        (*(*string + 1) >= 'a' && *(*string + 1) <= 'f') ||
        (*(*string + 1) >= 'A' && *(*string + 1) <= 'F')) {
      (*string)++;
      width--;
    }
  }

  if (**string == '0' && width != 0) {
    (*string)++;
    pre_flag = 1;
  }

  if ((**string == 'x' || **string == 'X') && width != 1 && pre_flag) {
    if ((*(*string + 1) >= '0' && *(*string + 1) <= '9') ||
        (*(*string + 1) >= 'a' && *(*string + 1) <= 'f') ||
        (*(*string + 1) >= 'A' && *(*string + 1) <= 'F')) {
      (*string) += 2;
      width -= 2;
    }
  }

  while (((**string >= '0' && **string <= '9') ||
          (**string >= 'a' && **string <= 'f') ||
          (**string >= 'A' && **string <= 'F')) &&
         width != 0) {
    (*string)++;
    width--;
  }
}

void create_tokens(char* format, token* tokens, int* tokens_count) {
  int i = 0, j = 0;
  int len = s21_strlen(format);

  while (i < len) {
    if (format[i] == '%') {
      i++;
      parse_token(format, &i, tokens, &j);
    } else {
      parse_str(format, &i, tokens, j, len);
    }
  }

  *tokens_count = j;
}

void parse_token(char* format, int* i, token* tokens, int* j) {
  if (format[*i] == '*') {
    tokens[*j].widthType = WIDTH_STAR;
    tokens[*j].width = 0;
    (*i)++;
  } else if (format[*i] >= '0' && format[*i] <= '9') {
    tokens[*j].widthType = WIDTH_NUMBER;
    tokens[*j].width = 0;
    while (format[*i] >= '0' && format[*i] <= '9') {
      tokens[*j].width = tokens[*j].width * 10 + (format[*i] - '0');
      (*i)++;
    }
  } else {
    tokens[*j].widthType = WIDTH_NONE;
    tokens[*j].width = 0;
  }

  if (format[*i] == 'h') {
    tokens[*j].lengthType = LENGTH_SHORT;
    (*i)++;
  } else if (format[*i] == 'l') {
    tokens[*j].lengthType = LENGTH_LONG_INT;
    (*i)++;
  } else if (format[*i] == 'L') {
    tokens[*j].lengthType = LENGTH_LONG_DOUBLE;
    (*i)++;
  } else {
    tokens[*j].lengthType = LENGTH_NONE;
  }

  if (format[*i] == '%') {
    tokens[*j].is_percent = 1;
    (*i)++;
  } else {
    tokens[*j].type = format[*i];
    (*i)++;
    (*j)++;
  }
}

void parse_str(char* format, int* i, token* tokens, int j, int len) {
  skip_space(format, i, tokens, j);

  if (tokens[j].type == 0 && format[*i] != '%') {
    tokens[j].str = &format[*i];
    tokens[j].is_space = 0;

    while (format[*i] != '%' && *i < len) {
      if (check_space(format[*i])) {
        skip_space(format, i, tokens, j);
        break;
      }
      tokens[j].str_len++;
      (*i)++;
    }
  }
}

int check_float_format(char* str, int width) {
  int flag = 1;

  if (width == 0) {
    width--;
  }

  if (width == 1 && (*str == '-' || *str == '+')) {
    flag = 0;
  } else if ((*str == '-' || *str == '+') && (width > 1 || width < 0) &&
             (!(*(str + 1) >= '0' && *(str + 1) <= '9') &&
              !(*(str + 1) == '.'))) {
    flag = 0;
  } else if ((*str == '-' || *str == '+') && (*(str + 1) == '.')) {
    if (width == 2) {
      flag = 0;
    }
    if (*(str + 2)) {
      if (!(*(str + 2) >= '0' && *(str + 2) <= '9')) {
        flag = 0;
      }
    } else {
      flag = 0;
    }
  } else if (*str == '.' && width == 1) {
    flag = 0;
  } else if (*str == '.' && (width > 1 || width < 0) &&
             !(*(str + 1) >= '0' && *(str + 1) <= '9')) {
    flag = 0;
  } else if (!(*str >= '0' && *str <= '9') && !(*str == '.') &&
             !(*str == '-') && !(*str == '+')) {
    flag = 0;
  } else if ((*str == '+' && *(str + 1) == '-') ||
             (*str == '-' && *(str + 1) == '+')) {
    flag = 0;
  }

  return flag;
}

long s21_strtol(const char* str, int radix) {
  int sign = 1, i = 0;
  long int res = 0;

  if (str[i] == '-') {
    sign = -1;
    i++;
  } else if (str[i] == '+') {
    i++;
  }
  if (str[i] == '0' && radix == 16) {
    if (str[i + 1]) {
      if (str[i + 1] == 'x' || str[i + 1] == 'X') i += 2;
    }
  }

  while (str[i]) {
    if ((!(str[i] >= '0' && str[i] <= radix + 47 && radix <= 10)) &&
        (!(((str[i] >= '0' && str[i] < '9') ||
            (str[i] >= 'A' && str[i] <= radix + 54) ||
            (str[i] >= 'a' && str[i] <= radix + 86)) &&
           radix > 10 && radix < 37))) {
      break;
    }

    else {
      if (str[i] >= 'A' && str[i] <= 'Z')
        res = res * radix + (str[i] - 'A' + 10);
      else if (str[i] >= 'a' && str[i] <= 'z')
        res = res * radix + (str[i] - 'a' + 10);
      else
        res = res * radix + str[i] - '0';
      i++;
    }
  }
  return res * sign;
}

long double s21_atold(char* str) {
  int sign = 1, i = 0;
  long double inc = 0.1L;

  long double res = 0.L;
  if (str[i] == '-') {
    sign = -1;
    i++;
  } else if (str[i] == '+') {
    i++;
  }
  while (str[i] != '.' && str[i] >= '0' && str[i] <= '9') {
    res = res * 10.L + str[i] - '0';
    i++;
  }
  if (str[i] == '.') i++;
  while (str[i] && str[i] >= '0' && str[i] <= '9') {
    res = res + (str[i] - '0') * inc;
    inc /= 10.L;
    i++;
  }
  inc = 10.L;
  if (str[i] == 'e' || str[i] == 'E') {
    i++;
    if (str[i] == '-') {
      inc = 0.1L;
      i++;
    } else if (str[i] == '+') {
      inc = 10.L;
      i++;
    }
    int count = 0;
    while (str[i] >= '0' && str[i] <= '9') {
      count = count * 10.L + str[i] - '0';
      i++;
    }

    while (count) {
      res *= inc;
      count--;
    }
  }
  return res * sign;
}

void write_type_n(char** string, va_list ap, s21_size_t string_len) {
  int* arg = va_arg(ap, int*);
  s21_size_t cur_len = s21_strlen(*string);
  *arg = string_len - cur_len;
}

char* skip_sign(char* str, int* width, int* point_flag) {
  if ((*str == '+' || *str == '-') && *width != 1) {
    if (*(str + 1) >= '0' && *(str + 1) <= '9') {
      *width -= 2;
      str += 2;
    } else if (*(str + 1) == '.' && *width != 2) {
      if (*(str + 2)) {
        if (*(str + 2) >= '0' && *(str + 2) <= '9') {
          *width -= 2;
          str += 2;
          *point_flag = 1;
        }
      }
    }
  }
  return str;
}

char* skip_digits(char* str, int* width, int* flag) {
  while (*str >= '0' && *str <= '9' && *width != 0) {
    str++;
    *flag = 1;
    (*width)--;
  }
  return str;
}

char* skip_point(char* str, int* width, int* flag, int* point_flag) {
  if (*str == '.' && !(*point_flag) && *width != 0) {
    if (*flag == 1)
      str++;
    else if (*(str + 1) >= '0' && *(str + 1) <= '9' && *width != 1)
      str++;
    (*width)--;
  }
  return str;
}

char* skip_exp(char* str, int* width, int* flag) {
  if ((*str == 'e' || *str == 'E') && *flag && *width != 1 && *width != 0) {
    if (*(str + 1) >= '0' && *(str + 1) <= '9') {
      str += 2;
      *width -= 2;
    } else if ((*(str + 1) == '-' || *(str + 1) == '+') && *width != 2) {
      if (*(str + 2)) {
        if (*(str + 2) >= '0' && *(str + 2) <= '9') {
          str += 3;
          *width -= 3;
        }
      }
    }
  }
  return str;
}

void skip_octal(char** string, int width) {
  if ((**string == '+' || **string == '-') && width != 1) {
    if (*(*string + 1) >= '0' && *(*string + 1) <= '7') {
      (*string) += 2;
      width -= 2;
    }
  }
  while ((**string >= '0' && **string <= '7') && width != 0) {
    (*string)++;
    width--;
  }
}

void skip_decimal(char** string, int width) {
  if ((**string == '+' || **string == '-') && width != 1) {
    if (*(*string + 1) >= '0' && *(*string + 1) <= '9') {
      (*string) += 2;
      width -= 2;
    }
  }
  while ((**string >= '0' && **string <= '9') && width != 0) {
    (*string)++;
    width--;
  }
}

int check_format(char* str, int width, int* base) {
  int flag = 1;
  if (*str == '0') {
    if (*(str + 1) == 'x' || *(str + 1) == 'X') {
      flag = check_hex_format(str, width);
      *base = 16;
    } else {
      flag = check_octal_format(str, width);
      *base = 8;
    }
  } else {
    flag = check_decimal_format(str, width);
    *base = 10;
  }
  return flag;
}

int check_decimal_format(char* str, int width) {
  int flag = 1;
  if (width == 0) width--;
  if (*str == '+' || *str == '-') {
    if (width == 1)
      flag = 0;
    else if (!(*(str + 1) >= '0' && *(str + 1) <= '9'))
      flag = 0;
  } else if (!(*(str) >= '0' && *(str) <= '9'))
    flag = 0;
  return flag;
}

void skip_num(char** string, int base, int width) {
  if (base == 10) {
    skip_decimal(string, width);
  } else if (base == 16) {
    skip_hex(string, width);
  } else if (base == 8) {
    skip_octal(string, width);
  }
}

int check_hex_format(char* str, int width) {
  int flag = 1;
  if (width == 0) width--;
  if (*str == '-' || *str == '+') {
    if (width == 1)
      flag = 0;
    else if (!((*(str + 1) >= '0' && *(str + 1) <= '9') ||
               (*(str + 1) >= 'A' && *(str + 1) <= 'F') ||
               (*(str + 1) >= 'a' && *(str + 1) <= 'f')))
      flag = 0;
  } else if (!((*str >= '0' && *str <= '9') || (*str >= 'A' && *str <= 'F') ||
               (*str >= 'a' && *str <= 'f')))
    flag = 0;
  return flag;
}

int check_octal_format(char* str, int width) {
  int flag = 1;
  if (width == 0) width--;
  if (*str == '-' || *str == '+') {
    if (width == 1)
      flag = 0;
    else if (!(*(str + 1) >= '0' && *(str + 1) <= '7'))
      flag = 0;
  } else if (!(*str >= '0' && *str <= '7'))
    flag = 0;
  return flag;
}

void write_type_d(token* tokens, int i, char** string, va_list ap,
                  int* arg_count, int* flag) {
  *flag = check_decimal_format(*string, tokens[i].width);
  if (*flag) {
    if (tokens[i].widthType != WIDTH_STAR) (*arg_count)++;
    write_number_from_string(tokens[i], string, ap, 10);
  }
}

void write_type_i(token* tokens, int i, char** string, va_list ap,
                  int* arg_count, int* flag) {
  int base = 0;
  *flag = check_format(*string, tokens[i].width, &base);
  if (*flag) {
    if (tokens[i].widthType != WIDTH_STAR) (*arg_count)++;
    write_number_from_string(tokens[i], string, ap, base);
  }
}

void write_type_u(token* tokens, int i, char** string, va_list ap,
                  int* arg_count, int* flag) {
  *flag = check_decimal_format(*string, tokens[i].width);
  if (*flag) {
    if (tokens[i].widthType != WIDTH_STAR) (*arg_count)++;
    write_unsigned_from_string(tokens[i], string, ap, 10);
  }
}

void write_type_x(token* tokens, int i, char** string, va_list ap,
                  int* arg_count, int* flag) {
  *flag = check_hex_format(*string, tokens[i].width);
  if (*flag) {
    if (tokens[i].widthType != WIDTH_STAR) (*arg_count)++;
    write_unsigned_from_string(tokens[i], string, ap, 16);
  }
}

void write_type_o(token* const tokens, int i, char** string, va_list ap,
                  int* arg_count, int* flag) {
  *flag = check_octal_format(*string, tokens[i].width);
  if (*flag) {
    if (tokens[i].widthType != WIDTH_STAR) (*arg_count)++;
    write_unsigned_from_string(tokens[i], string, ap, 8);
  }
}

void skip_space(char* format, int* i, token* tokens, int j) {
  while (check_space(format[*i])) {
    (*i)++;
    tokens[j].is_space = 1;
  }
}

int check_space(char c) {
  return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' ||
          c == '\r');
}
