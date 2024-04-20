
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



	for (;;);
}