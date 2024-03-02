#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include "ne.h"
#include "main.h"
#include "hexdump.h"

void ne_print_header_flags(uint16_t flags) {
    fprintf(stdout, "Flags: 0x%08x\n", flags);
    if (flags & 0x8000) { fprintf(stdout, "- Not a process\n"); }
    if (flags & 0x4000) { fprintf(stdout, "- Non conforming program\n"); }
    if (flags & 0x2000) { fprintf(stdout, "- Errors in image\n"); }
    if (flags & 0x0800) { fprintf(stdout, "- Bound as family app\n"); }
    if ((flags & 0x0700) == 0x0300) { fprintf(stdout, "- Uses Presentation Manager Windowing API\n"); }
    if ((flags & 0x0700) == 0x0200) { fprintf(stdout, "- Compatible with Presentation Manager Windowing\n"); }
    if ((flags & 0x0700) == 0x0100) { fprintf(stdout, "- Not compatible with Presentation Manager Windowing\n"); }
    if ((flags & 0x0700) == 0x0000) { fprintf(stdout, "- No subsystem\n"); }
    if (flags & 0x0080) { fprintf(stdout, "- Floating point instructions\n"); }
    if (flags & 0x0040) { fprintf(stdout, "- 386 Instructions\n"); }
    if (flags & 0x0020) { fprintf(stdout, "- 286 Instructions\n"); }
    if (flags & 0x0010) { fprintf(stdout, "- 8086 Instructions\n"); }
    if (flags & 0x0008) { fprintf(stdout, "- Runs in Protected mode only\n"); }
    if (flags & 0x0004) { fprintf(stdout, "- Global initialization\n"); }
    if ((flags & 0x0003) == 0x0) { fprintf(stdout, "- No DGROUP\n"); }
    if ((flags & 0x0003) == 0x1) { fprintf(stdout, "- Single DGROUP\n"); }
    if ((flags & 0x0003) == 0x2) { fprintf(stdout, "- Multiple DGROUPs\n"); }
    if ((flags & 0x0003) == 0x3) { fprintf(stdout, "- Unknown DGROUP type 3\n"); }
}

void ne_print_header_taget_os(uint8_t target_os) {
    fprintf(stdout, "Target OS: %d (0x%08x)\n", target_os, target_os);
    fprintf(stdout, "- %s\n", ne_exetypes[target_os]);
}

void ne_print_header_os2_flags(uint8_t os2_exe_flags) {
    fprintf(stdout, "OS/2 EXE flags: %d (0x%08x)\n", os2_exe_flags, os2_exe_flags);
    if (os2_exe_flags & 0x0001) { fprintf(stdout, "- Long filename support\n"); }
    if (os2_exe_flags & 0x0002) { fprintf(stdout, "- 2.x protected mode\n"); }
    if (os2_exe_flags & 0x0004) { fprintf(stdout, "- 2.x proportional fonts\n"); }
    if (os2_exe_flags & 0x0008) { fprintf(stdout, "- fast-load area\n"); }
    if (os2_exe_flags & 0xfff0) { fprintf(stdout, "- Unknown flags\n"); }
}

void ne_print_header(const struct ne_header *header) {
    fprintf(stdout, "\n");
    fprintf(stdout, "Linker version: %d.%d\n", header->version, header->revision);
    fprintf(stdout, "Entry table offset: %d (0x%08x)\n", header->entry_table_offset, header->entry_table_offset);
    fprintf(stdout, "Entry table size: %d bytes (0x%08x)\n", header->number_of_bytes_in_entry_table, header->number_of_bytes_in_entry_table);
    fprintf(stdout, "CRC: %d (0x%08x)\n", header->crc, header->crc);
    ne_print_header_flags(header->flags);
    fprintf(stdout, "Automatic data segment offset: %d (0x%08x)\n", header->automatic_data_segment_index, header->automatic_data_segment_index);
    fprintf(stdout, "Initial heap size: %d bytes (0x%08x)\n", header->initial_heap_size, header->initial_heap_size);
    fprintf(stdout, "Initial stack size: %d bytes (0x%08x)\n", header->initial_stack_size, header->initial_stack_size);
    fprintf(stdout, "Entry point: %d (0x%08x)\n", header->entry_point, header->entry_point);
    fprintf(stdout, "Initial stack pointer: %d (0x%08x)\n", header->initial_stack_pointer, header->initial_stack_pointer);
    fprintf(stdout, "Number of segments: %d (0x%08x)\n", header->number_of_segments, header->number_of_segments);
    fprintf(stdout, "Number of module references: %d (0x%08x)\n", header->number_of_module_references, header->number_of_module_references);
    fprintf(stdout, "Non-residential names table offset: %d (0x%08x)\n", header->non_resident_names_table_offset, header->non_resident_names_table_offset);
    fprintf(stdout, "Segment table offset: %d (0x%08x)\n", header->segment_table_offset, header->segment_table_offset);
    fprintf(stdout, "Resource table offset: %d (0x%08x)\n", header->resource_table_offset, header->resource_table_offset);
    fprintf(stdout, "Resident names table offset: %d (0x%08x)\n", header->resident_name_table_offset, header->resident_name_table_offset);
    fprintf(stdout, "Module referece table offset: %d (0x%08x)\n", header->module_reference_table_offset, header->module_reference_table_offset);
    fprintf(stdout, "Import name table offset: %d (0x%08x)\n", header->import_name_table_offset, header->import_name_table_offset);
    fprintf(stdout, "Non-resident names table offset: %d (0x%08x)\n", header->non_resident_names_table_offset, header->non_resident_names_table_offset);
    fprintf(stdout, "File alignment size shift count: %d bytes (0x%08x)\n", header->file_alignment_size_shift_count, header->file_alignment_size_shift_count);
    fprintf(stdout, "Number of resident names table entries: %d (0x%08x)\n", header->number_of_resident_name_table_entries, header->number_of_resident_name_table_entries);
    ne_print_header_taget_os(header->target_os);
    ne_print_header_os2_flags(header->os2_exe_flags);
    fprintf(stdout, "Return thunk offset: %d (0x%08x)\n", header->return_thunk_offset, header->return_thunk_offset);
    fprintf(stdout, "Segment reference thunk offset: %d (0x%08x)\n", header->segment_reference_thunk_offset, header->segment_reference_thunk_offset);
    fprintf(stdout, "Minimum code swap area size: %d bytes (0x%08x)\n", header->minimum_code_swap_area_size, header->minimum_code_swap_area_size);
    fprintf(stdout, "Minimum expected windows version: %d.%d (0x%08x)\n", (header->expected_minimum_windows_version >> 8), (header->expected_minimum_windows_version & 0xFF), header->expected_minimum_windows_version);
}

void dumpne() {
    struct ne ne;

    ne.mz_header = readdata(0);
    ne.ne_header = readdata(ne.mz_header->new_header_offset);

    ne_print_header(ne.ne_header);
}