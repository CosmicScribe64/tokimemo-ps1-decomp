#include "common.h"
#include "ovl/TACO.h"

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80156A80", func_80156A80);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80156A80", func_80156F50);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80156A80", func_801570A4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80156A80", func_801571E8);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80156A80", func_801572D8);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80156A80", func_801573C8);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80156A80", func_8015776C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80156A80", func_80157AE0);

void func_80157CD4(s32 arg0) {
    switch ((arg0 + D_8015EDB4)->unk7C) {
    case 0:
        (arg0 + D_8015EDB4)->unk74 += (D_8015EDB4[0].unk74 - (arg0 + D_8015EDB4)->unk74) / 10;
        (arg0 + D_8015EDB4)->unk76 += (D_8015EDB4[0].unk76 - (arg0 + D_8015EDB4)->unk76) / 10;
        D_8015EDB4[arg0].unk78 = -0x2710;
        if ((arg0 + D_8015EDB4)->unk7A >= 0x3D) {
            (arg0 + D_8015EDB4)->unk7C += 1;
            D_8015EDB4[arg0].unk7A = 0;
        }
        break;
    case 1:
        if ((arg0 + D_8015EDB4)->unk7A >= 0x5B) {
            (arg0 + D_8015EDB4)->unk7C += 1;
            D_8015EDB4[arg0].unk7A = 0;
        }
        break;
    case 2:
        (arg0 + D_8015EDB4)->unk66 = 0x800 - ((arg0 + D_8015EDB4)->unk7A << 6);
        if ((arg0 + D_8015EDB4)->unk66 <= 0) {
            (arg0 + D_8015EDB4)->unk7C += 1;
            D_8015EDB4[arg0].unk7A = 0;
        }
        break;
    case 3:
        (arg0 + D_8015EDB4)->unk2 = 0xFD;
        break;
    }
}

void func_80157EA8(s32 arg0) {
    switch ((arg0 + D_8015EDB4)->unk7C) {
    case 0:
        (arg0 + D_8015EDB4)->unk74 += (D_8015EDB4[0].unk74 - (arg0 + D_8015EDB4)->unk74) / 10;
        (arg0 + D_8015EDB4)->unk76 += (D_8015EDB4[0].unk76 - (arg0 + D_8015EDB4)->unk76) / 10;
        D_8015EDB4[arg0].unk78 = -0x2742;
        if ((arg0 + D_8015EDB4)->unk7A >= 0x3D) {
            (arg0 + D_8015EDB4)->unk7C += 1;
            D_8015EDB4[arg0].unk7A = 0;
        }
        break;
    case 1:
        if ((arg0 + D_8015EDB4)->unk7A >= 0x3D) {
            (arg0 + D_8015EDB4)->unk7C += 1;
        }
        break;
    case 2:
        (arg0 + D_8015EDB4)->unk2 = 0xFE;
        break;
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80156A80", func_8015800C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80156A80", func_801585D0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80156A80", func_801589B0);

void func_80158AB0(void) {
    /* FAKE: unused TcPos locals reproduce the original's frame (0x60, pos at sp+0x48); real source unknown. T-8060 */
    TcPos fake_hi2;
    TcPos fake_hi;
    TcPos pos;
    TcPos fake_lo;

    func_800AE120(D_801604C0);
    pos.y = 0x400;
    pos.x = 0;
    pos.z = 0;
    pos.unk6 = 0;
    func_8015ACCC(0xE, 0x15, D_801604D4, D_801604DC, pos, 0x10, 0xE0);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80156A80", func_80158B84);

void func_80158CDC(s16 arg0, s16 arg1, s16 arg2) {
    /* FAKE: the two unused TcPos locals reproduce the original's frame (0x58, pos at sp+0x48); real source unknown. T-8060 */
    TcPos fake_hi;
    TcPos pos;
    TcPos fake_lo;

    func_800AE120(D_801604C0);
    pos.x = arg0;
    pos.y = arg1;
    pos.z = arg2;
    func_8015ACCC(0xF, 0x16, D_801604D4, pos, D_801604E4, 4, 0xD0);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80156A80", func_80158DBC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80156A80", func_80158F48);
