#include "common.h"
#include "ovl/TACO.h"

void func_801447D0(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8014488C();
        break;
    case 1:
        func_80145DC4();
        break;
    case 2:
        func_8014616C();
        break;
    case 3:
        func_80146754();
        break;
    case 4:
        func_80144998();
        break;
    }
    if (D_800E6280.unk_110A != 5) {
        if (D_800E6280.unk_F88 & 0x860) {
            func_80042940(4);
        }
    }
}

void func_8014488C(void) {
    (&D_800E6280.unk_036)[0] = (&D_800E6280.unk_036)[1] = 8;
    D_800E6280.unk_038 = 0x10;
    draw2d3d(1, 1);
    func_80058D20();
    func_80146E3C();
    func_80146D60(-0xFA0);
    func_801471D0(0x8019C800);
    func_80144CC0((u8 *)D_800E6280.unk_163C);
    func_80144CC0(D_800E6280.unk_183C);
    func_8009C674(0);
    func_80144D90(7);
    func_80147488(1, D_8015F82C, 0);
    D_80127090.unk0 = 0;
    D_80128880[1].x = -0x5A;
    D_80128880[1].z = 0;
    D_80128880[1].y = 0x5A;
    func_80147488(2, D_8015F830, 0);
    D_801270A0.unk0 = 0;
    D_80128880[2].x = -0x5A;
    D_80128880[2].z = 0;
    D_80128880[2].y = -0x5A;
    func_8004284C();
}

