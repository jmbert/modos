

#include <acpi.h>
#include <mm/mmhelpers.h>
#include <string.h>
#include <panic.h>
#include <state.h>

#define BIOS_BEGINSEARCH 0xE0000
#define BIOS_ENDSEARCH 0xFFFFF

char rspd_sig[8] = ((char[8]){'R', 'S', 'D', ' ', 'P', 'T', 'R', ' '});
char madt_id[4] = ((char[4]){'A', 'P', 'I', 'C'});

void *find_table(struct rsdt *rsdt, char id[4]) {
	int entries = (rsdt->hdr.len - sizeof(rsdt->hdr)) / 4;
	for (int i = 0; i < entries; i++) {
    	    	struct sdthdr *hdr = (struct sdthdr *)(P2V(rsdt->sdt_ptrs[i]));
    	    	if (!memcmp(hdr->sig, id, 4)) {
    	        	return (void *)hdr;
		}
    	}
	
    	return NULL;
}

void init_acpi() {
	uintptr_t current_check = P2V(ebda_begin);
	for (size_t i = 0; i < EBDA_MAXSIZE; i += 16, current_check += 16) {
		int found = memcmp((void*)current_check, rspd_sig, 8);
		if (found == 0) {
			goto found_xsdp;
		}
	}
	current_check = P2V(BIOS_BEGINSEARCH);
	for (;current_check < P2V(BIOS_ENDSEARCH); current_check += 16) {
		int found = memcmp((void*)current_check, rspd_sig, 8);
		if (found == 0) {
			goto found_xsdp;
		}
	}

	panic("Couldn't find XSPD pointer\n");

found_xsdp:
	struct xsdp *xsdp = (struct xsdp*)current_check;
	log_printf("Found XSPD pointer\n");

	log_printf("OEM: %06s\n", xsdp->oemid);

	switch (xsdp->revision)
	{
	case ACPI_VER1:
		state.rsdt = (struct rsdt *)P2V(xsdp->rsdt_addr);
		break;
	default:
		panic("Unknown ACPI version %x\n", xsdp->revision);
		break;
	}

	state.madt = find_table(state.rsdt, madt_id);
	if (!state.madt) {
		panic("No APIC table found\n");
	}

	init_madt();
}