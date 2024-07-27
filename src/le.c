#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
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
    fprintf(stdout, "Number of import table entries: %d (0x%08x)\n", header->number_of_import_table_entries, header->number_of_import_table_entries);
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

void le_print_object_flags(uint32_t flags, uint16_t magic) {
    fprintf(stdout, "- Flags: 0x%08x\n", flags);
    if (flags & 0x0001) fprintf(stdout, "\t- Readable object\n");
    if (flags & 0x0002) fprintf(stdout, "\t- Writeable object\n");
    if (flags & 0x0004) fprintf(stdout, "\t- Executeable object\n");
    if (flags & 0x0008) fprintf(stdout, "\t- Resource object\n");
    else fprintf(stdout, "\t- Non-resource object\n");
    if (flags & 0x0010) fprintf(stdout, "\t- Discardable object\n");
    else fprintf(stdout, "\t- Non-discardable object\n");
    if (flags & 0x0020) fprintf(stdout, "\t- Sharable object\n");
    else fprintf(stdout, "\t- Non-sharable object\n");
    if (flags & 0x0040) fprintf(stdout, "\t- Object has preload pages\n");
    if (flags & 0x0080) fprintf(stdout, "\t- Object has invalid pages\n");
    else fprintf(stdout, "\t- Object has valid pages\n");
    if (flags & 0x0100) {
        if (magic == 0x454C) fprintf(stdout, "\t- Object is permanent and swapable\n"); // LE
        else if (magic == 0x584C) fprintf(stdout, "\t- Object has zero fill\n"); // LX
    }
    else fprintf(stdout, "\t- Object is non-swapable\n");
    if (flags & 0x0200) fprintf(stdout, "\t- Object is permanent and resident\n");
    if ((flags & 0x0300) & (magic == 0x584C)) fprintf(stdout, "\t - Object is permanent and contiguous\n");
    if (flags & 0x0400) fprintf(stdout, "\t- Object is permanent and lockable\n");
    if (flags & 0x1000) fprintf(stdout, "\t- 16:16 alias required\n");
    if ((flags & 0x2000)) fprintf(stdout, "\t- Big setting\n");
    else fprintf(stdout, "\t- Default setting\n");
    if (flags & 0x4000) fprintf(stdout, "\t- Conforming for code\n");
    else fprintf(stdout, "\t- Non-conforming code\n");
    if (flags & 0x3000) {
        if (flags & 0x8000) fprintf(stdout, "\t- Object IO privilege level: 1\n");
        else fprintf(stdout, "\t- Object IO privilege level: 0\n"); 
    }
}

void le_print_exports(struct le le) {
    size_t offset;
    uint8_t buffer[256];
    uint16_t ordinal;

    if (le.le_header->resident_names_table_offset > 0) {
        fprintf(stdout, "\n");
        fprintf(stdout, "Resident exports:\n");
        fprintf(stdout, "Ordinal\t\tName\n");
        offset = le.le_header->resident_names_table_offset + le.mz_header->new_header_offset;
        while (readbyte(offset) > 0) {
            memset(buffer, 0, 256);
            int len = readbyte(offset);
            offset++;
            for (int i = 0; i < len; i++) {
                buffer[i] = readbyte(offset);
                offset++;
            }
            ordinal = readword(offset);
            offset = offset + 2;
            fprintf(stdout, "%u\t\t%s\n", ordinal, buffer);
        }
    }
    if (le.le_header->non_resident_name_table_offset > 0) {
        fprintf(stdout, "\n");
        fprintf(stdout, "Non-resident exports:\n");
        fprintf(stdout, "Ordinal\t\tName\n");
        offset = le.le_header->non_resident_name_table_offset;
        while (readbyte(offset) > 0) {
            memset(buffer, 0, 256);
            int len = readbyte(offset);
            offset++;
            for (int i = 0; i < len; i++) {
                buffer[i] = readbyte(offset);
                offset++;
            }
            ordinal = readword(offset);
            offset = offset + 2;
            fprintf(stdout, "%u\t\t%s\n", ordinal, buffer);
        }
    }
}

