#ifndef PACKER_H
#define PACKER_H

#include <stdint.h>
#include <stdlib.h>

uint8_t next_byte(uint8_t **buf);
void pack_byte(uint8_t **buf, uint8_t val);
uint16_t next_16b(uint8_t **buf);
uint32_t remaining_length(uint8_t **buf);
uint8_t * next_nbytes(uint8_t **buf, size_t len);
uint16_t read_string16(uint8_t **buf, uint8_t **dest);


#endif
