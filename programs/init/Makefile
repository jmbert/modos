

PROGROOT=$(ROOT_DIR)/init/

PROGRAM=$(ROOT_DIR)/bin/init

GNUPREFIX=x86_64-elf-

LINKSCRIPT=$(PROGROOT)/link.ld

CFLAGS+=\
	-std=gnu18 \
	-Wall -Wextra \
	-MD \
	-mno-mmx -mno-sse -mno-sse2

LDFLAGS+=\
	-T$(LINKSCRIPT)

CC=$(GNUPREFIX)gcc
LD=$(GNUPREFIX)ld

OBJECTS=\
	init.o

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o:%.S
	$(CC) $(CFLAGS) -c $< -o $@

$(PROGRAM): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^