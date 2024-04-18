

#ifndef _LOG_H
#define _LOG_H

#include <state.h>
#include <seriallog.h>

#define log_printf(msg, ...) \
	((state.flags & SERIAL_ENABLED) ? serial_printf(msg __VA_OPT__(,) __VA_ARGS__) : 0)

#pragma GCC poison serial_printf

#endif