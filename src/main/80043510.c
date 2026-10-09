#include "common.h"
#include "game.h"

INCLUDE_ASM("asm/nonmatchings/main/80043510", func_80043510);

void func_800438DC(u8 arg0, u8 arg1) {
    D_800E7394 = arg0;
    D_800E7393 = arg1;
}

void func_800438F0(s32 arg0) {
    if (arg0 != 0) {
        D_800E62B9 = 1;
    } else {
        D_800E62B9 = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80043510", func_80043914);

INCLUDE_ASM("asm/nonmatchings/main/80043510", func_80043980);

INCLUDE_ASM("asm/nonmatchings/main/80043510", func_80043A00);

INCLUDE_ASM("asm/nonmatchings/main/80043510", func_80043A84);

INCLUDE_ASM("asm/nonmatchings/main/80043510", func_80043B74);

#ifdef NON_MATCHING
/* NON_MATCHING: T-0016, ugen temporaries: original lhu into t8/t9, IDO t0/t1 (register allocation, not the frame). */
void func_8004435C(u16 arg0, u16 arg1, s16 arg2, s16 arg3, void *arg4) {
    RECT rect;

    rect.x = arg0;
    rect.y = arg1;
    rect.w = arg2;
    rect.h = arg3;
    func_8009C884(&rect, arg4);
}
#else
INCLUDE_ASM("asm/nonmatchings/main/80043510", func_8004435C);
#endif

#ifdef NON_MATCHING
/* NON_MATCHING: T-0016, ugen temporaries: original lhu into t7/t8/t9, IDO t8/t9/t0 (register allocation, not the frame). */
void func_800443A0(u16 arg0, u16 arg1, u16 arg2, s16 arg3, u16 arg4, u16 arg5) {
    RECT rect;

    rect.x = arg0;
    rect.y = arg1;
    rect.w = arg2;
    rect.h = arg3;
    func_8009C93C(&rect, arg4, arg5);
}
#else
INCLUDE_ASM("asm/nonmatchings/main/80043510", func_800443A0);
#endif

#ifdef NON_MATCHING
/* NON_MATCHING: T-0016, ugen temporaries: original lhu into t8/t9, IDO t0/t1 (register allocation, not the frame). */
void func_800443F0(u16 arg0, u16 arg1, s16 arg2, s16 arg3, void *arg4) {
    RECT rect;

    rect.x = arg0;
    rect.y = arg1;
    rect.w = arg2;
    rect.h = arg3;
    func_8009C8E0(&rect, arg4);
}
#else
INCLUDE_ASM("asm/nonmatchings/main/80043510", func_800443F0);
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

INCLUDE_ASM("asm/nonmatchings/main/80043510", func_80044774);

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

INCLUDE_ASM("asm/nonmatchings/main/80043510", func_80044F94);

INCLUDE_ASM("asm/nonmatchings/main/80043510", func_8004500C);

INCLUDE_ASM("asm/nonmatchings/main/80043510", func_800450F4);
