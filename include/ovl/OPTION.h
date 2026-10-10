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
void dtd_on(s32 arg0);
void func_80049A40();
extern u8 D_800E71DF;
extern u8 D_800E7D68;
void func_8013A09C(void);
void func_8013A1BC(void);
void func_801320C0(void);
extern u8 D_800E62BA;
void func_8004E58C(void);
void func_8013A484(void);
void func_80133CB8(void);
void func_80133F50(void);
void func_801342A8(void);
void func_8013554C(void);
void func_801359A4(void);
void func_80135C0C(void);
void func_80136688(void);
void func_801368FC(void);
void func_80136A50(void);
void func_80042808(void);
extern u8 D_800E738D;
void func_80046318(s32 arg0, s32 arg1, s32 arg2);
void func_80048E78(void);
void func_80041584(void);

#endif /* OVL_OPTION_H */
