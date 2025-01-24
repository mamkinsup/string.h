#include "s21_string.h"

void *s21_trim(const char *src, const char *trim_chars) {
  char *result = S21_NULL;
  if (src) {
    const char *new_trim_chars = S21_NULL;
    if (trim_chars == S21_NULL || trim_chars[0] == '\0') {
      new_trim_chars = " \t\n\r\v\f";
    } else {
      new_trim_chars = trim_chars;
    }
    s21_size_t src_len = s21_strlen(src);
    s21_size_t trim_start = 0;
    s21_size_t trim_end = src_len;
    while (trim_start < src_len &&
           s21_strchr(new_trim_chars, src[trim_start]) != S21_NULL) {
      trim_start++;
    }
    while (trim_end > trim_start &&
           s21_strchr(new_trim_chars, src[trim_end - 1]) != S21_NULL) {
      trim_end--;
    }
    s21_size_t trimmed_len = trim_end - trim_start;
    result = (char *)calloc(trimmed_len + 1, sizeof(char));
    if (result) {
      s21_memcpy(result, src + trim_start, trimmed_len);
    }
  }
  return result;
}
