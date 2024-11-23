#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include "pe.h"
#include "main.h"
#include "hexdump.h"

void pe_print_byte_order(uint8_t order) {
    if (order == 0x00) fprintf(stdout, "Little endian\n");
    else fprintf(stdout, "Big endian\n");
}

void pe_print_target_machine(uint16_t target_machine) {
    fprintf(stdout, "Target machine: ");
    switch (target_machine) {
        case 0x8664:
            fprintf(stdout, "AMD64\n");
            break;
        case 0x01C0:
            fprintf(stdout, "ARM\n");
            break;
        case 0x014C:
            fprintf(stdout, "x86\n");
            break;
        case 0x200:
            fprintf(stdout, "IA64\n");
            break;
        default:
            fprintf(stdout, "Unknown (0x%08x)\n", target_machine);
            break;
    }
}

void pe_print_flags(uint16_t flags) {
    fprintf(stdout, "Flags: %d (0x%08x)\n", flags, flags);
    if (flags & 0x0001) fprintf(stdout,"- relocations stripped\n");
    if (flags & 0x0002) fprintf(stdout,"- executable\n");
    if (flags & 0x0004) fprintf(stdout,"- line numbers stripped\n");
    if (flags & 0x0008) fprintf(stdout,"- local symbols stripped\n");
    if (flags & 0x0010) fprintf(stdout,"- aggressively trimmed\n");
    if (flags & 0x0020) fprintf(stdout,"- large address aware\n");
    if (flags & 0x0040) fprintf(stdout,"- 16-bit\n");
    if (flags & 0x0080) fprintf(stdout,"- little-endian\n");
    if (flags & 0x0100) fprintf(stdout,"- 32-bit\n");
    if (flags & 0x0200) fprintf(stdout,"- debug info stripped\n");
    if (flags & 0x0400) fprintf(stdout,"- IMAGE_FILE_REMOVABLE_RUN_FROM_SWAP\n");
    if (flags & 0x0800) fprintf(stdout,"- IMAGE_FILE_NET_RUN_FROM_SWAP\n");
    if (flags & 0x1000) fprintf(stdout,"- system file\n");
    if (flags & 0x2000) fprintf(stdout,"- DLL\n");
    if (flags & 0x4000) fprintf(stdout,"- uniprocessor\n");
    if (flags & 0x8000) fprintf(stdout,"- big-endian\n");
}

void pe_print_header(const struct pe_header *header) {
    fprintf(stdout, "\n");
    fprintf(stdout, "Byte order: ");
    pe_print_byte_order(header->byte_order);
    fprintf(stdout, "Word order: ");
    pe_print_byte_order(header->word_order);
    pe_print_target_machine(header->target_machine);
    fprintf(stdout, "Size of section table: %d (0x%08x)\n", header->size_of_section_table, header->size_of_section_table);
    fprintf(stdout, "Time/Date stamp: %d (0x%08x)\n", header->time_date_stamp, header->time_date_stamp);
    fprintf(stdout, "Symbol table offset: %d (0x%08x)\n", header->symbol_table_offset, header->symbol_table_offset);
    fprintf(stdout, "Number of symbols: %d (0x%08x)\n", header->nuber_of_symbols, header->nuber_of_symbols);
    fprintf(stdout, "Size of optional header: %d bytes (0x%08x)\n", header->size_of_optional_header, header->size_of_optional_header);
    pe_print_flags(header->flags);
}


void dumppe() {
    struct pe pe;

    pe.mz_header = readdata(0);
    pe.pe_header = readdata(pe.mz_header->new_header_offset);

    pe_print_header(pe.pe_header);
}