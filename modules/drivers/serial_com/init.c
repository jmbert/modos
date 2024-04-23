
#include <log.h>
#include <fs.h>
#include <state.h>
#include <bios.h>
#include <portio.h>

int init_serial_port(uint16_t port) {

   	write_8(port + 1, 0x00);    // Disable all interrupts
   	write_8(port + 3, 0x80);    // Enable DLAB (set baud rate divisor)
   	write_8(port + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
   	write_8(port + 1, 0x00);    //                  (hi byte)
   	write_8(port + 3, 0x03);    // 8 bits, no parity, one stop bit
   	write_8(port + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
   	write_8(port + 4, 0x0B);    // IRQs enabled, RTS/DSR set
   	write_8(port + 4, 0x1E);    // Set in loopback mode, test the serial chip
   	write_8(port + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)
	
   	// Check if serial is faulty (i.e: not same byte as sent)
   	if(read_8(port + 0) != 0xAE) {
   	   	return 1;
   	}
	
   	// If serial is not faulty set it in normal operation mode
   	// (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
   	write_8(port + 4, 0x0F);

	return 0;
}

void serial_blockread(uint16_t port) {
   	while ((read_8(port + 5) & 1) == 0);
}

void serial_blockwrite(uint16_t port) {
	while ((read_8(port + 5) & 0x20) == 0);
}

int64_t write_serial(struct file *file, void *buffer, size_t len, size_t offset) {
	if (file->vnode->minor > 3) {
		return -1;
	}
	uint64_t port_base = BDA_WORD(file->vnode->minor);
	size_t i = 0;
	for (; i < len; i++) {
		serial_blockwrite(port_base);
		write_8(port_base, ((char*)buffer)[i]);
	}
	return i;
}

int64_t read_serial(struct file *file, void *buffer, size_t len, size_t offset) {
	if (file->vnode->minor > 3) {
		return -1;
	}
	uint64_t port_base = BDA_WORD(file->vnode->minor);
	size_t i = 0;
	for (; i < len; i++) {
		serial_blockread(port_base);
		((char*)buffer)[i] = read_8(port_base);
	}
	return i;
}

int open_serial(struct vfs_node *node, struct file *file) {
	if (node->minor > 4) {
		return -1;
	}
	uint64_t port_base = BDA_WORD(node->minor);

	return init_serial_port(port_base);
}

int init_module() {
	struct fops serial_fops = (struct fops){
		.write = write_serial,
		.open = open_serial,
		.read = read_serial,
	};

	register_chrdev(MAJOR_SERIAL, &serial_fops);
	struct file *devdir = openat(current_process->cwd, "/dev");
	mkcdev(devdir->vnode, "com1", MAJOR_SERIAL, 0);

	return 0;
}