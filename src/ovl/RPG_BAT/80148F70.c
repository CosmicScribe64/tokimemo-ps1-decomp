#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80148F70", func_80148F70);

void func_801495DC(void) {
    s32 rate;
    s32 lv;
    s32 base;
    s32 r;

    rate = D_800E6280.unk_10C.unk_02;
    lv = (rate > 300) ? 300 : rate;
    base = func_8013EA00(100, 100) + lv * 9 / 2;
    r = func_8013E9A8(0x1000);
    if (r == 0) {
        D_8015EBB0 = 9999;
    } else if (r < 0x40) {
        D_8015EBB0 = 0;
    } else if (r < 0x10E) {
        D_8015EBB0 = func_8013EA00(500, 1000) + base;
    } else {
        D_8015EBB0 = base;
    }
    if (D_8015EBB0 >= 10000) {
        D_8015EBB0 = 9999;
    }
    if ((D_8015EDC4 & 0x600000) && D_8015EBA4 == 0 && D_8015EBB0 != 0) {
        D_8015EDC0 -= 1500;
    }
}
