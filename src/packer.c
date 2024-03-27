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

uint16_t next_16b(const uint8_t **buf) {
    uint16_t val;
    memcpy(&val, *buf, sizeof(uint16_t));
    (*buf) += sizeof(uint16_t);
    return ntohs(val);
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

uint8_t next_nbytes(uint8_t **buf, size_t n) {
  uint8_t* dest=try_alloc(n+1);
  memcpy(dest, *buf, n);
  dest[n]='\0';
  (*buf) += n;
  return dest;
}

uint16_t read_string16(uint8_t **buf, uint8_t **dest) {
  uint16_t len = next_16b(buf);
  *dest = next_nbytes(buf, len);
  return len;
}