void le_print_imports(struct le le) {
    size_t offset;
    uint8_t buffer[256];
    uint16_t ordinal;

    if (le.le_header->number_of_import_table_entries > 0) {
        fprintf(stdout, "\n");
        fprintf(stdout, "Import table:\n");
        offset = le.le_header->import_name_table_offset + le.mz_header->new_header_offset;
        for (int i = 0; i < le.le_header->number_of_import_table_entries; i++) {
            uint8_t size = readbyte(offset);
            memset(buffer, 0, 256);
            offset++;
            for (int j = 0; j < size; j++) {
                buffer[j] = readbyte(offset);
                offset++;
            }

            fprintf(stdout, "%s\n", buffer);
        }
    }
    else fprintf(stdout, "\nNo import table.\n");

    if (le.le_header->import_procedure_name_table_offset > 0) {
        fprintf(stdout, "\n");
        fprintf(stdout, "Import procedure table:\n");
        offset = le.le_header->import_procedure_name_table_offset + le.mz_header->new_header_offset;
        if (readbyte(offset) == 0) {
            offset++;
            while (readbyte(offset) > 0)
            {
                uint8_t len = readbyte(offset);
                offset++;
                for (int i = 0; i < len; i++) {
                    fprintf(stdout, "%c",readbyte(offset));
                    offset++;
                }
                fprintf(stdout, "\n");
            }
        }
        else if (readbyte(offset) < 0x20 && readbyte(offset) != 0) {
            while (readbyte(offset) > 0) {
                uint8_t len = readbyte(offset);
                offset++;
                for (int i = 0; i < len; i++) {
                    fprintf(stdout, "%c",readbyte(offset));
                    offset++;
                }
                fprintf(stdout, "\n");
            }
        }
        else {
            while (readbyte(offset) > 0) {
                fprintf(stdout, "%c", readbyte(offset));
                offset++;
            }
            fprintf(stdout, "\n");
        }
    }
    else fprintf(stdout, "\nNo import procedure table.\n");
}

void le_print_page_flags(uint16_t flags) {
    fprintf(stdout, "- Page flags: 0x%04x\n", flags);
    switch (flags) {
        case 0x0000:
            fprintf(stdout, "\t- Legal Physical Page in the module (Offset from Preload Page Section)\n");
            break;
        case 0x0001:
            fprintf(stdout,"\t- Iterated Data Page (Offset from Iterated Data Pages Section)\n");
            break;
        case 0x0002:
            fprintf(stdout,"\t- Invalid Page (zero)\n");
            break;
        case 0x0003:
            fprintf(stdout,"\t- Zero Filled Page (zero)\n");
            break;
        case 0x0004:
            fprintf(stdout,"\t- Unused\n");
            break;
        case 0x0005:
            fprintf(stdout,"\t- Compressed Page (Offset from Preload Pages Section)\n");
            break;
    }
}

