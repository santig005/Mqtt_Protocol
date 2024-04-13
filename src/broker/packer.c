#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>


uint8_t next_byte(uint8_t **buffer) {
  uint8_t byte = **buffer;
  (*buffer)++;
  return byte;
}

void pack_byte(uint8_t **buf, uint8_t val) {
  **buf = val;
  (*buf) += sizeof(uint8_t);
}

uint16_t next_16b(uint8_t **buf) {
    uint16_t val;
    memcpy(&val, *buf, sizeof(uint16_t));
    (*buf) += sizeof(uint16_t);
    return ntohs(val);
    //return val;
}
void pack_16b(uint8_t **buf, uint16_t val) {
  uint16_t val_n = htons(val);
  memcpy(*buf, &val_n, sizeof(uint16_t));
  *buf += sizeof(uint16_t);
}

uint32_t remaining_length( uint8_t **buf) {

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

void pack_remaining_length(uint8_t **buf, uint32_t len) {
  do {
    uint8_t encoded_byte = len % 128;
    len /= 128;
    if (len > 0) {
      encoded_byte |= 128;
    }
    pack_byte(buf, encoded_byte);
  } while (len > 0);
}
uint8_t nbytes_remaining_length(uint32_t len) {
  uint8_t nbytes = 0;
  do {
    len /= 128;
    nbytes++;
  } while (len > 0);
  return nbytes;
}

uint8_t* next_nbytes(uint8_t **buf, size_t n) {
  uint8_t* dest=malloc(n+1);
  memcpy(dest, *buf, n);
  dest[n]='\0';
  (*buf) += n;
  return dest;
}
void pack_nbytes(uint8_t **buf, uint8_t *data, size_t n) {
  memcpy(*buf, data, n);
  *buf += n;
}

uint16_t read_string16(uint8_t **buf, uint8_t **dest) {
  uint16_t len = next_16b(buf);
  *dest = next_nbytes(buf, len);
  return len;
}
void write_string16(uint8_t **buf, uint8_t *str) {
  uint16_t len = strlen((const char *)str);
  uint16_t len_n = htons(len);
  memcpy(*buf, &len_n, sizeof(uint16_t));
  *buf += sizeof(uint16_t);
  memcpy(*buf, str, len);
  *buf += len;
}


