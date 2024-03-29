#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include "le.h"
#include "main.h"
#include "hexdump.h"

void le_print_byte_order(uint8_t order) {
    if (order == 0x00) fprintf(stdout, "Little endian\n");
    else fprintf(stdout, "Big endian\n");
}

void le_print_cpu_type(uint8_t cpu_type) {
    fprintf(stdout, "CPU type: ");
    switch(cpu_type) {
    case 0x01:
        fprintf(stdout,"Intel 286\n");
        break;
    case 0x02:
        fprintf(stdout,"Intel 386\n");
        break;
    case 0x03:
        fprintf(stdout, "Intel 486\n");
        break;
    case 0x04:
        fprintf(stdout,"Intel Pentium\n");
        break;
    case 0x20:
        fprintf(stdout,"Intel i860 (N10)\n");
        break;
    case 0x21:
        fprintf(stdout,"Intel N11\n");
        break;
    case 0x40:
        fprintf(stdout,"MIPS Mk1 (R2000, R3000)\n");
        break;
    case 0x41:
        fprintf(stdout,"MIPS Mk2 (R6000)\n");
        break;
    case 0x42:
        fprintf(stdout,"MIPS Mk3 (R4000)\n");
        break;
    default:
        fprintf(stdout,"Unknown CPU\n");
        break;
    }
}

void le_print_os_type(uint16_t os_type) {
    fprintf(stdout, "OS Type: ");
    switch(os_type) {
    case 0x01:
        fprintf(stdout,"OS/2\n");
        break;
    case 0x02:
        fprintf(stdout,"16 bit Windows\n");
        break;
    case 0x03:
        fprintf(stdout,"DOS\n");
        break;
    case 0x04:
        fprintf(stdout,"32 bit Windows\n");
        break;
    case 0x05:
        fprintf(stdout,"IBM Microkernel Personality Neutral\n");
        break;
    case 0x20:
        fprintf(stdout,"Windows NT\n");
        break;
    case 0x21:
        fprintf(stdout,"POSIX\n");
        break;
    default:
        fprintf(stdout,"Unknown OS\n");
        break;
    }
}

void le_print_exe_flags(uint32_t exe_flags) {
    fprintf(stdout, "EXE flags: 0x%08x\n", exe_flags);
    if (exe_flags & 0x00000001UL) fprintf(stdout,"- Single data\n");
    if (exe_flags & 0x00000004UL) fprintf(stdout,"- Per-process Initialization\n");
    else fprintf(stdout,"- Global Initialization\n");
    if (exe_flags & 0x00000010UL) fprintf(stdout,"- No internal fixup\n"); // ???
    if (exe_flags & 0x00000020UL) fprintf(stdout,"- No external fixup\n"); // ???
    if ((exe_flags & 0x00000300UL) == 0x00000100) fprintf(stdout,"- Incompatible with PM windowing\n");
    if ((exe_flags & 0x00000300UL) == 0x00000200) fprintf(stdout,"- Compatible with PM windowing\n");
    if ((exe_flags & 0x00000300UL) == 0x00000300) fprintf(stdout,"- Uses PM windowing API\n");
    if (exe_flags & 0x00000400UL) fprintf(stdout,"- Reserved\n");
    if (exe_flags & 0x00000800UL) fprintf(stdout,"- Reserved\n");
    if (exe_flags & 0x00001000UL) fprintf(stdout,"- Reserved\n");
    if (exe_flags & 0x00002000UL) fprintf(stdout,"- Module is not loadable\n");
    if (exe_flags & 0x00004000UL) fprintf(stdout,"- Reserved\n");
    if ((exe_flags & 0x00038000UL) == 0x00000000) fprintf(stdout,"- Program module (EXE)\n");
    if ((exe_flags & 0x00038000UL) == 0x00008000) fprintf(stdout,"- Library module (DLL)\n");
    if ((exe_flags & 0x00038000UL) == 0x00018000) fprintf(stdout,"- Protected Memory Library Module\n");
    if ((exe_flags & 0x00038000UL) == 0x00020000) fprintf(stdout,"- Physical Device Driver module\n");
    if ((exe_flags & 0x00038000UL) == 0x00028000) fprintf(stdout,"- Virtual Device Driver module\n");
    if (exe_flags & 0x00040000UL) fprintf(stdout,"- DLL\n");
    else fprintf(stdout,"- Program file\n");
    if (exe_flags & 0x00004000UL) fprintf(stdout,"- Reserved\n");
    if (exe_flags & 0x00080000UL) fprintf(stdout,"- MP Unsafe\n");
    if (exe_flags & 0x00100000UL) fprintf(stdout,"- Protected memory library module\n");
    if (exe_flags & 0x00200000UL) fprintf(stdout,"- Device driver\n");
    if (exe_flags & 0x40000000UL) fprintf(stdout,"- DLL Per-process termination\n");
    else fprintf(stdout,"- DLL global termination\n"); 

}

