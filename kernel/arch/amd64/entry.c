
#include <compiler.h>
#include <mm/vm.h>
#include <mm/pm.h>
#include <log.h>
#include <panic.h>
#include <linkage.h>
#include <state.h>

__noreturn void entry(void *mbbootinfo) {
	state.progress = LATE_ARCH_BOOT;
	init_serial();

	setup_hhdm();
	setup_pm(mbbootinfo);
	setup_vm();

	for (;;);
}