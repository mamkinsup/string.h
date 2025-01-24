#include "s21_sprintf.h"

int s21_sprintf(char *str, const char *format, ...) {
  flag my_flag = {0};
  va_list poiter_value;
  va_start(poiter_value, format);
  char *original_string = str;
  while (*format) {
    if (*format != '%') {
      *str++ = *format++;
      continue;
    } else {
      format++;
      s21_memset(&my_flag, 0, sizeof(flag));
    }
    format = fill_flags(format, &my_flag);

    format = fill_width(format, &my_flag, &poiter_value);
    format = fill_precision(format, &my_flag, &poiter_value);
    format = fill_length(format, &my_flag);
    my_flag.specifier = (*format);
    format++;

    char my_buffer[BUFF_SIZE] = {'\0'};

    if (my_flag.specifier == 'd' || my_flag.specifier == 'i') {
      conversion_int(my_buffer, &poiter_value, my_flag);
    } else if (my_flag.specifier == 'u') {
      conversion_int_u(my_flag, my_buffer, &poiter_value);
    } else if (my_flag.specifier == '%') {
      my_buffer[0] = '%';
    } else if (my_flag.specifier == 'c') {
      conversion_char(my_flag, my_buffer, &poiter_value);
    } else if (my_flag.specifier == 's') {
      conversion_string(my_flag, my_buffer, &poiter_value);
    } else if (my_flag.specifier == 'f') {
      conversion_double(my_flag, my_buffer, &poiter_value);
    } else if (my_flag.specifier == 'g' || my_flag.specifier == 'G') {
      conversion_flag_g(my_flag, my_buffer, &poiter_value);
    } else if (my_flag.specifier == 'e' || my_flag.specifier == 'E') {
      translation_scientific_format(my_flag, my_buffer, &poiter_value);
    } else if (my_flag.specifier == 'o') {
      octal_number(my_flag, my_buffer, &poiter_value);
    } else if (my_flag.specifier == 'x' || my_flag.specifier == 'X') {
      hexadecimal_number(my_flag, my_buffer, &poiter_value);
    } else if (my_flag.specifier == 'p') {
      enter_pointer(my_buffer, &poiter_value, my_flag);
    }

    for (int i = 0; my_buffer[i]; i++, str++) {
      *str = my_buffer[i];
    }
  }
  *str = '\0';
  va_end(poiter_value);
  return str - original_string;
}

const char *fill_flags(const char *format, flag *my_flag) {
  while (*format == '-' || *format == '+' || *format == ' ' || *format == '0' ||
         *format == '#') {
    switch (*format) {
      case '0':
        my_flag->zero = 1;
        break;
      case '-':
        my_flag->minus = 1;
        break;
      case '+':
        my_flag->plus = 1;
        break;
      case ' ':
        my_flag->space = 1;
        break;
      case '#':
        my_flag->hash = 1;
        break;
    }
    format++;
  }
  return format;
}
const char *fill_width(const char *format, flag *my_flag,
                       va_list *pointer_value) {
  if (*format == '*') {
    format++;
    my_flag->width = va_arg(*pointer_value, int);
  } else if (*format) {
    char tmp[BUFF_SIZE] = {'\0'};
    if (*format) {
      for (int i = 0; check_number(*format); i++, format++) {
        tmp[i] = *format;
      }
    }
    my_flag->width = s21_atoi(tmp);
  }
  return format;
}

int s21_atoi(const char *str) {
  int number = 0;
  int number_size = (int)pow(10, ((double)s21_strlen(str)) - 1);
  for (; *str; str++) {
    number += number_size * (((int)(*str)) - 48);
    number_size /= 10;
  }
  return number;
}

int check_number(char simbol) {
  int result = 0;
  if ((int)simbol >= 48 && (int)simbol <= 57) {
    result = 1;
  }
  return result;
}

