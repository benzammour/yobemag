#include <criterion/criterion.h>
#include <stdio.h>
#include <time.h>
#include "cli.h"
#include "cpu.h"
#include "logging.h"
#include "mmu.h"

void setup2(void) {
	// puts("Runs before the test");
	srand(time(NULL));
	cpu_init();
}

void teardown2(void) {
	puts("Runs after the test");
}

// Test SUB_A_A with no borrow
Test(simple, sub_a_a_no_borrow, .init = setup2, .fini = teardown2) {
	const uint8_t opcode = 0x97;
	const uint8_t a = 128;
	const uint8_t expected = a - a;
	const uint16_t address = rand() % (0xFFFF);

	// set appropriate program counter and value inside register
	cpu.PC = address;
	cpu.AF.bytes.high = a;

	// write opcode to memory
	// TODO: implement signature: `mmu_write_byte(address, value)`
	// mmu_write_byte(address, opcode);

	// do the actual emulation
	if (cpu_step()) {
		LOG_FATAL("error!");
	}

	// TODO: check flags
	
	// check if value is correct
	const uint8_t actual = cpu.AF.bytes.high;
	cr_assert(actual == expected);

	// check if pc is Pc is updated correctly
	cr_assert(cpu.PC == (address + 1));
}
