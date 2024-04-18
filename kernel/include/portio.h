

#ifndef _PORTIO_H
#define _PORTIO_H

#include <stdint.h>
#include <asm.h>

static inline void write_8(uint16_t port, uint8_t data) {
	asm volatile (
		"out " OPREG_8L(1) ", " OPREG_16(0) ::
		"d"(port), "a"(data)
	);
}

static inline void write_16(uint16_t port, uint16_t data) {
	asm volatile (
		"out " OPREG_16(1) ", " OPREG_16(0) ::
		"d"(port), "a"(data)
	);
}

static inline void write_32(uint16_t port, uint32_t data) {
	asm volatile (
		"out " OPREG_32(1) ", " OPREG_16(0) ::
		"d"(port), "a"(data)
	);
}

static inline uint8_t read_8(uint16_t port) {
	uint8_t data;
	asm volatile (
		"in " OPREG_16(1) "," OPREG_8L(0)  :
		"=a"(data) :
		"d"(port)
	);
	return data;
}

static inline uint16_t read_16(uint16_t port) {
	uint16_t data;
	asm volatile (
		"in " OPREG_16(1) "," OPREG_16(0) :
		"=a"(data) :
		"d"(port)
	);
	return data;
}

static inline uint32_t read_32(uint16_t port) {
	uint32_t data;
	asm volatile (
		"in " OPREG_16(1) "," OPREG_32(0) :
		"=a"(data) :
		"d"(port)
	);
	return data;
}

#endif