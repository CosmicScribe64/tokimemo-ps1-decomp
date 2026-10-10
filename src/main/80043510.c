#include "common.h"
#include "game.h"

INCLUDE_ASM("asm/nonmatchings/main/80043510", func_80043510);

void draw2d3d(u8 arg0, u8 arg1) {
    D_800E6280.unk_1114 = arg0;
    D_800E6280.unk_1113 = arg1;
}

void back_clear_switch(s32 arg0) {
    if (arg0 != 0) {
        D_800E6280.unk_039 = 1;
    } else {
        D_800E6280.unk_039 = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80043510", load_palette);

INCLUDE_ASM("asm/nonmatchings/main/80043510", load_csr_ab);

INCLUDE_ASM("asm/nonmatchings/main/80043510", load_csr_tp);

INCLUDE_ASM("asm/nonmatchings/main/80043510", csr_load_vram);

INCLUDE_ASM("asm/nonmatchings/main/80043510", palette_load_vram);

void LoadSquare(u16 arg0, u16 arg1, u16 arg2, u16 arg3, void *arg4) {
    RECT rect;

    rect.x = arg0;
    rect.y = arg1;
    rect.w = arg2;
    rect.h = arg3;
    func_8009C884(&rect, arg4);
}

void MoveSquare(u16 arg0, u16 arg1, u16 arg2, u16 arg3, u16 arg4, u16 arg5) {
    RECT rect;

    rect.x = arg0;
    rect.y = arg1;
    rect.w = arg2;
    rect.h = arg3;
    func_8009C93C(&rect, arg4, arg5);
}

void StoreSquare(u16 arg0, u16 arg1, u16 arg2, u16 arg3, void *arg4) {
    RECT rect;

    rect.x = arg0;
    rect.y = arg1;
    rect.w = arg2;
    rect.h = arg3;
    func_8009C8E0(&rect, arg4);
}

void func_80044434(void) {
}

INCLUDE_ASM("asm/nonmatchings/main/80043510", func_8004443C);

void func_80044700(s32 arg0, s32 arg1, s32 arg2) {
    RECT rect;
    s32 x;

    x = arg1;
    rect.x = x << 4;
    rect.y = arg0 + 0x1E0;
    rect.w = 0x10;
    rect.h = 1;
    func_8009C93C(&rect, 0x100, arg2 + 0x1E0);
}

s32 func_80044750(s32 arg0) {
    func_80079B10(arg0 & 0xFFFF);
    return 1;
}


s32 func_80044774(s32 arg0) {
    s32 *p;

    if (arg0 != 0) {
        p = (s32 *)0x8002E800;
    } else {
        p = (s32 *)0x8001C000;
    }
    if ((*p & 0xFFFF) != 0x40) {
        return -1;
    }
    if (arg0 != 0) {
        func_8004500C(D_800B3D44, D_800B3D48, arg0);
    } else {
        func_80044750(0x1200);
    }
    D_800B3D40 = arg0;
    return arg0 & 0xFF;
}

u8 func_8004480C(void) {
    return D_800B3D40;
}

u8 func_8004481C(void) {
    return D_800B3D44;
}

INCLUDE_ASM("asm/nonmatchings/main/80043510", func_8004482C);

INCLUDE_ASM("asm/nonmatchings/main/80043510", func_80044890);

INCLUDE_ASM("asm/nonmatchings/main/80043510", func_80044C98);

INCLUDE_ASM("asm/nonmatchings/main/80043510", func_80044D54);

INCLUDE_ASM("asm/nonmatchings/main/80043510", func_80044E8C);

s32 func_80044F94(s32 arg0) {
    if (arg0 == 0) {
        if (func_80079E00(0) != 0 && func_80079E00(1) != 0) {
            return 1;
        }
        return 0;
    }
    if (func_80079E00(2) != 0 && func_80079E00(3) != 0) {
        return 1;
    }
    return 0;
}

s32 func_8004500C(s32 arg0, s32 arg1) {
    s32 *var_v0_2;

    if (arg0 != 0) {
        var_v0_2 = (s32 *)0x8002E800;
    } else {
        var_v0_2 = (s32 *)0x8001C000;
    }
    if ((*var_v0_2 & 0xFFFF) != 0x40) {
        return -1;
    }
    if (arg0 == 0) {
        func_80044750(0x22);
        func_80044750(0x21);
        func_80044750(0x2F);
        func_80044750((arg1 | 0x200) & 0xFFFF);
    } else {
        func_80044750(0x23);
        func_80044750(0x24);
        func_80044750(0x2E);
        func_80044750((arg1 | 0x200) & 0xFFFF);
    }
    D_800B3D44 = (u8) arg0;
    D_800B3D48 = (u8) arg1;
    D_800B3D40 = 1;
    /* no return here: the original returns the v0 of the last func_80044750 call (implicit int) */
}

INCLUDE_ASM("asm/nonmatchings/main/80043510", func_800450F4);
