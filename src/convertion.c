#include "convertion.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint8_t *hextobytes(char *hexWord) {
  size_t len = strlen(hexWord);
  if (len % 2 != 0) {
    printf("Invalid input: Hexadecimal word must have an even number of "
           "characters.\n");
    return NULL;
  }

  size_t byteCount = len / 2;
  uint8_t *bytes = (uint8_t *)malloc(byteCount);
  if (bytes == NULL) {
    printf("Memory allocation failed.\n");
    return NULL;
  }

  for (size_t i = 0; i < byteCount; i++) {
    sscanf(hexWord + (i * 2), "%2hhx", &bytes[i]);
  }
  return bytes;
}