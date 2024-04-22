

#ifndef _EXPORT_H
#define _EXPORT_H

#include <compiler.h>

#define EXPORT_SYM_MAGIC "EXPORT SYM\0" 
__packed struct export_sym {
	char magic_begin[11];
	uint64_t reloc_addr;
	char *sym_name;
};

#define EXPORT_SYM(sym) \
	__section(".export.sym") const struct export_sym __export_sym_ ## sym = (struct export_sym){\
		.magic_begin = EXPORT_SYM_MAGIC, \
		.reloc_addr = (uint64_t)&(sym), \
		.sym_name = #sym, \
	};

#endif