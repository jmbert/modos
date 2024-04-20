
#ifndef _COMPILER_H
#define _COMPILER_H

#define __noreturn __attribute__((noreturn))
#define __align(x) __attribute__((aligned(x)))
#define unreachable() __builtin_unreachable()
#define __alias(old) __attribute__((alias(#old)))
#define __page_align __align(0x1000)
#define __section(name) __attribute__((section(#name)))
#define __option(...) ___attribute__((target(__VA_ARGS__)))
#define __packed __attribute__((packed))
#define __noinline __attribute__((noinline))

#endif