

MODROOT=$(ROOT_DIR)/drivers/keyboard_ps2/

MODULE=$(ROOT_DIR)/keyboard_ps2.ko

GNUPREFIX=x86_64-elf-

LINKSCRIPT=$(MODROOT)/link.ld

CFLAGS+=\
	-std=gnu18 \
	-I$(KERNEL_HEADERS) \
	-Wall -Wextra \
	-ffreestanding \
	-MD \
	-mcmodel=large\
	-mno-red-zone -mno-mmx -mno-sse -mno-sse2\
	-DPRINTF_DISABLE_SUPPORT_FLOAT\


LDFLAGS+=\
	-shared \
	-T$(LINKSCRIPT)

CC=$(GNUPREFIX)gcc
LD=$(GNUPREFIX)ld

OBJECTS=\
	init.o

DEPENDENCIES=$(OBJECTS:.o=.d)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o:%.S
	$(CC) $(CFLAGS) -c $< -o $@

$(MODULE): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

clean:
	-rm $(MODULE) $(OBJECTS) $(DEPENDENCIES)

-include $(DEPENDENCIES)