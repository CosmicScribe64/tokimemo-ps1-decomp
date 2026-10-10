#include "common.h"
#include "ovl/TT.h"

void func_80148210(void) {
    s32 i;
    u8 *p;

    i = 0;
    p = D_80158AA8;
    for (; i < 0x80; i++) {
        *p = 0;
        p += 0x14;
    }
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80148210", func_80148244);

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80148210", func_80148460);

void func_80148584(u16 arg0, s32 arg1) {
    u32 t = arg0;
    u8 *p = D_8015694C[arg1];

    if (t == 0x8CA0) {
        p[0] = 9;
        p[2] = 5;
        p[3] = 9;
        p[5] = 9;
        p[6] = 9;
        return;
    }
    p[0] = t / 3600;
    p[2] = (t % 3600) / 600;
    p[3] = (t % 600) / 60;
    p[5] = (t % 60) * 10 / 60;
    p[6] = t * 10 / 6 % 10;
}

void func_801486D8(s32 arg0, s32 arg1) {
    func_800AE0A0(D_8015694C[arg1], arg0, 3);
}

void func_80148714(u16 arg0, s32 arg1) {
    u8 *p = D_8015694C[arg1];

    arg0 = arg0 % 100;
    p[6] = arg0 / 10;
    p[7] = arg0 % 10;
}

INCLUDE_ASM("asm/ovl/TT/nonmatchings/TT/80148210", func_80148764);
