

ROOT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

HEADERS=$(ROOT_DIR)/include
	
GNUPREFIX=x86_64-elf-

OBJECTS=\
	_stdfuncs.o \
	state.o \
	boot/p32/pmodeEntry.o\
	entry.o\
	create_initrd.o\
	seriallog.o\
	mm/hhdm.o\
	mm/pm/setup_pm.o\
	mm/pm/alloc_pages.o\
	mm/pm/parse_memory_map.o\
	mm/vm/setup_vm.o\
	mm/vm/switch_stack.o\
	mm/vm/map_pages.o\
	mm/vm/request_pages.o\
	mm/vm/kmalloc.o\
	idt/register_handler.o\
	idt/lidt.o\
	idt/entry_shim.o\
	traps/enable_traps.o\
	traps/exception_handlers.o\
	gdt/reload_gdt.o\
	apic/lapic.o\
	apic/ioapic.o\
	apic/enable_apic.o\
	acpi/init_acpi.o\
	acpi/search_madt.o\
	bios/init_biosdata.o\
	irq/irq_handler.o\
	proc/exec_process.o\
	proc/exec_file.o\
	proc/switch_process.o\
	ramdisk/new_ramdisk_mount.o\
	tar/create_vfs.o\
	tar/tar_fileops.o\
	fs/fsutils.o\
	fs/open.o\
	fs/walk_path.o\
	proc/exec_elf.o\
	syscalls/register_syscalls.o\
	syscalls/enable_syscalls.o\
	syscalls/sysentry.o\
	tss/init_tss.o\
	tss/switch_stack.o
	
LINKSCRIPT=$(ROOT_DIR)/link_kernel.ld

CFLAGS+=\
	-std=gnu18 \
	-I$(HEADERS) \
	-Wall -Wextra \
	-ffreestanding \
	-MD \
	-mcmodel=large\
	-mno-red-zone -mno-mmx -mno-sse -mno-sse2\
	-DPRINTF_DISABLE_SUPPORT_FLOAT\
	-DSERIAL_PORT=$(SERIAL_PORT)\

LDFLAGS+=\
	-T$(LINKSCRIPT)

CC=$(GNUPREFIX)gcc
LD=$(GNUPREFIX)ld

DEPENDENCIES=$(OBJECTS:.o=.d)

KERNEL_FILE=kernel

install_headers:
ifndef INSTALL_DESTINATION
	$(error No headers destination specified)
endif
	cp --preserve=all -a $(HEADERS)/. $(INSTALL_DESTINATION)

install_kernel:
ifndef INSTALL_DESTINATION
	$(error No kernel destination specified)
endif
	cp --preserve=all $(KERNEL_FILE) $(INSTALL_DESTINATION)/

build_kernel: $(KERNEL_FILE)

$(KERNEL_FILE): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

%.o:%.c
	$(CC) -c $(CFLAGS) -o $@ $<
%.o:%.S
	$(CC) -c $(CFLAGS) -o $@ $<

-include $(DEPENDENCIES)

clean:
	rm $(KERNEL_FILE) $(OBJECTS) $(DEPENDENCIES)
