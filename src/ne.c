#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include "ne.h"
#include "main.h"
#include "hexdump.h"

void ne_print_header_flags(uint16_t flags) {
    fprintf(stdout, "Flags: 0x%08x\n", flags);
    if (flags & 0x8000) fprintf(stdout, "- Not a process\n");
    if (flags & 0x4000) fprintf(stdout, "- Non conforming program\n");
    if (flags & 0x2000) fprintf(stdout, "- Errors in image\n");
    if (flags & 0x0800) fprintf(stdout, "- Bound as family app\n");
    if ((flags & 0x0700) == 0x0300) fprintf(stdout, "- Uses Presentation Manager Windowing API\n");
    if ((flags & 0x0700) == 0x0200) fprintf(stdout, "- Compatible with Presentation Manager Windowing\n");
    if ((flags & 0x0700) == 0x0100) fprintf(stdout, "- Not compatible with Presentation Manager Windowing\n");
    if ((flags & 0x0700) == 0x0000) fprintf(stdout, "- No subsystem\n");
    if (flags & 0x0080) fprintf(stdout, "- Floating point instructions\n");
    if (flags & 0x0040) fprintf(stdout, "- 386 Instructions\n");
    if (flags & 0x0020) fprintf(stdout, "- 286 Instructions\n");
    if (flags & 0x0010) fprintf(stdout, "- 8086 Instructions\n");
    if (flags & 0x0008) fprintf(stdout, "- Runs in Protected mode only\n");
    if (flags & 0x0004) fprintf(stdout, "- Global initialization\n");
    if ((flags & 0x0003) == 0x0) fprintf(stdout, "- No DGROUP\n");
    if ((flags & 0x0003) == 0x1) fprintf(stdout, "- Single DGROUP\n");
    if ((flags & 0x0003) == 0x2) fprintf(stdout, "- Multiple DGROUPs\n");
    if ((flags & 0x0003) == 0x3) fprintf(stdout, "- Unknown DGROUP type 3\n");
}

void ne_print_header_taget_os(uint8_t target_os) {
    fprintf(stdout, "Target OS: 0x%02x\n", target_os);
    switch (target_os) {
        case 0x01:
            fprintf(stdout, "- OS/2\n");
            break;
        case 0x02:
            fprintf(stdout, "- Windows (16 bit)\n");
            break;
        case 0x03:
            fprintf(stdout, "- European DOS 4.x\n");
            break;
        case 0x04:
            fprintf(stdout, "- Windows 386 (32-bit)\n");
            break;
        case 0x05:
            fprintf(stdout, "- Borland Operating System Services, HX DPMI-16\n");
            break;
        case 0x06:
            fprintf(stdout, "- HX DPMI-32\n");
            break;
        case 0x81:
            fprintf(stdout, "- Phar lap 286 DOS Extender emulating OS/2\n");
            break;
        case 0x82:
            fprintf(stdout, "- Phar lap 386 DOS Extender emulating Windows\n");
            break;
        default:
            fprintf(stdout, "- Unknown\n");
            break;
   }
}

void ne_print_header_os2_flags(uint8_t os2_exe_flags) {
    fprintf(stdout, "OS/2 EXE flags: 0x%02x\n", os2_exe_flags);
    if (os2_exe_flags & 0x0001) fprintf(stdout, "- Long filename support\n");
    if (os2_exe_flags & 0x0002) fprintf(stdout, "- 2.x protected mode\n");
    if (os2_exe_flags & 0x0004) fprintf(stdout, "- 2.x proportional fonts\n");
    if (os2_exe_flags & 0x0008) fprintf(stdout, "- Fast-load area\n");
    if (os2_exe_flags & 0xfff0) fprintf(stdout, "- Unknown flags\n");
}

