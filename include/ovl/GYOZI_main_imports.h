#ifndef OVL_GYOZI_MAIN_IMPORTS_H
#define OVL_GYOZI_MAIN_IMPORTS_H

#include "common.h"

void func_8004C670();

void func_8004C6A0();

void func_8004DDD8();

void func_8004DE1C(void);

void func_8004DEAC();

void func_8004DEE4();

void func_8004EDE0();

void func_8004EDF4();

void func_8004EE18();

void func_8004F860();

void func_800504CC();

s32 func_80050AB8();

void func_80050D60();

void func_80051DD8();

void func_800530E0();

void func_80053EFC();

void func_80054864();

void func_8005493C();

void func_800549E8();

void *func_80054B60();

void func_8005ABC0();

void func_8005AEFC();

void func_8005B06C();

void func_8005B264();

s8 func_8005B28C();

s32 func_8005B32C();

u32 func_8005B368();

void func_8005BD20();

s32 func_8005E0E0();

u32 func_8005EBE0(s32 arg0); /* GYOZI calls only; the s32 parameter as in get_h_tokimeki, needed by func_8013907C (T-25120) */

void func_80081D30();

/* GYOZI calls this entry with an implicit-int ABI; semantics unknown. T-13202. */
s32 func_800BCE40();

#ifndef MAIN_API_OVERRIDE_func_800BCDF0
void func_800BCDF0();
#endif

#ifndef MAIN_API_OVERRIDE_func_800BCE10
void func_800BCE10();
#endif

s32 func_800BDC20();

#endif
