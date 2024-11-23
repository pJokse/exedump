#ifndef NE_H_INCLUDED
#define NE_H_INCLUDED

#include <stdint.h>
#include "mz.h"

#pragma pack(1)

struct ne_header {
    uint16_t magic;
    uint8_t version;
    uint8_t revision;
    uint16_t entry_table_offset;
    uint16_t number_of_bytes_in_entry_table;
    uint32_t crc;
    uint16_t flags;
    uint16_t automatic_data_segment_index;
    uint16_t initial_heap_size;
    uint16_t initial_stack_size;
    uint32_t entry_point;
    uint32_t initial_stack_pointer;
    uint16_t number_of_segments;
    uint16_t number_of_module_references;
    uint16_t non_resident_names_table_size;
    uint16_t segment_table_offset;
    uint16_t resource_table_offset;
    uint16_t resident_name_table_offset;
    uint16_t module_reference_table_offset;
    uint16_t import_name_table_offset;
    uint32_t non_resident_names_table_offset;
    uint16_t movable_entry_count;
    uint16_t file_alignment_size_shift_count;
    uint16_t number_of_resident_name_table_entries;
    uint8_t target_os;
    uint8_t os2_exe_flags;
    uint16_t return_thunk_offset;
    uint16_t segment_reference_thunk_offset;
    uint16_t minimum_code_swap_area_size;
    uint16_t expected_minimum_windows_version;
};

struct ne {
    const struct mz_header *mz_header;

    const struct ne_header *ne_header;
    
};

#pragma pack()

#endif