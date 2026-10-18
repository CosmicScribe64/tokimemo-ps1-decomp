#ifndef PSX_TYPES_H
#define PSX_TYPES_H

#include "libgpu.h"

/* T-14200: PsyQ-compatible assignment of a rectangle packet size. */
#define setWH(p, _w, _h) ((p)->w = (_w), (p)->h = (_h))

/* Shared packet and geometry types for recovered rendering code. T-9610. */
/* PsyQ 3.x packet ABI, written here from field widths and GPU command order.
 * These are DMA packets: tag is one 32-bit word, including on LP64 hosts.
 * No SDK header text or initialization macros are imported. T-9610. */
typedef struct POLY_F3 {
    /* 0x00 */ u32 tag;
    /* 0x04 */ u8 r0;
    /* 0x05 */ u8 g0;
    /* 0x06 */ u8 b0;
    /* 0x07 */ u8 code;
    /* 0x08 */ s16 x0;
    /* 0x0A */ s16 y0;
    /* 0x0C */ s16 x1;
    /* 0x0E */ s16 y1;
    /* 0x10 */ s16 x2;
    /* 0x12 */ s16 y2;
} POLY_F3; /* size 0x14 */

typedef struct POLY_F4 {
    /* 0x00 */ u32 tag;
    /* 0x04 */ u8 r0;
    /* 0x05 */ u8 g0;
    /* 0x06 */ u8 b0;
    /* 0x07 */ u8 code;
    /* 0x08 */ s16 x0;
    /* 0x0A */ s16 y0;
    /* 0x0C */ s16 x1;
    /* 0x0E */ s16 y1;
    /* 0x10 */ s16 x2;
    /* 0x12 */ s16 y2;
    /* 0x14 */ s16 x3;
    /* 0x16 */ s16 y3;
} POLY_F4; /* size 0x18 */

typedef struct POLY_FT3 {
    /* 0x00 */ u32 tag;
    /* 0x04 */ u8 r0;
    /* 0x05 */ u8 g0;
    /* 0x06 */ u8 b0;
    /* 0x07 */ u8 code;
    /* 0x08 */ s16 x0;
    /* 0x0A */ s16 y0;
    /* 0x0C */ u8 u0;
    /* 0x0D */ u8 v0;
    /* 0x0E */ u16 clut;
    /* 0x10 */ s16 x1;
    /* 0x12 */ s16 y1;
    /* 0x14 */ u8 u1;
    /* 0x15 */ u8 v1;
    /* 0x16 */ u16 tpage;
    /* 0x18 */ s16 x2;
    /* 0x1A */ s16 y2;
    /* 0x1C */ u8 u2;
    /* 0x1D */ u8 v2;
    /* 0x1E */ u16 pad1;
} POLY_FT3; /* size 0x20 */

typedef struct POLY_FT4 {
    /* 0x00 */ u32 tag;
    /* 0x04 */ u8 r0;
    /* 0x05 */ u8 g0;
    /* 0x06 */ u8 b0;
    /* 0x07 */ u8 code;
    /* 0x08 */ s16 x0;
    /* 0x0A */ s16 y0;
    /* 0x0C */ u8 u0;
    /* 0x0D */ u8 v0;
    /* 0x0E */ u16 clut;
    /* 0x10 */ s16 x1;
    /* 0x12 */ s16 y1;
    /* 0x14 */ u8 u1;
    /* 0x15 */ u8 v1;
    /* 0x16 */ u16 tpage;
    /* 0x18 */ s16 x2;
    /* 0x1A */ s16 y2;
    /* 0x1C */ u8 u2;
    /* 0x1D */ u8 v2;
    /* 0x1E */ u16 pad1;
    /* 0x20 */ s16 x3;
    /* 0x22 */ s16 y3;
    /* 0x24 */ u8 u3;
    /* 0x25 */ u8 v3;
    /* 0x26 */ u16 pad2;
} POLY_FT4; /* size 0x28 */

typedef struct POLY_G3 {
    /* 0x00 */ u32 tag;
    /* 0x04 */ u8 r0;
    /* 0x05 */ u8 g0;
    /* 0x06 */ u8 b0;
    /* 0x07 */ u8 code;
    /* 0x08 */ s16 x0;
    /* 0x0A */ s16 y0;
    /* 0x0C */ u8 r1;
    /* 0x0D */ u8 g1;
    /* 0x0E */ u8 b1;
    /* 0x0F */ u8 pad1;
    /* 0x10 */ s16 x1;
    /* 0x12 */ s16 y1;
    /* 0x14 */ u8 r2;
    /* 0x15 */ u8 g2;
    /* 0x16 */ u8 b2;
    /* 0x17 */ u8 pad2;
    /* 0x18 */ s16 x2;
    /* 0x1A */ s16 y2;
} POLY_G3; /* size 0x1C */

