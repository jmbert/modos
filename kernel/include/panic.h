

#ifndef _PANIC_H
#define _PANIC_H

#include <string.h>
#include <compiler.h>
#include <log.h>

#define panic(msg, ...) \
	log_printf(msg __VA_OPT__(,) __VA_ARGS__);\
	asm volatile ("cli;hlt");\
	unreachable()

#endif