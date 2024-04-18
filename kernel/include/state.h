

#ifndef _STATE_H
#define _STATE_H

#include <stdint.h>


enum kernel_flags {
	SERIAL_ENABLED = 1 << 0,
};

enum kernel_progress {
	EARLY_ARCH_BOOT = 0,
	LATE_ARCH_BOOT,
	BOOT_MAIN,
	NORMAL_MAIN,
};

struct kernel_state {
	uint8_t flags;
	uint8_t progress;
};

extern struct kernel_state state;

#endif