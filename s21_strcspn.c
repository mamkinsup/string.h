#include "s21_string.h"

s21_size_t s21_strcspn(const char *str1, const char *str2) {
  s21_size_t i = 0;
  while (str1[i] != '\0') {
    s21_size_t j = 0;
    while (str2[j] != '\0') {
      if (str1[i] == str2[j]) {
        return i;
      }
      j++;
    }
    i++;
  }
  return i;
}
