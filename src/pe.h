#ifndef PE_H_INCLUDED
#define PE_H_INCLUDED

#include <stdint.h>
#include "mz.h"

#pragma pack(1)

struct pe_header {
    uint16_t magic;
    uint8_t byte_order;
    uint8_t word_order;
    uint16_t target_machine;
    uint16_t size_of_section_table;
    uint32_t time_date_stamp;
    uint32_t symbol_table_offset;
    uint32_t nuber_of_symbols;
    uint16_t size_of_optional_header;
    uint16_t flags;
};

struct pe {

    uint16_t magic;
    const struct mz_header *mz_header;

    const struct pe_header *pe_header;
};

#pragma pack()
#endif