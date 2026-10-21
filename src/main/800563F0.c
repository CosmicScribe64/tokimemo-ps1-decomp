#include "common.h"
#include "game.h"

void set_movie_offset(s16 arg0, s16 arg1) {
    D_800B58F8 = arg0;
    D_800B58FC = arg1;
}

void strInit(s32 arg0, s32 arg1) {
    DecDCTReset(0);
    D_80125C58->endflag = 0;
    func_800869C8(arg1);
    func_80088150((u8 *)D_80125C58 + 0x44, 0x20);
    func_80088180(D_800B5900 & 1, 1, -1, 0, 0);
    strKickCD(arg0);
}

void strSetDefDecEnv(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4)
{
    u32 width;
    u32 height;
    StrDecEnv30 *env;

    env = arg0;
    env->vlcbuf[0] = (u8 *) D_80125C58 + 0x12D44;
    env->vlcbuf[1] = (u8 *) D_80125C58 + 0x48D44;
    env->vlcid = 0;
    env->imgbuf = (u8 *) D_80125C58 + 0x10044;
    env->rectid = 0;
    env->isdone = 0;
    width = 0x140;
    height = 0xF0;
    if (D_800B5900 & 1) {
        width = 0x1E0;
        height = 0xF0;
        /* FAKE: one source line gives as1 the original halfword-store order. T-9930. */
        env->rect[0].w = width; env->rect[0].h = height;
        env->rect[0].x = arg1;
        env->rect[0].y = arg2;
        env->rect[1].x = arg3;
        env->rect[1].y = arg4;
        env->rect[1].h = height;
        env->rect[1].w = width;
        env->slice.w = 0x18;
        env->slice.h = height;
        env->slice.y = arg2;
        env->slice.x = arg1;
        return;
    }
    env->rect[0].x = arg1;
    env->rect[0].y = arg2;
    env->rect[0].w = width;
    env->rect[0].h = height;
    env->rect[1].x = arg3;
    env->rect[1].y = arg4;
    env->rect[1].h = height;
    env->rect[1].w = width;
    env->slice.w = 0x10;
    env->slice.h = height;
    env->slice.y = arg2;
    env->slice.x = arg1;
}

INCLUDE_ASM("asm/nonmatchings/main/800563F0", strCallback);

s32 strNextVlc(StrDecEnv30 *arg0) {
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
    if (arg0->vlcid != 0) {
        arg0->vlcid = 0;
    } else {
        arg0->vlcid = 1;
    }
    func_80086870(vlc, arg0->vlcbuf[arg0->vlcid]);
    func_80088CB0(vlc);
    return 0;
}

s32 strNext(StrDecEnv30 *env) {
    u32 *addr;
    StHEADER *sector;
    s32 cnt;
    RECT rect;
    s32 w;

    cnt = 0x800000;
    while (func_80088D70(&addr, &sector) != 0) {
        if (--cnt == 0) {
            return 0;
        }
    }
    if (sector->frameCount >= D_800B58F4) {
        D_80125C58->endflag = 1;
    }
    if (sector->headm != addr[0] || sector->headv != addr[1]) {
        func_80088CB0(addr);
        return 0;
    }
    D_800B58F0 = sector->frameCount;
    w = D_80125C58->width;
    /* FAKE: `w * 1` keeps the sector width first in the bne (a plain w would go first); the local
     * holds $v0 so the width takes $a0. T-17014 */
    if (w * 1 != sector->width || D_80125C58->height != sector->height) {
        rect.x = 0;
        rect.y = 0;
        rect.w = 0x280;
        rect.h = 0x1E0;
        func_8009C7F8(&rect, 0, 0, 0);
        D_80125C58->width = sector->width;
        D_80125C58->height = sector->height;
    }
    if (D_800B5900 & 1) {
        env->rect[0].w = env->rect[1].w = D_80125C58->width * 3 / 2;
    } else {
        env->rect[0].w = env->rect[1].w = D_80125C58->width;
    }
    env->rect[0].h = env->rect[1].h = D_80125C58->height;
    env->slice.h = D_80125C58->height;
    return (s32)addr;
}

/* Implicit-int return type with no return statement: it changes how as1 fills the loop's
 * back-branch delay slot (T-9140, found by the permuter in T-1330). */
u32 strSync(StrDecEnv30 *arg0, s32 arg1) {
    volatile s32 timeout = 0x800000;

    while (arg0->isdone == 0) {
        if (--timeout == 0) {
            arg0->isdone = 1;
            if (arg0->rectid != 0) {
                arg0->rectid = 0;
            } else {
                arg0->rectid = 1;
            }
            arg0->slice.x = arg0->rect[arg0->rectid].x;
            arg0->slice.y = arg0->rect[arg0->rectid].y;
        }
    }
    arg0->isdone = 0;
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

void func_8005715C(s32 arg0) {
    /* FAKE: unused scalar reserving the original's fourth stack slot (declared first = highest). T-17014 */
    s32 pad;
    s32 d;
    s32 i;
    u8 *p;

    p = D_800B5904;
    for (i = 0; i < 22; i++) {
        if (arg0 < (s32)(*p * 0x50 / 0xF)) {
            break;
        }
        p++;
    }
    if (i != 0) {
        k_disp_switch(i - 1, 0);
        k_disp_start(i + 1);
    }
    if (p >= &D_800B5904[4] && p < &D_800B5904[9]) {
        func_8006C760(0);
    } else {
        func_8006C760(1);
    }
    if (p >= &D_800B5904[21]) {
        func_8006C760(0);
        func_800570B8(0, 1);
        func_8004E58C();
        return;
    }
    if (D_800B58F8 != 0) {
        d = *p * 0x50 / 0xF - arg0;
        if (d == 4 || d == 5) {
            func_800570B8((D_800B5900 + D_800E7D35) * 0xDF7FCF + 0xFF208031, 1);
        } else {
            func_800570B8((D_800B5900 + D_800E7D35) * 0xDF7FCF + 0xFF208031, 0x81);
        }
    }
}
