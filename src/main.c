#include <getopt.h>
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include "main.h"
#include "mz.h"

size_t file_size;
uint8_t file_type;
uint8_t *binary;

static const char help_message[] = 
    "dump: This tool is for displaying information about 16, 32 and 64 bit DOS, Windows and OS/2 executables, "
    "as well as symbolic files, library files and object files.\n"
    "Usage: dump [options] <file(s)>\n"
    "Options:\n"
    "\t-h, --help            Show this message.\n"
    "\t-v, --verbose         Be more verbose.\n"
    "\t-x, --hex-dump        Include hex dumps\n\n"
    "\t-d, --dos-header      Always show the MZ part regardless of the type of executable."
    "Supported formats:\n"
    "\t MZ EXE (DOS) .exe\n"
    "\t LE/LX EXE (Windows VXD, OS/2 Executeables) .exe, .dll, .vxd, .386, .sys\n"
    "\t NE EXE (Windows 16 bit Executables) .exe, .dll, .fon, .ttf, .drv\n"
    "\t PE EXE (Windows 32 and 64 bit Executables) .exe, .dll\n";

static const struct option long_options[] = {
    {"help", no_argument, NULL, 'h'},
    {"verbose", no_argument, NULL, 'v'},
    {0}
};

static const filetype exe_filetype[] = { // ENUM filetype, short text, long description
    { FILETYPE_UNKNOWN, "Unknown","Unknown" },
    { FILETYPE_MZ, "MZ", "MS-DOS MZ Executable (.exe)" },
    { FILETYPE_LE, "LE", "Windows VxD, OS/2 Executable (.exe, .dll, .vxd, .386, .sys)" },
    { FILETYPE_LX, "LX", "OS/2 Executable (.exe, .dll)" },
    { FILETYPE_NE, "NE", "Windows New Executable (.exe, .dll, .fon, .drv)" },
    { FILETYPE_PE, "PE", "Windows Portable Executable (.exe, .dll)" },
    { FILETYPE_PHARLAP_MP, "MP", "Old Phar Lap Executable (.exe, .exp) "},
    { FILETYPE_PHARLAP_P2, "P2", "Phar Lap 286 Executable (.exe, .exp)" },
    { FILETYPE_PHARLAP_P3, "P3", "Phar Lap 386 Executable (.exe, .exp)" },
    { FILETYPE_OMF_OBJ, "OBJ_OMF", "OMF Object File" },
    { FILETYPE_OMF_LIB, "LIB_OMF", "OMF Library File" },
    { FILETYPE_SYM, "SYM", "Windows Symbolic Debug File (*.sym)" },
    { FILETYPE_W3, "W3", "Windows VxD Compacted file (*.exe, *.vxd)" },
};

uint8_t get_filetype(char *filename) {
    size_t offset;
    uint16_t magic;
    struct mz_header *mz;

    mz = readdata(0);
    magic = (mz->signature[0] << 8) + mz->signature[1];
    
    if (magic == 0x4D5A) {
        /*if (mz->new_header_offset > exe_size) {
            fprintf(stderr, "New header offset bigger than file size.\n");
            exit(-1);
        }*/
        if (readword(offsetof(struct mz_header, relocation_table_offset)) == 0x40 || mz->relocation_table_offset < sizeof(struct mz_header)) {
            offset = mz->new_header_offset;
            if (offset <= file_size + sizeof(uint16_t)) {
                magic = readword(offset);
            }
            switch (magic) {
                case 0x454C:
                    return FILETYPE_LE;
                case 0x584C:
                    return FILETYPE_LX;
                case 0x454E:
                    return FILETYPE_NE;
                case 0x4550:
                    return FILETYPE_PE;
                case 0x3357:
                    return FILETYPE_W3;
                default:
                    return FILETYPE_MZ;
            }
        }
    }
    if (((magic & 0xFF00) >> 8 == 0x80) && ((!strcmp(strrchr(filename, '\0') - 4, ".obj")) || (!strcmp(strrchr(filename, '\0') - 4, ".OBJ")))) {
        return FILETYPE_OMF_OBJ;
    }

    if (((magic & 0xFF00) >> 8 == 0xF0) && ((!strcmp(strrchr(filename, '\0') - 4, ".lib")) || (!strcmp(strrchr(filename, '\0') - 4, ".LIB")))) {
        return FILETYPE_OMF_LIB;
    }

    return FILETYPE_UNKNOWN;
}

void print_help_message() {
    fprintf(stdout, help_message);
}

void dump(char *filename) {
    FILE *fd;

    if((fd = fopen(filename, "rb")) < 0) {
        fprintf(stderr, "Cannot open file %s.\n", filename);
        exit(-1);
    }
    fseek(fd, 0, SEEK_END);
    file_size = ftell(fd);

    if (file_size == 0) {
        fprintf(stderr, "Empty file.\n");
        exit(-1);
    }

    binary = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fileno(fd), 0);

    if (binary == MAP_FAILED) {
        fprintf(stderr, "Cannot allocate %lu bytes.\n", file_size);
        exit(-1);
    }

    file_type = get_filetype(filename);

    fprintf(stdout, "File: %s\n", filename);
    fprintf(stdout, "Size: 0x%08lx bytes (%lu)\n", file_size, file_size);
    ((binary[0x3D] << 8) + binary[0x3C]) ? fprintf(stdout, "Extended Header Offset: 0x%04x (%u)\n", ((binary[0x3D] << 8) + binary[0x3C]), ((binary[0x3D] << 8) + binary[0x3C])):fprintf(stdout, "No extended header\n");
    fprintf(stdout, "Filetype: %s\n", exe_filetype[file_type].description);

    fclose(fd);

    switch (file_type) {
        case FILETYPE_MZ:
            dumpmz();
            break;
        case FILETYPE_LE:
        case FILETYPE_LX:
            dumple();
            break;
        case FILETYPE_NE:
            fprintf(stdout, "MZ Header:\n");
            dumpmz();
            fprintf(stdout, "NE Header:\n");
            dumpne();
            break;
        case FILETYPE_PE:
            dumppe();
            break;
        case FILETYPE_OMF_OBJ:
        case FILETYPE_OMF_LIB:
            dumpomf();
            break;
        case FILETYPE_W3:
            dumpw3();
        default:
            break;
    }

    munmap(binary, file_size);
}

int main(int argc, char *argv[]) {
    int opt;
    
    while((opt = getopt_long(argc, argv, ":::hvx", long_options, NULL)) >= 0) {
        switch(opt) {
        case 'h': // Help
            print_help_message();
            exit(0);
            break;
        case 'v': // Verbose
            break;
        case 'x': // Hex dumps
            break;
        default:
            print_help_message();
            exit(0);
            break;
        }
    }

    if(optind == argc) {
        print_help_message();
        exit(0);
    }

    while (optind < argc) {
        dump(argv[optind++]);
        if(optind < argc) {
            fprintf(stdout, "\n\n");
        }
    }

}