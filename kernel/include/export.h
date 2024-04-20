

#ifndef _EXPORT_H
#define _EXPORT_H

#include <compiler.h>

#define EXPORT_SYM(sym) \
	__section(".export.sym") const char __export_sym_ ## sym[] = #sym;

#endif