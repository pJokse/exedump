#ifndef W3_H_INCLUDED
#define W3_H_INCLUDED

#include <stdint.h>
#include "mz.h"

#pragma pack(1)
struct w3_header {
    uint8_t signature[2];
    union {
        uint16_t vmm_version;
        struct {
            uint8_t vmm_minor;
            uint8_t vmm_major;
        };
    };
    uint16_t module_count;
    uint8_t reserved[10];
};

struct w3_module_entry {
    char name[8];
    uint32_t offset;
    uint32_t size;
};

#pragma pack()

struct w3 {
    uint32_t magic;

    const struct mz_header *mz_header;

    const struct w3_header *w3_header;    
};

#endif