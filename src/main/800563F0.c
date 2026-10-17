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

#ifdef NON_MATCHING
void strCallback(void) {
    RECT rect;
    s32 odd;

    odd = D_800B5900 & 1;
    if (odd != 0 && D_80125C5C != 0) {
        func_80088210();
        odd = D_800B5900 & 1;
        D_80125C5C = 0;
    }
    rect = D_80125C58->dec.slice;
    if (odd != 0) {
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
#else
INCLUDE_ASM("asm/nonmatchings/main/800563F0", strCallback);
#endif

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

#ifdef NON_MATCHING
/* NON_MATCHING: T-17014, 2 differing lines; bne operand order of the width compare (t0,a0 expected). */
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
    if (sector->width != w || D_80125C58->height != sector->height) {
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
#else
INCLUDE_ASM("asm/nonmatchings/main/800563F0", strNext);
#endif

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

#ifdef NON_MATCHING
s32 func_80056BA8(s32 arg0, s32 arg1, s32 arg2) {
    CdlFILE file;
    DRAWENV draw;
    DISPENV disp;
    u32 start;
    s32 limit;
    s32 frames;
    s32 i;
    s32 id;
    s16 vol;

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
    strNextVlc((u8 *)&D_80125C58->dec);
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
    limit = arg1 - 3;
    start = D_800E6280.unk_10F4;
    for (;;) {
        func_800868BC(D_80125C58->dec.vlcbuf[D_80125C58->dec.vlcid], (D_800B5900 & 1) + 2);
        func_80086944(D_80125C58->dec.imgbuf, D_80125C58->dec.slice.w * D_80125C58->dec.slice.h / 2);
        strNextVlc((u8 *)&D_80125C58->dec);
        if (!(D_800B5900 & 1)) {
            func_800422C8();
            func_8005715C((D_800E6280.unk_10F4 - start) / 6);
            func_800476C0();
            disp_string();
            func_8009AC20();
            func_8009AD00(&D_800E8C70 + D_8011ECA0 * 5);
        }
        strSync((SyncObj *)&D_80125C58->dec, 0);
        func_800AD950(0);
        id = 1;
        if (D_80125C58->dec.rectid != 0) {
            id = 0;
        }
        SetDefDispEnv(&disp, D_80125C58->dec.rect[id].x, D_80125C58->dec.rect[id].y, D_80125C58->dec.rect[id].w, D_80125C58->dec.rect[id].h);
        func_8009F57C(&draw, D_80125C58->dec.rect[id].x, D_80125C58->dec.rect[id].y, D_80125C58->dec.rect[id].w, D_80125C58->dec.rect[id].h);
        if (D_800B5900 & 1) {
            disp.isrgb24 = 1;
            disp.disp.w = disp.disp.w * 2 / 3;
        }
        func_8009CCF4(&disp);
        func_8009CC04(&draw);
        func_8009C5E0(1);
        if (!(D_800B58F0 < limit && frames < arg1 - 0xF)) {
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
    vol = 0x7F;
    do {
        func_8008B750(vol, vol);
        func_8008BFB0(0, vol, vol);
        vol -= 10;
    } while (vol > 0);
    func_8009C5E0(1);
    func_8009C674(0);
}
#else
INCLUDE_ASM("asm/nonmatchings/main/800563F0", func_80056BA8);
#endif

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
