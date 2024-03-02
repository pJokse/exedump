#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED
#include <stdint.h>

// Data reads
#define readdata(x) (void *)(binary + x)
#define readbyte(x) (uint8_t)(binary[x])
#define readword(x) *(uint16_t *)(binary + x)
#define readdword(x) *(uint32_t *)(binary + x)
#define readqword(x) *(uint64_t *)(binary + x)

// Filetypes
enum FILETYPES {
    FILETYPE_UNKNOWN,
    FILETYPE_MZ,
    FILETYPE_LE,
    FILETYPE_LX,
    FILETYPE_NE,
    FILETYPE_PE,
    FILETYPE_PHARLAP_MP,
    FILETYPE_PHARLAP_P2,
    FILETYPE_PHARLAP_P3,
    FILETYPE_OMF_OBJ,
    FILETYPE_OMF_LIB,
    FILETYPE_SYM
};

typedef struct {
    uint8_t exe_type;
    char description_short[12];
    char description[64];
} filetype;

extern uint8_t *binary;
extern size_t file_size;
extern uint8_t file_type;

void dumpmz();
void dumpne();
void dumple();
void dumppe();
void dumpomf();

#endif