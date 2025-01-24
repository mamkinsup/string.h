#include "s21_string.h"

int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  const unsigned char *ptr1 = (const unsigned char *)str1;
  const unsigned char *ptr2 = (const unsigned char *)str2;
  int exit_status = 0;
  s21_size_t i = 0;
  while (i < n && exit_status == 0) {
    if (ptr1[i] != ptr2[i]) {
      exit_status = ptr1[i] - ptr2[i];
    }
    i++;
  }
  return exit_status;
}
