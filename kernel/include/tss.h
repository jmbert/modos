

#ifndef _TSS_H
#define _TSS_H

#include <compiler.h>
#include <stdint.h>

__packed struct tss {
	uint32_t reserved1;
	uint32_t rsp[6];
	uint64_t reserved2;
	uint32_t ist[14];
	uint32_t reserved3[2];
	uint32_t reserved4;
	uint16_t reserved5;
	uint16_t iopb;
};

void init_tss();

void switch_tss_stack(uint64_t new_kernel_stack);

extern struct tss tss;

#endif