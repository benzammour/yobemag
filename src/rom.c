#include <stdio.h>
#include "rom.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>

static const char *cartridge_types[0xFF+1] = {
    [0x00] = "ROM ONLY",
    [0x01] = "MBC1",
    [0x02] = "MBC1+RAM",
    [0x03] = "MBC1+RAM+BATTERY",
    [0x05] = "MBC2",
    [0x06] = "MBC2+BATTERY",
    [0x08] = "ROM+RAM",
    [0x09] = "ROM+RAM+BATTERY",
    [0x0B] = "MMM01",
    [0x0C] = "MMM01+RAM",
    [0x0D] = "MMM01+RAM+BATTERY",
    [0x0F] = "MBC3+TIMER+BATTERY",
    [0x10] = "MBC3+TIMER+RAM+BATTERY",
    [0x11] = "MBC3",
    [0x12] = "MBC3+RAM",
    [0x13] = "MBC3+RAM+BATTERY",
    [0x15] = "MBC4",
    [0x16] = "MBC4+RAM",
    [0x17] = "MBC4+RAM+BATTERY",
    [0x19] = "MBC5",
    [0x1A] = "MBC5+RAM",
    [0x1B] = "MBC5+RAM+BATTERY",
    [0x1C] = "MBC5+RUMBLE",
    [0x1D] = "MBC5+RUMBLE+RAM",
    [0x1E] = "MBC5+RUMBLE+RAM+BATTERY",
    [0x20] = "MBC6",
    [0x22] = "MBC7+SENSOR+RUMBLE+RAM+BATTERY",
    [0xFC] = "POCKET CAMERA",
    [0xFD] = "BANDAI TAMA5",
    [0xFE] = "HuC3",
    [0xFF] = "HuC1+RAM+BATTERY",
};

static const char *rom_sizes[] = {
    [0x00] = "32KByte (no ROM banking)",
    [0x01] = "64KByte (4 banks)",
    [0x02] = "64KByte (4 banks)",
    [0x03] = "256KByte (16 banks",
    [0x04] = "512KByte (32 banks)",
    [0x05] = "1MByte (64 banks)",
    [0x06] = "2MByte (128 banks)",
    [0x07] = "4MByte (256 banks)",
    [0x08] = "8MByte (512 banks)",
    [0x52] = "1.1MByte (72 banks)",
    [0x53] = "1.2MByte (80 banks)",
    [0x54] = "1.5MByte (96 banks)",
};

static uint8_t *rom_bytes;

void rom_setup(uint8_t *rom_data) {
    char title[17];

    memcpy(title, &rom_data[0x134], 16);
    title[16] = '\0';

    printf("Rom title: %s\n", title);

    uint8_t type = rom_data[0x147];
    printf("Cartridge type: %s (%02x)\n", cartridge_types[type], type);

    uint8_t romsize_index = rom_data[0x148];
    printf("Cartridge rom size: %s (%02x)\n", rom_sizes[romsize_index], romsize_index);

    rom_bytes = rom_data;
}

ErrorCode rom_load(const char *filename) {
    int f;
    struct stat st;

    uint8_t *bytes;
    size_t rom_size;

    f = open(filename, O_RDONLY);
    if(f == -1)
        return ERR_FILE_IO;

    if(fstat(f, &st) == -1)
        return ERR_FILE_IO;

    rom_size = (size_t) st.st_size;

    bytes = mmap(NULL, rom_size, PROT_READ, MAP_PRIVATE, f, 0);
    if(!bytes)
        return ERR_FAILURE;

    rom_setup(bytes);
    return ERR_SUCCESS;
}

uint8_t *get_rom_bytes(void) {
    return rom_bytes;
}
