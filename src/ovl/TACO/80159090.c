#include "common.h"
#include "ovl/TACO.h"

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80159090", func_80159090);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80159090", func_80159260);

void func_80159678(s32 arg0) {
    (arg0 + D_8015EDB4)->unk74 -= (((((arg0 + D_8015EDB4)->unk78 << 5) / 25000) * func_800A0140((arg0 + D_8015EDB4)->unk7A << 5)) >> 0xC);
    (arg0 + D_8015EDB4)->unk76 -= (((((arg0 + D_8015EDB4)->unk78 << 4) / 25000) * func_800A0070((arg0 + D_8015EDB4)->unk7A << 7)) >> 0xC);
    if ((arg0 + D_8015EDB4)->unk7A >= 0x12D) {
        (arg0 + D_8015EDB4)->unk78 += 0x100;
        (arg0 + D_8015EDB4)->unk74 += ((D_8015EDB4->unk74 + (arg0 + D_8015EDB4)->unk7C) - (arg0 + D_8015EDB4)->unk74) / 10;
        (arg0 + D_8015EDB4)->unk76 += ((D_8015EDB4->unk76 + (arg0 + D_8015EDB4)->unk7E) - (arg0 + D_8015EDB4)->unk76) / 10;
    }
    if ((arg0 + D_8015EDB4)->unk78 >= 0x1001) {
        func_8015C208(arg0);
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80159090", func_80159878);

void func_80159F60(s32 arg0) {
    TcPos pos;

    (arg0 + D_8015EDB4)->unk74 += -(arg0 + D_8015EDB4)->unk74 / 50;
    if ((arg0 + D_8015EDB4)->unk7A >= 0xB5) {
        (arg0 + D_8015EDB4)->unk7A = 0;
    }
    if (((arg0 + D_8015EDB4)->unk7A % 10) == 0) {
        if (func_800AE0C0((arg0 + D_8015EDB4)->unk74) < 0x3E8) {
            pos.x = (arg0 + D_8015EDB4)->unk74;
            pos.y = (arg0 + D_8015EDB4)->unk76;
            pos.z = (arg0 + D_8015EDB4)->unk78;
            switch ((arg0 + D_8015EDB4)->unk76) {
            case -1200:
                if ((arg0 + D_8015EDB4)->unk7A < 0x3C) {
                    func_8013DB80(pos, 0x180, 0xC0);
                }
                break;
            case -700:
                if ((arg0 + D_8015EDB4)->unk7A >= 0x3D && (arg0 + D_8015EDB4)->unk7A < 0x78) {
                    func_8013DB80(pos, 0x180, 0x82);
                }
                break;
            case -200:
                if ((arg0 + D_8015EDB4)->unk7A >= 0x79) {
                    func_8013DB80(pos, 0x180, 0xC0);
                }
                break;
            }
        }
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80159090", func_8015A164);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80159090", func_8015A378);

void func_8015A680(s32 arg0) {
    TcPos pos;

    pos.x = (arg0 + D_8015EDB4)->unk74;
    pos.y = (arg0 + D_8015EDB4)->unk76;
    pos.z = (arg0 + D_8015EDB4)->unk78;
    switch ((arg0 + D_8015EDB4)->unk7C) {
    case 0:
        (arg0 + D_8015EDB4)->unk74 = 0;
        D_8015EDB4[arg0].unk76 = -0x2BC;
        D_8015EDB4[arg0].unk78 = -0x4E20;
        D_8015EDB4[arg0].unk6E = 1;
        (arg0 + D_8015EDB4)->unk7C += 1;
        break;
    case 1:
        (arg0 + D_8015EDB4)->unk6E = (arg0 + D_8015EDB4)->unk7A << 9;
        (arg0 + D_8015EDB4)->unk84[0] = ((arg0 + D_8015EDB4)->unk7A * 0x16) / 32;
        if ((arg0 + D_8015EDB4)->unk6E >= (arg0 + D_8015EDB4)->unk6C) {
            (arg0 + D_8015EDB4)->unk7C += 1;
            D_8015EDB4[arg0].unk7A = 0;
        }
        break;
    case 2:
        if (((arg0 + D_8015EDB4)->unk7A % 15) == 0) {
            func_8013DB80(pos, 0x100, 0xC2);
        }
        if ((arg0 + D_8015EDB4)->unk7A >= 0x259) {
            (arg0 + D_8015EDB4)->unk7C += 1;
            D_8015EDB4[arg0].unk7A = 0;
        }
        break;
    case 3:
        (arg0 + D_8015EDB4)->unk6E = (0x10 - (arg0 + D_8015EDB4)->unk7A) << 9;
        (arg0 + D_8015EDB4)->unk84[0] = ((0x10 - (arg0 + D_8015EDB4)->unk7A) << 5) / 32;
        if ((arg0 + D_8015EDB4)->unk6E <= 0) {
            func_8015C208(arg0);
        }
        break;
    }
}
