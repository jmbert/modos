

#ifndef _SERIALLOG_H
#define _SERIALLOG_H

#include <stddef.h>
#include <stdint.h>

int init_serial();

void serial_puts(char *str, size_t len);

int serial_printf(char *fmt, ...);

extern uint16_t serial_port;

#endif