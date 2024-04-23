

#include <linkage.h>
#include <module.h>
#include <stdint.h>
#include <elf.h>
#include <stddef.h>
#include <fs.h>
#include <proc.h>
#include <mm/vm.h>
#include <mm/mmhelpers.h>
#include <string.h>
#include <log.h>
#include <export.h>

static uintptr_t lookup_sym(struct file *module_file, uintptr_t base_addr, Elf64_Dyn *dynamic, size_t dynlen, char *sym);
static int resolve_syms(struct file *module_file, uintptr_t base_addr, Elf64_Dyn *dynamic, size_t dynlen);

void *load_module(struct file *module_file) {
	Elf64_Ehdr hdr;
	if (module_file->vnode->fops->read(module_file, &hdr, sizeof(hdr), 0) < 0) {
		return NULL;
	}
	if (memcmp(hdr.e_ident, ELFMAG, 4)) {
		return NULL;
	}
	if (hdr.e_ident[EI_CLASS] != ELFCLASS64) {
		return NULL;
	}
	if (hdr.e_ident[EI_DATA] != ELFDATA2LSB) {
		return NULL;
	}
	if (hdr.e_ident[EI_VERSION] != EV_CURRENT) {
		return NULL;
	}
	if (hdr.e_type != ET_DYN) {
		// Add support for dynamic objects later
		return NULL;
	}
	if (hdr.e_machine != EM_X86_64) {
		return NULL; 
	}
	if (hdr.e_version != EV_CURRENT) {
		return NULL;
	}

	size_t phoff = hdr.e_phoff;
	size_t phnum = hdr.e_phnum;
	size_t phsize = hdr.e_phentsize * phnum;

	struct process_mapping *mappings = kmalloc(sizeof(*mappings)*phnum);
	size_t mappings_len = 0;
	uint64_t base_vaddr = 0;
	uint64_t max_vaddr = 0;



	Elf64_Phdr *phdrs = kmalloc(phsize);
	if (module_file->vnode->fops->read(module_file, phdrs, phsize, phoff) < 0) {
		return NULL;
	}
	Elf64_Dyn *dynamic = NULL;
	size_t dynlen = 0;
	for (size_t i = 0; i < phnum; i++) {
		Elf64_Phdr phdr = phdrs[i];
		if (phdr.p_type == PT_DYNAMIC) {
			dynamic = kmalloc(phdr.p_filesz);
			dynlen = phdr.p_filesz / sizeof(*dynamic);
			module_file->vnode->fops->read(module_file, dynamic, phdr.p_filesz, phdr.p_offset);
			log_printf("Dynamic %p: %x (%x)\n", dynamic, phdr.p_offset, dynlen);
			continue;
		}
		if (phdr.p_type != PT_LOAD) {
			continue;
		}
		if (i == 0) {
			base_vaddr = phdr.p_vaddr;
			max_vaddr = phdr.p_memsz;
		} else if (i == phnum-1) {
			max_vaddr = phdr.p_vaddr;
		}
		size_t n_pages = (phdr.p_memsz / PAGE_SIZE + 1) * PAGE_SIZE;
		void *data_ref = request_pages(0, n_pages, PG_WRITE);
		void *filedat = kmalloc(phdr.p_filesz);
		if (module_file->vnode->fops->read(module_file, filedat, phdr.p_filesz, phdr.p_offset) < 0) {
			return NULL;
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

		mappings[mappings_len] = map;
		mappings_len++;
		log_printf("Loading phdr at %p-%p\n", map.begin, map.end);
	}
	if (dynamic == NULL) {
		return NULL;
	}
	log_printf("Module min-max vaddrs %p-%p\n", base_vaddr, max_vaddr);

	size_t memsize = max_vaddr - base_vaddr;
	size_t n_pages = (memsize / PAGE_SIZE + 1) * PAGE_SIZE;
	uintptr_t base_addr = (uintptr_t)request_pages(0, n_pages, PG_WRITE);
	if (base_addr == 0) {
		return NULL;
	}
	base_addr += base_vaddr;

	log_printf("Module loaded at addresses %p:%p-%p\n", base_addr, base_vaddr, max_vaddr);
	for (size_t i = 0; i < mappings_len; i++) {
		mappings->begin += base_addr;
		mappings->end += base_addr;
		log_printf("Loaded %p-%p\n", mappings->begin, mappings->end);
		memcpy(mappings->begin, mappings->data_ref, mappings->end - mappings->begin);
	}

	int done = resolve_syms(module_file, base_addr, dynamic, dynlen);
	if (done != 0) {
		return NULL;
	}

	uintptr_t entry_point = lookup_sym(module_file, base_addr, dynamic, dynlen, "init_module");

	log_printf("Entry point at %p\n", entry_point);

	return (void*)entry_point;
}

static int resolve_syms(struct file *module_file, uintptr_t base_addr, Elf64_Dyn *dynamic, size_t dynlen) {
	Elf64_Sym *syms = NULL;
	size_t symlen = 0;
	char *strtab = NULL;
	size_t strtlen = 0;
	Elf64_Rela *rela = NULL;
	size_t relasz = 0;
	size_t relalen = 0;

	for (size_t i = 0; i < dynlen; i++) {
		switch (dynamic[i].d_tag)
		{
		case DT_SYMTAB:
			syms = (Elf64_Sym*)(dynamic[i].d_un.d_ptr + base_addr);
			break;
		case DT_STRTAB:
			strtab = (char*)(dynamic[i].d_un.d_ptr + base_addr);
			break;
		case DT_STRSZ:
			strtlen = dynamic[i].d_un.d_val;
			break;
		case DT_HASH:
			Elf32_Word *hashtab = (Elf32_Word*)(dynamic[i].d_un.d_ptr + base_addr);
			symlen = hashtab[1];
			break;
		case DT_RELA:
			rela = (Elf64_Rela*)(dynamic[i].d_un.d_ptr + base_addr);
			break;
		case DT_RELASZ:
			relasz += dynamic[i].d_un.d_val;
			break;
		case DT_PLTRELSZ:
			relasz += dynamic[i].d_un.d_val;
			break;
		default:
			break;
		}
	}
	if (syms == NULL || strtab == NULL || rela == NULL) {
		return 0;
	}
	relalen = (relasz / sizeof(Elf64_Rela));

	struct export_sym *ksyms = kmalloc(KERNEL_EXPORT_SYM_END-KERNEL_EXPORT_SYM_BEGIN);
	size_t n_ksyms = 0;
	
	for (char *magic_checker = (char*)KERNEL_EXPORT_SYM_BEGIN; magic_checker < KERNEL_EXPORT_SYM_END;) {
		if (memcmp(magic_checker, EXPORT_SYM_MAGIC, 11)) {
			magic_checker++;
			continue;
		}

		ksyms[n_ksyms] = *(struct export_sym *)magic_checker;
		n_ksyms++;

		magic_checker += sizeof(struct export_sym);
	}
	
	log_printf("%d symbols to relocate\n", relalen);
	for (size_t i = 0; i < relalen; i++) {
		Elf64_Sym sym = syms[ELF64_R_SYM(rela[i].r_info)];
		log_printf("Relocating %s\n", strtab+sym.st_name);
		if (ELF64_R_SYM(rela[i].r_info) == 0 || sym.st_name == 0) {
			continue;
		} 
		for (size_t j = 0; j < n_ksyms; j++) {
			log_printf("%s == %s\n", strtab+sym.st_name, ksyms[j].sym_name);
			if (strcmp(strtab+sym.st_name, ksyms[j].sym_name)) {
				continue;
			}
			log_printf("Resolved symbol %s (%p) at address %p\n", strtab+sym.st_name, rela[i].r_offset + base_addr, ksyms[j].reloc_addr);
			*(uint64_t*)(rela[i].r_offset + base_addr) = ksyms[j].reloc_addr;
			goto next_symbol;
		}
		log_printf("Failed to resolve %s\n", strtab+sym.st_name);
		next_symbol:
	}

	return 0;
}

static uintptr_t lookup_sym(struct file *module_file, uintptr_t base_addr, Elf64_Dyn *dynamic, size_t dynlen, char *sym) {
	Elf64_Sym *syms = NULL;
	size_t symlen = 0;
	char *strtab = NULL;
	size_t strtlen = 0;

	for (size_t i = 0; i < dynlen; i++) {
		switch (dynamic[i].d_tag)
		{
		case DT_SYMTAB:
			syms = (Elf64_Sym*)(dynamic[i].d_un.d_ptr + base_addr);
			break;
		case DT_STRTAB:
			strtab = (char*)(dynamic[i].d_un.d_ptr + base_addr);
			break;
		case DT_STRSZ:
			strtlen = dynamic[i].d_un.d_val;
			break;
		case DT_HASH:
			Elf32_Word *hashtab = (Elf32_Word*)(dynamic[i].d_un.d_ptr + base_addr);
			symlen = hashtab[1];
			break;
		default:
			break;
		}
	}
	if (syms == NULL || strtab == NULL) {
		return 0;
	}
	
	for (size_t i = 0; i < symlen; i++) {
		if (syms[i].st_name > strtlen) {
			continue;
		}
		char *name = strtab + syms[i].st_name;
		if (!strcmp(name, sym)) {
			return syms[i].st_value + base_addr;
		}
	}
	return 0;
}