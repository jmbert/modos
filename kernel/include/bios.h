

#ifndef _BIOS_H
#define _BIOS_H

#include <mm/mmhelpers.h>

#define BDA_BEGIN 0x400

#define EBDA_MAXSIZE 0x3000

#define BDA_ADDR(_offset) P2V(BDA_BEGIN + _offset)

#define BDA_WORD(_offset) (*((uint16_t*)(BDA_ADDR(_offset))))

extern uint32_t ebda_begin;

void init_biosdata();

#endif