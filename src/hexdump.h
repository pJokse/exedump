#ifndef HEXDUMP_H_INCLUDED
#define HEXDUMP_H_INCLUDED

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include "main.h"

static void print_hex_dump(size_t offset, uint16_t datasize, size_t printable_offset) {
    uint16_t i, j;
    uint8_t leftover;
    leftover = datasize % 16;
    for (i = 0; i < datasize / 16; i++) {
        fprintf(stdout, "0x%08lx    ", printable_offset + (i * 16));
        for (j = 0; j < 16; j++) {
            fprintf(stdout, "%02x ", readbyte(offset + (i * 16) + j));
        }
        fprintf(stdout, "    ");
        for (j = 0; j < 16; j++) {
            fprintf(stdout, "%c", isprint(readbyte(offset + (i * 16) + j)) ? readbyte(offset + (i * 16) + j) : '.');
        }
        fprintf(stdout, "\n");
    }
    if (leftover > 0) {
        fprintf(stdout, "0x%08lx    ", printable_offset + datasize - leftover);
        for (j = 0; j < leftover - 1; j++) {
            fprintf(stdout, "%02x ", readbyte(offset + datasize - leftover + j));
        }
        for (i = 0; i < 16 - leftover + 1; i++) fprintf(stdout, "   ");
        fprintf(stdout, "    ");
        for (j = 0; j < leftover - 1; j++) {
            fprintf(stdout, "%c", isprint(readbyte(offset + datasize - leftover + j)) ? readbyte(offset + datasize - leftover + j) : '.');
        }
    }
}

#endif