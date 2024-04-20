

#ifndef _TRAP_HANDLERS_H
#define _TRAP_HANDLERS_H

#include <stdint.h>

enum trap_irqs {
	TRAP_DIV_ERR,
	TRAP_DEBUG,
	TRAP_NMI,
	TRAP_BRK,
	TRAP_OVRFLW,
	TRAP_BNDRE,
	TRAP_IIO,
	TRAP_DNA,
	TRAP_DOUBLE,
	TRAP_CSO,
	TRAP_ITSS,
	TRAP_SNP,
	TRAP_SSF,
	TRAP_GPF,
	TRAP_PGF,

	TRAP_X87FPE = 0x10,
	TRAP_ALIGN,
	TRAP_MCA,
	TRAP_SIMDFPE,
	TRAP_VIRT,
	TRAP_CPE,

	TRAP_HIE = 0x1C,
	TRAP_VCE,
	TRAP_SEC,
};

void default_trap_handler(uint8_t irq, uint64_t from, uint64_t error);
void page_fault_handler(uint8_t irq, uint64_t from, uint64_t error);

#endif