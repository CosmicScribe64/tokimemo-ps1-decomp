#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013E4D0", func_8013E4D0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013E4D0", func_8013E54C);

void func_8013E7C0(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    u8 *temp_v0;

    temp_v0 = D_8011ECD0 + arg0 * 0x44;
    *(s16 *)(temp_v0 + 0x1988) = 0;
    *(u8 *)(temp_v0 + 0x1982) = arg2;
    if (arg3 == 1) {
        *(u8 *)(temp_v0 + 0x1983) = 0x80;
    } else {
        *(u8 *)(temp_v0 + 0x1983) = 0;
    }
    *(s16 *)(temp_v0 + 0x1998) = 0;
    if (arg1 != 0xFF) {
        *(s16 *)(temp_v0 + 0x1996) = arg1;
    }
}

void func_8013E810(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    u8 *temp_v0;

    temp_v0 = D_8011ECD0 + arg0 * 0x44;
    *(s16 *)(temp_v0 + 0x1988) = 0;
    *(u8 *)(temp_v0 + 0x1982) = arg2;
    if (arg3 == 1) {
        *(u8 *)(temp_v0 + 0x1983) = 0x84;
    } else {
        *(u8 *)(temp_v0 + 0x1983) = 4;
    }
    *(s16 *)(temp_v0 + 0x1998) = 0;
    if (arg1 != 0xFF) {
        *(s16 *)(temp_v0 + 0x1996) = arg1;
    }
}

void func_8013E864(s32 arg0, s32 arg1, s32 arg2) {
    u8 *temp_v0;

    temp_v0 = D_8011ECD0 + arg0 * 0x44;
    *(s16 *)(temp_v0 + 0x8) = 0;
    *(u8 *)(temp_v0 + 0x2) = 1;
    if (arg2 == 1) {
        *(u8 *)(temp_v0 + 0x3) = 0x80;
    } else {
        *(u8 *)(temp_v0 + 0x3) = 0;
    }
    *(s16 *)(temp_v0 + 0x18) = 0;
    if (arg1 != 0xFF) {
        *(s16 *)(temp_v0 + 0x16) = arg1;
    }
}

void func_8013E8B8(s32 arg0, s32 arg1, s32 arg2) {
    u8 *temp_v0;

    temp_v0 = D_8011ECD0 + arg0 * 0x44;
    *(s16 *)(temp_v0 + 0x1108) = 0;
    *(u8 *)(temp_v0 + 0x1102) = 1;
    if (arg2 == 1) {
        *(u8 *)(temp_v0 + 0x1103) = 0x80;
    } else {
        *(u8 *)(temp_v0 + 0x1103) = 0;
    }
    *(s16 *)(temp_v0 + 0x1118) = 0;
    if (arg1 != 0xFF) {
        *(s16 *)(temp_v0 + 0x1116) = arg1;
    }
}

s32 func_8013E90C(s32 i) {
    return (*(&D_80120652 + i * 0x44) & 1) == 0;
}

void func_8013E934(s32 arg0, s32 arg1, s32 arg2) {
    func_8004E788(arg0 - 0xA0, arg1 - 0x78, 0, arg2, 0);
}

void func_8013E97C(s32 i, s32 x, s32 y) {
    u8 *p = D_8011ECD0 + i * 0x44;
    *(s16 *)(p + 0x19A6) = x - 0xA0;
    *(s16 *)(p + 0x19AA) = y - 0x78;
}

s32 func_8013E9A8(s32 arg0) {
    if (arg0 == 0) {
        arg0 = 1;
    }
    return func_800AE0D0() % arg0;
}

s32 func_8013EA00(s32 arg0, s32 arg1) {
    s32 a = func_8013E9A8(arg0);
    s32 b = func_8013E9A8(arg1);
    return a + b;
}

