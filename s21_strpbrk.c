#include "s21_string.h"

char *s21_strpbrk(const char *str1, const char *str2) {
  while (*str1 != '\0') {
    const char *ptr = str2;
    while (*ptr != '\0') {
      if (*str1 == *ptr) {
        return (char *)str1;
      }
      ptr++;
    }
    str1++;
  }
  return S21_NULL;
}
