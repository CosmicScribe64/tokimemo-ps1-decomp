#ifndef OVL_OPTION_H
#define OVL_OPTION_H

#include "common.h"

/* Main-exe data and functions used by OPTION. */
extern u8 D_800E738A;
s32 func_8004500C();

/* OPTION overlay data (bss, not in the overlay file). */
extern u8 D_8013D3A8;
extern u8 D_8013D3B0;
extern u8 D_8013D3B4;

void func_80132AB8(void);

void func_80042878(s32 arg0);
void func_80042908(s32 arg0);
void func_80042940(s32 arg0);
s32 func_800460CC(void);
s32 func_80044E8C();
void func_80048EB8(s32 arg0);
void func_8004955C(s32 arg0);
void func_80049A40();
extern u8 D_800E71DF;
extern u8 D_800E7D68;
void func_8013A09C(void);
void func_8013A1BC(void);
void func_801320C0(void);
#endif /* OVL_OPTION_H */
