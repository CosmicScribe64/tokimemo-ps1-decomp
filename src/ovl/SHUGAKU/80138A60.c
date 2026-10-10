#include "common.h"
#include "ovl/SHUGAKU.h"

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80138A60", func_80138A60);

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80138A60", func_80138ADC);

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80138A60", func_80138B20);

void func_80138BD8(void) {
    func_801386C4();
    if (D_800E71DF == 4 && ((u32)D_800E652A >> 4) == ((u32)D_800E6374 >> 0xC)) {
        *(u8 *)(*(s32 *)(D_800CA2D4 + D_800CA2DC * 4) + 3) = 0xFF;
    }
}

void func_80138C48(void) {
    if (D_800E738D == 0) {
        if (D_8013C97C == 4 && ((u32)D_800E652A >> 4) == ((u32)D_800E6374 >> 0xC)) {
            D_800CA2DC = 0x2E;
        }
        D_800E738D = 1;
    }
    func_801386C4();
}

void func_80138CB8(void) {
    u8 **p;

    p = (u8 **)D_800CA2D4;
    if (p[D_800CA2DC][D_800CA2E0] == 9) {
        func_8004284C();
        return;
    }
    normal_date_girl_out();
}

void func_80138D24(void) {
    if (D_8013C974 == 1) {
        func_8004284C();
        return;
    }
    normal_date_girl_out();
}

void func_80138D64(void) {
    u8 **p;

    p = (u8 **)D_800CA2D4;
    if (p[D_800CA2DC][D_800CA2E0] == 9) {
        func_8004284C();
        D_8013C974 = 1;
        return;
    }
    normal_date_girl_in();
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80138A60", func_80138DD8);

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80138A60", func_80138E40);

void func_80138FAC(void) {
    if (D_800E71DF != 3 || D_800CA2CC != 1) {
        normal_date_girl_out();
        return;
    }
    D_800E69A1 = D_800E7389 + 1;
    D_800E69A2 = 0;
    func_80042908(9);
}

INCLUDE_RODATA("asm/ovl/SHUGAKU/data/SHUGAKU/80138A60.rodata", D_8013AEF0);

void func_80139018(void) {
    bg_read_sub2(0x4225);
    func_800AE0F0(D_800CA19C, &D_8013AEF0);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80138A60", func_80139054);

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80138A60", func_80139118);

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80138A60", func_801392B4);

void func_801393C4(void) {
    D_800CA2DC = (D_800CA2DC - D_800CA2CC) + 2;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80138A60", func_80139400);

void func_80139498(void) {
    if (D_80122CDC != 0) {
        if (D_800E71DF == 2 || D_800E71DF == 7 || D_800E71DF == 9) {
            func_80083418();
            return;
        }
        func_800833F0();
        return;
    }
    func_8004284C();
}

void func_80139508(void) {
    if (D_80122CDC != 0) {
        D_800E738A += 0x11;
    }
    func_8004284C();
}

void func_80139548(void) {
    if (D_80122CDC != 0) {
        D_800CA2DC = 0;
        D_800CA2D0 = D_8013C450;
        D_800CA2D4 = D_8013C484;
        D_800CA2D8 = D_8013C4B8;
        D_800CA2EC = 1;
        D_800E71DF = 0xD;
        func_800847B8(0xDU);
        D_800E738A = 0x33;
        return;
    }
    func_8004284C();
}

void func_801395DC(void) {
    D_800CA2DC = 0xE;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80138A60", func_80139604);

void func_80139698(void) {
    D_800CA2DC = D_800CA2CC + 0x1A;
    if (D_800CA2EC != 0) {
        D_800CA2DC = D_800CA2CC + 3;
    }
    D_800E738A -= 0x2E;
    func_8004284C();
}

void func_801396F4(void) {
    if (D_800CA2EC != 0) {
        func_80062CD0(0x6C32);
    } else {
        func_8004284C();
        func_8004284C();
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80138A60", func_80139740);

void func_80139870(void) {
    D_800CA2DC = 0;
    D_800CA2D0 = D_8013C450;
    D_800CA2D4 = D_8013C484;
    D_800CA2D8 = D_8013C4B8;
    D_800E738A = 0x33;
}

void func_801398B8(void) {
    get_g_zyotai_s(D_800E71DF);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80138A60", func_801398E8);

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80138A60", func_801399F4);

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80138A60", func_80139C80);
