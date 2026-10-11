#include "common.h"
#include "ovl/TACO.h"

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80154960", func_80154960);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80154960", func_80154F74);

void func_80155230(s32 arg0) {
    s16 t;

    (arg0 + D_8015EDB4)->unk78 += 0x100;
    t = ((arg0 + D_8015EDB4)->unk7C << 9) + 0x200;
    (arg0 + D_8015EDB4)->unk70 = t;
    D_8015EDB4[arg0].unk6E = t;
    D_8015EDB4[arg0].unk6C = t;
    switch ((arg0 + D_8015EDB4)->unk6A) {
    case 0:
        (arg0 + D_8015EDB4)->unk64 += 0x32;
        break;
    case 1:
        (arg0 + D_8015EDB4)->unk66 += 0x32;
        break;
    case 2:
        (arg0 + D_8015EDB4)->unk68 += 0x32;
        break;
    case 3:
        (arg0 + D_8015EDB4)->unk64 += 0x63;
        break;
    case 4:
        (arg0 + D_8015EDB4)->unk66 += 0x63;
        break;
    case 5:
        (arg0 + D_8015EDB4)->unk68 += 0x63;
        break;
    }
    if ((arg0 + D_8015EDB4)->unk78 >= 0x1001) {
        func_8015C208(arg0);
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80154960", func_801553AC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80154960", func_801557E8);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80154960", func_8015595C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80154960", func_80155C64);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80154960", func_80155E10);

void func_80155F98(s32 arg0) {
    TcPos pos;

    switch ((arg0 + D_8015EDB4)->unk7E) {
    case 0:
        if (((arg0 + D_8015EDB4)->unk7A % 7) == 0) {
            pos.x = (arg0 + D_8015EDB4)->unk74;
            pos.y = (arg0 + D_8015EDB4)->unk76;
            pos.z = (arg0 + D_8015EDB4)->unk78;
            func_8013DB80(pos, 0x100, 0xC0);
        }
        if ((arg0 + D_8015EDB4)->unk7A == 0x3C) {
            if (D_8015EDB4->unk74 < (arg0 + D_8015EDB4)->unk7C) {
                (arg0 + D_8015EDB4)->unk7C = 1;
            } else {
                (arg0 + D_8015EDB4)->unk7C = -1;
            }
            D_8015EDB4[arg0].unk7A = 0;
            (arg0 + D_8015EDB4)->unk7E += 1;
        }
        break;
    case 1:
        (arg0 + D_8015EDB4)->unk66 += (arg0 + D_8015EDB4)->unk7C * 0x32;
        if ((arg0 + D_8015EDB4)->unk7A >= 0x29) {
            (arg0 + D_8015EDB4)->unk7E += 1;
        }
        break;
    case 2:
        (arg0 + D_8015EDB4)->unk78 -= 0x320;
        if ((arg0 + D_8015EDB4)->unk78 < -0x7530) {
            func_8015C208(arg0);
        }
        break;
    }
}

void func_80156178(void) {
    TcPos p0;
    TcPos p2;
    TcPos pos;

    func_800AE120(D_801604C0);
    /* FAKE: x and y on one line so as1 orders their stores y, x like the original (decomp-permuter); real source unknown. T-9180 */
    pos.x = -0x100; pos.y = 0x800;
    pos.z = 0;
    func_8015ACCC(6, 6, D_801604D4, D_801604D4, pos, 0xFF, 0xF0);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80154960", func_80156244);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80154960", func_801563E4);

void func_80156924(s32 arg0) {
    (arg0 + D_8015EDB4)->unk78 += 0xFA;
    (arg0 + D_8015EDB4)->unk74 += (D_8015EDB4->unk74 - (arg0 + D_8015EDB4)->unk74) / (arg0 + D_8015EDB4)->unk7C;
    (arg0 + D_8015EDB4)->unk76 += (D_8015EDB4->unk76 - (arg0 + D_8015EDB4)->unk76) / (arg0 + D_8015EDB4)->unk7C;
    if ((arg0 + D_8015EDB4)->unk7E == 0) {
        (arg0 + D_8015EDB4)->unk68 += 0xC8;
    } else {
        (arg0 + D_8015EDB4)->unk68 -= 0xC8;
    }
    if ((arg0 + D_8015EDB4)->unk78 >= 0x7D1) {
        func_8015C208(arg0);
    }
}
