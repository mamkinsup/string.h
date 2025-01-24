#include "s21_string.h"

void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  unsigned char *dest_ptr = (unsigned char *)dest;
  const unsigned char *src_ptr = (const unsigned char *)src;

  for (s21_size_t i = 0; i < n; i++) {
    dest_ptr[i] = src_ptr[i];
  }

  return dest;
}
