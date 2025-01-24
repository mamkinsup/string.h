#include "s21_string.h"

char *s21_strtok(char *str, const char *delim) {
  static char *last_token = S21_NULL;
  if (str) {
    last_token = str;
  } else if (last_token == S21_NULL) {
    return S21_NULL;
  }
  while (*last_token != '\0' && s21_strchr(delim, *last_token) != S21_NULL) {
    last_token++;
  }
  if (*last_token == '\0') {
    return S21_NULL;
  }
  char *token_start = last_token;
  char *token_end = s21_strpbrk(token_start, delim);
  if (token_end != S21_NULL) {
    *token_end = '\0';
    last_token = token_end + 1;
  } else {
    last_token = S21_NULL;
  }
  return token_start;
}
