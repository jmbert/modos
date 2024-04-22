


#include <syscalls.h>
#include "syscalls.h"
#include <idt.h>
#include <gdt.h>
#include <state.h>
#include <log.h>

uint64_t default_syscall_handler(struct regs *regs) {
	return -1;
}

extern uint64_t sysentry();
extern void enable_syscalls(uint64_t handler);

syscall syscall_table[SYSCALL_MAX];

void register_syscalls() {
	for (size_t i = 0; i < SYSCALL_MAX; i++) {
		syscall_table[i] = default_syscall_handler;
	}
	

	enable_syscalls((uint64_t)sysentry);
}