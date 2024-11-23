#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include "w3.h"
#include "main.h"

void w3_print_header(const struct w3_header *header) {
    fprintf(stdout, "\n");
    fprintf(stdout, "VMM version: %u.%u\n", header->vmm_major, header->vmm_minor);
}

void w3_print_module_table(struct w3 w3) {
    size_t offset;
    struct w3_module_entry *module;
    fprintf(stdout, "\n");
    fprintf(stdout, "VxD Module Table (%u modules):\n", w3.w3_header->module_count);
    fprintf(stdout, "ID\tName\t\t\tOffset\t\tSize\n");
    offset = w3.mz_header->new_header_offset + sizeof(struct w3_header);
    for(int i = 0; i < w3.w3_header->module_count; i++) {
        module = readdata(offset);
        offset = offset + sizeof(struct w3_module_entry);
        fprintf(stdout, "%u\t%s\t\t0x%08x\t%u bytes (0x%08x)\n", i, module->name, module->offset, module->size, module->size);
    }
}

void dumpw3() {
    struct w3 w3;

    w3.mz_header = readdata(0);
    w3.w3_header = readdata(w3.mz_header->new_header_offset);
    w3_print_header(w3.w3_header);

    w3_print_module_table(w3);
}