typedef struct POLY_G4 {
    /* 0x00 */ u32 tag;
    /* 0x04 */ u8 r0;
    /* 0x05 */ u8 g0;
    /* 0x06 */ u8 b0;
    /* 0x07 */ u8 code;
    /* 0x08 */ s16 x0;
    /* 0x0A */ s16 y0;
    /* 0x0C */ u8 r1;
    /* 0x0D */ u8 g1;
    /* 0x0E */ u8 b1;
    /* 0x0F */ u8 pad1;
    /* 0x10 */ s16 x1;
    /* 0x12 */ s16 y1;
    /* 0x14 */ u8 r2;
    /* 0x15 */ u8 g2;
    /* 0x16 */ u8 b2;
    /* 0x17 */ u8 pad2;
    /* 0x18 */ s16 x2;
    /* 0x1A */ s16 y2;
    /* 0x1C */ u8 r3;
    /* 0x1D */ u8 g3;
    /* 0x1E */ u8 b3;
    /* 0x1F */ u8 pad3;
    /* 0x20 */ s16 x3;
    /* 0x22 */ s16 y3;
} POLY_G4; /* size 0x24 */

typedef struct POLY_GT3 {
    /* 0x00 */ u32 tag;
    /* 0x04 */ u8 r0;
    /* 0x05 */ u8 g0;
    /* 0x06 */ u8 b0;
    /* 0x07 */ u8 code;
    /* 0x08 */ s16 x0;
    /* 0x0A */ s16 y0;
    /* 0x0C */ u8 u0;
    /* 0x0D */ u8 v0;
    /* 0x0E */ u16 clut;
    /* 0x10 */ u8 r1;
    /* 0x11 */ u8 g1;
    /* 0x12 */ u8 b1;
    /* 0x13 */ u8 p1;
    /* 0x14 */ s16 x1;
    /* 0x16 */ s16 y1;
    /* 0x18 */ u8 u1;
    /* 0x19 */ u8 v1;
    /* 0x1A */ u16 tpage;
    /* 0x1C */ u8 r2;
    /* 0x1D */ u8 g2;
    /* 0x1E */ u8 b2;
    /* 0x1F */ u8 p2;
    /* 0x20 */ s16 x2;
    /* 0x22 */ s16 y2;
    /* 0x24 */ u8 u2;
    /* 0x25 */ u8 v2;
    /* 0x26 */ u16 pad2;
} POLY_GT3; /* size 0x28 */

typedef struct POLY_GT4 {
    /* 0x00 */ u32 tag;
    /* 0x04 */ u8 r0;
    /* 0x05 */ u8 g0;
    /* 0x06 */ u8 b0;
    /* 0x07 */ u8 code;
    /* 0x08 */ s16 x0;
    /* 0x0A */ s16 y0;
    /* 0x0C */ u8 u0;
    /* 0x0D */ u8 v0;
    /* 0x0E */ u16 clut;
    /* 0x10 */ u8 r1;
    /* 0x11 */ u8 g1;
    /* 0x12 */ u8 b1;
    /* 0x13 */ u8 p1;
    /* 0x14 */ s16 x1;
    /* 0x16 */ s16 y1;
    /* 0x18 */ u8 u1;
    /* 0x19 */ u8 v1;
    /* 0x1A */ u16 tpage;
    /* 0x1C */ u8 r2;
    /* 0x1D */ u8 g2;
    /* 0x1E */ u8 b2;
    /* 0x1F */ u8 p2;
    /* 0x20 */ s16 x2;
    /* 0x22 */ s16 y2;
    /* 0x24 */ u8 u2;
    /* 0x25 */ u8 v2;
    /* 0x26 */ u16 pad2;
    /* 0x28 */ u8 r3;
    /* 0x29 */ u8 g3;
    /* 0x2A */ u8 b3;
    /* 0x2B */ u8 p3;
    /* 0x2C */ s16 x3;
    /* 0x2E */ s16 y3;
    /* 0x30 */ u8 u3;
    /* 0x31 */ u8 v3;
    /* 0x32 */ u16 pad3;
} POLY_GT4; /* size 0x34 */

typedef struct SPRT {
    /* 0x00 */ u32 tag;
    /* 0x04 */ u8 r0;
    /* 0x05 */ u8 g0;
    /* 0x06 */ u8 b0;
    /* 0x07 */ u8 code;
    /* 0x08 */ s16 x0;
    /* 0x0A */ s16 y0;
    /* 0x0C */ u8 u0;
    /* 0x0D */ u8 v0;
    /* 0x0E */ u16 clut;
    /* 0x10 */ s16 w;
    /* 0x12 */ s16 h;
} SPRT; /* size 0x14 */

