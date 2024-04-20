

#include <apic.h>
#include <state.h>
#include <stdint.h>

uint32_t read_lapic_reg(uint32_t offset) {
	return LAPIC_REMAP_DESTINATION[offset << 2];
}
void write_lapic_reg(uint32_t offset, uint32_t value) {
	LAPIC_REMAP_DESTINATION[offset << 2] = value;
}