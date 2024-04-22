
#include <tss.h>

struct tss tss = {0};

void switch_tss_stack(uint64_t new_kernel_stack) {
	tss.rsp[0] = new_kernel_stack;
	tss.rsp[1] = new_kernel_stack >> 32;
}