void le_print_vxd_info(struct le_vxd vxd) {
    fprintf(stdout,"Windows VXD version information resource offset: %d (0x%08x)\n", vxd.windows_vxd_version_info_resource_offset, vxd.windows_vxd_version_info_resource_offset);
    fprintf(stdout, "Windows VXD version information resource length: %d bytes (0x%08x)\n", vxd.windows_vxd_version_info_resource_length, vxd.windows_vxd_version_info_resource_length);
    fprintf(stdout, "Device ID: %d (0x%08x)\n", vxd.device_id, vxd.device_id);
    fprintf(stdout, "DDK version: %d.%d (0x%08x)\n", (vxd.ddk_version >> 8), vxd.ddk_version & 0xFF, vxd.ddk_version);
}

void le_print_header(const struct le_header *header) {
    (header->magic == 0x454C) ? fprintf(stdout, "Magic: LE\n") : fprintf(stdout, "Magic: LX\n");
    fprintf(stdout, "\n");
    fprintf(stdout, "Byte order: ");
    le_print_byte_order(header->byte_order);
    fprintf(stdout, "Word order: ");
    le_print_byte_order(header->word_order);
    fprintf(stdout, "EXE format level: %d (0x%08x)\n", header->exe_format_level, header->exe_format_level);
    le_print_cpu_type(header->cpu_type);
    le_print_os_type(header->os_type);
    fprintf(stdout, "EXE version: %d (0x%08x)\n", header->exe_version, header->exe_version);
    le_print_exe_flags(header->exe_flags);
    fprintf(stdout, "Number of pages: %d (0x%08x)\n", header->number_of_pages, header->number_of_pages);
    fprintf(stdout, "Object starting number: %d (0x%08x)\n", header->starting_object_number, header->starting_object_number);
    fprintf(stdout, "EIP: %d (0x%08x)\n", header->eip, header->eip);
    fprintf(stdout, "Stack object number: %d (0x%08x)\n", header->stack_object_number_for_pointer, header->stack_object_number_for_pointer);
    fprintf(stdout, "ESP: %d (0x%08x)\n", header->esp, header->esp);
    fprintf(stdout, "EXE page size: %d (0x%08x)\n", header->exe_page_size, header->exe_page_size);
    if (header->magic == 0x454C) fprintf(stdout, "Size of last page: %d (0x%08x)\n", header->page.size_of_last_page, header->page.size_of_last_page);
    else fprintf(stdout, "Page left shift offset: %d (0x%08x)\n", header->page.page_shift_offset, header->page.page_shift_offset);
    fprintf(stdout, "Fixup section size: %d bytes (0x%08x)\n", header->fixup_section_size, header->fixup_section_size);
    fprintf(stdout, "Fixup section checksum: %d (0x%08x)\n", header->fixup_section_checksum, header->fixup_section_checksum);
    fprintf(stdout, "Loader section size: %d bytes (0x%08x)\n", header->loader_section_size, header->loader_section_size);
    fprintf(stdout, "Loader section checksum: %d (0x%08x)\n", header->loader_section_checksum, header->loader_section_checksum);
    fprintf(stdout, "Object table offset: %d (0x%08x)\n", header->object_table_offset, header->object_table_offset);
    fprintf(stdout, "Number of objects: %d (0x%08x)\n", header->number_of_objects, header->number_of_objects);
    fprintf(stdout, "Object map offset: %d (0x%08x)\n", header->object_map_offset, header->object_map_offset);
    fprintf(stdout, "Iterated data map offset: %d (0x%08x)\n", header->iterated_data_map_offset, header->iterated_data_map_offset);
    fprintf(stdout, "Resource table offset: %d (0x%08x)\n", header->resource_table_offset, header->resource_table_offset);
    fprintf(stdout, "Number of resource table entries: %d (0x%08x)\n", header->number_of_resource_entries, header->number_of_resource_entries);
    fprintf(stdout, "Resident names table offset: %d (0x%08x)\n", header->resident_names_table_offset, header->resident_names_table_offset);
    fprintf(stdout, "Entry table offset: %d (0x%08x)\n", header->entry_table_offset, header->entry_table_offset);
    fprintf(stdout, "Module directives table offset: %d (0x%08x)\n", header->module_directives_table_offset, header->module_directives_table_offset);
    fprintf(stdout, "Number of module directives: %d (0x%08x)\n", header->number_of_module_directives, header->number_of_module_directives);
    fprintf(stdout, "Fixup page table offset: %d (0x%08x)\n", header->fixup_page_table_offset, header->fixup_page_table_offset);
    fprintf(stdout, "Fixup record table offset: %d (0x%08x)\n", header->fixup_record_table_offset, header->fixup_record_table_offset);
    fprintf(stdout, "Import name table offset: %d (0x%08x)\n", header->import_name_table_offset, header->import_name_table_offset);
    fprintf(stdout, "Number of input table entries: %d (0x%08x)\n", header->number_of_input_table_entries, header->number_of_input_table_entries);
    fprintf(stdout, "Import procedure name table offset: %d (0x%08x)\n", header->import_procedure_name_table_offset, header->import_procedure_name_table_offset);
    fprintf(stdout, "Per page checksum table offset: %d (0x%08x)\n", header->per_page_checksum_table_offset, header->per_page_checksum_table_offset);
    fprintf(stdout, "Enumerated data pages offset: %d (0x%08x)\n", header->enumerated_data_pages_offset, header->enumerated_data_pages_offset);
    fprintf(stdout, "Number of enumerated data pages: %d (0x%08x)\n", header->number_of_enumerated_data_pages, header->number_of_enumerated_data_pages);
    fprintf(stdout, "Non-resident name table offset: %d (0x%08x)\n", header->non_resident_name_table_offset, header->non_resident_name_table_offset);
    fprintf(stdout, "Non-resident name table size: %d bytes (0x%08x)\n", header->non_resident_name_table_size, header->non_resident_name_table_size);
    fprintf(stdout, "Non-resident name table checksum: %d (0x%08x)\n", header->non_resident_name_table_checksum, header->non_resident_name_table_checksum);
    fprintf(stdout, "Debug information offset: %d (0x%08x)\n", header->debug_offset, header->debug_offset);
    fprintf(stdout, "Debug information size: %d bytes (0x%08x)\n", header->debug_size, header->debug_size);
    fprintf(stdout, "Heap size: %d bytes (0x%08x)\n", header->heap_size, header->heap_size);
    fprintf(stdout, "Stack size: %d bytes (0x%08x)\n", header->stack_size, header->stack_size);
    if (header->magic == 0x454C) le_print_vxd_info(header->reserved.vxd);
};

void dumple() {
    struct le le;

    le.mz_header = readdata(0);
    le.le_header = readdata(le.mz_header->new_header_offset);

    le_print_header(le.le_header);

};