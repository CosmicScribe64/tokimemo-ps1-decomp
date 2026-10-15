#ifndef COMMON_H
#define COMMON_H

/* Fixed-width types (CODING_STANDARDS.md section 8). PsyQ headers use
 * u_char/u_short/u_long (sys/types.h), so these names do not clash. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

/* Select include/macro.inc for INCLUDE_ASM; labels.inc (splat output for the
 * original assembler) is unused here. One path only. */
#define INCLUDE_ASM_USE_MACRO_INC 1
#include "include_asm.h"

#endif /* COMMON_H */
