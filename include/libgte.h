#ifndef LIBGTE_H
#define LIBGTE_H


#include "common.h"

/* PsyQ 3.x geometry ABI, independently expressed with fixed-width types.
 * MATRIX translations begin at 0x14 after the aligned 3x3 halfword matrix.
 * T-9610. */
typedef struct SVECTOR {
    /* 0x00 */ s16 vx;
    /* 0x02 */ s16 vy;
    /* 0x04 */ s16 vz;
    /* 0x06 */ s16 pad;
} SVECTOR; /* size 0x08 */

typedef struct VECTOR {
    /* 0x00 */ s32 vx;
    /* 0x04 */ s32 vy;
    /* 0x08 */ s32 vz;
    /* 0x0C */ s32 pad;
} VECTOR; /* size 0x10 */

typedef struct MATRIX {
    /* 0x00 */ s16 m[3][3];
    /* 0x12: implicit alignment padding */
    /* 0x14 */ s32 t[3];
} MATRIX; /* size 0x20 */

void InitGeom(void);
s32 rsin();
s32 func_800A0140(s32 a);
void RotMatrix();
void MulMatrix();
void ScaleMatrix();
void SetRotMatrix(MATRIX *m); /* returns nothing: an implicit int view adds a $v0 range (T-19009) */
s32 SquareRoot0(s32 a);
void func_800AB984();
void func_800ABA38();
void func_800ABAB8();

extern s32 D_800DE500[];

#endif /* LIBGTE_H */
