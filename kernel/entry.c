
#include <compiler.h>
#include <mm/vm.h>
#include <mm/pm.h>
#include <log.h>
#include <panic.h>
#include <linkage.h>
#include <state.h>
#include <traps.h>
#include <idt.h>
#include <gdt.h>
#include <apic.h>
#include <acpi.h>
#include <kinfo.h>
#include <proc.h>

__noreturn void entry(void *mbbootinfo) {
	init_serial();

	reload_gdt();

	setup_hhdm();
	setup_pm(mbbootinfo);
	setup_vm();

	init_biosdata();
	init_acpi();

	enable_traps();
	enable_apic();
	enable_idt();

	state.n_procs = 1;
	struct process *init_proc = kmalloc(sizeof(*init_proc));

	state.procs[0] = init_proc;

	exec_file("/bin/init", init_proc);

	for (;;);
}