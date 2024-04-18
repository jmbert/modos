
#ifndef _ARCH_LINKAGE_H
#define _ARCH_LINKAGE_H

#include <linkage.h>


extern char *__KERNEL_RELOC_OFFSET;
#define KERNEL_RELOC_OFFSET ((uintptr_t)(&__KERNEL_RELOC_OFFSET))

extern char *__KERNEL_TEXT;
#define KERNEL_TEXT ((uintptr_t)(&__KERNEL_TEXT))

extern char *__KERNEL_SIZE;
#define KERNEL_SIZE ((uintptr_t)(&__KERNEL_SIZE))

extern char *__KERNEL_END;
#define KERNEL_END ((uintptr_t)(&__KERNEL_END))

extern char *__KERNEL_START_LOW;
#define KERNEL_START_LOW ((uintptr_t)(&__KERNEL_START_LOW))

#define KERNEL_END_PHYSICAL (KERNEL_END-KERNEL_RELOC_OFFSET)

#endif