#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include "mz.h"
#include "main.h"
#include "hexdump.h"

void mz_print_header(const struct mz_header *header) {
    fprintf(stdout, "\n");
    fprintf(stdout, "Minimum extra allocations: %d (0x%04x)\n", header->min_extra_paragraphs, header->min_extra_paragraphs);
    fprintf(stdout, "Maximum extra allocations: %d (0x%04x)\n", header->max_extra_paragraphs, header->max_extra_paragraphs);
    fprintf(stdout, "Initial stack location: %d (0x%08x)\n",realaddress(header->ss, header->sp), realaddress(header->ss, header->sp));
    fprintf(stdout, "Relative program entry point: %d (0x%08x)\n", realaddress(header->cs, header->ip), realaddress(header->cs, header->ip));
    fprintf(stdout, "CS:IP: 0x%04x:0x0%04x\n", header->cs, header->ip);
    fprintf(stdout, "SS:SP: 0x%04x:0x0%04x\n", header->ss, header->sp);
    fprintf(stdout, "Overlay numbers: %d (0x%04x)\n", header->overlay_number, header->overlay_number);
    fprintf(stdout, "Blocks in file: %d (0x%04x)\n", header->blocks_in_file, header->blocks_in_file);
    fprintf(stdout, "Header paragraph size: %d (0x%04x)\n", header->header_paragraphs_size, header->header_paragraphs_size);
    fprintf(stdout, "Execution start point: %d (0x%08x)\n", (16 * header->header_paragraphs_size + 16 * header->cs + header->ip),(16 * header->header_paragraphs_size + 16 * header->cs + header->ip));
    fprintf(stdout, "Bytes on last page: %d (0x%04x)\n", header->bytes_in_last_block, header->bytes_in_last_block);
    fprintf(stdout, "Relocation table offset: %d (0x%08x)\n", header->relocation_table_offset, header->relocation_table_offset);
    fprintf(stdout, "Number of relocations: %d (0x%04x)\n", header->number_of_relocations, header->number_of_relocations);
    fprintf(stdout, "Checksum: %d (0x%04x)\n", header->checksum, header->checksum);
}

void dumpmz() {
    struct mz mz;

    mz.header = readdata(0);
    mz.entry = realaddress(mz.header->cs, mz.header->ip);
    mz.relocation_table = readdata(mz.header->relocation_table_offset);
    mz.start = mz.header->header_paragraphs_size * 16;
    mz.code_start = (16 * mz.header->header_paragraphs_size + (16 * mz.header->cs + mz.header->ip));

    mz_print_header(mz.header);
    fprintf(stdout, "\n");
    for (int i = 0; i < mz.header->blocks_in_file; i++) {
        fprintf(stdout, "Block: %d (0x%04x)\n", i, i);
        fprintf(stdout, "Offset in file: %d (0x%08x)\n", (mz.start + (i * 0x200)), (mz.start + (i * 0x200)));
        if (i < mz.header->blocks_in_file - 1) {
            fprintf(stdout, "Block size: %d (0x%04x)\n", 0x200, 0x200);
            //print_hex_dump(mz.start + (i * 0x200), 0x200);
        }
        else {
            fprintf(stdout, "Block size: %d (0x%04x)\n", mz.header->bytes_in_last_block, mz.header->bytes_in_last_block);
            //print_hex_dump(mz.start + (i * 0x200), mz.header->bytes_in_last_block);
        }
    }
    fprintf(stdout,"\n");
    if (mz.header->number_of_relocations >0) {
        fprintf(stdout, "Relocation table:\n");
        for (int i = 0; i < mz.header->number_of_relocations; i++) {
            fprintf(stdout, "Relocation %d (0x%04x)\n", i, i);
            fprintf(stdout, "Relocation segment: %d (0x%08x)\n", mz.relocation_table[i].segment, mz.relocation_table[i].segment);
            fprintf(stdout, "Relocation offset: %d (0x%08x)\n", mz.relocation_table[i].offset, mz.relocation_table[i].offset);
            fprintf(stdout, "Relocation real offset in file: %d (0x%08x)\n\n", (mz.start + realaddress(mz.relocation_table[i].segment, mz.relocation_table[i].offset)), (mz.start + realaddress(mz.relocation_table[i].segment, mz.relocation_table[i].offset)));
        }
    }
}