const char *fill_precision(const char *format, flag *my_flag,
                           va_list *pointer_value) {
  if ((*format) == '.') {
    my_flag->dot = 1;
    format++;
    if ((*format) == '*') {
      my_flag->precision = va_arg(*pointer_value, int);
      format++;
    } else {
      char tmp[BUFF_SIZE] = {'\0'};
      for (int i = 0; check_number(*format); i++, format++) {
        tmp[i] = (*format);
      }
      my_flag->precision = s21_atoi(tmp);
    }
  }
  return format;
}

const char *fill_length(const char *format, flag *my_flag) {
  int count = 1;
  if (((*format) == 'h') && count) {
    my_flag->length = 'h';
    format++;
    count--;
  }
  if (((*format) == 'l') && count) {
    my_flag->length = 'l';
    format++;
    count--;
  }
  if (((*format) == 'L') && count) {
    my_flag->length = 'L';
    format++;
    count--;
  }
  return format;
}

void conversion_int(char *my_buf, va_list *pointer_value, flag my_flag) {
  int64_t number = va_arg(*pointer_value, int64_t);

  if (my_flag.length == 0) {
    number = (int32_t)number;

  } else if (my_flag.length == 'h') {
    number = (int16_t)number;
  }
  char tmp[BUFF_SIZE] = {'\0'};
  int start_of_number = BUFF_SIZE - 2;
  int negative = 0;
  if (number < 0) {
    number = -number;
    negative = 1;
  }
  if (number == 0) {
    tmp[start_of_number] = '0';
  }
  while (number > 0) {
    start_of_number--;
    tmp[start_of_number] = "0123456789"[number % 10];
    number /= 10;
  }
  for (int i = 0; tmp[start_of_number]; i++, start_of_number++) {
    if (negative && i == 0) {
      my_buf[i] = '-';
      i++;
    }
    my_buf[i] = tmp[start_of_number];
  }
  format_precision(my_flag, my_buf);
  format_flags(my_flag, my_buf);
}

void format_precision(flag my_flag, char *my_buf) {
  int signer_number = 0;
  char tmp[BUFF_SIZE] = {'\0'};
  int length = s21_strlen(my_buf);
  if (my_buf[0] == '-') {
    signer_number = 1;
    tmp[0] = '-';
    length--;
  }
  if (my_flag.precision > length) {
    int pointer_number = signer_number;
    for (int i = signer_number;
         i < (my_flag.precision) - length + signer_number; i++) {
      tmp[i] = '0';
      pointer_number++;
    }
    for (int i = signer_number; my_buf[i]; i++, pointer_number++) {
      tmp[pointer_number] = my_buf[i];
    }
    s21_strcpy(my_buf, tmp);
  }
  int int_specifier_check = 0;
  if (my_flag.specifier == 'd' || my_flag.specifier == 'i' ||
      my_flag.specifier == 'u') {
    int_specifier_check = 1;
  }
  if (my_flag.dot && my_flag.precision == 0 && int_specifier_check &&
      my_buf[0] == '0') {
    my_buf[0] = '\0';
  }
}

