#include "s21_string.h"

char *s21_strncpy(char *dest, const char *src, s21_size_t n) {
  char *ptr = dest;
  s21_size_t i = 0;
  while (i < n && *src != '\0') {
    *ptr = *src;
    ptr++;
    src++;
    i++;
  }
  while (i < n) {
    *ptr = '\0';
    ptr++;
    i++;
  }
  return dest;
}
