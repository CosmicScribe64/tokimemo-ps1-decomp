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

INCLUDE_ASM("asm/nonmatchings/main/800563F0", strNext);

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

/* bulk_m2c declarations (T-11190) */
extern s32 D_800B58F0;
extern s32 D_800B58F4;
extern s32 D_800E7208;
extern s32 D_800E7374;
extern s32 D_800E7378;
extern s32 strCallback;
extern u8 D_8018003C;
/* end bulk_m2c declarations */
void func_80056BA8(s32 arg0, s32 arg1, s32 arg2) {
    s8 spFD;
    s16 spF0;
    s32 spEC[1];
    s32 sp90[1];
    u8 sp5C;
    s32 sp54;
    s32 sp48;
    s16 var_s0_2;
    s32 temp_s1;
    s32 var_a0;
    s32 var_s0;
    s32 var_s2;
    s32 var_v0;
    s32 var_v0_2;
    u8 *temp_s0;
    u8 *temp_s0_2;

    D_800B58F4 = arg1;
    DecDCTReset(0);
    func_800869C8(0);
    func_800869A4(0);
    func_8009C210(1);
    if (D_8011ECA0 == 0) {
        func_8009AC20();
        func_800422C8();
    }
    D_800B58F0 = 0;
    D_80125C58 = &D_8018003C;
    D_8018003C = 0x140;
    *(s32 *)((unsigned char *)(&D_8018003C) + (4)) = 0xF0;
    var_s2 = 0;
    var_s0 = 0;
    if (arg2 == 0) {
        if (func_80088E34((s32) &sp5C, arg0) == 0) {
            var_v0 = 0 < 0x10000;
loop_5:
            var_s0 += 1;
            if (var_v0 == 0) {
                return;
            }
            var_v0 = var_s0 < 0x10000;
            if (func_80088E34((s32) &sp5C, arg0) != 0) {
                goto block_8;
            }
            goto loop_5;
        }
block_8:
        (*(s32 *)D_80125C58) = sp5C;
        *(u8 *)((unsigned char *)D_80125C58 + (1)) = sp5D;
        *(u8 *)((unsigned char *)D_80125C58 + (2)) = sp5E;
        goto block_10;
    }
    func_80087EE8(arg2, (s32) &D_8018003C);
block_10:
    strSetDefDecEnv((s32) (D_80125C58 + 4), 0, 0, 0, /* extra? */ 0xF0);
    strInit((s32) D_80125C58, (s32) &strCallback);
    strNextVlc((s32) (D_80125C58 + 4));
    func_8008BEE0(0, 0, 1);
    func_8008BFB0(0, 0x7FFF, 0x7FFF);
    SetDefDispEnv((s32) &spEC, (s32) *(s16 *)((unsigned char *)D_80125C58 + (28)), (s32) *(s16 *)((unsigned char *)D_80125C58 + (30)), (s32) *(s16 *)((unsigned char *)D_80125C58 + (32)), /* extra? */ (s32) *(s16 *)((unsigned char *)D_80125C58 + (34)));
    func_8009F57C((s32) &sp90, (s32) *(s16 *)((unsigned char *)D_80125C58 + (28)), (s32) *(s16 *)((unsigned char *)D_80125C58 + (30)), (s32) *(s16 *)((unsigned char *)D_80125C58 + (32)), (s32) *(s16 *)((unsigned char *)D_80125C58 + (34)));
    if (D_800B5900 & 1) {
        spFD = 1;
        spF0 = (s16) ((s32) (spF0 * 2) / 3);
    }
    func_8009CCF4((s32) &spEC);
    func_8009CC04((s32) &sp90);
    sp48 = arg1 - 3;
    sp54 = D_800E7374;
loop_13:
    func_800868BC(*(s32 *)((unsigned char *)(D_80125C58 + (*(s32 *)((unsigned char *)D_80125C58 + (12)) * 4)) + (4)), (D_800B5900 & 1) + 2);
    func_80086944(*(s32 *)((unsigned char *)D_80125C58 + (16)), (s32) (*(s16 *)((unsigned char *)D_80125C58 + (44)) * *(s16 *)((unsigned char *)D_80125C58 + (46))) / 2);
    strNextVlc((s32) (D_80125C58 + 4));
    if (!(D_800B5900 & 1)) {
        func_800422C8();
        func_8005715C((s32) ((u32) (D_800E7374 - sp54) / 6U));
        func_800476C0();
        disp_string();
        func_8009AC20();
        func_8009AD00((s32) ((D_8011ECA0 * 0x14) + &D_800E8C70));
    }
    strSync((s32) (D_80125C58 + 4), 0);
    func_800AD950(0);
    var_v0_2 = 1;
    if (*(s32 *)((unsigned char *)D_80125C58 + (36)) != 0) {
        var_v0_2 = 0;
    }
    temp_s1 = var_v0_2 * 8;
    temp_s0 = D_80125C58 + temp_s1;
    SetDefDispEnv((s32) &spEC, (s32) *(s16 *)((unsigned char *)temp_s0 + (20)), (s32) *(s16 *)((unsigned char *)temp_s0 + (22)), (s32) *(s16 *)((unsigned char *)temp_s0 + (24)), /* extra? */ (s32) *(s16 *)((unsigned char *)temp_s0 + (26)));
    temp_s0_2 = D_80125C58 + temp_s1;
    func_8009F57C((s32) &sp90, (s32) *(s16 *)((unsigned char *)temp_s0_2 + (20)), (s32) *(s16 *)((unsigned char *)temp_s0_2 + (22)), (s32) *(s16 *)((unsigned char *)temp_s0_2 + (24)), (s32) *(s16 *)((unsigned char *)temp_s0_2 + (26)));
    if (D_800B5900 & 1) {
        spFD = 1;
        spF0 = (s16) ((s32) (spF0 * 2) / 3);
    }
    func_8009CCF4((s32) &spEC);
    func_8009CC04((s32) &sp90);
    func_8009C5E0(1);
    if ((D_800B58F0 < sp48) && (var_s2 < (arg1 - 0xF))) {
        D_800E7378 += 1;
        var_s2 += 1;
        if (*(s32 *)((unsigned char *)D_80125C58 + (52)) != 1) {
            func_80042A00();
            if (!(D_800E7208 & 0x860)) {
                goto loop_13;
            }
        }
    }
    spFD = 0;
    func_8009CCF4((s32) &spEC);
    func_8009C5E0(0);
    func_8004111C();
    var_s0_2 = 0x7F;
    var_a0 = 0x7F << 0x10;
    do {
        func_8008B750(var_a0 >> 0x10, (s32) var_s0_2);
        func_8008BFB0(0, (s32) var_s0_2, (s32) var_s0_2);
        var_s0_2 -= 0xA;
        var_a0 = var_s0_2 << 0x10;
    } while (var_s0_2 > 0);
    func_8009C5E0(1);
    func_8009C674(0);
}

void func_800570B8(s32 arg0, u8 arg1) {
    s32 t;

    t = arg1;
    func_80049A40(-0x90, 0x5B, 0x90, 0xE, 4, arg0, t);
    func_80049A40(0, 0x5B, 0x90, 0xE, 4, arg0, t);
    func_80049A40(-0x90, 0x69, 0x100, 0x10, 1, 0, 1);
}

INCLUDE_ASM("asm/nonmatchings/main/800563F0", func_8005715C);
