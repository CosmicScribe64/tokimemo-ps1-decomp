#include "common.h"
#include "ovl/ETC.h"

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/801492C0", func_801492C0);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/801492C0", func_80149394);

typedef struct {
    u32 pad0 : 25;
    u32 grade : 2;
    u32 pad1 : 3;
    u32 flag : 1;
    u32 pad2 : 1;
} EtcSlot;

s32 func_80149468(void) {
    s32 i;

    for (i = 0; i < 11; i++) {
        if (((EtcSlot *)&D_800E6280.unk_66C[i])->grade >= 3 && ((EtcSlot *)&D_800E6280.unk_66C[i])->flag) {
            return i;
        }
    }
    return -1;
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/801492C0", func_801495CC);

s32 func_8014979C(void) {
    menu_check(0, D_8011ECF6, D_8011ECFA);
    menu_bar_show(0);
    func_8004FC10(0);
    if (D_800E6280.unk_F88 & 0x20) {
        switch (D_800E6280.unk_1093) {
        case 0:
            if (D_800E6280.unk_0F6.b[1] != 0) {
                func_80042908(2);
            } else {
                func_80042908(1);
            }
            break;
        case 1:
            if (D_800E6280.unk_0F6.b[1] != 0) {
                func_80042908(1);
            } else {
                func_80042908(2);
            }
            break;
        case 2:
            func_80042908(3);
            break;
        }
    }
}

void func_8014988C(void) {
    if (D_8011F553 & 0x80) {
        parameter_show();
    }
}

s32 func_801498BC(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80149394();
        break;
    case 1:
        func_801495CC();
        break;
    case 2:
        func_8014979C();
        break;
    }
}

s32 func_80149928(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        D_800E6280.unk_0F6.b[1] = 0;
        func_8004EAAC();
        func_80050DFC(&D_800E6280.unk_0D4);
        func_80050E8C(&D_8015093C, 0, 0x1F);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_80051DBC();
        break;
    case 2:
        func_8004EAAC();
        func_80050C24(0, 3);
        x_taku_string_set((s32)D_8015099C, (s32)D_801509B0, (s32)D_801509C4, 3);
        func_8004EAD4(3);
        D_800E6280.unk_1093 = 0;
        func_8004284C();
        break;
    }
}

s32 func_80149A08(void) {
    menu_check(0, D_8011ECF6, D_8011ECFA);
    menu_bar_show(0);
    func_8004FC10(0);
    if (D_800E6280.unk_F88 & 0x20) {
        switch (D_800E6280.unk_1093) {
        case 0:
            D_80150E98 = 0;
            func_8004284C();
            break;
        case 1:
            D_80150E98 = 1;
            func_8004284C();
            break;
        case 2:
            D_80150E98 = 2;
            func_8004284C();
            break;
        }
    }
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/801492C0", func_80149AC4);

/* FAKE: the u8 view of the selector keeps the compares on the global ($v1), as in the original (T-5010) */
s32 func_80149BC0(void) {
    s32 r;

    switch (*(u8 *)&D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_80050DFC(D_800E6280.unk_0D4);
        func_80050E8C(D_80150D20, 0, 0x1F);
        func_8004EAD4(2);
        r = func_8005742C(0x463B, 1);
        if (r != -1) {
            if (r == 0 || r == 1) {
                D_800E6280.unk_110D += 2;
                func_80057418(1, r);
                func_80057390(0x80);
            }
        } else {
            D_800E6280.unk_110D += 1;
        }
        break;
    case 1:
        if (func_800460CC() & 1) {
            func_80057418(1, func_8005751C(1));
            if (D_800B594C == -1) {
                func_800573AC();
                func_8005742C(0x463B, 1);
            } else {
                func_80057390(0x80);
                D_800E6280.unk_110D += 1;
            }
        }
        break;
    case 2:
        func_80063930(0);
        func_800578F4(1);
        func_80057390(0x80);
        func_800649D4();
        func_8004284C();
        break;
    }
}

s32 func_80149D3C(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_80044890(1, 0xBF98, 0xBF79, 0xCB48, 0xCAFD, 0xCAF7);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        if (func_80044E8C() == 1) {
            func_8004500C(1, 0x205);
            func_8004284C();
        }
        break;
    }
}

s32 func_80149DD8(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_80046318(9U, 0x801E0000, 0x7D81);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        if (func_800460CC() & 1) {
            func_8007C6A8(1);
            func_80149E68();
            func_8004284C();
        }
        break;
    }
}