void format_flags(flag my_flag, char *my_buf) {
  int negative = (my_buf[0] == '-');
  char tmp[BUFF_SIZE + 1] = {'\0'};
  s21_size_t length = s21_strlen(my_buf);
  if (my_flag.width > (int)length && my_flag.zero &&
      (negative || my_flag.plus || my_flag.space)) {
    int number_position = my_flag.width - length;
    if (negative) {
      char sign = '-';
      my_buf[0] = '0';
      my_flag.plus = 0;
      my_flag.space = 0;
      adding_zeros(number_position, my_buf, tmp, sign);
    } else if (my_flag.plus) {
      char sign = '+';
      my_flag.plus = 0;
      my_flag.space = 0;
      adding_zeros(number_position, my_buf, tmp, sign);
    } else if (my_flag.space) {
      char sign = ' ';
      adding_zeros(number_position, my_buf, tmp, sign);
      my_flag.space = 0;
    }
    s21_strcpy(my_buf, tmp);
  }
  if (my_flag.plus && my_flag.specifier != 'u') {
    if (my_buf[0] == '-') {
      tmp[0] = '-';
    } else {
      tmp[0] = '+';
    }
    if (my_buf[0] == '-') {
      s21_strcpy(tmp, my_buf);
      s21_strcpy(my_buf, tmp);
    } else {
      s21_strcpy(tmp + 1, my_buf);
      s21_strcpy(my_buf, tmp);
    }
  } else if (my_flag.space && my_buf[0] != '-' && my_flag.specifier != 'u') {
    tmp[0] = ' ';
    s21_strcpy(tmp + 1, my_buf);
    s21_strcpy(my_buf, tmp);
  }
  if (my_flag.width > (int)length) {
    int new_number_position = my_flag.width - s21_strlen(my_buf);
    if (!my_flag.minus) {
      if (my_flag.zero) {
        s21_memset(tmp, '0', new_number_position);

        s21_strcpy(tmp + new_number_position, my_buf);
      } else {
        s21_memset(tmp, ' ', new_number_position);
        s21_strcpy(tmp + new_number_position, my_buf);
      }

    } else {
      s21_strcpy(tmp, my_buf);
      s21_memset(tmp + s21_strlen(tmp), ' ', new_number_position);
    }
    s21_strcpy(my_buf, tmp);
  }
}

void adding_zeros(int position, char *buff, char *tmp, char signer_number) {
  s21_memset(tmp, '0', position);
  s21_strncat(tmp, buff, sizeof(buff));
  tmp[0] = signer_number;
}

void conversion_int_u(flag my_flag, char *my_buf, va_list *list_pointer) {
  uint64_t number = va_arg(*list_pointer, uint64_t);
  if (my_flag.length == 'h') {
    number = (uint64_t)number;
  } else if (my_flag.length == 'l') {
    number = (uint64_t)number;
  } else if (my_flag.length == 0) {
    number = (uint32_t)number;
  }
  char tmp[BUFF_SIZE + 1] = {'\0'};
  int number_position = BUFF_SIZE - 1;
  if (number == 0) {
    tmp[number_position] = '0';
    number_position--;
  }
  for (; number_position && number; --number_position, number /= 10) {
    tmp[number_position] = "0123456789"[number % 10];
  }
  for (int j = 0; tmp[number_position + 1]; number_position++, j++) {
    my_buf[j] = tmp[number_position + 1];
  }
  format_precision(my_flag, my_buf);
  format_flags(my_flag, my_buf);
}

void conversion_char(flag my_flag, char *my_buf, va_list *list_pointer) {
  char simbol = va_arg(*list_pointer, int);
  conversion_char_w(my_flag, my_buf, simbol);
}

void conversion_char_w(flag my_flag, char *my_buf, int wide_character) {
  if (!my_flag.minus && my_flag.width) {
    for (int i = 0; i < my_flag.width; i++) {
      my_buf[i] = ' ';
      if (i == my_flag.width - 1) my_buf[i] = wide_character;
    }
  } else if (my_flag.width) {
    my_buf[0] = wide_character;
    for (int i = s21_strlen(my_buf); i < my_flag.width; i++) my_buf[i] = ' ';
  } else {
    my_buf[0] = wide_character;
  }
}

void conversion_string(flag my_flag, char *my_buf, va_list *list_pointer) {
  char *str = va_arg(*list_pointer, char *);
  conversion_non_wide_string(my_flag, my_buf, str);
}

