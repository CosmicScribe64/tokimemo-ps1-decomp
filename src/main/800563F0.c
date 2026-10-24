#define MAIN_API_OVERRIDE_func_8008B750 /* original narrows both volume arguments (func_80056BA8); T-25118 */
#define MAIN_API_OVERRIDE_func_8008BFB0 /* original narrows both volume arguments (func_80056BA8); T-25118 */
/* main_api.h overrides (T-3340, tools/sync_protos.py): the views this file was matched with. */

#include "common.h"
#include "game.h"
#include "libcd.h"
#include "psx_types.h"

void func_8008B750(s16, s16);
void func_8008BFB0(s32, s16, s16);

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

void strCallback(void) {
    RECT rect;

    /* FAKE: T-26000: never-true arm that ugen drops (as1 deletes the dead byte load). Its
     * indirect store puts the RECT copy below in block-copy state F (tools/ugen_pass.py), as in
     * the original (one scratch register before the second copy word; 75 rows without it).
     * IDO 4.1's ugen gives this body F without the arm, because it also sets F at a branch on a
     * value (`D_800B5900 & 1`); the pass does not model that and counts the store of an arm ugen
     * drops, which 4.1 ignores (copy_dropped_arm_store). Still needed in T-26002. */
    if ((u32)D_800B0896 < 0) {
        *(u8 *)0 = 0;
    }
    if ((D_800B5900 & 1) && D_80125C5C != 0) {
        func_80088210();
        D_80125C5C = 0;
    }
    rect = D_80125C58->dec.slice;
    if (D_800B5900 & 1) {
        rect.x += (D_800B58F8 * 3 + 2) / 3;
        rect.y += (D_800B58FC * 3 + 2) / 3;
    } else {
        rect.x += D_800B58F8;
        rect.y += D_800B58FC;
    }
    func_8009C884(&rect, D_80125C58->dec.imgbuf);
    func_8009C674(0);
    D_80125C58->dec.slice.x += D_80125C58->dec.slice.w;
    if (D_80125C58->dec.slice.x < D_80125C58->dec.rect[D_80125C58->dec.rectid].x + D_80125C58->dec.rect[D_80125C58->dec.rectid].w) {
        func_80086944(D_80125C58->dec.imgbuf, D_80125C58->dec.slice.w * D_80125C58->dec.slice.h / 2);
        return;
    }
    D_80125C58->dec.isdone = 1;
    if (D_80125C58->dec.rectid != 0) {
        D_80125C58->dec.rectid = 0;
    } else {
        D_80125C58->dec.rectid = 1;
    }
    D_80125C58->dec.slice.x = D_80125C58->dec.rect[D_80125C58->dec.rectid].x;
    D_80125C58->dec.slice.y = D_80125C58->dec.rect[D_80125C58->dec.rectid].y;
}

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

/* The PsyQ movie sample's player loop (CdSearchFile retry, DecDCTin/DecDCTout, double-buffered
 * DISPENV/DRAWENV) on the StrCtx at 0x80180000; arg0 is the file name, arg1 the end frame and arg2
 * the start sector (CdIntToPos when it is not 0). Returns 0 when the file is not found; the normal
 * exit has no return statement, as in the original (v0 is left by the last call). T-17014, T-25118 */
