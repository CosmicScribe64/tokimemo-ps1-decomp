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

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E7ABC;
/* end bulk_m2c declarations */
s32 func_8014616C(void) {
    s32 temp_s0;

    temp_s0 = D_800E6280.unk_1104.w;
    if (D_800E6280.unk_1104.w == 1) {
        func_80144DF8(1);
        func_80147488(1, D_8015F82C, 9);
        D_80127080[1].unk0 = 0;
        D_80122754 = 0;
        D_80122750 = 0;
        D_8012274C = 0;
        func_80099540(&D_80122740);
    } else if ((D_800E6280.unk_1104.w >= 2) && (D_800E6280.unk_1104.w < 0x32)) {
        func_80147488(1, D_8015F82C, 0xA);
    } else if ((D_800E6280.unk_1104.w >= 0x32) && (D_800E6280.unk_1104.w < 0x64)) {
        func_80147488(1, D_8015F82C, 0xB);
    } else if ((D_800E6280.unk_1104.w >= 0x64) && (D_800E6280.unk_1104.w < 0x78)) {
        func_80147488(1, D_8015F82C, 0xC);
    } else if ((D_800E6280.unk_1104.w >= 0x78) && (D_800E6280.unk_1104.w < 0x91)) {
        func_80147488(1, D_8015F82C, 0xD);
    } else if ((D_800E6280.unk_1104.w >= 0x91) && (D_800E6280.unk_1104.w < 0xB4)) {
        func_80147488(1, D_8015F82C, 0xE);
    } else if ((D_800E6280.unk_1104.w >= 0xB4) && (D_800E6280.unk_1104.w < 0xC8)) {
        func_80147488(1, D_8015F82C, 0x10);
    } else if ((D_800E6280.unk_1104.w >= 0xC8) && (D_800E6280.unk_1104.w < 0xEB)) {
        func_80147488(1, D_8015F82C, 0x11);
    } else if ((D_800E6280.unk_1104.w >= 0xEB) && (D_800E6280.unk_1104.w < 0x11D)) {
        func_80147488(1, D_8015F82C, 0x12);
    } else if ((D_800E6280.unk_1104.w >= 0x11D) && (D_800E6280.unk_1104.w < 0x14F)) {
        func_80147488(1, D_8015F82C, 0x13);
    } else if ((D_800E6280.unk_1104.w >= 0x14F) && (D_800E6280.unk_1104.w < 0x1C2)) {
        func_80147488(1, D_8015F82C, 0x14);
    } else if ((D_800E6280.unk_1104.w >= 0x1CC) && (D_800E6280.unk_1104.w < 0x1EA)) {
        func_80147488(1, D_8015F82C, 0x15);
    } else if ((D_800E6280.unk_1104.w >= 0x1F4) && (D_800E6280.unk_1104.w < 0x208)) {
        func_80147488(1, D_8015F82C, 0x16);
    }
    if ((temp_s0 >= 0x64) && (temp_s0 < 0xBE)) {
        func_8014559C(0, -0x41, 0x32, 0x11, temp_s0 - 0x55, 0);
    }
    if ((temp_s0 >= 0xB4) && (temp_s0 < 0x118)) {
        func_8014559C(0, -0x1E, 0x1E, 0x4B, temp_s0 - 0xAA, 0);
    }
    if ((temp_s0 >= 0xAA) && (temp_s0 < 0x10E)) {
        func_8014559C(0, 0x32, 0x3C, 0xA, temp_s0 - 0xAA, 0);
    }
    if ((temp_s0 >= 0xBE) && (temp_s0 < 0x122)) {
        func_8014559C(0, -0x50, 0xA, 0x50, temp_s0 - 0xBE, 0);
    }
    if ((temp_s0 >= 0xEB) && (temp_s0 < 0x14F)) {
        func_8014559C(0, -0x3C, 0x19, 0x14, temp_s0 - 0xE1, 0);
    }
    if ((temp_s0 >= 0x10E) && (temp_s0 < 0x168)) {
        func_8014559C(0, -0x17, 0x1C, 0x1E, temp_s0 - 0xFA, 0);
    }
    if ((temp_s0 >= 0x11D) && (temp_s0 < 0x181)) {
        func_8014559C(0, 0x14, 0x23, 0x28, temp_s0 - 0x113, 0);
    }
    if ((temp_s0 >= 0x136) && (temp_s0 < 0x19A)) {
        func_8014559C(0, 0x3C, 0x2D, 0x1E, temp_s0 - 0x12C, 0);
    }
    if ((temp_s0 >= 0x154) && (temp_s0 < 0x1B8)) {
        func_8014559C(0, -0x50, 0x3C, 0x14, temp_s0 - 0x140, 0);
    }
    if ((temp_s0 >= 0x168) && (temp_s0 < 0x1CC)) {
        func_8014559C(0, 0, 0x14, 0x1E, temp_s0 - 0x168, 0);
    }
    if ((temp_s0 >= 0x186) && (temp_s0 < 0x1EA)) {
        func_8014559C(0, 0x28, 0x32, 0x1E, temp_s0 - 0x186, 0);
    }
    if ((temp_s0 >= 0x1AE) && (temp_s0 < 0x212)) {
        func_8014559C(0, -0x5A, 0x14, 0x1E, temp_s0 - 0x1AE, 0);
    }
    if ((temp_s0 >= 0x1CC) && (temp_s0 < 0x230)) {
        func_8014559C(0, -0x28, 0x28, 0x1E, temp_s0 - 0x1CC, 0);
    }
    if ((temp_s0 >= 0x1E0) && (temp_s0 < 0x244)) {
        func_8014559C(0, 0x32, 0x3C, 5, temp_s0 - 0x1E0, 0);
    }
    if (temp_s0 == 0xA) {
        func_801335A0(0, 0x505);
    }
    if (temp_s0 == 0x64) {
        func_801335A0(0, 0x506);
    }
    if (temp_s0 == 0xDC) {
        func_801335A0(0, 0x506);
    }
    if (temp_s0 == 0x190) {
        func_801335A0(0, 0x507);
    }
    if (temp_s0 == 0x1E0) {
        func_801335A0(0, 0x505);
    }
    if (temp_s0 < 0x244) {
        func_801449D8((s32) &D_800E6280.unk_163C[0], -0xA, 0);
        func_801449D8((s32) &D_800E7ABC, -8, 0);
    }
    if (temp_s0 == 0x280) {
        D_80127080[1].unk0 = 0x80000000;
        func_8004284C();
    }
    D_800E6280.unk_1104.w += 1;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/801447D0", func_80146754);
