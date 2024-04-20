

#ifndef _ACPI_H
#define _ACPI_H

#include <stdint.h>
#include <bios.h>
#include <compiler.h>

enum acpi_vers {
	ACPI_VER1 = 0,
	ACPI_VER2X = 1,
};

__packed struct xsdp {
	char sig[8];
	uint8_t checksum;
	char oemid[6];
	uint8_t revision;
	uint32_t rsdt_addr;

	uint32_t len;
	uint64_t xsdt_addr;
	uint8_t checksum_extended;
	uint8_t reserved[3];
};

__packed struct sdthdr {
	char sig[4];
	uint32_t len;
	uint8_t revision;
	uint8_t checksum;
	char oemid[6];
	char oemtabid[6];
	uint32_t oemrevision;
	uint32_t creatorid;
	uint32_t creatorrevision;
};

__packed struct rsdt {
	struct sdthdr hdr;
	uint32_t sdt_ptrs[];
};

enum madt_types {
	PROC_LOCALAPIC = 0,
	IOAPIC = 1,
	IOAPIC_ISO = 2,
	LAPIC_NMISRC = 3,
	LAPIC_NMI = 4,
	LAPIC_OVERRIDE = 5,
};

__packed struct madt_hdr {
	uint8_t type;
	uint8_t len;
};

__packed struct acpi_ioapic {
	struct madt_hdr hdr;
	uint8_t ioapic_id;
	uint8_t reserved;
	uint32_t ioapic_addr;
	uint32_t gsid;
};

__packed struct ioapiciso {
	struct madt_hdr hdr;
	uint8_t bus_source;
	uint8_t irq_source;
	uint32_t gsi;
	uint16_t flags;
};

__packed struct lapicoverride {
	struct madt_hdr hdr;
	uint16_t reserved;
	uint64_t addr;
};

__packed struct acpi_lapic {
	struct madt_hdr hdr;
	uint8_t acpi_pid;
	uint8_t lapic_id;
	uint32_t flags;
};

__packed struct madt {
	struct sdthdr hdr;
	uint32_t local_apic_addr;
	uint32_t flags;
	struct madt_hdr first_hdr;
};

void init_acpi();
void init_madt();

#endif