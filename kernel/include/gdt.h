

#ifndef _GDT_H
#define _GDT_H

#define GDT_KERNEL_CODE 0x8
#define GDT_KERNEL_DATA 0x10
#define GDT_USER_CODE 	0x18
#define GDT_USER_DATA 	0x20

#ifndef __ASSEMBLER__
void reload_gdt();
#endif

#endif