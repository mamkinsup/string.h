#include "s21_string.h"

void *s21_insert(const char *src, const char *str, s21_size_t start_index) {
  if (!str || !src || start_index > s21_strlen(src)) return S21_NULL;
  s21_size_t res_len = s21_strlen(src) + s21_strlen(str);
  char *res_str = calloc(res_len + 1, (res_len + 1) * sizeof(char));
  if (res_str) {
    for (s21_size_t i = 0; i < start_index; i++) res_str[i] = src[i];
    for (s21_size_t i = start_index, j = 0; i < start_index + s21_strlen(str);
         i++, j++)
      res_str[i] = str[j];
    for (s21_size_t i = start_index + s21_strlen(str); i < res_len; i++)
      res_str[i] = src[i - s21_strlen(str)];
  }
  return res_str;
}
