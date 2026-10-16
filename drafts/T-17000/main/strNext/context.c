#include "common.h"
#include "game.h"

void set_movie_offset(s16 arg0, s16 arg1) {
    D_800B58F8 = arg0;
    D_800B58FC = arg1;
}

void strInit(s32 arg0, s32 arg1) {
    DecDCTReset(0);
    *(s32 *)(D_80125C58 + 0x34) = 0;
    func_800869C8(arg1);
    func_80088150(D_80125C58 + 0x44, 0x20);
    func_80088180(D_800B5900 & 1, 1, -1, 0, 0);
    strKickCD(arg0);
}

void strSetDefDecEnv(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    u32 width;
    u32 height;
    StrDecEnv30 *env;

    env = arg0;
    env->buffer0 = (u8 *) (D_80125C58 + 0x12D44);
    env->buffer1 = (u8 *) (D_80125C58 + 0x48D44);
    env->unk_08 = 0;
    env->buffer2 = (u8 *) (D_80125C58 + 0x10044);
    env->index = 0;
    env->flag = 0;
    width = 0x140;
    height = 0xF0;
    if (D_800B5900 & 1) {
        width = 0x1E0;
        height = 0xF0;
        /* FAKE: one source line gives as1 the original halfword-store order. T-9930. */
        env->rect0.w = width; env->rect0.h = height;
        env->rect0.x = arg1;
        env->rect0.y = arg2;
        env->rect1.x = arg3;
        env->rect1.y = arg4;
        env->rect1.h = height;
        env->rect1.w = width;
        env->rect2.w = 0x18;
        env->rect2.h = height;
        env->rect2.y = arg2;
        env->rect2.x = arg1;
        return;
    }
    env->rect0.x = arg1;
    env->rect0.y = arg2;
    env->rect0.w = width;
    env->rect0.h = height;
    env->rect1.x = arg3;
    env->rect1.y = arg4;
    env->rect1.h = height;
    env->rect1.w = width;
    env->rect2.w = 0x10;
    env->rect2.h = height;
    env->rect2.y = arg2;
    env->rect2.x = arg1;
}

INCLUDE_ASM("asm/nonmatchings/main/800563F0", strCallback);

s32 strNextVlc(u8 *arg0) {
    s32 vlc;
    s32 timeout;

    timeout = 0x800000;
    vlc = strNext(arg0);
    while (vlc == 0) {
        if (--timeout == 0) {
            return -1;
        }
        func_80088CB0(vlc);
        vlc = strNext(arg0);
    }
    if (((MovieVlcState *)arg0)->index != 0) {
        ((MovieVlcState *)arg0)->index = 0;
    } else {
        ((MovieVlcState *)arg0)->index = 1;
    }
    func_80086870(vlc, ((MovieVlcState *)arg0)->buffers[((MovieVlcState *)arg0)->index]);
    func_80088CB0(vlc);
    return 0;
}

/* bulk_m2c declarations (T-11190) */
extern u32 D_800B58F0;
extern u32 D_800B58F4;
extern s32 D_7FFFFF;
extern s32 NULL;
/* end bulk_m2c declarations */
s32 strNext(void *arg0) {
    u8 *sp54;
    u8 *sp50;
    s16 sp4A;
    s16 sp48;
    s16 sp46;
    s16 sp44;
    s32 *var_s0;
    s16 temp_v1;
    s32 temp_t3;
    s32 var_a0;

    var_s0 = &D_7FFFFF;
    if (func_80088D70((s32) &sp54, (s32) &sp50) != 0) {
loop_1:
        if (var_s0 == NULL) {
            return NULL;
        }
        var_s0 -= 1;
        if (func_80088D70((s32) &sp54, (s32) &sp50) == 0) {
            goto block_4;
        }
        goto loop_1;
    }
block_4:
    if ((u32) *(u32 *)((unsigned char *)sp50 + (8)) >= (u32) D_800B58F4) {
        *(s32 *)((unsigned char *)D_80125C58 + (52)) = 1;
    }
    if ((*(s32 *)((unsigned char *)sp50 + (20)) != (*(s32 *)sp54)) || (*(s32 *)((unsigned char *)sp50 + (24)) != *(s32 *)((unsigned char *)sp54 + (4)))) {
        func_80088CB0((s32) sp54);
        return NULL;
    }
    D_800B58F0 = *(u32 *)((unsigned char *)sp50 + (8));
    var_a0 = *(s32 *)((unsigned char *)D_80125C58 + (60));
    if ((*(u16 *)((unsigned char *)sp50 + (16)) != var_a0) || (*(u16 *)((unsigned char *)sp50 + (18)) != *(s32 *)((unsigned char *)D_80125C58 + (64)))) {
        sp44 = 0;
        sp46 = 0;
        sp48 = 0x280;
        sp4A = 0x1E0;
        func_8009C7F8((RECT *) &sp44, 0, 0, 0);
        *(s32 *)((unsigned char *)D_80125C58 + (60)) = (s32) *(u16 *)((unsigned char *)sp50 + (16));
        *(s32 *)((unsigned char *)D_80125C58 + (64)) = (s32) *(u16 *)((unsigned char *)sp50 + (18));
        var_a0 = *(s32 *)((unsigned char *)D_80125C58 + (60));
    }
    if (D_800B5900 & 1) {
        temp_t3 = (s32) (var_a0 * 3) / 2;
        *(s16 *)((unsigned char *)arg0 + (28)) = (s16) temp_t3;
        *(s16 *)((unsigned char *)arg0 + (20)) = (s16) temp_t3;
    } else {
        *(s16 *)((unsigned char *)arg0 + (28)) = (s16) var_a0;
        *(s16 *)((unsigned char *)arg0 + (20)) = (s16) var_a0;
    }
    temp_v1 = (s16) *(s32 *)((unsigned char *)D_80125C58 + (64));
    *(s16 *)((unsigned char *)arg0 + (30)) = temp_v1;
    *(s16 *)((unsigned char *)arg0 + (22)) = temp_v1;
    *(s16 *)((unsigned char *)arg0 + (42)) = (s16) *(s32 *)((unsigned char *)D_80125C58 + (64));
    return sp54;
}

/* Implicit-int return type with no return statement: it changes how as1 fills the loop's
 * back-branch delay slot (T-9140, found by the permuter in T-1330). */
u32 strSync(SyncObj *arg0, s32 arg1) {
    volatile s32 timeout = 0x800000;

    while (arg0->flag == 0) {
        if (--timeout == 0) {
            arg0->flag = 1;
            if (arg0->idx != 0) {
                arg0->idx = 0;
            } else {
                arg0->idx = 1;
            }
            arg0->unk_24 = arg0->tbl[arg0->idx].unk_00;
            arg0->unk_26 = arg0->tbl[arg0->idx].unk_02;
        }
    }
    arg0->flag = 0;
}

void strKickCD(s32 arg0) {
    while (func_800879D0(0x15, arg0, 0) == 0) {
    }
    while (func_800880C0(0x1C0) == 0) {
    }
}

INCLUDE_ASM("asm/nonmatchings/main/800563F0", func_80056BA8);

void func_800570B8(s32 arg0, u8 arg1) {
    s32 t;

    t = arg1;
    func_80049A40(-0x90, 0x5B, 0x90, 0xE, 4, arg0, t);
    func_80049A40(0, 0x5B, 0x90, 0xE, 4, arg0, t);
    func_80049A40(-0x90, 0x69, 0x100, 0x10, 1, 0, 1);
}

INCLUDE_ASM("asm/nonmatchings/main/800563F0", func_8005715C);