void conversion_non_wide_string(flag my_flag, char *my_buf, char *my_string) {
  char tmp[BUFF_SIZE] = {'\0'};
  s21_strcpy(tmp, my_string);
  if (my_flag.dot) tmp[my_flag.precision] = '\0';

  int shift = my_flag.width - s21_strlen(tmp);
  int len = s21_strlen(tmp);

  if (my_flag.minus && shift > 0) {
    s21_strcpy(my_buf, tmp);
    s21_memset(my_buf + len, ' ', shift);
  } else if (shift > 0) {
    s21_memset(my_buf, ' ', shift);
    s21_strcpy(my_buf + shift, tmp);
  } else {
    s21_strcpy(my_buf, tmp);
  }
}

void conversion_double(flag my_flag, char *my_buf, va_list *list_pointer) {
  long double number = 0;
  if (my_flag.length == 'L') {
    number = va_arg(*list_pointer, long double);
  } else {
    number = va_arg(*list_pointer, double);
  }
  if (!my_flag.dot) {
    my_flag.precision = 6;
  }
  double_converter(number, my_buf, my_flag);
  format_flags(my_flag, my_buf);
}

void double_converter(long double number, char *result, flag my_flag) {
  char my_buf[BUFF_SIZE] = {'\0'};
  int number_position = BUFF_SIZE - 2;
  int negative = 0;
  if (number < 0) {
    number = -number;
    negative = 1;
  }
  long double altogether = 0;
  long double fraction = modfl(number, &altogether);
  if (my_flag.precision == 0) {
    altogether = roundl(number);
    fraction = 0;
  }
  char remainder[BUFF_SIZE] = {'\0'};
  for (int i = 0; i < my_flag.precision; i++) {
    fraction *= 10;
    remainder[i] = (int)fraction + 48;
  }
  long long long_fraction = roundl(fraction);
  long long long_altogether = altogether;
  if (!long_fraction) {
    for (int i = 0; i < my_flag.precision; number_position--, i++)
      my_buf[number_position] = '0';
  } else {
    for (int i = s21_strlen(remainder); long_fraction || i > 0;
         long_fraction /= 10, number_position--, i--)
      my_buf[number_position] = (int)(long_fraction % 10 + 0.05) + 48;
  }
  if ((my_flag.dot && my_flag.precision != 0) || (int)fraction ||
      (!my_flag.dot && number == 0) || s21_strlen(remainder) || my_flag.hash) {
    my_buf[number_position--] = '.';
  }
  if (!long_altogether) {
    my_buf[number_position] = '0';
    number_position--;
  } else {
    for (; long_altogether; long_altogether /= 10, number_position--) {
      my_buf[number_position] = (int)(long_altogether % 10) + '0';
    }
  }
  for (int i = 0; my_buf[number_position + 1]; number_position++, i++) {
    if (negative && i == 0) {
      result[i] = '-';
      i++;
    }
    result[i] = my_buf[number_position + 1];
  }
}
void translation_scientific_format(flag my_flag, char *buffer,
                                   va_list *pointer_value) {
  long double number = 0;
  if (my_flag.length == 'L') {
    number = va_arg(*pointer_value, long double);
  } else {
    number = va_arg(*pointer_value, double);
  }

  char my_buf[BUFF_SIZE] = {'\0'};
  int number_position = BUFF_SIZE - 2;
  int degree = 0;
  int negative = 0;
  if (number < 0) {
    number = -number;
    negative = 1;
  }
  if (number >= 10.0) {
    while (number >= 10.0) {
      number /= 10.0;
      degree++;
    }
  } else if (number > 0.0 && number < 1.0) {
    while (number < 1.0) {
      number *= 10.0;
      degree--;
    }
  }
  char tmp_number[BUFF_SIZE] = {'\0'};
  s21_itoa(degree, tmp_number);
  if (s21_strlen(tmp_number) > 1) {
    for (s21_size_t i = s21_strlen(tmp_number) - 1; i < s21_strlen(tmp_number);
         i--) {
      my_buf[number_position] = tmp_number[i];
      number_position--;
    }
  } else {
    for (s21_size_t i = s21_strlen(tmp_number) - 1; i < s21_strlen(tmp_number);
         i--) {
      my_buf[number_position] = tmp_number[i];
      number_position--;
      my_buf[number_position] = '0';
      number_position--;
    }
  }
  if (degree < 0) {
    my_buf[number_position] = '-';
    number_position--;
  } else {
    my_buf[number_position] = '+';
    number_position--;
  }
  if (my_flag.specifier == 'e' || my_flag.specifier == 'g') {
    my_buf[number_position] = 'e';
    number_position--;
  } else {
    my_buf[number_position] = 'E';
    number_position--;
  }

  long double altogether = 0;
  long double fraction = modfl(number, &altogether);
  int precision = 6;
  if (my_flag.dot) {
    precision = my_flag.precision;
  }
  if (precision < 2) {
    altogether = lround(number);
    fraction = 0;
  }
  long double count_fraction = fraction;
  char remainder[BUFF_SIZE] = {'\0'};
  for (int i = 0; i < precision; i++) {
    fraction *= 10;
    count_fraction *= 10;
    remainder[i] = (int)count_fraction + 48;
    count_fraction -= (int)count_fraction;
  }
  long long long_fraction;
  if (precision < 17) {
    long_fraction = roundl(fraction);
  } else {
    long_fraction = fraction;
  }
  int count_zero = 0;
  if (!long_fraction) {
    if (my_flag.specifier != 'g' && my_flag.specifier != 'G') {
      for (int i = 0; i < precision; number_position--, i++) {
        my_buf[number_position] = '0';
      }
    }
    if (my_flag.specifier == 'g' || my_flag.specifier == 'G') {
      count_zero = 1;
    }
  } else {
    if (precision < 17) {
      for (int i = s21_strlen(remainder); long_fraction || i > 0;
           long_fraction /= 10, number_position--, i--) {
        my_buf[number_position] = (int)(long_fraction % 10 + 0.05) + 48;
      }
    } else {
      for (int i = s21_strlen(remainder); long_fraction || i > 0;
           long_fraction /= 10, number_position--, i--) {
        my_buf[number_position] = (int)(long_fraction % 10 + 0.05) + 48;
      }
    }
  }
  if (precision != 0 && !count_zero) {
    my_buf[number_position] = '.';
    number_position--;
  }
  my_buf[number_position] = altogether + 48;
  number_position--;
  for (int i = 0; my_buf[number_position + 1]; number_position++, i++) {
    if (negative && i == 0) {
      buffer[i] = '-';
      i++;
    }
    buffer[i] = my_buf[number_position + 1];
  }

  format_flags(my_flag, buffer);
}

