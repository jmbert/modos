


#include <elf.h>
#include <fs.h>
#include <log.h>
#include <string.h>
#include <mm/vm.h>

void exec_elf(struct file *file, struct process *proc) {
	Elf64_Ehdr hdr;
	if (file->vnode->fops->read(file, &hdr, sizeof(hdr), 0) < 0) {
		return;
	}
	if (memcmp(hdr.e_ident, ELFMAG, 4)) {
		return;
	}
	if (hdr.e_ident[EI_CLASS] != ELFCLASS64) {
		return;
	}
	if (hdr.e_ident[EI_DATA] != ELFDATA2LSB) {
		return;
	}
	if (hdr.e_ident[EI_VERSION] != EV_CURRENT) {
		return;
	}
	if (hdr.e_type != ET_EXEC) {
		// Add support for dynamic objects later
		return;
	}
	if (hdr.e_machine != EM_X86_64) {
		return;
	}
	if (hdr.e_version != EV_CURRENT) {
		return;
	}
	
	proc->entry_point = hdr.e_entry;

	size_t phoff = hdr.e_phoff;
	size_t phnum = hdr.e_phnum;
	size_t phsize = hdr.e_phentsize * phnum;

	proc->mappings = kmalloc(sizeof(*proc->mappings)*phnum);

	Elf64_Phdr *phdrs = kmalloc(phsize);
	if (file->vnode->fops->read(file, phdrs, phsize, phoff) < 0) {
		return;
	}
	for (size_t i = 0; i < phnum; i++) {
		Elf64_Phdr phdr = phdrs[i];
		if (phdr.p_type != PT_LOAD) {
			continue;
		}
		size_t n_pages = (phdr.p_memsz / PAGE_SIZE + 1) * PAGE_SIZE;
		void *data_ref = request_pages(0, n_pages, PG_WRITE);
		void *filedat = kmalloc(phdr.p_filesz);
		if (file->vnode->fops->read(file, filedat, phdr.p_filesz, phdr.p_offset) < 0) {
			return;
		}
		memcpy(data_ref, filedat, phdr.p_filesz);

		struct process_mapping map;
		map.begin = (void*)phdr.p_vaddr;
		map.end = (void*)(phdr.p_vaddr + phdr.p_memsz);
		map.flags = 0;
		map.data_ref = data_ref;

		if (phdr.p_flags & PF_W) {
			map.flags |= PG_WRITE;
		}

		proc->mappings[proc->mappings_len] = map;
		proc->mappings_len++;
	}

	
	exec_process(proc);
}