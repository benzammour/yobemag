#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "rom.h"
#include "logging.h"

/******************************************************
 *** LOCAL VARIABLES                                ***
 ******************************************************/

#define ROM_TITLE_LEN (17)
#define ROM_TITLE_START_ADDR (0x134)
#define CARTRIDGE_TYPE_ADDR (0x147)
#define CARTRIDGE_SIZE_ADDR (0x148)

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

static uint8_t* rom_bytes = NULL;
static size_t rom_size;

/******************************************************
 *** LOCAL METHODS                                  ***
 ******************************************************/

static void rom_setup(void) {
    char title[ROM_TITLE_LEN];
    memcpy(title, &rom_bytes[ROM_TITLE_START_ADDR], ROM_TITLE_LEN - 1);
    title[ROM_TITLE_LEN - 1] = '\0';
    LOG_INFO("Rom title: %s", title);

    uint8_t type = rom_bytes[CARTRIDGE_TYPE_ADDR];
    LOG_INFO("Cartridge type: %s (%02x)", cartridge_types[type], type);

    uint8_t rom_size_index = rom_bytes[CARTRIDGE_SIZE_ADDR];
    LOG_INFO("Cartridge rom size: %s (%02x)", rom_sizes[rom_size_index], rom_size_index);
}

/******************************************************
 *** EXPOSED METHODS                                ***
 ******************************************************/

void rom_init(const char *file_name) {
    int f = open(file_name, O_RDONLY);
    if (f == -1)
		YOBEMAG_EXIT("Opening file %s failed: %s", file_name, strerror(errno));

	struct stat st;
    if (fstat(f, &st) == -1)
		YOBEMAG_EXIT("Retrieving information about file %s failed: %s", file_name, strerror(errno));

	rom_size = (size_t) st.st_size;
	rom_bytes = mmap(NULL, rom_size, PROT_READ, MAP_PRIVATE, f, 0);
    if (rom_bytes == MAP_FAILED)
		YOBEMAG_EXIT("mmap for file %s failed: %s", file_name, strerror(errno));

    rom_setup();
}

void rom_destroy(void) {
	if (rom_bytes != NULL && munmap(rom_bytes, rom_size) == -1) {
		// We cannot use YOBEMAG_EXIT here since it calls rom_destroy
		LOG_FATAL("munmap failed: %s", strerror(errno));
		fflush(stdout);
		fflush(stderr);
		SDL_Quit();
		exit(EXIT_FAILURE);
	}
}

uint8_t* get_rom_bytes(void) {
    return rom_bytes;
}