char *s21_itoa(long int number, char *str) {
  int position = 0;
  if (number < 0) {
    number = -number;
  }
  for (int i = BUFF_SIZE - 2; i >= 0; i--) {
    str[i] = 48 + number % 10;
    number /= 10;
    position = i;
    if ((int)number == 0) {
      break;
    }
  }
  for (int i = 0, j = position; i <= BUFF_SIZE - 2 - position; i++, j++) {
    str[i] = str[j];
  }
  return str;
}

void conversion_flag_g(flag my_flag, char *buffer, va_list *pointer_value) {
  int my_precision = 6;
  if (my_flag.dot) {
    my_precision = my_flag.precision;
  }
  long double number = 0;
  va_list my_list;
  va_copy(my_list, *pointer_value);
  if (my_flag.length == 'L') {
    number = va_arg(my_list, long double);
  } else {
    number = va_arg(my_list, double);
  }
  long int left = (int)number;
  int left_count = 0;
  if (left == 0) {
    left_count = 1;
  }
  for (; left; left /= 10) {
    left_count++;
  }
  long double right = number - (int)number;
  int right_count = 0;

  for (int i = 0; i < 4; i++) {
    right *= 10;
    if ((int)right != 0) {
      right_count = 1;
    }
  }
  if (number == 0) {
    buffer[0] = '0';
  } else {
    if (((left_count > my_precision) ||
         ((!right_count) && ((int)number) == 0)) &&
        my_precision != 0) {
      translation_scientific_format(my_flag, buffer, pointer_value);

    } else {
      convert_flag_g(my_flag, buffer, pointer_value);
    }
  }
  va_end(my_list);
}