void func_80149E68(void) {
    func_80048F64(0x60);
    D_80120650[1] = 1;
    D_80120650[2] = 4;
    *(s32 *)&D_80120650[0x38] = 0x01000000;
    D_80120650[3] = 0x84;
    *(u8 **)&D_80120650[0xC] = D_80125CA8;
    *(u8 **)&D_80120650[0x10] = D_80125CAC;
    *(u8 **)&D_80120650[0x34] = D_80125CB0;
    D_80120650[5] = 2;
    D_80120650[6] = 1;
    D_80120650[0x43] = 0x10;
    *(s16 *)&D_80120650[0x16] = 0;
    *(s16 *)&D_80120650[0x26] = -0x50;
    *(s16 *)&D_80120650[0x2A] = -0x40;
    load_palette(D_80125CA4, 0x10, 1, 1, 0);
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/801492C0", func_80149F48);

s32 func_8014A180(void) {
    if (D_80120652 & 1) {
        D_800E6280.unk_1104.w = 0;
        return;
    }
    if (D_80120658 == 0) {
        if (D_80120666 != 0) {
            func_80044750(0x502);
        } else {
            func_80044750(0x501);
        }
    }
    if (D_80120658++ >= 0x3D) {
        if (D_80150E94 >= 0xA) {
            D_800E6280.unk_110D += 1;
            return;
        }
        D_800E6280.unk_110D -= 1;
    }
}

s32 func_8014A258(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_80149F48();
        break;
    case 1:
        func_8014A180();
        break;
    case 2:
        func_8004284C();
        break;
    }
}

s32 func_8014A2C4(void) {
    switch (D_80150E9C) {
    case 10:
        return 0;
    case 8:
    case 9:
        return 1;
    case 7:
        return 2;
    case 5:
    case 6:
        return 3;
    case 3:
    case 4:
        return 4;
    case 1:
    case 2:
    default:
        return 5;
    }
}

s32 func_8014A32C(void) {
    if (D_800E6280.unk_110.unk_02 + D_800E6280.unk_114.unk_02 > 0x100) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/801492C0", func_8014A360);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/801492C0", func_8014A534);

void func_8014A6D4(void) {
    D_800E6280.unk_1104.w += 1;
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80149928();
        return;
    case 1:
        func_80149A08();
        return;
    case 2:
        func_80149AC4();
        return;
    case 3:
        func_80149BC0();
        return;
    case 4:
        func_80149D3C();
        return;
    case 5:
        func_80149DD8();
        return;
    case 6:
        func_8014A258();
        return;
    case 7:
        func_8014A534();
        /* fallthrough */
    default:
        return;
    }
}

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/801492C0", func_8014A7A4);

INCLUDE_ASM("asm/ovl/ETC/nonmatchings/ETC/801492C0", func_8014A8A8);

void func_8014A9A4(void) {
    D_800E6280.unk_1104.w += 1;
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8014A7A4();
        return;
    case 1:
        func_80149A08();
        return;
    case 2:
        func_8014A8A8();
        return;
    case 3:
        func_80149BC0();
        return;
    case 4:
        func_80149D3C();
        return;
    case 5:
        func_80149DD8();
        return;
    case 6:
        func_8014A258();
        return;
    case 7:
        func_8014A534();
        /* fallthrough */
    default:
        return;
    }
}

s32 func_8014AA74(void) {
    s32 temp_v0;
    s32 sp28;

    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_80050DFC(&D_800E6280.unk_0D4);
        temp_v0 = func_80149468();
        sp28 = temp_v0;
        func_80050E8C(D_80150CCC[temp_v0], 0, 0xF);
        if ((u16)D_800E6280.unk_66C[sp28].unk_00 >= 0x19BU) {
            D_800E6280.unk_0F6.b[1] = 0x40;
        } else {
            D_800E6280.unk_0F6.b[1] = 0xC0;
        }
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_80051DBC();
        break;
    case 2:
        func_80042908(4);
        break;
    }
}

void func_8014AB5C(void) {
    if (D_800E6280.unk_110A == 0) {
        func_8014AA74();
    }
}

void func_8014AB88(void) {
    func_80048E78();
    func_800AE0B0("shinro %x \n", D_800E6280.unk_0F6.b[1]);
    func_80072B5C(1);
}
