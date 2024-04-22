
#include <acpi.h>
#include <apic.h>
#include <log.h>
#include <mm/vm.h>

void init_madt() {
	uint64_t lapic_base = state.madt->local_apic_addr;
	log_printf("LAPIC Found: %p\n", lapic_base);

	struct madt_hdr *hdr = &(state.madt->first_hdr);
	size_t size = (state.madt->hdr.len - sizeof(*state.madt));
	for (size_t i = 0; i < size; 
		i+= hdr->len, 
		hdr = (struct madt_hdr*)(((char*)hdr) + hdr->len)) {
		log_printf("MADT header: %p:%x\n", hdr, hdr->len);
		switch (hdr->type)
		{
		case IOAPIC:
			struct acpi_ioapic *ioapic = (struct acpi_ioapic *)hdr;
			log_printf("Found IOAPIC\n");
			log_printf("\tID: %x\n", ioapic->ioapic_id);
			log_printf("\tAddr: %p\n", ioapic->ioapic_addr);
			log_printf("\tGSID Base: %x\n", ioapic->gsid);
			struct ioapic ioapic_state;
			ioapic_state.id = ioapic->ioapic_id;
			ioapic_state.ioapic_regs = ioapic->ioapic_addr;
			ioapic_state.gsid_base = ioapic->gsid;
			ioapic_state.relocate_len = 0;

			map_pages(IOAPIC_REMAP_BASE(ioapic_state.id), (uintptr_t)ioapic_state.ioapic_regs, 1, PG_WRITE | PG_PCD);

			state.ioapics[state.n_ioapics] = ioapic_state;
			state.n_ioapics++;
			break;
		case LAPIC_OVERRIDE:
			struct lapicoverride *override = (struct lapicoverride *)hdr;
			lapic_base = override->addr;
			log_printf("Found LAPIC address override: %p\n", override->addr);
			break;
		case PROC_LOCALAPIC:
			struct acpi_lapic *lapic = (struct acpi_lapic *)hdr;
			struct lapic lapic_state;
			lapic_state.id = lapic->lapic_id;
			log_printf("Found LAPIC: %d\n", lapic->lapic_id);
			state.lapics[state.cpu_n] = lapic_state;
			state.cpu_n++;
			break;
		case IOAPIC_ISO:
			struct ioapiciso *iso = (struct ioapiciso *)hdr;
			struct ioapic_legacymap lmap;
			lmap.legacy_irq = iso->irq_source;
			lmap.gsi_irq = iso->gsi;
			log_printf("IOAPIC %x = LEGACY %x\n", iso->gsi, iso->irq_source);
			state.legacymaps[state.n_legacymaps] = lmap;
			state.n_legacymaps++;
			break; 
		default:
			break;
		}
	}
	
	map_pages(LAPIC_REMAP_DESTINATION, lapic_base, 1, PG_WRITE | PG_PCD);
}