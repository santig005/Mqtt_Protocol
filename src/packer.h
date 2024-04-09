#ifndef PACKER_H
#define PACKER_H

#include <stdint.h>
#include <stdlib.h>

uint8_t next_byte(uint8_t **buf);
void pack_byte(uint8_t **buf, uint8_t val);
uint16_t next_16b(uint8_t **buf);
void pack_16b(uint8_t **buf, uint16_t val);
uint32_t remaining_length(uint8_t **buf);
void pack_remaining_length(uint8_t **buf, uint32_t len);
uint8_t nbytes_remaining_length(uint32_t len);
uint8_t * next_nbytes(uint8_t **buf, size_t len);
void pack_nbytes(uint8_t **buf, uint8_t *val, size_t len);
uint16_t read_string16(uint8_t **buf, uint8_t **dest);
void write_string16(uint8_t **buf, uint8_t *str);


#endif