void ne_print_header(const struct ne_header *header) {
    fprintf(stdout, "\n");
    fprintf(stdout, "Linker version: %d.%d\n", header->version, header->revision);
    fprintf(stdout, "Entry table offset: 0x%08x (%u)\n", header->entry_table_offset, header->entry_table_offset);
    fprintf(stdout, "Entry table size: 0x%08x bytes (%u)\n", header->number_of_bytes_in_entry_table, header->number_of_bytes_in_entry_table);
    fprintf(stdout, "CRC: 0x%08x (%u)\n", header->crc, header->crc);
    ne_print_header_flags(header->flags);
    fprintf(stdout, "Automatic data segment offset: 0x%08x (%u)\n", header->automatic_data_segment_index, header->automatic_data_segment_index);
    fprintf(stdout, "Initial heap size: 0x%08x bytes (%u)\n", header->initial_heap_size, header->initial_heap_size);
    fprintf(stdout, "Initial stack size: 0x%08x bytes (%u)\n", header->initial_stack_size, header->initial_stack_size);
    fprintf(stdout, "Entry point: 0x%08x (%u)\n", header->entry_point, header->entry_point);
    fprintf(stdout, "Initial stack pointer: 0x%08x (%u)\n", header->initial_stack_pointer, header->initial_stack_pointer);
    fprintf(stdout, "Number of segments: 0x%04x (%u)\n", header->number_of_segments, header->number_of_segments);
    fprintf(stdout, "Number of module references: 0x%04x (%u)\n", header->number_of_module_references, header->number_of_module_references);
    fprintf(stdout, "Non-resident names table size: 0x%08x bytes (%u)\n", header->non_resident_names_table_size, header->non_resident_names_table_size);
    fprintf(stdout, "Segment table offset: 0x%04x (%u)\n", header->segment_table_offset, header->segment_table_offset);
    fprintf(stdout, "Resource table offset: 0x%04x (%u)\n", header->resource_table_offset, header->resource_table_offset);
    fprintf(stdout, "Resident names table offset: 0x%04x (%u)\n", header->resident_name_table_offset, header->resident_name_table_offset);
    fprintf(stdout, "Module referece table offset: 0x%04x (%u)\n", header->module_reference_table_offset, header->module_reference_table_offset);
    fprintf(stdout, "Import name table offset: 0x%04x (%u)\n", header->import_name_table_offset, header->import_name_table_offset);
    fprintf(stdout, "Non-resident names table offset: 0x%08x (%u)\n", header->non_resident_names_table_offset, header->non_resident_names_table_offset);
    fprintf(stdout, "File alignment size shift count: 0x%04x bytes (%u)\n", header->file_alignment_size_shift_count, header->file_alignment_size_shift_count);
    fprintf(stdout, "Number of resident names table entries: 0x%04x (%u)\n", header->number_of_resident_name_table_entries, header->number_of_resident_name_table_entries);
    ne_print_header_taget_os(header->target_os);
    ne_print_header_os2_flags(header->os2_exe_flags);
    fprintf(stdout, "Return thunk offset: 0x%04x (%u)\n", header->return_thunk_offset, header->return_thunk_offset);
    fprintf(stdout, "Segment reference thunk offset: 0x%04x (%u)\n", header->segment_reference_thunk_offset, header->segment_reference_thunk_offset);
    fprintf(stdout, "Minimum code swap area size: 0x%04x bytes (%u)\n", header->minimum_code_swap_area_size, header->minimum_code_swap_area_size);
    fprintf(stdout, "Minimum expected windows version: %d.%d (0x%04x)\n", (header->expected_minimum_windows_version >> 8), (header->expected_minimum_windows_version & 0xFF), header->expected_minimum_windows_version);
}

void ne_print_exports(const struct ne_header *header) {

}

void ne_print_imports(const struct ne_header *header) {

}

void dumpne() {
    struct ne ne;

    ne.mz_header = readdata(0);
    ne.ne_header = readdata(ne.mz_header->new_header_offset);

    ne_print_header(ne.ne_header);
    //ne_print_exports(ne.ne_header);
    //ne_print_imports(ne.ne_header);
}