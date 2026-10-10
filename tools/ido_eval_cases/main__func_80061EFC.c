#include "common.h"
#include "game.h"
#include "main_only.h"

extern s8 D_800E62B6;
extern s8 D_800E62B7;
extern s8 D_800E62B8;
extern u8 D_801220E0;
extern u8 D_801220E1;
extern u8 D_801220E2;

void func_80061EFC(void) {
    if ((u8) D_800E62BA < 0x80U) {
        D_801220E1 = D_800E62BA;
        D_801220E2 = D_800E62BA;
        D_801220E0 = D_800E62BA;
        D_800E62BA += 1;
    }
    if ((D_800E738D == 0) && ((u32) D_800E7384 >= 0x259U)) {
        func_8004284C();
    }
    if (D_800E7208 & 0x860) {
        D_800E62B7 = 0xFF;
        D_800E62B6 = 0xFF;
        D_800E62B8 = 0xFF;
        back_clear_switch(1);
        func_80059048();
        D_800E62BA = 0x80;
        func_80042808();
    }
}
