#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "objomf.h"
#include "hexdump.h"

void dumpomf() {
    size_t offset;
    size_t record_offset;
    struct omf_record_header *omf_record_header;
    size_t records_count;
    uint8_t string_length;
    uint8_t is32bit;
    uint8_t omf_comment_type;
    uint8_t omf_comment_class;
    uint8_t base_group;
    uint8_t base_segment;
    uint16_t base_frame;
    uint16_t group_name_index;
    uint8_t segment_attrib;
    uint16_t segment_index;

    offset = 0;
    records_count = 0;
    is32bit = 0;
    
    fprintf(stdout, "\n");

    while (offset < file_size) {
        omf_record_header = readdata(offset);
        if (omf_record_header->record_size > 1024) {
            fprintf(stdout, "Wrong size record (size %d > 1024 bytes) 0x%02x TYPE\n", omf_record_header->record_size ,omf_record_header->record_type);
            return;
        }
        offset = offset + sizeof(struct omf_record_header);
        record_offset = offset;
        is32bit = 0;
        switch (omf_record_header->record_type) {
            case OMF_THEADR:
                fprintf(stdout, "THEADR: ");
                string_length = readbyte(record_offset);
                record_offset++;
                for (int i = 0; i < string_length; i++) {
                    fprintf(stdout, "%c", readbyte(record_offset + i));
                }
                fprintf(stdout, "\n");
                break;
            case OMF_COMENT:
                fprintf(stdout, "COMENT:\n");
                omf_comment_type = readbyte(record_offset);
                record_offset++;
                omf_comment_class = readbyte(record_offset);
                record_offset++;
                switch (omf_comment_type) {
                    case OMF_COMENT_TYPE_NO_LIST:
                        fprintf(stdout, "- Don't list\n");
                        break;
                    case OMF_COMENT_TYPE_NO_PURGE:
                        fprintf(stdout, "- Don't purge\n");
                        break;
                    case 0x00:
                        break;
                    default:
                        fprintf(stdout, "- Unknown COMENT Type 0x%02x\n", omf_comment_type);
                        break;
                }
                switch (omf_comment_class) {
                    /*case OMF_COMENT_CLASS_TRANSLATOR:
                        break;
                    case OMF_COMENT_CLASS_DEPENDENCY_FILE:
                        break;
                    case OMF_COMENT_CLASS_LINK_PASS_SEPERATOR:
                        break;*/
                    case OMF_COMENT_CLASS_MEMORY_MODEL:
                        fprintf(stdout, "- Memory model: ");
                        for (int i = 0; i < omf_record_header->record_size - 3; i++) {
                            // fprintf(stdout, "%c", readbyte(record_offset + i));
                            switch (readbyte(record_offset + i)) {
                                case 'O':
                                     fprintf(stdout, "Optimized");
                                    break;
                                case '0':
                                    fprintf(stdout, "8086");
                                    break;
                                case '1':
                                    fprintf(stdout, "80186");
                                    break;
                                case '2':
                                    fprintf(stdout, "80286");
                                    break;
                                case '3':
                                    fprintf(stdout, "80386");
                                    break;
                                case 'A':
                                    fprintf(stdout, "68000");
                                    break;
                                case 'B':
                                    fprintf(stdout, "68010");
                                    break;
                                case 'C':
                                    fprintf(stdout, "68020");
                                    break;
                                case 'D':
                                    fprintf(stdout, "68030");
                                    break;
                                case 's':
                                    fprintf(stdout, "small");
                                    break;
                                case 'm':
                                    fprintf(stdout, "medium");
                                    break;
                                case 'c':
                                    fprintf(stdout, "compact");
                                    break;
                                case 'l':
                                    fprintf(stdout, "large");
                                    break;
                                case 'h':
                                    fprintf(stdout, "huge");
                                    break;
                            default:
                                fprintf(stdout, "Unknown: %c, ", readbyte(record_offset + i));
                                break;
                            }
                            if (i < omf_record_header->record_size - 4) fprintf(stdout, ", "); // Don't write out ', ' after last item
                        }
                        fprintf(stdout, "\n");
                    case OMF_COMENT_CLASS_OMF_EXT:
                        fprintf(stdout, "- Has OMF extensions\n");
                        break;
                    case OMF_COMENT_CLASS_NEW_OMF_EXT:
                        fprintf(stdout, "- Contains new OMF extensions\n");
                        break;
                    case OMF_COMENT_CLASS_LIBMOD:
                        string_length = readbyte(record_offset);
                        record_offset++;
                        fprintf(stdout, "- Library module: ");
                        for (int i = 0; i < string_length; i++) {
                            fprintf(stdout, "%c", readbyte(record_offset + i));
                        }
                        fprintf(stdout, "\n");
                        break;
                    case OMF_COMENT_CLASS_BORLAND_DEPENDENCY_FILES:
                        record_offset = record_offset + 5;
                        fprintf(stdout, "- Dependency: ");
                        for (int i = 0; i < omf_record_header->record_size - 8; i++) {
                            fprintf(stdout, "%c", readbyte(record_offset + i));
                        }
                        fprintf(stdout, "\n");
                        break;
                    default:
                        fprintf(stdout, "- Unknown COMENT Class 0x%02x\n", omf_comment_class);
                        break;
                }
                break;
            case OMF_MODEND32:
                fprintf(stdout, "MODEND32: TBD");
                fprintf(stdout, "\n");
                is32bit = 0xFF;
                break;
            case OMF_MODEND16:
                uint8_t module_type;
                module_type = readbyte(record_offset);
                record_offset++;
                is32bit ? fprintf(stdout, "MODEND32: ") : fprintf(stdout, "MODEND16: ");
                if ((module_type & 0xC0) == 0x80) fprintf(stdout, "Main module, ");
                fprintf(stdout, "\n");
                break;
            case OMF_TYPDEF:
                fprintf(stdout, "TYPDEF: TBD");
                fprintf(stdout, "\n");
                break;
            case OMF_PUBDEF32:
                fprintf(stdout, "PUBDEF32: TBD");
                fprintf(stdout, "\n");
                is32bit = 0xFF;
                break;
            case OMF_PUBDEF16:
                base_group = readbyte(record_offset);
                record_offset++;
                base_segment = readbyte(record_offset);
                record_offset++;
                if (base_segment == 0x00) {
                    base_frame = readword(record_offset);
                    record_offset = record_offset + 2;
                }
                is32bit ? fprintf(stdout, "PUBDEF32:\n") : fprintf(stdout, "PUBDEF16:\n");
                while (record_offset < offset + omf_record_header->record_size - 1) {
                    fprintf(stdout, "- ");
                    string_length = readbyte(record_offset);
                    record_offset++;
                    for (int i = 0; i < string_length; i++) {
                        fprintf(stdout, "%c", readbyte(record_offset));
                        record_offset++;
                    }
                    fprintf(stdout, ", Public offset: 0x%04x", readword(record_offset));
                    record_offset = record_offset + 2;
                    fprintf(stdout, ", Type index: 0x%02x", readbyte(record_offset));
                    record_offset++;
                    fprintf(stdout, "\n");
                }
                break;
            case OMF_EXTDEF:
                fprintf(stdout, "EXTDEF:\n");
                while (record_offset < offset + omf_record_header->record_size - 1) {
                    fprintf(stdout, "- ");
                    string_length = readbyte(record_offset);
                    record_offset++;
                    for (int i = 0; i < string_length; i++) {
                        fprintf(stdout, "%c", readbyte(record_offset));
                        record_offset++;
                    }
                    if (omf_record_header->record_size < string_length + 6) {
                        fprintf(stdout, "Type 0x%04x", readword(record_offset));
                        record_offset = record_offset + 2;
                    }
                    else {
                        fprintf(stdout, "Type 0x%02x", readbyte(record_offset));
                        record_offset++;
                    }
                    fprintf(stdout, "\n");
                }
                break;
            case OMF_LNAMES:
                fprintf(stdout, "LNAMES: ");
                while (record_offset < offset + omf_record_header->record_size - 1) {
                    string_length = readbyte(record_offset);
                    record_offset++;
                    for (int i = 0; i < string_length; i++) {
                        fprintf(stdout, "%c", readbyte(record_offset));
                        record_offset++;
                    }
                    if ((record_offset < offset + omf_record_header->record_size - 1)) fprintf(stdout, ",");
                }
                fprintf(stdout, "\n");
                break;
            case OMF_SEGDEF32:
                is32bit = 0xFF;
            case OMF_SEGDEF16:
                segment_attrib = readbyte(record_offset);
                record_offset++;
                is32bit ? fprintf(stdout, "SEGDEF32: ") : fprintf(stdout, "SEGDEF16: ");
                fprintf(stdout, "\n");
                fprintf(stdout, "- Attributes: ");
                if ((segment_attrib & 0xE0) == 0x00) fprintf(stdout, "Absolute segment, ");
                if ((segment_attrib & 0xE0) == 0x20) fprintf(stdout, "Relocateable (byte aligned), ");
                if ((segment_attrib & 0xE0) == 0x40) fprintf(stdout, "Relocateable (word/2 byte,16 bit aligned), ");
                if ((segment_attrib & 0xE0) == 0x60) fprintf(stdout, "Relocateable (16 byte aligned), ");
                if ((segment_attrib & 0xE0) == 0x80) fprintf(stdout, "Relocateable (page boundry aligned), ");
                if ((segment_attrib & 0xE0) == 0xA0) fprintf(stdout, "Relocateable (dword/4 byte/32 bit aligned), ");
                if ((segment_attrib & 0xE0) == 0xC0) fprintf(stdout, "Not supported, ");
                if ((segment_attrib & 0xE0) == 0xE0) fprintf(stdout, "Unknown 0x%02x, ", segment_attrib);
                if ((segment_attrib & 0x1C) == 0x00) fprintf(stdout, "Private, ");
                if ((segment_attrib & 0x1C) == 0x04) fprintf(stdout, "Reserved, ");
                if ((segment_attrib & 0x1C) == 0x08) fprintf(stdout, "Public, ");
                if ((segment_attrib & 0x1C) == 0x0C) fprintf(stdout, "Reserved, ");
                if ((segment_attrib & 0x1C) == 0x10) fprintf(stdout, "Public, ");
                if ((segment_attrib & 0x1C) == 0x14) fprintf(stdout, "Stack, ");
                if ((segment_attrib & 0x1C) == 0x18) fprintf(stdout, "Common, ");
                if ((segment_attrib & 0x1C) == 0x1C) fprintf(stdout, "Public, ");
                if ((segment_attrib & 0x02) == 0x02) fprintf(stdout, "Big, ");
                if ((segment_attrib & 0x01) == 0x00) fprintf(stdout, "Use16, ");
                if ((segment_attrib & 0x01) == 0x01) fprintf(stdout, "Use32, ");
                fprintf(stdout, "\n");
                break;
            case OMF_GRPDEF:
                uint8_t segment_type;
                fprintf(stdout, "GRPDEF: ");
                if (readbyte(record_offset) & 0x80) {
                    group_name_index = readindex(record_offset);
                    record_offset = record_offset + 2;
                }
                else {
                    group_name_index = readbyte(record_offset);
                    record_offset++;
                }
                fprintf(stdout, "Groupname index: 0x%04x\n", group_name_index);
                fprintf(stdout, "- ");
                while (record_offset < offset + omf_record_header->record_size - 1) {
                    segment_type = readbyte(record_offset);
                    record_offset++;
                    if (readbyte(record_offset) & 0x80) {
                        segment_index = readindex(record_offset);
                        record_offset = record_offset + 2;
                    }
                    else {
                        segment_index = readbyte(record_offset);
                        record_offset++;
                    }
                    if (segment_type != 0xFF) {
                        fprintf(stdout, "0x%02x type not supported, ", segment_type);
                    }
                    else {
                        fprintf(stdout, "0x%02x = 0x%04x, ", segment_type, segment_index);
                    }
                }
                fprintf(stdout, "\n");
                break;
            case OMF_FIXUP32:
                is32bit = 0xFF;
            case OMF_FIXUP16:
                is32bit ? fprintf(stdout, "FIXUP32:\n") : fprintf(stdout, "FIXUP16:\n");
                uint8_t fixup_record;
                while (record_offset < offset + omf_record_header->record_size - 1) {
                    fixup_record = readbyte(record_offset);
                    record_offset++;
                    if (fixup_record & 0x80) { // FIXUP subrecord
                        uint16_t fixup;
                        record_offset = record_offset - 1;
                        fixup = readword(record_offset);
                        record_offset = record_offset + 2;
                        fprintf(stdout, "- FIXUP subrecord\n");
                        (fixup_record & 0x40) ? fprintf(stdout, "- Segment relative fixup\n") : fprintf(stdout, "- Self-relative fixup\n");
                        fprintf(stdout, "- LOCATION type: ");
                        switch (fixup & 0x3C00) {
                            case 0x0000:
                                fprintf(stdout, "Low order byte (8 bit displacement or low byte of 16bit offset)\n");
                                break;
                            case 0x0400:
                                fprintf(stdout, "16bit offset\n");
                                break;
                            case 0x0800:
                                fprintf(stdout, "16bit base, logical segment base\n");
                                break;
                            case 0x0C00:
                                fprintf(stdout, "32bit long pointer (16bit base:16bit offset)\n");
                                break;
                            case 0x1000:
                                fprintf(stdout, "High order byte (high byte of 16bit offset)\n");
                                break;
                            case 0x1400:
                                fprintf(stdout, "16bit loader-resolved offset\n");
                                break;
                            case 0x1800:
                                fprintf(stdout, "Reserved\n");
                                break;
                            case 0x1C00:
                                fprintf(stdout, "Reserved\n");
                                break;
                            case 0x2000:
                                fprintf(stdout, "Reserved\n");
                                break;
                            case 0x2400:
                                fprintf(stdout, "32bit offset\n");
                                break;
                            case 0x2800:
                                fprintf(stdout, "Reserved\n");
                                break;
                            case 0x2C00:
                                fprintf(stdout, "48bit pointer (16bit base:32bit offset)\n");
                                break;
                            case 0x3000:
                                fprintf(stdout, "Reserved\n");
                                break;
                            case 0x3400:
                                fprintf(stdout, "32bit loader-resolved offset\n");
                                break;
                            default:
                                fprintf(stdout, "- Unknown LOCATION type 0x%04x\n", (fixup & 0x3C00));
                                break;
                        }
                        fprintf(stdout, "- LOCATION position: 0x%04x\n", (fixup & 0x03FF));
                        uint16_t frame_datum;
                        uint16_t target_datum;
                        uint32_t target_displacement;
                        uint8_t fix_data;
                        fix_data = readbyte(record_offset);
                        record_offset++;
                        if (is32bit) {
                            if ((fix_data & 0x80) || (fix_data & 0x70) < 0x40) {
                                 frame_datum = readword(record_offset);
                                 record_offset = record_offset + 2;
                            }
                            if ((fix_data & 0x04)) {
                                 target_displacement = readword(record_offset);
                                 record_offset = record_offset + 2;
                            }
                        }
                        else {
                            if ((fix_data & 0x80) || (fix_data & 0x70) < 0x40) {
                                 frame_datum = readbyte(record_offset);
                                 record_offset++;
                            }
                            if ((fix_data & 0x04)) {
                                 target_displacement = readbyte(record_offset);
                                 record_offset++;
                            }

                        }
                    }
                    else { // THREAD subrecord
                        fprintf(stdout, "- THREAD subrecord\n");
                        (fixup_record & 0x40) ? fprintf(stdout, "- FRAME thread\n") : fprintf(stdout, "- TARGET thread\n");
                        switch (fixup_record & 0x5C) {
                        case 0x00:
                            fprintf(stdout, "- The FRAME is specified by SEGDEF index\n");
                            break;
                        case 0x04:
                            fprintf(stdout, "- The FRAME is specified by GRPDEF index\n");
                            break;
                        case 0x08:
                            fprintf(stdout, "- The FRAME is specified by EXTDEF index\n");
                            break;
                        case 0x0C:
                            fprintf(stdout, "- Invalid (The FRAME is identified by an explicit frame number)\n");
                            break;
                        case 0x10:
                            fprintf(stdout, "- The FRAME is determined by the segment index of the previous LEDATA or LIDATA record\n");
                            break;
                        case 0x14:
                            fprintf(stdout, "- The FRAME is determined by the TARGET's segment, group, or external index\n");
                            break;
                        case 0x18:
                            fprintf(stdout, "- Invalid\n");
                            break;
                        case 0x40:
                            fprintf(stdout, "- Specified by SEGDEF index\n");
                            break;
                        case 0x44:
                            fprintf(stdout, "- Specified by GRPDEF index\n");
                            break;
                        case 0x48:
                            fprintf(stdout, "- Specified by EXTDEF index\n");
                            break;
                        case 0x4C:
                            fprintf(stdout, "- Specified by an explicit frame number\n");
                            break;
                        case 0x50:
                            fprintf(stdout, "- Specified by SEGDEF index only\n");
                            break;
                        case 0x54:
                            fprintf(stdout, "- Specified by GRPDEF index only\n");
                            break;
                        case 0x58:
                            fprintf(stdout, "- Specified by EXTDEF index only\n");
                            break;
                        }
                        fprintf(stdout, "- Thred: 0x%1x\n", (fixup_record & 0x03));
                        record_offset++;
                        if ((fixup_record & 0x1C) < 0x0C) {
                            if (is32bit) {
                                fprintf(stdout, "- Index: 0x%04x\n", readword(record_offset));
                                record_offset = record_offset + 2;
                            }
                            else {
                                fprintf(stdout, "- Index: 0x%02x\n", readbyte(record_offset));
                                record_offset++;
                            }
                        }
                    }
                }
                break;
            case OMF_LEDATA32:
                fprintf(stdout, "LEDATA32: TBD");
                fprintf(stdout, "\n");
                is32bit = 0xFF;
                break;
            case OMF_LEDATA16:
                uint16_t enumerated_data_offset;
                is32bit ? fprintf(stdout, "LEDATA32: ") : fprintf(stdout, "LEDATA16: ");
                segment_index = readbyte(record_offset);
                record_offset++;
                enumerated_data_offset = readword(record_offset);
                record_offset = record_offset + 2;
                fprintf(stdout, "Segment index: 0x%02x, Enumerated data offset: 0x%04x\n", segment_index, enumerated_data_offset);
                print_hex_dump(record_offset, omf_record_header->record_size - 3, record_offset);
                fprintf(stdout, "\n");
                break;
            case OMF_LIDATA32:
                fprintf(stdout, "LIDATA32: TBD");
                fprintf(stdout, "\n");
                is32bit = 0xFF;
                break;
            case OMF_LIDATA16:
                uint16_t iterated_data_offset;
                is32bit ?  fprintf(stdout, "LIDATA32: ") : fprintf(stdout, "LIDATA16: ");
                segment_index = readbyte(record_offset);
                record_offset++;
                iterated_data_offset = readword(record_offset);
                record_offset = record_offset + 2;
                fprintf(stdout, "Segment index: 0x%02x, Iteerated data offset: 0x%04x\n", segment_index, iterated_data_offset);
                print_hex_dump(record_offset, omf_record_header->record_size - 3, record_offset);
                fprintf(stdout, "\n");
                break;
            case OMF_COMDEF:
                uint8_t allocation;
                fprintf(stdout, "COMDEF:\n");
                while (record_offset < offset + omf_record_header->record_size - 1) {
                    fprintf(stdout, "- ");
                    string_length = readbyte(record_offset);
                    record_offset++;
                    for (int i = 0; i < string_length; i++) {
                        fprintf(stdout, "%c", readbyte(record_offset));
                        record_offset++;
                    }
                    if (readbyte(record_offset) == 0x00) record_offset++;
                    fprintf(stdout, " Data type: 0x%02x, ", readbyte(record_offset));
                    record_offset++;
                    fprintf(stdout, "Allocation: ");
                    allocation = readbyte(record_offset);
                    record_offset++;
                    if (allocation < 0x81) fprintf(stdout, "0x%02x", allocation);
                    if (allocation == 0x81) {
                        fprintf(stdout, "0x%04x", readword(record_offset));
                        record_offset = record_offset + 2;
                    }
                    if (allocation == 0x84) {
                        fprintf(stdout, "0x%06x", (readbyte(record_offset) << 24) + readword(record_offset + 1));
                        record_offset = record_offset + 3;
                    }
                    if (allocation == 0x88) {
                        fprintf(stdout, "0x%06x", readdword(record_offset));
                        record_offset = record_offset + 4;
                    }
                    fprintf(stdout, "\n");
                }
                break;
            case OMF_LEXTDEF32:
                fprintf(stdout, "LEXTDEF32: TBD");
                fprintf(stdout, "\n");
                is32bit = 0xFF;
                break;
            case OMF_LEXTDEF16:
                is32bit ? fprintf(stdout, "LEXTDEF32:\n") : fprintf(stdout, "LEXTDEF16:\n");
                while (record_offset < offset + omf_record_header->record_size - 1) {
                    string_length = readbyte(record_offset);
                    record_offset++;
                    fprintf(stdout, "- ");
                    for (int i = 0; i < string_length; i++) {
                        fprintf(stdout, "%c", readbyte(record_offset));
                        record_offset++;
                    }
                    fprintf(stdout, "Type 0x%02x", readbyte(record_offset));
                    record_offset++;
                    fprintf(stdout, "\n");
                }
                break;
            case OMF_LPUBDEF32:
                fprintf(stdout, "LPUBDEF32: TBD");
                fprintf(stdout, "\n");
                is32bit = 0xFF;
                break;
            case OMF_LPUBDEF16:
                is32bit ? fprintf(stdout, "LPUBDEF32:\n") : fprintf(stdout, "LPUBDEF16:\n");
                base_group = readbyte(record_offset);
                record_offset++;
                base_segment = readbyte(record_offset);
                record_offset++;
                if (base_segment == 0) {
                    base_frame = readword(record_offset);
                    record_offset = record_offset + 2;
                }
                while (record_offset < offset + omf_record_header->record_size - 1) {
                    string_length = readbyte(record_offset);
                    record_offset++;
                    fprintf(stdout, "- ");
                    for (int i = 0; i < string_length; i++) {
                        fprintf(stdout, "%c", readbyte(record_offset));
                        record_offset++;
                    }
                    fprintf(stdout, " Offset 0x%04x, ", readword(record_offset));
                    record_offset = record_offset + 2;
                    fprintf(stdout, "Type 0x%02x", readbyte(record_offset));
                    record_offset++;
                    fprintf(stdout, "\n");
                }
                break;
            case OMF_COMDAT32:
                fprintf(stdout, "COMDAT32: TBD");
                fprintf(stdout, "\n");
                is32bit = 0xFF;
                break;
            case OMF_COMDAT16:
                uint8_t flags;
                uint8_t attributes;
                is32bit ? fprintf(stdout, "COMDAT32: ") : fprintf(stdout, "COMDAT16: ");
                flags = readbyte(record_offset);
                record_offset++;
                fprintf(stdout, "\n");
                fprintf(stdout, "- Flags: ");
                if ((flags & 0x0F) == 0x01) fprintf(stdout, "Continuation, ");
                if ((flags & 0x0F) == 0x02) fprintf(stdout, "Iterated data, ");
                if ((flags & 0x0F) == 0x04) fprintf(stdout, "Local, ");
                if ((flags & 0x0F) == 0x08) fprintf(stdout, "Data in code segment, ");
                attributes = readbyte(record_offset);
                record_offset++;
                fprintf(stdout, "\n");
                fprintf(stdout, "- Attributes: ");
                if ((attributes & 0xF0) == 0x00) fprintf(stdout, "No match, ");
                if ((attributes & 0x30) == 0x10) fprintf(stdout, "Pick any, ");
                if ((attributes & 0x30) == 0x20) fprintf(stdout, "Same size, ");
                if ((attributes & 0x30) == 0x30) fprintf(stdout, "Exact match, ");
                if ((attributes & 0xF0) >= 0x40) fprintf(stdout, "Reserved, ");
                if ((attributes & 0x0F) == 0x00) fprintf(stdout, "Explicit, ");
                if ((attributes & 0x03) == 0x01) fprintf(stdout, "Far code, ");
                if ((attributes & 0x03) == 0x02) fprintf(stdout, "Far data, ");
                if ((attributes & 0x03) == 0x03) fprintf(stdout, "Code32, ");
                if ((attributes & 0x0F) == 0x04) fprintf(stdout, "Data32, ");
                if ((attributes & 0x0F) >= 0x04) fprintf(stdout, "Reserved, ");
                fprintf(stdout, "\n");
                break;
            case OMF_ALIAS:
                fprintf(stdout, "ALIAS: ");
                string_length = readbyte(record_offset);
                record_offset++;
                while (record_offset < offset + omf_record_header->record_size) {
                    for (int i = 0; i < string_length; i++) {
                        fprintf(stdout, "%c", readbyte(record_offset));
                        record_offset++;
                    }
                    string_length = readbyte(record_offset);
                    record_offset++;
                    fprintf(stdout, " -> ");
                    for (int i = 0; i < string_length; i++) {
                        fprintf(stdout, "%c", readbyte(record_offset));
                        record_offset++;
                    }
                    string_length = readbyte(record_offset);
                    record_offset++;
                    if ((record_offset < offset + omf_record_header->record_size - 1)) fprintf(stdout, ",");
                }
                
                fprintf(stdout, "\n");
                break;
            case OMF_LIBRARY:
                fprintf(stdout, "LIBRARY:\n");
                break;
            case OMF_LIBEND:
                fprintf(stdout, "LIBEND:\n");
                break;
            default:
                fprintf(stdout, "Unkown record type 0x%02x, size: %d bytes\n", omf_record_header->record_type, omf_record_header->record_size);
                break;
        }
        fprintf(stdout, "- Record size: %d bytes\n", omf_record_header->record_size);
        offset = offset + omf_record_header->record_size;
        records_count++;
        if (omf_record_header->record_type == OMF_LIBEND) break;
        while (readbyte(offset) == 0x00) offset++; // Skip 0 byte paddings
    }
    fprintf(stdout, "\n");
    fprintf(stdout, "Total amount of records: %ld\n", records_count);

}

