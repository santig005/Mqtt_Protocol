#include <stdint.h>
#include <stdio.h>

uint8_t next_byte(uint8_t **buffer) {
  uint8_t byte = **buffer;
  (*buffer)++;
  return byte;
}

void pack_byte(uint8_t **buf, uint8_t val) {
  **buf = val;
  (*buf) += sizeof(uint8_t);
}

uint32_t remaining_length(const uint8_t **buf) {

  char c;
  int multiplier = 1;
  uint32_t value = 0LL;

  do {
    c = **buf;
    value += (c & 127) * multiplier;
    multiplier *= 128;
    (*buf)++;
  } while ((c & 128) != 0);

  return value;
}
