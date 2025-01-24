#include "s21_string.h"

int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
  s21_size_t i = 0;
  while (i < n && (str1[i] != '\0' || str2[i] != '\0')) {
    if (str1[i] != str2[i]) {
      return (int)(unsigned char)str1[i] - (int)(unsigned char)str2[i];
    }
    i++;
  }
  return 0;
}
