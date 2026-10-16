#include "common.h"
#include "ovl/TACO.h"

void func_80159090(void) {
    D_801604C0 += 1;
    func_8015B59C();
    if (((s32) D_801604C0 % 30) == 0) {
        if (D_801604C4 < 4) {
            if (func_8015CCC8(2) < 0xA) {
                func_80159260();
            }
        } else if ((D_801604C4 < 6) && (func_8015CCC8(2) < 3)) {
            func_80159260();
        }
    }
    switch (D_801604C4) {
    case 0:
        func_80159878(D_801604C4);
        func_8015A900();
    default:
        break;
    case 1:
    case 2:
    case 3:
        if (func_8015CCC8(3) == 0) {
            func_80159878(D_801604C4);
            func_8015A900();
        }
        break;
    case 4:
        if (func_8015CCC8(3) == 0) {
            func_8015A900();
        }
        break;
    case 5:
        func_8015A900();
        break;
    case 6:
        func_8015A900();
        break;
    case 7:
    case 8:
    case 9:
    case 10:
        if (func_8015CE64() == 0) {
            func_8015A164(D_801604C4 - 3);
            func_8015A900();
        }
        break;
    case 11:
        if (func_8015CCC8(0xFF) == 0) {
            func_8015A900();
        }
        break;
    case 12:
        if (func_8015CE64() == 0) {
            func_8015ABF0();
            func_8004284C();
        }
        break;
    }
}

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

s32 func_80159878(s32 arg0) {
    TcPos sp50;
    TcPos sp48;

    func_800AE120(D_801604C0);
    sp48.x = 0;
    sp48.z = 0;
    sp50.z = -0x4E20;
    switch (arg0) {
    case 0:
    case 2:
        sp50.x = 0x1B58;
        sp48.y = 0x400;
        sp50.y = -0x4B0;
        func_8015ACCC(3, 0x21, D_801604D4, sp50, sp48, 8, 0);
        sp50.y = -0xC8;
        func_8015ACCC(3, 0x21, D_801604D4, sp50, sp48, 8, 0);
        sp50.x = -0x1B58;
        sp48.y = 0xC00;
        sp50.y = -0x2BC;
        if (arg0 == 0) {
            func_8015ACCC(3, 0x21, D_801604D4, sp50, sp48, 0xA, 0xE0);
            return;
        }
        func_8015ACCC(3, 0x21, D_801604D4, sp50, sp48, 0xA, 0);
        return;
    case 1:
        /* fallthrough */
    case 3:
        sp50.x = -0x1B58;
        sp48.y = 0xC00;
        sp50.y = -0x4B0;
        if (arg0 == 3) {
            func_8015ACCC(3, 0x21, D_801604D4, sp50, sp48, 8, 0x50);
        } else {
            func_8015ACCC(3, 0x21, D_801604D4, sp50, sp48, 8, 0);
        }
        sp50.y = -0xC8;
        if (arg0 == 1) {
            func_8015ACCC(3, 0x21, D_801604D4, sp50, sp48, 8, 0x50);
        } else {
            func_8015ACCC(3, 0x21, D_801604D4, sp50, sp48, 8, 0);
        }
        sp50.x = 0x1B58;
        sp48.y = 0x400;
        sp50.y = -0x2BC;
        if (arg0 == 3) {
            func_8015ACCC(3, 0x21, D_801604D4, sp50, sp48, 0xA, 0x80);
            return;
        }
        func_8015ACCC(3, 0x21, D_801604D4, sp50, sp48, 8, 0);
        return;
    }
}

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

void func_8015A164(s32 arg0) {
    /* FAKE: unused leading TcPos local reproduces the frame (see func_80156F50); real locals unknown. T-9180 */
    TcPos p0;
    TcPos pos;
    TcPos vel;
    s32 i;
    s32 j;

    func_800AE120(D_801604C0);
    pos.z = 0x4E20;
    func_8015ACCC(0xD, 0x23, D_801604D4, pos, D_801604E4, 0x28, 0xD0);
    i = 0;
    if (arg0 > 0) {
        j = 0;
        do {
            vel.y = 0x800;
            vel.x = 0;
            vel.z = 0;
            vel.unk6 = j / arg0;
            func_8015ACCC(0xD, 0x22, D_801604D4, pos, vel, 0x14, (func_800AE0D0() % 9) * 0x10);
            i += 1;
            j += 0x1000;
        } while (i != arg0);
    }
}

void func_8015A378(s32 arg0) {
    switch ((arg0 + D_8015EDB4)->unk7C) {
    case 0:
        D_8015EDB4[arg0].unk74 = (func_800A0070((arg0 + D_8015EDB4)->unk6A) * 0x3E8) >> 0xC;
        D_8015EDB4[arg0].unk76 = ((func_800A0140((arg0 + D_8015EDB4)->unk6A) * 0x3E8) >> 0xC) - 0x2BC;
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
        D_8015EDB4[arg0].unk74 = (func_800A0070((arg0 + D_8015EDB4)->unk6A) * 0x3E8) >> 0xC;
        D_8015EDB4[arg0].unk76 = ((func_800A0140((arg0 + D_8015EDB4)->unk6A) * 0x3E8) >> 0xC) - 0x2BC;
        (arg0 + D_8015EDB4)->unk6A += 0x20;
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
