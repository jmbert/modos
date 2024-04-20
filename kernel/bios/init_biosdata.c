


#include <bios.h>
#include <seriallog.h>
#include <log.h>

uint32_t ebda_begin;

void init_biosdata() {
	ebda_begin = BDA_WORD(0x13) << 0xA;
	for (size_t i = 0; i < 4; i++) {
		uint16_t serial = BDA_WORD(i);
		if (serial) {
			serial_port = serial;
			break;
		}
	}
	
	log_printf("EBDA Begin: %x\n", ebda_begin);
	log_printf("Serial Port: %04x\n", serial_port);
}