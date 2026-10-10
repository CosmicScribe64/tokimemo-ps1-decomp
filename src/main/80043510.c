#include "common.h"
#include "game.h"
#include "main_only.h"

INCLUDE_ASM("asm/nonmatchings/main/80043510", func_80043510);

void draw2d3d(u8 arg0, u8 arg1) {
    D_800E7394 = arg0;
    D_800E7393 = arg1;
}

void back_clear_switch(s32 arg0) {
    if (arg0 != 0) {
        D_800E62B9 = 1;
    } else {
        D_800E62B9 = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80043510", load_palette);

INCLUDE_ASM("asm/nonmatchings/main/80043510", load_csr_ab);

INCLUDE_ASM("asm/nonmatchings/main/80043510", load_csr_tp);

INCLUDE_ASM("asm/nonmatchings/main/80043510", csr_load_vram);

INCLUDE_ASM("asm/nonmatchings/main/80043510", palette_load_vram);

#ifdef NON_MATCHING
/* NON_MATCHING: T-0016, ugen temporaries: original lhu into t8/t9, IDO t0/t1 (register allocation, not the frame). */
void LoadSquare(u16 arg0, u16 arg1, s16 arg2, s16 arg3, void *arg4) {
    RECT rect;

    rect.x = arg0;
    rect.y = arg1;
    rect.w = arg2;
    rect.h = arg3;
    func_8009C884(&rect, arg4);
}
#else
INCLUDE_ASM("asm/nonmatchings/main/80043510", LoadSquare);
#endif

#ifdef NON_MATCHING
/* NON_MATCHING: T-0016, ugen temporaries: original lhu into t7/t8/t9, IDO t8/t9/t0 (register allocation, not the frame). */
void MoveSquare(u16 arg0, u16 arg1, u16 arg2, s16 arg3, u16 arg4, u16 arg5) {
    RECT rect;

    rect.x = arg0;
    rect.y = arg1;
    rect.w = arg2;
    rect.h = arg3;
    func_8009C93C(&rect, arg4, arg5);
}
#else
INCLUDE_ASM("asm/nonmatchings/main/80043510", MoveSquare);
#endif

#ifdef NON_MATCHING
/* NON_MATCHING: T-0016, ugen temporaries: original lhu into t8/t9, IDO t0/t1 (register allocation, not the frame). */
void StoreSquare(u16 arg0, u16 arg1, s16 arg2, s16 arg3, void *arg4) {
    RECT rect;

    rect.x = arg0;
    rect.y = arg1;
    rect.w = arg2;
    rect.h = arg3;
    func_8009C8E0(&rect, arg4);
}
#else
INCLUDE_ASM("asm/nonmatchings/main/80043510", StoreSquare);
#endif

void func_80044434(void) {
}

INCLUDE_ASM("asm/nonmatchings/main/80043510", func_8004443C);

#ifdef NON_MATCHING
/* NON_MATCHING: T-0016, original frame has 8 more bytes of locals (0x38 vs 0x30); unknown extra local. */
void func_80044700(s32 arg0, s32 arg1, s32 arg2) {
    RECT rect;

    rect.x = arg1 << 4;
    rect.y = arg0 + 0x1E0;
    rect.w = 0x10;
    rect.h = 1;
    func_8009C93C(&rect, 0x100, arg2 + 0x1E0);
}
#else
INCLUDE_ASM("asm/nonmatchings/main/80043510", func_80044700);
#endif

INCLUDE_ASM("asm/nonmatchings/main/80043510", func_80044750);

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

INCLUDE_ASM("asm/nonmatchings/main/80043510", func_8004500C);

INCLUDE_ASM("asm/nonmatchings/main/80043510", func_800450F4);
