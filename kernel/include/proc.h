
#ifndef _PROC_H
#define _PROC_H

#include <stddef.h>
#include <stdint.h>
#include <cpu.h>
#include <compiler.h>
#include <fs.h>

#define USER_STACK_TOP 0x00007fffffffffff

typedef size_t pid;

struct process_mapping {
	void *begin;
	void *end;
	void *data_ref;
	uint16_t flags;
};

enum a_types {
	AT_NULL,
	AT_IGNORE,
	AT_EXECFD,
	AT_PHDR,
	AT_PHENT,
	AT_PHNUM,
	AT_PAGESZ,
	AT_BASE,
	AT_FLAGS,
	AT_ENTRY,
	AT_NOTELF,
	AT_UID,
	AT_EUID,
	AT_GID,
	AT_EGID,
};

struct auxv {
	uint32_t a_type;
	uint64_t a_val;
};

struct process {
	pid _pid;
	char **argv;
	size_t argc;
	size_t arglen;

	char **envv;
	size_t envc;
	size_t envlen;

	struct auxv *auxv;
	size_t auxc;

	struct process_mapping *mappings;
	size_t mappings_len;

	uintptr_t entry_point;
	struct regs regs;

	uintptr_t cr3_phys;

	struct vfs_node *root;
};

__noreturn void exec_process(struct process *proc);

__noreturn void exec_file(char *path, struct process *proc); 

#endif