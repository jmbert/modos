

ROOT_DIR:=$(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

HEADERS_PORTABLE=$(ROOT_DIR)/include

ARCH_DIR=$(ROOT_DIR)/arch/$(ARCH_NAME)
include $(ARCH_DIR)/makecfg

CFLAGS+=\
	-I$(HEADERS) \
	-Wall -Wextra \
	-ffreestanding \
	-MD \
	-mcmodel=kernel

LDFLAGS+=\
	-T$(ARCH_LINKER)

CC=$(ARCH_GNUPREFIX)gcc
LD=$(ARCH_GNUPREFIX)ld

KERNEL_FILE=kernel

install_headers:
ifndef INSTALL_DESTINATION
	$(error No headers destination specified)
endif
	cp --preserve=all -a $(HEADERS_PORTABLE)/. $(INSTALL_DESTINATION)
	cp --preserve=all -a $(ARCH_HEADERS)/. $(INSTALL_DESTINATION)

install_kernel:
ifndef INSTALL_DESTINATION
	$(error No kernel destination specified)
endif
	cp --preserve=all $(KERNEL_FILE) $(INSTALL_DESTINATION)/

build_kernel: $(KERNEL_FILE)

$(KERNEL_FILE): $(OBJECTS_PORTABLE) $(ARCH_OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

%.o:%.c
	$(CC) -c $(CFLAGS) $(ARCH_CFLAGS) -o $@ $<
%.o:%.S
	$(CC) -c $(CFLAGS) $(ARCH_CFLAGS) -o $@ $<

-include $(OBJECTS_PORTABLE:.o=.d) $(ARCH_OBJECTS:.o=.d)

clean:
	rm $(KERNEL_FILE) $(OBJECTS_PORTABLE) $(ARCH_OBJECTS)