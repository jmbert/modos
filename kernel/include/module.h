

#ifndef _MODULE_H
#define _MODULE_H

#include <compiler.h>
#include <fs.h>

void *load_module(struct file *module_file);

typedef uint64_t (*modinit)();

#endif