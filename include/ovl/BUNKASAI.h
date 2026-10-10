#ifndef OVL_BUNKASAI_H
#define OVL_BUNKASAI_H

#include "common.h"
#include "game.h"

/* BUNKASAI overlay (load address 0x80132000): externs and types. */

extern u32 D_801604F0; /* three data pointers set by the 0x34-byte setters at the start */
extern u32 D_801604F4;
extern u32 D_801604F8;

extern s32 D_8015D774;

/* defined in C in one object, called from another (T-0500) */
void func_80132000(void);
void func_80132034(void);
void func_80132068(void);
void func_8013209C(void);
void func_801320D0(void);
void func_80132104(void);
void func_80132138(void);
void func_8013216C(void);
void func_801321A0(void);
void func_801321D4(void);
void func_80132208(void);
void func_8013223C(void);
void func_80132270(void);
void func_801322A4(void);
void func_801322D8(void);
void func_8013230C(void);
void func_80132340(void);
void func_80132374(void);

#endif
