
#include <proc.h>
#include <mm/vm.h>
#include <mm/pm.h>
#include <mm/mmhelpers.h>
#include <string.h>
#include <cpu.h>
#include <gdt.h>
#include <compiler.h>
#include <log.h>

static int load_mapping(struct process_mapping procmap) {
	ptrdiff_t size = procmap.end - procmap.begin;
	size_t n_pages = 0;
	if (size & 0xFFF) {
		n_pages = (size / PAGE_SIZE) + 1;
	} else {
		n_pages = size / PAGE_SIZE;
	}


	int ret = mapalloc_pages(procmap.begin, n_pages, procmap.flags | PG_USER);
	memcpy(procmap.begin, procmap.data_ref, size);
	return ret;
}

extern __noreturn void switch_process(void *rsp, uint8_t dseg);

__noreturn void exec_process(struct process *proc) {
	proc->cr3_phys = alloc_pages(1);
	uintptr_t current_phys = get_toplevel_real();
	memcpy((void*)(P2V(proc->cr3_phys)+0x800), (void*)(P2V(current_phys)+0x800), 0x800);
	set_toplevel(proc->cr3_phys);
	activate_toplevel();

	for (size_t i = 0; i < proc->mappings_len; i++) {
		load_mapping(proc->mappings[i]);
	}

	void *user_stack = (void*)USER_STACK_TOP; 

	user_stack -= proc->arglen;
	uintptr_t argvs = (uintptr_t)user_stack;

	user_stack -= proc->envlen;
	uintptr_t envvs = (uintptr_t)user_stack;

	user_stack -= sizeof(struct auxv); // Null entry
	user_stack -= sizeof(struct auxv)*proc->auxc;

	user_stack -= 8; // Null entry
	user_stack -= sizeof(char*)*proc->envc;

	user_stack -= 8; // Null entry
	user_stack -= sizeof(char*)*proc->argc;

	user_stack -= 8; // Argc

	size_t initial_stack_size = USER_STACK_TOP - (uintptr_t)user_stack;
	if (initial_stack_size & 0xFFF) {
		initial_stack_size = (initial_stack_size / PAGE_SIZE + 1) * PAGE_SIZE;
	}
	mapalloc_pages((void*)((uintptr_t)user_stack & ~0xFFF), initial_stack_size / PAGE_SIZE, PG_WRITE | PG_USER);

	((uint64_t*)user_stack)[0] = proc->argc;

	size_t arg_written = 0;
	for (size_t i = 0; i < proc->argc; i++) {
		((uint64_t*)user_stack)[1+i] = argvs+arg_written;
		log_printf("%p = %p (%p)\n", (void*)(argvs+arg_written), proc->argv[i], strlen(proc->argv[i]));
		memcpy((void*)(argvs+arg_written), proc->argv[i], strlen(proc->argv[i]));
		arg_written += strlen(proc->argv[i]);
	}
	((uint64_t*)user_stack)[1+proc->argc] = 0;
	
	size_t env_written = 0;
	for (size_t i = 0; i < proc->envc; i++) {
		((uint64_t*)user_stack)[2+proc->argc+i] = envvs+env_written;
		log_printf("%p = %p (%p)\n", (void*)(envvs+env_written), proc->envv[i], strlen(proc->envv[i]));
		memcpy((void*)(envvs+env_written), proc->envv[i], strlen(proc->envv[i]));
		env_written += strlen(proc->envv[i]);
	}
	((uint64_t*)user_stack)[2+proc->argc+proc->envc] = 0;

	for (size_t i = 0; i < proc->auxc; i++) {
		((uint64_t*)user_stack)[3+proc->argc+proc->envc+i] = proc->auxv[i].a_type;
		((uint64_t*)user_stack)[4+proc->argc+proc->envc+i] = proc->auxv[i].a_val;
	}
	((uint64_t*)user_stack)[3+proc->argc+proc->envc] = 0;
	((uint64_t*)user_stack)[4+proc->argc+proc->envc] = 0;

	struct regs init_regs = {0};
	init_regs.rsp = (uintptr_t)user_stack;
	init_regs.cs = GDT_USER_CODE | 0x3;
	init_regs.ss = GDT_USER_DATA | 0x3;
	init_regs.rip = proc->entry_point;
	init_regs.rflags = 0;

	init_regs.error_code = 0;
	init_regs.irq = 0x80;

	proc->regs = init_regs;

	user_stack -= sizeof(struct regs); // Registers
	*(struct regs *)user_stack = init_regs;

	switch_process(user_stack, GDT_USER_DATA | 0x3);
}