void convert_flag_g(flag my_flag, char *buffer, va_list *pointer_value) {
  long double number = 0;
  if (my_flag.length == 'L') {
    number = va_arg(*pointer_value, long double);
  } else {
    number = va_arg(*pointer_value, double);
  }
  int number_position = 0;
  if (number < 0) {
    number = -number;
    buffer[0] = '-';
    number_position++;
  }
  int precision = 6;
  int current_precision = 0;
  if (my_flag.dot) {
    precision = my_flag.precision;
  }

  if (my_flag.precision == 0) {
    precision = 6;
  }
  char my_buffer[BUFF_SIZE] = {'\0'};
  int integer_part = (int)number;

  char my_number[BUFF_SIZE] = {'\0'};
  s21_itoa(integer_part, my_number);
  if ((int)s21_strlen(my_number) >= precision) {
    for (s21_size_t i = 0; (int)i < precision - 1; i++) {
      my_buffer[i] = my_number[i];
      current_precision++;
    }
    if (current_precision + 1 == precision) {
      my_buffer[current_precision] = lround(number) % 10 + 48;
      current_precision++;
    }
  } else {
    for (s21_size_t i = 0; i < s21_strlen(my_number); i++) {
      my_buffer[i] = my_number[i];
      current_precision++;
    }
  }

  if (current_precision < precision || my_flag.hash) {
    my_buffer[current_precision] = '.';
    current_precision++;
  }
  long double fraction = number - integer_part;

  int count_current_precision = current_precision;
  if ((current_precision - 1) < precision) {
    for (int i = 0; i < precision - (current_precision - 1); i++) {
      fraction *= 10.0;
    }
    int end_fraction = lround(fraction) % 10;
    count_current_precision = current_precision;
    for (int i = 0; i < precision - (current_precision - 1); i++) {
      fraction /= 10.0;
      count_current_precision++;
    }
    count_current_precision = current_precision;
    int osn_count_zero = 1;
    for (int i = 0; i < precision - (count_current_precision - 1); i++) {
      fraction *= 10;
      if (i == (precision - (count_current_precision - 1) - 1)) {
        fraction = lround(fraction);
      }
      if ((int)fraction % 10 == 0) {
        int count_zero = 1;
        long double copy_fraction = fraction;
        for (int j = i + 1; j <= precision - (count_current_precision - 1);
             j++) {
          copy_fraction *= 10;
          if ((int)copy_fraction % 10 != 0) {
            count_zero = 0;
          }
        }
        if (count_zero) {
          osn_count_zero = 0;
          break;
        }
      } else {
        if (my_flag.precision == 0 && my_flag.dot) {
          my_buffer[current_precision] = (int)fraction % 10 + 48;
          current_precision++;
          break;
        }
      }
      my_buffer[current_precision] = (int)fraction % 10 + 48;
      current_precision++;
    }
    if (current_precision <= precision && osn_count_zero &&
        my_flag.precision != 0) {
      my_buffer[current_precision] = end_fraction + 48;
      current_precision++;
    }
  }
  for (s21_size_t i = 0; i < s21_strlen(my_buffer); i++) {
    buffer[number_position] = my_buffer[i];
    number_position++;
  }
  format_flags(my_flag, buffer);
}

