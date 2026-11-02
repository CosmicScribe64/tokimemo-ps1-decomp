#ifndef ENGLISH_NAMES_H
#define ENGLISH_NAMES_H

#include "common.h"

/* Stable save-compatible pair codes, T-39506. NULL for an ordinary byte. */
u8 *english_name_pair(u32 code);
/* Separate buffers required. Return encoded/decoded byte count, or -1 when
 * size is insufficient. Output is always terminated when size is positive. */
s32 english_name_pack(u8 *dst, u8 *src, s32 size);
s32 english_name_unpack(u8 *dst, u8 *src, s32 size);

#endif
