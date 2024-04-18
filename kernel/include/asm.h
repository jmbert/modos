

#ifndef _ASM_H
#define _ASM_H

#define OPREG_8L(opn) "%b" #opn
#define OPREG_8H(opn) "%h" #opn
#define OPREG_16(opn) "%w" #opn
#define OPREG_32(opn) "%k" #opn
#define OPREG_64(opn) "%q" #opn

#endif