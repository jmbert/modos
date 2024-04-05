

ENTRY(pmode_entry);

KERNEL_START_LOW = 1M;

KERNEL_START = -2048M;

SECTIONS {
	. = KERNEL_START_LOW;

	.multiboot : {
		* ( .multiboot )
	} :multiboot

	KERNEL_TEXT_START_LOW = .;

	.text.low : {
		* ( .text.low )
	} :low_text
	
	.bss.low : {
		* ( .data.low )
	} :low_data

	KERNEL_LOW_END = .;
	. = KERNEL_START;

	.text : AT ( KERNEL_LOW_END ) {
		* ( .text )
	} :text
}

PHDRS {
	headers PT_PHDR PHDRS;

	multiboot PT_LOAD FILEHDR PHDRS;
	low_text PT_LOAD;
	low_data PT_LOAD;
	text PT_LOAD;
	data PT_LOAD;
}