void le_print_objects_map(struct le le) {
    size_t offset;

    if (le.le_header->object_table_offset > 0) {
        fprintf(stdout, "\n");
        fprintf(stdout, "Objects:\n");
        offset = le.le_header->object_table_offset + le.mz_header->new_header_offset;
        for (int i = 0; i < le.le_header->number_of_objects; i++) {
            uint32_t size;
            uint32_t relocation_base_address;
            uint32_t flags;
            uint32_t page_table_index;
            uint32_t page_table_entries;
            uint32_t reserved;

            size = readdword(offset);
            offset = offset + 4;
            relocation_base_address = readdword(offset);
            relocation_base_address = le.mz_header->new_header_offset +  le.le_header->object_table_offset + (i * relocation_base_address);
            offset = offset + 4;
            flags = readdword(offset);
            offset = offset + 4;
            page_table_index = readdword(offset);
            offset = offset + 4;
            page_table_entries = readdword(offset);
            offset = offset + 4;
            reserved = readdword(offset);
            offset = offset + 4;
            fprintf(stdout, "Object 0x%04x (%u)\n", i, i);
            fprintf(stdout, "- Virtual size: 0x%08x (%u bytes)\n", size, size);
            fprintf(stdout, "- Relocation base address: 0x%08x (%u)\n", relocation_base_address, relocation_base_address);
            fprintf(stdout, "- Page map index: 0x%08x (%u)\n", page_table_index, page_table_index);
            fprintf(stdout, "- Page map entries count 0x%08x (%u)\n", page_table_entries, page_table_entries);
            if (reserved > 0) fprintf(stdout, "- Name: %c%c%c%c\n", reserved >> 24 & 0xFF, reserved >> 16 & 0xFF, reserved >> 8 & 0xFF, reserved & 0xFF);
            le_print_object_flags(flags, le.le_header->magic);
        }
    }
}
void le_print_object_page_tables(struct le le) {
    size_t offset;
    
    if (le.le_header->object_map_offset > 0) {
        uint32_t page_data_offset;
        uint16_t page_data_size;
        uint16_t page_data_flags;
        uint32_t page_exe_offset;

        fprintf(stdout, "\nBROKEN FIXME!!!!!\n");
        fprintf(stdout, "Object page table:\n");
        for (int i = 0; i < le.le_header->number_of_pages; i++) {
            offset = le.mz_header->new_header_offset + le.le_header->object_table_offset + i * le.le_header->exe_page_size;
            if (le.le_header->magic == 0x454C) { // LE

                page_data_offset = ((readbyte(offset) << 16) | (readbyte(offset + 1) << 8) | readbyte(offset + 2));
                offset = offset + 4;
                if (i + 1 == le.le_header->number_of_pages) {
                    page_data_size = le.le_header->page.size_of_last_page;
                }
                else {
                    page_data_size = le.le_header->exe_page_size;
                }
                page_data_flags = readbyte(offset);
                offset++;
            }
            else { // LX
                page_data_offset = readdword(offset) << le.le_header->page.page_shift_offset;
                offset = offset + 4;
                page_data_size = readword(offset);
                offset = offset + 2;
                page_data_flags = readword(offset);
                offset = offset + 2;
            }
            //page_exe_offset = le.le_header->object_table_offset + le.le_header->object_table_offset + (i * 0x18);
            fprintf(stdout, "Page 0x%08x (%u)\n", i + 1, i + 1);
            fprintf(stdout, "- Page Offset: 0x%08x (%u)\n", page_data_offset, page_data_offset);
            //fprintf(stdout, "- .EXE offset: 0x%08x (%u)\n", page_exe_offset, page_exe_offset);
            fprintf(stdout, "- Page size: 0x%04x (%u bytes)\n", page_data_size, page_data_size);
            le_print_page_flags(page_data_flags);
            //print_hex_dump(page_data_offset, page_data_size, page_data_offset);
        }
    }
}

void le_print_modules_directive_table(struct le le) {
    size_t offset;

    if (le.le_header->number_of_module_directives > 0) {
        fprintf(stdout, "\n");
        fprintf(stdout, "Module directory table:\n");
        fprintf(stdout, "Directive\t\tOffset\t\tLength\n");
        offset = le.le_header->module_directives_table_offset;
        for (int i = 0; i < le.le_header->number_of_module_directives; i++) {
            uint16_t directive_number;
            uint16_t directive_lenght;
            uint32_t directive_offset;
            directive_number = readword(offset);
            offset = offset + 2;
            directive_lenght = readword(offset);
            offset = offset + 2;
            directive_offset = readdword(offset);
            offset = offset + 4;
            fprintf(stdout, "%u\t\t0x%08x\t\t0x%04x (%u bytes)\n", directive_number, directive_offset, directive_lenght, directive_lenght);
        }
    }
    else {
        fprintf(stdout, "\n");
        fprintf(stdout, "No module directive tables.\n");
    }
}

void le_print_fixup_page_table(struct le le) {
    size_t offset;
    fprintf(stdout, "\n");
    if (le.le_header->magic == 0x454C && le.le_header->reserved.vxd.windows_vxd_version_info_resource_length > 0) {
        offset = le.le_header->reserved.vxd.windows_vxd_version_info_resource_offset;
        fprintf(stdout, "DDB Info:\n");
        print_hex_dump(offset, le.le_header->reserved.vxd.windows_vxd_version_info_resource_length, offset);
    }
    else {
        fprintf(stdout, "No DDB.\n");
    }
}

