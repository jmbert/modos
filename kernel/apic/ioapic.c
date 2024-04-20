
#include <apic.h>
#include <stdint.h>
#include <stddef.h>
#include <state.h>
#include <log.h>
#include <compiler.h>

__noinline uint32_t read_ioapic_reg(struct ioapic *ioapic, uint32_t offset) {
	IOAPIC_REMAP_BASE(ioapic->id)[0x0] = offset;

	return IOAPIC_REMAP_BASE(ioapic->id)[0x4];
}

__noinline void write_ioapic_reg(struct ioapic *ioapic, uint32_t offset, uint32_t value) {
	IOAPIC_REMAP_BASE(ioapic->id)[0x0] = offset;

	IOAPIC_REMAP_BASE(ioapic->id)[0x4] = value;
}

__noinline uint64_t read_ioapic_redir(struct ioapic *ioapic, uint8_t offset) {
	uint64_t lo = read_ioapic_reg(ioapic, IOAPIC_REDIR_BASE + offset*2);
	uint64_t hi = read_ioapic_reg(ioapic, IOAPIC_REDIR_BASE + offset*2 + 1);
	return (hi << 32) | lo;
}

__noinline void write_ioapic_redir(struct ioapic *ioapic, uint8_t offset, uint64_t value) {
	write_ioapic_reg(ioapic, IOAPIC_REDIR_BASE + offset*2, value & 0xFFFFFFFF);
	write_ioapic_reg(ioapic, IOAPIC_REDIR_BASE + offset*2 + 1, (value >> 32) & 0xFFFFFFFF);
	log_printf("IOAPIC %x, pin %x redirects %lx\n", ioapic->id, offset, value);
}

void write_gsid_redir(uint8_t gsid, uint64_t value) {
	for (size_t i = 0; i < state.n_ioapics; i++) {
		struct ioapic *ioapic = &state.ioapics[i];
		if (ioapic->gsid_base <= gsid) {
			write_ioapic_redir(ioapic, gsid-ioapic->gsid_base, value);
		}
	}
}

uint64_t read_gsid_redir(uint8_t gsid) {
	for (size_t i = 0; i < state.n_ioapics; i++) {
		struct ioapic *ioapic = &state.ioapics[i];
		if (ioapic->gsid_base <= gsid) {
			return read_ioapic_redir(ioapic, gsid-ioapic->gsid_base);
		}
	}
}