#include <stdio.h>
#include <stdint.h>

uint8_t next_byte(uint8_t **buffer){
    uint8_t byte = **buffer;
    (*buffer)++;
    return byte;
}