typedef struct SPRT_8 {
    /* 0x00 */ u32 tag;
    /* 0x04 */ u8 r0;
    /* 0x05 */ u8 g0;
    /* 0x06 */ u8 b0;
    /* 0x07 */ u8 code;
    /* 0x08 */ s16 x0;
    /* 0x0A */ s16 y0;
    /* 0x0C */ u8 u0;
    /* 0x0D */ u8 v0;
    /* 0x0E */ u16 clut;
} SPRT_8; /* size 0x10 */

typedef struct SPRT_16 {
    /* 0x00 */ u32 tag;
    /* 0x04 */ u8 r0;
    /* 0x05 */ u8 g0;
    /* 0x06 */ u8 b0;
    /* 0x07 */ u8 code;
    /* 0x08 */ s16 x0;
    /* 0x0A */ s16 y0;
    /* 0x0C */ u8 u0;
    /* 0x0D */ u8 v0;
    /* 0x0E */ u16 clut;
} SPRT_16; /* size 0x10 */

typedef struct TILE {
    /* 0x00 */ u32 tag;
    /* 0x04 */ u8 r0;
    /* 0x05 */ u8 g0;
    /* 0x06 */ u8 b0;
    /* 0x07 */ u8 code;
    /* 0x08 */ s16 x0;
    /* 0x0A */ s16 y0;
    /* 0x0C */ s16 w;
    /* 0x0E */ s16 h;
} TILE; /* size 0x10 */

typedef struct TILE_1 {
    /* 0x00 */ u32 tag;
    /* 0x04 */ u8 r0;
    /* 0x05 */ u8 g0;
    /* 0x06 */ u8 b0;
    /* 0x07 */ u8 code;
    /* 0x08 */ s16 x0;
    /* 0x0A */ s16 y0;
} TILE_1; /* size 0x0C */

typedef struct TILE_8 {
    /* 0x00 */ u32 tag;
    /* 0x04 */ u8 r0;
    /* 0x05 */ u8 g0;
    /* 0x06 */ u8 b0;
    /* 0x07 */ u8 code;
    /* 0x08 */ s16 x0;
    /* 0x0A */ s16 y0;
} TILE_8; /* size 0x0C */

typedef struct TILE_16 {
    /* 0x00 */ u32 tag;
    /* 0x04 */ u8 r0;
    /* 0x05 */ u8 g0;
    /* 0x06 */ u8 b0;
    /* 0x07 */ u8 code;
    /* 0x08 */ s16 x0;
    /* 0x0A */ s16 y0;
} TILE_16; /* size 0x0C */

typedef struct DR_MODE {
    /* 0x00 */ u32 tag;
    /* 0x04 */ u32 code[2];
} DR_MODE; /* size 0x0C */

typedef struct DR_TWIN {
    /* 0x00 */ u32 tag;
    /* 0x04 */ u32 code[2];
} DR_TWIN; /* size 0x0C */

typedef struct DR_AREA {
    /* 0x00 */ u32 tag;
    /* 0x04 */ u32 code[2];
} DR_AREA; /* size 0x0C */

typedef struct DR_OFFSET {
    /* 0x00 */ u32 tag;
    /* 0x04 */ u32 code[2];
} DR_OFFSET; /* size 0x0C */

typedef struct DR_MOVE {
    /* 0x00 */ u32 tag;
    /* 0x04 */ u32 code[5];
} DR_MOVE; /* size 0x18 */

typedef struct DR_ENV {
    /* 0x00 */ u32 tag;
    /* 0x04 */ u32 code[15];
} DR_ENV; /* size 0x40 */

/* PsyQ 3.4 image upload packet. */
typedef struct DR_LOAD {
    /* 0x00 */ u32 tag;
    /* 0x04 */ u32 code[3];
    /* 0x10 */ u32 p[13];
} DR_LOAD; /* size 0x44 */

/* PsyQ 3.5 texture-page packet. */
typedef struct DR_TPAGE {
    /* 0x00 */ u32 tag;
    /* 0x04 */ u32 code[1];
} DR_TPAGE; /* size 0x08 */

/* PsyQ 4.1 STP-control packet; provided for later SDK objects. */
typedef struct DR_STP {
    /* 0x00 */ u32 tag;
    /* 0x04 */ u32 code[2];
} DR_STP; /* size 0x0C */

#include "libgte.h"

/* Two-colour line DMA packet; offsets from func_80049FF0 (T-24041). */
typedef struct LINE_G2 {
    /* 0x00 */ u32 tag;
    /* 0x04 */ u8 r0;
    /* 0x05 */ u8 g0;
    /* 0x06 */ u8 b0;
    /* 0x07 */ u8 code;
    /* 0x08 */ s16 x0;
    /* 0x0A */ s16 y0;
    /* 0x0C */ u8 r1;
    /* 0x0D */ u8 g1;
    /* 0x0E */ u8 b1;
    /* 0x0F */ u8 pad;
    /* 0x10 */ s16 x1;
    /* 0x12 */ s16 y1;
} LINE_G2; /* size 0x14 */

#endif /* PSX_TYPES_H */
