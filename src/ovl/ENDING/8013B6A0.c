#include "common.h"
#include "ovl/ENDING.h"

void func_8013B6A0(void) {
    D_8013CBB0 = 0x801C22CC;
    D_8013CBB4 = 0x801C22D4;
    D_8013CBB8 = 0x801C22F0;
    D_8013CBBC = *(s16 *)0x801C2300;
    D_8013CBC0 = 0x801A0000;
    D_8013CBC4 = 0x801C2000;
    D_8013CBC8 = 0x801AA000;
    D_8013CBCC = 0x801AE000;
    D_8013CBD0 = 0x801B2000;
    D_8013CBD4 = 0x801B6000;
    D_8013CBD8 = 0x801BA000;
    D_8013CBDC = 0x801BE000;
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/8013B6A0", func_8013B760);

void func_8013B868(void) {
    func_80042908(6);
}

void func_8013B888(void) {
    func_80042908(8);
}

void func_8013B8A8(void) {
    if (D_800E683E != 0) {
        func_8004284C();
        return;
    }
    normal_date_girl_out();
}

void func_8013B8E4(void) {
    func_80046318(0x45, 0x801A0000, 0x727A);
    func_8013B6A0();
    func_8004284C();
}

s32 func_8013B91C(void) {
    if (D_800E683E != 0) {
        if ((u32) D_800E7384++ < 0x78U) {
            return 0;
        }
    }
    D_800E71DF = 0xD;
    D_800CA148 = 0;
    D_800CA14C = 0;
    func_800847B8(D_800E683E);
    func_80132000();
    D_800CA160 = D_8013C280;
    D_800CA164 = D_8013C2C4;
    D_800CA168 = D_8013C308;
    if (D_800E683E != 0) {
        D_800CA148 = 2;
        D_800E738A += 0xA;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/8013B6A0", func_8013B9F0);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/8013B6A0", func_8013BB9C);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/8013B6A0", func_8013BC58);

void func_8013BDD8(s16 arg0) {
    u8 *p;

    p = D_8013CC48 + arg0 * 0x19;
    k_disp_start(set_kanji_string((s16) (-func_800AE0E0(p) * 7 / 2), 0x50, 0, p, 0));
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/8013B6A0", func_8013BE64);