s32 func_80056BA8(s32 arg0, s32 arg1, s32 arg2) {
    s32 frames;
    s16 vol;
    DISPENV disp;
    DRAWENV draw;
    s32 i;
    s32 id;
    s32 pad[5]; /* FAKE: measured frame homes (draw at sp+0x90, file at sp+0x5C), not a source recovery. T-25118 */
    CdlFILE file;
    s32 pad2; /* FAKE: measured frame home (start at sp+0x54), not a source recovery. T-25118 */
    u32 start;

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
    D_80125C58 = (StrCtx *)0x80180000;
    D_80125C58->width = 0x140;
    D_80125C58->height = 0xF0;
    frames = 0;
    i = 0;
    if (arg2 == 0) {
        while (func_80088E34(&file, arg0) == 0) {
            if (i++ >= 0x10000) {
                return 0;
            }
        }
        D_80125C58->pos[0] = file.pos[0];
        D_80125C58->pos[1] = file.pos[1];
        D_80125C58->pos[2] = file.pos[2];
    } else {
        func_80087EE8(arg2, D_80125C58);
    }
    strSetDefDecEnv(&D_80125C58->dec, 0, 0, 0, 0xF0);
    strInit((s32)D_80125C58, (s32)strCallback);
    strNextVlc(&D_80125C58->dec);
    func_8008BEE0(0, 0, 1);
    func_8008BFB0(0, 0x7FFF, 0x7FFF);
    SetDefDispEnv(&disp, D_80125C58->dec.rect[1].x, D_80125C58->dec.rect[1].y, D_80125C58->dec.rect[1].w, D_80125C58->dec.rect[1].h);
    func_8009F57C(&draw, D_80125C58->dec.rect[1].x, D_80125C58->dec.rect[1].y, D_80125C58->dec.rect[1].w, D_80125C58->dec.rect[1].h);
    if (D_800B5900 & 1) {
        disp.isrgb24 = 1;
        disp.disp.w = disp.disp.w * 2 / 3;
    }
    func_8009CCF4(&disp);
    func_8009CC04(&draw);
    start = D_800E6280.unk_10F4;
    for (;;) {
        func_800868BC(D_80125C58->dec.vlcbuf[D_80125C58->dec.vlcid], (D_800B5900 & 1) + 2);
        func_80086944(D_80125C58->dec.imgbuf, D_80125C58->dec.slice.w * D_80125C58->dec.slice.h / 2);
        strNextVlc(&D_80125C58->dec);
        if (!(D_800B5900 & 1)) {
            func_800422C8();
            func_8005715C((D_800E6280.unk_10F4 - start) / 6);
            func_800476C0();
            disp_string();
            func_8009AC20();
            func_8009AD00((u8 *)&D_800E8C70 + D_8011ECA0 * 0x14);
        }
        strSync(&D_80125C58->dec, 0);
        func_800AD950(0);
        id = D_80125C58->dec.rectid ? 0 : 1;
        SetDefDispEnv(&disp, D_80125C58->dec.rect[id].x, D_80125C58->dec.rect[id].y, D_80125C58->dec.rect[id].w, D_80125C58->dec.rect[id].h);
        func_8009F57C(&draw, D_80125C58->dec.rect[id].x, D_80125C58->dec.rect[id].y, D_80125C58->dec.rect[id].w, D_80125C58->dec.rect[id].h);
        if (D_800B5900 & 1) {
            disp.isrgb24 = 1;
            disp.disp.w = disp.disp.w * 2 / 3;
        }
        func_8009CCF4(&disp);
        func_8009CC04(&draw);
        func_8009C5E0(1);
        /* FAKE: the empty test adds two uopt basic blocks inside the loop; with them the first use of a
         * new saved register costs more than keeping &draw or arg0 in one, as in the original. T-25118 */
        if (D_800B58F0) {
        }
        if (!(D_800B58F0 < arg1 - 3 && frames < arg1 - 0xF)) {
            break;
        }
        D_800E6280.unk_10F8 += 1;
        frames++;
        if (D_80125C58->endflag == 1) {
            break;
        }
        func_80042A00();
        if (D_800E6280.unk_F88 & 0x860) {
            break;
        }
    }
    disp.isrgb24 = 0;
    func_8009CCF4(&disp);
    func_8009C5E0(0);
    func_8004111C();
    for (vol = 0x7F; vol > 0; vol -= 10) {
        func_8008B750(vol, vol);
        func_8008BFB0(0, vol, vol);
    }
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
