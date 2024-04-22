

ENTRY(pmode_entry);

__KERNEL_START_LOW = 1M;

__KERNEL_TEXT = -2048M;

SECTIONS {
	. = __KERNEL_START_LOW;

	.multiboot : {
		* ( .multiboot )
	} :multiboot

	__KERNEL_TEXT_START_LOW = .;

	.text.low : {
		* ( .text.low )
	} :low_text
	
	.data.low : {
		* ( .data.low )
	} :low_data
	
	.bss.low : {
		* ( .data.low )
	} :low_data

	. = ALIGN(2M);

	__KERNEL_TEXT_PHYSICAL = .;
	. = __KERNEL_TEXT;

	__KERNEL_RELOC_OFFSET = __KERNEL_TEXT - __KERNEL_TEXT_PHYSICAL;

	.text : AT ( ADDR(.text) - __KERNEL_RELOC_OFFSET ) {
		* ( .text )
	} :text

	.data : AT ( ADDR(.data) - __KERNEL_RELOC_OFFSET ) {
		* ( .data )
	} :data

	.rodata : AT ( ADDR(.rodata) - __KERNEL_RELOC_OFFSET ) {
		* ( .rodata )
	} :data

	.bss : AT ( ADDR(.bss) - __KERNEL_RELOC_OFFSET ) {
		* ( .bss )
		* ( .lbss )
		* ( COMMON )
	} :data

	.export.sym : AT ( ADDR( .export.sym ) - __KERNEL_RELOC_OFFSET ) {
		* ( .export.sym )
	} :data
	__KERNEL_EXPORT_SYM_BEGIN = ADDR(.export.sym);
	__KERNEL_EXPORT_SYM_END = .;

	. = ALIGN(4K);

	__KERNEL_END = .;
	__KERNEL_SIZE = __KERNEL_END - __KERNEL_TEXT;
}

PHDRS {
	headers PT_PHDR PHDRS;

	multiboot PT_LOAD FILEHDR PHDRS;
	low_text PT_LOAD;
	low_data PT_LOAD;
	text PT_LOAD;
	data PT_LOAD;
}