#include "s21_string.h"

char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  char *dest_end = dest;
  while (*dest_end != '\0') {
    dest_end++;
  }
  while (*src != '\0' && n > 0) {
    *dest_end = *src;
    dest_end++;
    src++;
    n--;
  }
  *dest_end = '\0';
  return dest;
}
