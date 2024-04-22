

#include <seriallog.h>
#include <portio.h>
#include <stdarg.h>
#include <compiler.h>
#include <state.h>
#include <export.h>

#ifndef SERIAL_PORT
#define SERIAL_PORT 0x0000
#endif

uint16_t serial_port = SERIAL_PORT;

static void serial_blockread() {
   	while ((read_8(serial_port + 5) & 1) == 0);
}

static void serial_blockwrite() {
	while ((read_8(serial_port + 5) & 0x20) == 0);
}

void serial_puts(char *str, size_t len) {
	for (size_t i = 0; i < len; i++) {
		serial_blockwrite();
		write_8(serial_port, str[i]);
	}
}

static void _putchar(char c) {
	serial_blockwrite();
	write_8(serial_port, c);
}

#include "printf.c"

int serial_printf(char *fmt, ...) __alias(printf_);
EXPORT_SYM(serial_printf);

int init_serial() {

   	write_8(serial_port + 1, 0x00);    // Disable all interrupts
   	write_8(serial_port + 3, 0x80);    // Enable DLAB (set baud rate divisor)
   	write_8(serial_port + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
   	write_8(serial_port + 1, 0x00);    //                  (hi byte)
   	write_8(serial_port + 3, 0x03);    // 8 bits, no parity, one stop bit
   	write_8(serial_port + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
   	write_8(serial_port + 4, 0x0B);    // IRQs enabled, RTS/DSR set
   	write_8(serial_port + 4, 0x1E);    // Set in loopback mode, test the serial chip
   	write_8(serial_port + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)
	
   	// Check if serial is faulty (i.e: not same byte as sent)
   	if(read_8(serial_port + 0) != 0xAE) {
   	   	return 1;
   	}
	
   	// If serial is not faulty set it in normal operation mode
   	// (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
   	write_8(serial_port + 4, 0x0F);

	state.flags |= SERIAL_ENABLED;
	return 0;
}