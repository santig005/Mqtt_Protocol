#ifndef PACKER_H
#define PACKER_H

#include <stdint.h>

uint8_t next_byte(uint8_t **buffer);
void pack_byte(uint8_t **buf, uint8_t val);
uint32_t remaining_length(const uint8_t **buf);


#endif
