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

#endif /* OVL_OPTION_H */