void func_8013EA30(s32 arg0) {
    s32 i;

    for (i = 0; i < 96; i++) {
        D_8011ECD0[0x1107 + i * 0x44] = arg0;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013E4D0", func_8013EA60);

void func_8013EEAC(u32 arg0) {
    switch (arg0) {
    case 0:
        D_8015EC28 = 0x78;
        D_8015EC2C = 0x68;
        return;
    case 1:
        D_8015EC28 = 0x40;
        D_8015EC2C = 0x58;
        return;
    case 2:
        D_8015EC28 = 0x38;
        D_8015EC2C = 0x88;
        return;
    case 3:
        D_8015EC28 = 0x48;
        D_8015EC2C = 0x60;
        return;
    case 4:
        D_8015EC28 = 0x48;
        D_8015EC2C = 0x60;
        return;
    case 5:
        D_8015EC28 = 0x48;
        D_8015EC2C = 0x60;
        return;
    case 16:
        D_8015EC28 = 0x48;
        D_8015EC2C = 0x60;
        return;
    case 17:
        D_8015EC28 = 0x48;
        D_8015EC2C = 0x60;
        return;
    case 18:
        D_8015EC28 = 0x48;
        D_8015EC2C = 0x60;
        /* fallthrough */
    default:
        return;
    }
}

void func_8013EFD0(u32 arg0) {
    switch (arg0) {
    case 0:
        D_8015EC28 = 0x78;
        D_8015EC2C = 0x68;
        return;
    case 1:
        D_8015EC28 = 0x40;
        D_8015EC2C = 0x58;
        return;
    case 2:
        D_8015EC28 = 0x38;
        D_8015EC2C = 0x88;
        return;
    case 3:
        D_8015EC28 = 0x48;
        D_8015EC2C = 0x40;
        return;
    case 4:
        D_8015EC28 = 0x48;
        D_8015EC2C = 0x50;
        return;
    case 5:
        D_8015EC28 = 0x48;
        D_8015EC2C = 0x60;
        return;
    case 16:
        D_8015EC28 = 0x48;
        D_8015EC2C = 0x60;
        return;
    case 17:
        D_8015EC28 = 0x48;
        D_8015EC2C = 0x60;
        return;
    case 18:
        D_8015EC28 = 0x48;
        D_8015EC2C = 0x60;
        /* fallthrough */
    default:
        return;
    }
}

void func_8013F0F4(s32 a, s32 b, s32 c) {
    if (b == 0) {
        b = -1;
    }
    if (D_8015ED3C[c] == 0) {
        func_80044750((u16)((a & 0xFF) | 0x600));
        D_8015ED3C[c] = b;
    }
    D_8015ED3C[c] -= 1;
}

void func_8013F15C(s32 a, s32 b, s32 c) {
    if (b == 0) {
        b = -1;
    }
    if (D_8015ED3C[c] == 0) {
        func_80044750((u16)((a & 0xFF) | 0x500));
        D_8015ED3C[c] = b;
    }
    D_8015ED3C[c] -= 1;
}

void func_8013F1C4(s32 a, s32 b, s32 c) {
    s32 *p;

    p = &D_8015ED64[0][c];
    if (*p == 0) {
        D_800E6280.unk_F5F = 0xE;
        func_80046290(a, b, 0xE);
        func_80044750(0x300);
        *p = 1;
    }
}

void func_8013F220(void) {
    s32 i;

    for (i = 0; i < 3; i++) {
        D_8015ED64[i][0] = 0;
        D_8015ED64[i][1] = 0;
        D_8015ED64[i][2] = 0;
        D_8015ED64[i][3] = 0;
    }
}

void func_8013F250(s32 arg0) {
    if (D_8015ED8C[1] == 0) {
        D_8015EBE4 += 1;
        if (func_800460EC() == 4 || D_8015EBE4 >= 0x3D) {
            if (D_8015EBE4 >= 0x3D) {
                func_80044750(0x74);
            }
            D_8015EE44.unk_04 = 4;
            D_8015ED8C[1] = 1;
            D_8015EBE4 = 0;
        }
    }
    if (D_8015EE44.unk_04 == 0) {
        D_8015ED8C[1] = 0;
        D_8015ED8C[0] += 1;
    }
    if (arg0 != 0 && D_8015EE44.unk_00 - 3 >= arg0) {
        D_8015ED8C[1] = 0;
        D_8015ED8C[0] += 1;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013E4D0", func_8013F350);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013E4D0", func_8013F3F0);
