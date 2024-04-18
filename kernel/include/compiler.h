
#ifndef _COMPILER_H
#define _COMPILER_H

#define __noreturn __attribute__((noreturn))
#define __align(x) __attribute__((aligned(x)))
#define unreachable() __builtin_unreachable()
#define __alias(old) __attribute__((alias(#old)))

#endif