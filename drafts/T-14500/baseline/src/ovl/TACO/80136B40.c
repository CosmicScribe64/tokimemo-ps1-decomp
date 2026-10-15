#include "common.h"
#include "ovl/TACO.h"

void func_80136B40(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80136C00();
        return;
    case 1:
        func_80136D00();
        return;
    case 2:
        D_8015F400 = 0;
        func_8004284C();
        return;
    case 3:
        func_80136D78();
        return;
    case 4:
        func_80136F4C();
        return;
    case 5:
        func_80136E08();
        return;
    case 6:
        func_80136C60();
        return;
    case 7:
        func_80136CD8();
        /* fallthrough */
    default:
        return;
    }
}

void func_80136C00(void) {
    func_8013AFDC();
    func_800450F4(0, 0x202);
    D_800E6280.unk_036 = 0;
    D_800E6280.unk_037 = 0;
    D_800E6280.unk_038 = 0;
    back_clear_switch(1);
    D_8015EDB0 = 0;
    func_80059048();
    func_8004284C();
}

s32 func_80136C60(void) {
    func_8013A790(1, 0);
    D_8015F4F0 = 0x80 - D_800E6280.unk_110D * 3;
    D_800E6280.unk_110D++;
    if (D_800E6280.unk_110D * 3 > 0x80) {
        func_8004284C();
    }
}

void func_80136CD8(void) {
    func_80044750(0xC1);
    func_80042908(2);
}

void func_80136D00(void) {
    if (D_8015F4F4 < 0x100) {
        D_8015F4F0 = D_8015F4F4 / 2;
    }
    func_8013A790(1, 0);
    D_8015F4F4 += 4;
    if (D_8015F4F4 > 0x100) {
        D_8015F4F0 = 0x80;
        func_8004284C();
    }
}

void func_80136D78(void) {
    if (D_800E6280.unk_F80 & 0x40) {
        D_8015F400 += 0x78;
    } else {
        D_8015F400 += 0x3C;
    }
    func_8013A790(1, 0);
    func_80136FBC(D_8015F400, 3);
    if (D_8015F400 > 0x1000) {
        func_8004284C();
    }
}

void func_80136E08(void) {
    D_8015F400 += 0x1F4;
    func_8013A790(1, 0);
    func_80136FBC(D_8015F400, 0);
    if (D_8015F400 >= 0x9001) {
        D_8015F400 = 0;
        func_8004284C();
    }
}

void func_80136E70(u8 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    s32 v;

    if (arg4 < 0x1800) {
        v = (arg4 << 7) / 4096;
    } else {
        v = 0xC0;
    }
    func_80139424(arg0, arg1, 3, v, (arg2 * 12 - 0x6C) * arg4 / 4096, (arg3 * 22 - 0x42) * arg4 / 4096 - 0x14, arg4, arg4, arg5);
}

void func_80136F4C(void) {
    func_8013A790(1, 0);
    func_80136FBC(0x1000, 3);
    if (D_800E6280.unk_F80 & 0x40) {
        func_8004284C();
    }
    if (D_800E6280.unk_F88 & 0x20) {
        func_8004284C();
    }
}

void func_80136FBC(s32 arg0, s32 arg1) {
    func_801371B4("PLAYERS RANKING", arg0, 0, arg1, 2, 1);
    func_801371B4("1ST", arg0, 1, arg1, 1, 4);
    func_801371B4("2ND", arg0, 4, arg1, 1, 5);
    func_801371B4("3RD", arg0, 3, arg1, 1, 6);
    func_801371B4(D_8015EE04, arg0, 1, arg1, 0xF, 4);
    func_801371B4(D_8015EE08, arg0, 4, arg1, 0xF, 5);
    func_801371B4(D_8015EE0C, arg0, 3, arg1, 0xF, 6);
    func_80137264(D_8015F404, D_8015EDF8);
    func_80137264(D_8015F410, D_8015EDFC);
    func_80137264(D_8015F41C, D_8015EE00);
    func_801371B4(D_8015F404, arg0, 2, arg1, 6, 4);
    func_801371B4(D_8015F410, arg0, 5, arg1, 6, 5);
    func_801371B4(D_8015F41C, arg0, 6, arg1, 6, 6);
}

void func_801371B4(u8 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    s32 i;
    u8 *p;
    s32 x;

    i = 0;
    if (*arg0 != 0) {
        p = arg0;
        x = arg4;
        do {
            func_80136E70(*p, arg2, x, arg5, arg1, arg3);
            i++;
            p++;
            x++;
        } while (i < 0x15 && *p != 0);
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80136B40", func_80137264);
