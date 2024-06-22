#ifndef LE_H_INCLUDED
#define LE_H_INCLUDED

#include <stdint.h>
#include "mz.h"

#pragma pack(1)

struct le_vxd {
    uint8_t reserved[8];
    uint32_t windows_vxd_version_info_resource_offset;
    uint32_t windows_vxd_version_info_resource_length;
    uint16_t device_id;
    uint16_t ddk_version;
};

struct le_header {
    uint16_t magic;
    uint8_t byte_order;
    uint8_t word_order;
    uint32_t exe_format_level;
    uint16_t cpu_type;
    uint16_t os_type;
    uint32_t exe_version;
    uint32_t exe_flags;
    uint32_t number_of_pages;
    uint32_t starting_object_number;
    uint32_t eip;
    uint32_t stack_object_number_for_pointer;
    uint32_t esp;
    uint32_t exe_page_size;
    union {
        uint32_t size_of_last_page; // LE
        uint32_t page_shift_offset; // LX
    } page;
    uint32_t fixup_section_size;
    uint32_t fixup_section_checksum;
    uint32_t loader_section_size;
    uint32_t loader_section_checksum;
    uint32_t object_table_offset;
    uint32_t number_of_objects;
    uint32_t object_map_offset;
    uint32_t iterated_data_map_offset;
    uint32_t resource_table_offset;
    uint32_t number_of_resource_entries;
    uint32_t resident_names_table_offset;
    uint32_t entry_table_offset;
    uint32_t module_directives_table_offset;
    uint32_t number_of_module_directives;
    uint32_t fixup_page_table_offset;
    uint32_t fixup_record_table_offset;
    uint32_t import_name_table_offset;
    uint32_t number_of_import_table_entries;
    uint32_t import_procedure_name_table_offset;
    uint32_t per_page_checksum_table_offset;
    uint32_t enumerated_data_pages_offset;
    uint32_t number_of_enumerated_data_pages;
    uint32_t non_resident_name_table_offset;
    uint32_t non_resident_name_table_size;
    uint32_t non_resident_name_table_checksum;
    uint32_t autodata_segment_object_number;
    uint32_t debug_offset;
    uint32_t debug_size;
    uint32_t heap_size;
    uint32_t stack_size;
    union reserved {
        uint8_t reserved[20];
        struct le_vxd vxd;
    } reserved;
};

struct le {
    const struct mz_header *mz_header;

    const struct le_header *le_header;
};

#pragma pack()

#endif