void octal_number(flag my_flag, char *buffer, va_list *pointer_value) {
  int degree = 0;
  long int number = 0;
  long int result = 0;
  if (my_flag.length == 'l') {
    number = va_arg(*pointer_value, long int);
  } else {
    number = va_arg(*pointer_value, int);
  }
  for (; number; number /= 8) {
    result += number % 8 * pow(10, degree);
    degree++;
  }
  char my_bus[BUFF_SIZE] = {'\0'};
  int width = my_flag.width;
  s21_itoa(result, my_bus);
  my_flag.width = width;
  int number_position = 0;
  if (my_flag.hash) {
    buffer[number_position] = '0';
    number_position++;
  }

  for (int i = 0; my_bus[i]; i++) {
    buffer[number_position] = my_bus[i];
    number_position++;
  }
  format_precision(my_flag, buffer);
  format_flags(my_flag, buffer);
}

void hexadecimal_number(flag my_flag, char *buffer, va_list *pointer_value) {
  long int number = 0;
  int degree = 0;
  if (my_flag.length == 'l') {
    number = va_arg(*pointer_value, long int);
  } else {
    number = va_arg(*pointer_value, int);
  }
  int count_zero = 0;
  if (number == 0) {
    count_zero = 1;
  }
  if (number < 0) {
    if (number > (-2147483647)) {
      number = 4294967296 + number;
    } else {
      number = 2147483648;
    }
  }
  if (number == 0) {
    buffer[0] = '0';
  }

  char *number_hexadecimal = "0123456789abcdef";
  if (my_flag.specifier == 'x') {
    number_hexadecimal = "0123456789abcdef";
  } else {
    number_hexadecimal = "0123456789ABCDEF";
  }
  long int copy_number = number;
  for (; copy_number; copy_number /= 16) {
    degree++;
  }
  int number_positions = 0;
  if (my_flag.dot && my_flag.precision > degree) {
    int my_precision = my_flag.precision;
    for (; my_precision - degree; my_precision--) {
      buffer[number_positions] = '0';
      number_positions++;
    }
  }

  char my_buf[BUFF_SIZE] = {'\0'};
  long int copy_number2 = number;
  for (int i = 0; copy_number2; copy_number2 /= 16, i++) {
    my_buf[i] = number_hexadecimal[copy_number2 % 16];
  }
  for (s21_size_t i = (s21_strlen(my_buf) - 1); i < s21_strlen(my_buf); i--) {
    buffer[number_positions] = my_buf[i];
    number_positions++;
  }

  if (my_flag.hash && my_flag.specifier == 'x') {
    for (s21_size_t i = s21_strlen(buffer) - 1, j = s21_strlen(buffer) + 1;
         i < s21_strlen(buffer); i--, j--) {
      buffer[j] = buffer[i];
    }
    buffer[0] = '0';
    if (!count_zero) {
      buffer[1] = 'x';
    }
  } else if (my_flag.hash && my_flag.specifier == 'X' && !count_zero) {
    for (s21_size_t i = s21_strlen(buffer) - 1, j = s21_strlen(buffer) + 1;
         i < s21_strlen(buffer); i--, j--) {
      buffer[j] = buffer[i];
    }
    buffer[0] = '0';
    buffer[1] = 88;
  }
  format_flags(my_flag, buffer);
}

void enter_pointer(char *buffer, va_list *pointer_value, flag my_flag) {
  buffer[0] = '0';
  buffer[1] = 'x';
  void *my_pointer;
  my_pointer = va_arg(*pointer_value, void *);
  long int my_int_pointer = (long int)my_pointer;
  int number_position = 1;
  char *number_hexadecimal = "0123456789abcdef";
  long int copy_numer_16 = my_int_pointer;
  int degree = 0;
  for (; copy_numer_16; copy_numer_16 /= 16) {
    degree++;
  }
  if (my_int_pointer == 0) {
    number_position++;
    buffer[number_position] = '0';
  }
  for (; my_int_pointer; my_int_pointer /= 16) {
    buffer[number_position + degree] = number_hexadecimal[my_int_pointer % 16];
    degree--;
  }
  number_position = 1 + degree;
  format_flags(my_flag, buffer);
}
