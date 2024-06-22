#ifndef MZ_H_INCLUDED
#define MZ_H_INCLUDED

#include <stdint.h>

#define realaddress(segment, offset) (((uint32_t)segment < 0xFFF0) ? (uint32_t)((segment * 0x10) + offset):(uint32_t)(((segment * 0x10) + offset) - 0x100000))

#pragma pack(1)
struct mz_header {
    uint8_t signature[2];
    uint16_t bytes_in_last_block;
    uint16_t blocks_in_file;
    uint16_t number_of_relocations;
    uint16_t header_paragraphs_size;
    uint16_t min_extra_paragraphs;
    uint16_t max_extra_paragraphs;
    int16_t ss;
    uint16_t sp;
    uint16_t checksum;
    uint16_t ip;
    int16_t cs;
    uint16_t relocation_table_offset;
    uint16_t overlay_number;
    uint16_t reserved[0x10];
    uint32_t new_header_offset;
};

struct relocation_table {
    uint16_t offset;
    uint16_t segment;
};

struct mz {
    const struct mz_header *header;
    const struct relocation_table *relocation_table;

    uint32_t entry;
    uint8_t *flags;
    uint32_t start;
    uint32_t length;
    uint32_t code_start;
};

#pragma pack()

#endif