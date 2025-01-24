#include "s21_string.h"

void *s21_to_upper(const char *str) {
  char *result = S21_NULL;
  if (str) {
    s21_size_t length = s21_strlen(str);
    result = (char *)malloc((length + 1) * sizeof(char));
    if (result) {
      for (s21_size_t i = 0; i < length; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
          result[i] = str[i] - ('a' - 'A');
        } else {
          result[i] = str[i];
        }
      }
      result[length] = '\0';
    }
  }
  return result;
}
