

#ifndef _PM_H
#define _PM_H

#include <mm/hhdm.h>
#include <stdint.h>
#include <stddef.h>

void setup_pm(void *mbbootinfo_phys);

extern uintptr_t arena_begin;
extern uintptr_t arena_head;
extern uintptr_t arena_end;

uintptr_t alloc_pages(size_t n_pages);

#endif