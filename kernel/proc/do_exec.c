


#include <proc.h>
#include <mm/vm.h>
#include <state.h>
#include <string.h>

__noreturn void do_exec(char *file, char **argv, size_t argc, char **envv, size_t envc) {
	struct process *proc = state.procs[state.current_pid];

	proc->argc = argc;
	proc->argv = argv;
	proc->arglen = 0;
	for (size_t i = 0; i < argc; i++) {
		proc->arglen += strlen(proc->argv[i]);
	}
	
	proc->envc = envc;
	proc->envv = envv;
	proc->envlen = 0;
	for (size_t i = 0; i < envc; i++) {
		proc->envlen += strlen(proc->envv[i]);
	}

	proc->auxc = 0;
	proc->auxv = kmalloc(sizeof(*proc->auxv)*AUX_MAX);

	exec_file(file, proc);
}