void le_print_entry_table(struct le le) {
    size_t offset;
    uint16_t ordinal;

    fprintf(stdout, "\n");
    if (le.le_header->entry_table_offset > 0) {
        fprintf(stdout, "Entry table:\n");
        fprintf(stdout, "Type:\t\t\tTarget:\n");
        offset = le.le_header->entry_table_offset + le.mz_header->new_header_offset;
        ordinal = 1;
        while (readbyte(offset) > 0) {
            uint8_t count;
            uint8_t type;
            uint16_t obj_number;
            uint8_t flags;
            uint32_t obj_offset;
            uint16_t callgate;

            count = readbyte(offset);
            offset++;
            type = readbyte(offset);
            offset++;
            obj_number = readword(offset);
            offset = offset + 2;

            switch (type) {
                case 0x00: // Unused
                    break;
                case 0x01: // 16bit entry
                    fprintf(stdout, "16bit entry\t\tObject, %u ", obj_number);
                    break;
                case 0x02: // 286 gate entry call
                    fprintf(stdout, "286 gate entry call\t\tObject, %u ", obj_number);
                    break;
                case 0x03: // 32bit entry
                    fprintf(stdout, "32bit entry\t\tObject %u, ", obj_number);
                    break;
                case 0x04: // Forwarder entry
                    fprintf(stdout, "Forwarder\n");
                    break;
                case 0x80: // Parameter typing info present
                    fprintf(stdout, "16bit entry\t\tObject %u, ", obj_number);
                    break;
                default:
                    fprintf(stdout, "Unknown\n");
                    break;
            }
            for (int i = 0; count > 0; count--) {
                if (i != 0) fprintf(stdout, "\n");
                switch (type) {
                case 0x00: // Unused
                    break;
                case 0x01: // 16bit entry
                    flags = readbyte(offset);
                    offset++;
                    obj_offset = readdword(offset);
                    offset = offset + 4;
                    fprintf(stdout, "Ordinal: %u, Flags: 0x%02x, Offset: 0x%08x", ordinal, flags, obj_offset);
                    break;
                case 0x02: // 286 gate entry call
                    flags = readbyte(offset);
                    offset++;
                    obj_offset = readdword(offset);
                    offset = offset + 4;
                    callgate = readword(offset);
                    offset + 2;
                    fprintf(stdout, "Ordinal: %u, Flags: 0x%02x, Callgate: 0x%04x, Offset 0x%08x", ordinal, flags, obj_offset);
                    break;
                case 0x03: // 32bit entry
                    flags = readbyte(offset);
                    offset++;
                    obj_offset = readdword(offset);
                    offset = offset + 4;
                    fprintf(stdout, "Ordinal: %u, Flags: 0x%02x, Offset: 0x%08x", ordinal, flags, obj_offset);
                    break;
                case 0x04: // Forwarder entry
                    flags = readbyte(offset);
                    offset++;
                    obj_offset = readdword(offset);
                    offset = offset + 4;
                    fprintf(stdout, "Ordinal: %u, Flags: 0x%02x, Offset: 0x%08x", ordinal, flags, obj_offset);
                    break;
                case 0x80: // Parameter typing info present
                    fprintf(stdout, "0x80 BROKEN!\n");
                    break;
                default:
                    fprintf(stdout, "Unknown\n");
                    break;
                }
                ordinal++;
                fprintf(stdout, "\n");
            }
        }

    }
    else {
        fprintf(stdout, "No entry table.\n");
    }
}

void le_print_ddb(struct le le) {

}

void dumple() {
    struct le le;

    //fprintf(stdout, "DEBUG: %u sizeof struct le_header\n", sizeof(struct le_header));

    le.mz_header = readdata(0);
    le.le_header = readdata(le.mz_header->new_header_offset);

    le_print_header(le.le_header);
    le_print_exports(le);
    le_print_imports(le);
    le_print_objects_map(le);
    le_print_object_page_tables(le);
    le_print_modules_directive_table(le);
    le_print_fixup_page_table(le);
    //le_print_fixup_record_table(le);
    le_print_entry_table(le);
    le_print_ddb(le);
};