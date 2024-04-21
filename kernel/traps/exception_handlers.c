
#include <panic.h>
#include <stdint.h>
#include <cpu.h>

void default_trap_handler(struct regs *regs) {
	panic(	"Unhandled trap #%02x:\n"
		"\tAt: %p\n"
		"\tError Code: %lx\n", regs->irq, regs->rip, regs->error_code);
}
void page_fault_handler(struct regs *regs) {
	uint64_t fault_addr;
	asm volatile (
		"mov %%cr2, %0\n" :
		"=r"(fault_addr)
	);
	panic("Unhandled page fault\n"
		"\tAt: %p\n"
		"\tAccessing: %p\n"
		"\tError Code: %lx\n", regs->rip, fault_addr, regs->error_code);
}