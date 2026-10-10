#include "common.h"
#include "ovl/SHUGAKU.h"

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80138A60", func_80138A60);

void func_80138ADC(void) {
    func_8004E884(func_8004E788(-0x80, 0x30, 2, "【団体行動中】", 0));
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80138A60", func_80138B20);

void func_80138BD8(void) {
    func_801386C4();
    if (D_800E6280.unk_F5F == 4 && ((u32)D_800E6280.unk_1BC[4].unk_0C.b[2] >> 4) == ((u32)D_800E6280.unk_0F4.h >> 0xC)) {
        *(u8 *)(*(s32 *)(D_800CA2D4 + D_800CA2DC * 4) + 3) = 0xFF;
    }
}

void func_80138C48(void) {
    if (D_800E6280.unk_110D == 0) {
        if (D_8013C97C == 4 && ((u32)D_800E6280.unk_1BC[4].unk_0C.b[2] >> 4) == ((u32)D_800E6280.unk_0F4.h >> 0xC)) {
            D_800CA2DC = 0x2E;
        }
        D_800E6280.unk_110D = 1;
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
    if (D_800E6280.unk_F5F != 3 || D_800CA2CC != 1) {
        normal_date_girl_out();
        return;
    }
    D_800E6280.unk_721 = D_800E6280.unk_1109 + 1;
    D_800E6280.unk_722 = 0;
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
        if (D_800E6280.unk_F5F == 2 || D_800E6280.unk_F5F == 7 || D_800E6280.unk_F5F == 9) {
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
        D_800E6280.unk_110A += 0x11;
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
        D_800E6280.unk_F5F = 0xD;
        func_800847B8(0xDU);
        D_800E6280.unk_110A = 0x33;
        return;
    }
    func_8004284C();
}

void func_801395DC(void) {
    D_800CA2DC = 0xE;
    func_8004284C();
}

void func_80139604(void) {
    s32 var_v0;
    u8 x;
    u32 t;

    if (D_800CA2EC == 0) {
        x = func_80051A68(D_800E6280.unk_F5F);
        t = x & 0x7F;
        if (t < 2U) {
            var_v0 = 0;
        } else if (t == 2) {
            var_v0 = 3;
        } else if (t == 3) {
            var_v0 = 6;
        } else {
            var_v0 = 9;
        }
        D_800CA2DC += var_v0;
    }
    func_8004284C();
}

void func_80139698(void) {
    D_800CA2DC = D_800CA2CC + 0x1A;
    if (D_800CA2EC != 0) {
        D_800CA2DC = D_800CA2CC + 3;
    }
    D_800E6280.unk_110A -= 0x2E;
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
    D_800E6280.unk_110A = 0x33;
}

void func_801398B8(void) {
    get_g_zyotai_s(D_800E6280.unk_F5F);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80138A60", func_801398E8);

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80138A60", func_801399F4);

typedef struct {
    void (*f[18])();
} FnTbl18; /* size 0x48 */
extern FnTbl18 D_8013CAA0;

void func_80139C80(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl18 tbl;

    tbl = D_8013CAA0;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}
