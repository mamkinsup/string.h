#include "s21_string.h"

void *s21_memchr(const void *str, int c, s21_size_t n) {
  const unsigned char *ptr = (const unsigned char *)str;
  for (s21_size_t i = 0; i < n && ptr; i++) {
    if (*ptr == (unsigned char)c) {
      return (void *)ptr;
    }
    ptr++;
  }
  return S21_NULL;
}