void dumpomf__() {
    size_t offset, off2;
    size_t size;
    uint8_t len;
    uint8_t tag;
    uint8_t type;
    uint8_t class;
    uint16_t len2;

    offset = 0;
    off2 = 0;
    size = 0;
    tag = 0;

    fprintf(stdout, "\n");

    while (offset < file_size) {
        tag = readbyte(offset);
        offset++;
        size = readword(offset);
        offset = offset + 2;
        len = readbyte(offset);
        offset++;
        switch (tag) {
            case 0x80: // 
                fprintf(stdout, "THEADR: ");
                for (int i = 0; i < len; i++) {
                    fprintf(stdout, "%c", readbyte(offset + i));
                }
                fprintf(stdout, " ");
                break;
            case 0x88: // Comment record
                off2 = offset;
                type = readbyte(off2);
                off2++;
                class = readbyte(off2);
                off2++;
                fprintf(stdout, "COMENT (");
                switch (type) {
                    case 0x00:
                    fprintf(stdout, "Translator): ");
                    for (int i = 0; i < size - 3; i++) {
                        fprintf(stdout, "%c", readbyte(offset  + i));
                    }
                    break;
                default:
                    fprintf(stdout, "UNKN 0x%02x): ", type);
                    break;
                }
                break;
            case 0x8A: // Module end record | 16bit
            case 0x8B: // 32bit
                fprintf(stdout, "MODEND: ");
                break;
            case 0x8C: // External names definition record
                off2 = offset;
                fprintf(stdout, "EXTDEF: ");
                while (off2 < offset + size - 1) {
                    int i = 0;
                    for(i = 0; i < len; i++) {
                        fprintf(stdout, "%c", readbyte(off2 + i));
                    }
                    off2 = off2 + i;
                    fprintf(stdout, " %d", readbyte(off2));
                    off2++;
                    len = readbyte(off2);
                    off2++;
                    if ((off2 < offset + size - 1)) fprintf(stdout, ",");
                }
                fprintf(stdout, " ");
                break;
            case 0x90: // Public names definition record | 16bit
                off2 = offset;
                uint16_t base_group = (len << 8) + readbyte(off2);
                off2++;
                uint16_t base_seg_index = readword(off2);
                off2 = off2 + 2;
                uint16_t base_frame = 0;
                if (base_seg_index == 0x00) {
                    base_frame = 0;
                }
                else {
                    base_frame = readword(off2);
                    off2 = off2 + 2;
                }
                len2 = readbyte(off2);
                off2++;
                fprintf(stdout, "PUBDEF (16bit): ");
                fprintf(stdout, "Base group 0x%02x, Base segment index: 0x%02x, Base frame: 0x%02x ", base_group, base_seg_index, base_frame);
                while (off2 < offset + size - 1) {
                    int i = 0;
                    for(i = 0; i < len2; i++) {
                        fprintf(stdout, "%c", readbyte(off2 + i));
                    }
                    off2 = off2 + i + 7;
                    if ((off2 < offset + size - 1)) fprintf(stdout, ",");
                }
                fprintf(stdout, " ");
                break;
            case 0x91: // 32bit
                off2 = offset;
                fprintf(stdout, "PUBDEF (32bit): ");
                break;
            case 0x96: // List of names record
                off2 = offset;
                if (len == 0x00) {
                    len2 = readbyte(off2);
                    off2++;
                }
                else {
                    len2 = len;
                }
                fprintf(stdout, "LNAMES: ");
                while (off2 < offset + size - 1) {
                    int i = 0;
                    for(i = 0; i < len2; i++) {
                        fprintf(stdout, "%c", readbyte(off2 + i));
                    }
                    len2 = readbyte(off2 + i);
                    if (len2 == 0x00) {
                        off2++;
                        len2 = readbyte(off2 + i);
                    }
                    off2 = off2 + i + 1;
                    if ((off2 < offset + size - 1)) fprintf(stdout, ",");
                }
                fprintf(stdout, " ");
                break;
            case 0x98: // Segment definition record
            case 0x99:
                fprintf(stdout, "SEGDEF: ");
                break;
            case 0x9A: // Group definition record
                fprintf(stdout, "GRPDEF: ");
                break;
            case 0x9C: // Fixup record
            case 0x9D:
                fprintf(stdout, "FIXUP: ");
                break;
            case 0xA0: // Logical enumerated data record
            case 0xA1:
                fprintf(stdout, "LEDATA: ");
                break;
            case 0xA2: // Logical iterated data record
            case 0xA3:
                fprintf(stdout, "LIDATA: ");
                break;
            case 0xB0: // Communal names definition record
                fprintf(stdout, "COMDEF: ");
                break;
            case 0xB4: // Local external names definition record
                fprintf(stdout, "LEXTDEF: ");
                break;
            case 0xB6: // Local public names definition record
            case 0xB7:
                fprintf(stdout, "LPUBDEF: ");
                break;
            case 0xBC: // COMDAT external names definition record
                fprintf(stdout, "CEXTDEF: ");
                break;
            case 0xC2: // Initialized communal data record
            case 0xC3:
                fprintf(stdout, "COMDAT: ");
                break;
            case 0xC6: // Alias definiton record
                fprintf(stdout, "ALIAS: ");
                break;
            case 0xF0: // Library header record
                fprintf(stdout, "LIBRARY: ");
                break;
            case 0xF1: // Library end record
                fprintf(stdout, "LIBEND: ");
                break;
            default:
                fprintf(stdout, "UNKN: 0x%02x ", tag);
                break;
        }
        fprintf(stdout, "(%ld bytes)\n", size);
        offset = offset + size - 1;
        if (tag == 0xF1) {
            return;
        }
        while (readbyte(offset) == 0x00) offset++;
    }
}