void func_80144998(void) {
    D_80127090.unk0 = 0x80000000;
    D_801270A0.unk0 = 0x80000000;
    draw2d3d(1, 0);
    func_80042908(2);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/801447D0", func_801449D8);

void func_80144CC0(u8 *arg0) {
    s32 i;

    for (i = 0; i != 0x19; i++) {
        *(s32 *)(arg0 + i * 8) = func_800AE0D0() % 320;
        *(s32 *)(arg0 + i * 8 + 4) = func_800AE0D0() % 240 - 0x78;
    }
}

void func_80144D90(s32 arg0) {
    RECT r;

    if (arg0 < 0) {
        arg0 = 0;
    } else if (arg0 >= 8) {
        arg0 = 7;
    }
    /* FAKE: the assignment pairs share a source line each; as1 orders the stores by line (T-3330), T-6050 */
    r.x = arg0 * 8; r.y = 0x1F1;
    r.w = 8; r.h = 1;
    func_8009C93C(&r, 0xF8, 0x1F0);
}

void func_80144DF8(s32 arg0) {
    TcObj50 *o;
    TcPos *p;

    o = &D_80127480[arg0];
    p = &D_80128880[arg0];
    o->unk18 = 0;
    o->unk1C = 0;
    o->unk20 = 0;
    p->x = 0;
    p->y = 0;
    p->z = 0;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/801447D0", func_80144E3C);

void func_8014559C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    if ((arg4 >= 0) && (arg4 < 0x1F)) {
        func_80144E3C(arg0, arg1, arg2, arg3, (arg4 & 0xFF) * 8, 0, arg5);
        return;
    }
    if ((arg4 >= 0x1F) && (arg4 < 0x23)) {
        func_80144E3C(arg0, arg1, arg2, arg3, 0, 2, arg5);
        return;
    }
    if ((arg4 >= 0x23) && (arg4 < 0x61)) {
        func_80144E3C(arg0, arg1, arg2, arg3, ((arg4 - 0x23) & 0xFF) * 4, 1, arg5);
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/801447D0", func_80145650);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/801447D0", func_80145DC4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/801447D0", func_8014616C);

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E7384;
extern s32 D_800E78BC;
extern s32 D_800E7ABC;
/* end bulk_m2c declarations */
s32 func_80146754(void) {
    s32 sp3C;
    s32 temp_a1;
    s32 temp_s0;

    temp_s0 = D_800E7384;
    if (D_800E7384 == 1) {
        func_80144DF8(2);
        func_80147488(2, D_8015F830, 9);
        D_801270A0.unk0 = 0;
        D_80122754 = 0;
        D_80122750 = 0;
        D_8012274C = 0;
        func_80099540(&D_80122740);
    } else if ((D_800E7384 >= 2) && (D_800E7384 < 0x64)) {
        func_80147488(2, D_8015F830, 0xA);
    } else if ((D_800E7384 >= 0x65) && (D_800E7384 < 0x78)) {
        func_80147488(2, D_8015F830, 0xB);
    } else if ((D_800E7384 >= 0x79) && (D_800E7384 < 0x8C)) {
        func_80147488(2, D_8015F830, 0xC);
    } else if ((D_800E7384 >= 0x8D) && (D_800E7384 < 0xA0)) {
        func_80147488(2, D_8015F830, 0xD);
    } else if ((D_800E7384 >= 0xA1) && (D_800E7384 < 0xB4)) {
        func_80147488(2, D_8015F830, 0xE);
    } else if ((D_800E7384 >= 0xB5) && (D_800E7384 < 0xC8)) {
        func_80147488(2, D_8015F830, 0xF);
    } else if ((D_800E7384 >= 0xC9) && (D_800E7384 < 0xDC)) {
        func_80147488(2, D_8015F830, 0x10);
    } else if ((D_800E7384 >= 0xDD) && (D_800E7384 < 0xFA)) {
        func_80147488(2, D_8015F830, 0x11);
    } else if ((D_800E7384 >= 0xFB) && (D_800E7384 < 0x122)) {
        func_80147488(2, D_8015F830, 0x12);
    } else if ((D_800E7384 >= 0x123) && (D_800E7384 < 0x15E)) {
        func_80147488(2, D_8015F830, 0x13);
    } else if ((D_800E7384 >= 0x15F) && (D_800E7384 < 0x186)) {
        func_80147488(2, D_8015F830, 0x14);
    } else if ((D_800E7384 >= 0x187) && (D_800E7384 < 0x1A4)) {
        func_80147488(2, D_8015F830, 0x15);
    } else if ((D_800E7384 >= 0x1A5) && (D_800E7384 < 0x1F4)) {
        func_80147488(2, D_8015F830, 0x16);
    }
    if ((temp_s0 >= 0x32) && (temp_s0 < 0x96)) {
        func_8014559C(0x14, -0xAA, 0x1E, 0x14, temp_s0 - 0x1E, 0x3C);
    }
    if ((temp_s0 >= 0x69) && (temp_s0 < 0xC8)) {
        func_8014559C(0xAA, -0xAA, 0x50, 8, temp_s0 - 0x50, 0x3C);
    }
    if ((temp_s0 >= 0x78) && (temp_s0 < 0xDC)) {
        func_8014559C(-0x1E, -0xAA, 0x50, 0xA, temp_s0 - 0x78, 0x3C);
    }
    if ((temp_s0 >= 0xB4) && (temp_s0 < 0x118)) {
        func_8014559C(0xC8, -0xAA, 0x32, 0xF, temp_s0 - 0xAA, 0x3C);
    }
    if ((temp_s0 >= 0xDC) && (temp_s0 < 0x140)) {
        func_8014559C(0x5A, -0xAA, 0x3C, 0xA, temp_s0 - 0xD2, 0x3C);
    }
    if ((temp_s0 >= 0x122) && (temp_s0 < 0x136)) {
        func_8014559C(0xF, -0x96, 0x2D, 0xA, temp_s0 - 0x118, 0x3C);
    }
    if ((temp_s0 >= 0x136) && (temp_s0 < 0x186)) {
        func_8014559C((temp_s0 * 2) - 0x25D, -0x96, 0x2D, 0xA, temp_s0 - 0x118, ((s32) (temp_s0 - 0x136) / 3) + 0x3C);
    }
    if ((temp_s0 >= 0x190) && (temp_s0 < 0x1F4)) {
        func_8014559C(0x15E, -0x96, 0x50, 0xA, temp_s0 - 0x186, 0x78);
    }
    if ((temp_s0 >= 0x1A4) && (temp_s0 < 0x1F4)) {
        func_8014559C(0xDC, -0x96, 0x3C, 0x14, temp_s0 - 0x19A, 0x78);
    }
    if ((temp_s0 >= 0x1B8) && (temp_s0 < 0x21C)) {
        func_8014559C(0x96, -0x96, 0x28, 0x14, temp_s0 - 0x1AE, 0x78);
    }
    if (temp_s0 == 0x64) {
        func_801335A0(0, 0x507);
    }
    if (temp_s0 == 0xE6) {
        func_801335A0(0, 0x505);
    }
    if (temp_s0 == 0x12C) {
        func_801335A0(0, 0x506);
    }
    if (temp_s0 == 0x190) {
        func_801335A0(0, 0x505);
    }
    if (temp_s0 < 0x12C) {
        func_801449D8((s32) &D_800E78BC, -5, -0xA);
        func_801449D8((s32) &D_800E7ABC, -5, -8);
    } else if ((temp_s0 >= 0x12C) && (temp_s0 < 0x190)) {
        temp_a1 = ((s32) (temp_s0 - 0x12C) / 10) - 5;
        sp3C = temp_a1;
        func_801449D8((s32) &D_800E78BC, temp_a1, -0xA);
        func_801449D8((s32) &D_800E7ABC, temp_a1, -8);
    } else if (temp_s0 >= 0x190) {
        func_801449D8((s32) &D_800E78BC, 5, -0xA);
        func_801449D8((s32) &D_800E7ABC, 5, -8);
    }
    if (temp_s0 == 0x226) {
        func_80042940(4);
    }
    D_800E7384 += 1;
}
