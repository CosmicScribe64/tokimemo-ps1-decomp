#include "common.h"
#include "game.h"
#include "psx_types.h"

void func_80085E30(s32 arg0, s32 arg1) {
    draw2d3d(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048E78();
    back_clear_switch(arg0);
    D_800E6280.unk_036 = 0;
    D_800E6280.unk_037 = 0;
    D_800E6280.unk_038 = 0;
    D_800E6280.unk_03A = 0x80;
    func_8004E58C();
    D_800E6280.unk_10A2 = 0;
    if (arg1 != 0) {
        tpage_buf_clear_all();
        return;
    }
    tpage_buf_clear();
}

void Vblnk_Timer_Init(void) {
    D_800E36F0 = D_800E6280.unk_10F4;
    D_800E36F4 = 0;
}

s32 Vblnk_Timer(void) {
    D_800E36F4 = D_800E6280.unk_10F4 - D_800E36F0;
    return D_800E36F4;
}

void Scroll(s32 arg0, s32 arg1, s16 arg2, s16 arg3, s16 arg4) {
    POLY_FT4 *p;
    s32 i;
    u16 lo;
    s32 pad1;
    s32 pad2;

    for (i = 0; i < 5; i++) {
        p = (POLY_FT4 *)GetWorkBase(0x28, D_8011ECA0);
        func_8009F040(p);
        SetSemiTrans(p, 0);
        SetShadeTex(p, 0);
        p->r0 = D_800E6280.unk_03A;
        p->g0 = D_800E6280.unk_03A;
        p->b0 = D_800E6280.unk_03A;
        lo = (arg0 * 2 + (i % 2) * 0x40) & 0x7F;
        p->u1 = lo + 0x40;
        p->u3 = lo + 0x40;
        p->u0 = lo;
        p->u2 = lo;
        /* FAKE: dead stores to pad1/pad2 order the loop-invariant expression bit positions and stack home slots (`i * 0x20` before `i * 0x40`, `(u32)arg4` at 0x60($sp)). T-24127 */
        pad1 = i * 0x20;
        pad2 = (u32)arg4;
        p->v3 = arg2 + arg3;
        p->v2 = arg2 + arg3;
        p->v1 = arg2;
        p->v0 = arg2;
        p->x0 = D_800E6280.unk_014[D_8011ECA0] + i * 0x40 - 0xA0;
        p->y0 = D_800E6280.unk_018[D_8011ECA0] + arg2 - 0x78;
        p->x1 = D_800E6280.unk_014[D_8011ECA0] + i * 0x40 - 0x60;
        p->y1 = D_800E6280.unk_018[D_8011ECA0] + arg2 - 0x78;
        p->x2 = D_800E6280.unk_014[D_8011ECA0] + i * 0x40 - 0xA0;
        p->y2 = D_800E6280.unk_018[D_8011ECA0] + arg2 + arg3 - 0x78;
        p->x3 = D_800E6280.unk_014[D_8011ECA0] + i * 0x40 - 0x60;
        p->y3 = D_800E6280.unk_018[D_8011ECA0] + arg2 + arg3 - 0x78;
        p->tpage = func_8009ECB0(1, 0, (((u32)(i * 0x20) + (u32)arg0) / 0x40 * 0x40) % (u32)arg4 + 0x140, 0);
        p->clut = func_8009ED74(0, 0x1F0);
        AddPrim(D_800E8CA0 + (D_8011ECA0 << 10) + arg1 * 4, p);
    }
    /* FAKE: raises `(u8)arg2` priority above `arg3` and `arg2 + arg3` so `$s6 = arg2, $s7 = arg3, $s8 = arg2 + arg3`. T-24127 */
    if ((u8)arg2) {}
}

void Rot_2D(s32 arg0, s32 *arg1, s32 *arg2, s32 *arg3, s32 *arg4) {
    s32 s;
    s32 c;

    s = rsin();
    c = func_800A0140(arg0);
    *arg3 = c * *arg1 / 4096 - *arg2 * s / 4096;
    *arg4 = s * *arg1 / 4096 + *arg2 * c / 4096;
}

void Fade_Out_Init(void) {
    Vblnk_Timer_Init();
}

/* temp: the byte is read into a local after the timer test; with a direct compare IDO loads it into $v0 and puts the li v0,1 in the branch delay slot (T-17022, matching-notes) */
s32 Fade_Out_Main(void) {
    u8 temp;
    if (Vblnk_Timer() != 0) {
        temp = D_800E6280.unk_03A;
        if (temp < 3U) {
            D_800E6280.unk_03A = 0;
            return 1;
        }
        D_800E6280.unk_03A -= 4;
        Vblnk_Timer_Init();
        return 0;
    }
    return 0;
}

void Fade_In_Init(void) {
    Vblnk_Timer_Init();
}

/* temp: same byte local as Fade_Out_Main (T-17022, matching-notes) */
s32 Fade_In_Main(void) {
    u8 temp;
    if (Vblnk_Timer() != 0) {
        temp = D_800E6280.unk_03A;
        if (temp >= 0x7EU) {
            D_800E6280.unk_03A = 0x80;
            return 1;
        }
        D_800E6280.unk_03A += 4;
        Vblnk_Timer_Init();
        return 0;
    }
    return 0;
}

void Yubi(s32 idx) {
    u8 *p;
    func_8006BA40();
    p = D_8011ECD0 + idx * 0x44;
    *(s16 *)(p + 0x26) = D_8011ECF6;
    *(s16 *)(p + 0x2A) = D_8011ECFA;
}

void Default_Disp(void) {
    k_disp_inc2();
    check_k_scroll();
    func_80083A10();
    message_window_show();
    hizuke_show();
    func_80066C08(2);
    func_80066334();
    func_80047560();
}

s32 func_8008647C(void) {
    s32 sum1;
    s32 sum2;
    s32 cur;
    s32 list[14]; /* FAKE: 14-word list keeps list at 0x6C($sp) in the 0xB0 frame alongside sum1/sum2/cur. T-24111 */
    u32 n;
    s32 i;
    s32 best;
    s32 bscore;
    s32 score;
    u32 t;

    n = 0;
    for (i = 0; i < 11; i++) {
        if (D_800E6280.unk_1BC[i].unk_0C.f.b1 == 1) {
            list[n] = i;
            n = (n + 1) & 0xFF;
        }
    }
    t = get_h_yuukou(0) & 0xFF;
    bscore = get_h_tokimeki(0) + t;
    best = 0;
    for (i = 1; i < n; i++) {
        t = get_h_tokimeki(list[i]) & 0xFF;
        score = get_h_yuukou(list[i]) + t;
        if (bscore < score && (t = func_8008667C(best) & 0xFF, t >= func_8008667C(list[i]))) {
            best = list[i];
            bscore = score;
        } else if (bscore == score) {
            t = func_8008667C(best) & 0xFF;
            if (t >= func_8008667C(list[i])) {
                cur = list[i];
                /* FAKE: explicit (s32) cast on unk_06 evaluates unk_06 before unk_02 while keeping the sum in $a0/$a1. T-24111 */
                sum1 = D_800E6280.unk_1BC[best].unk_02 + (s32)D_800E6280.unk_1BC[best].unk_06;
                sum2 = D_800E6280.unk_1BC[cur].unk_02 + (s32)D_800E6280.unk_1BC[cur].unk_06;
                if (sum1 < sum2) {
                    bscore = score;
                    best = cur;
                }
            }
        }
    }
    return best;
}

/* FAKE: same-line swap and pointer read retain the original halfword load order. T-9550 */
s32 func_80086640(void)
{

  s16 *right;
  s16 old_right;
  s16 old_left;
  old_left = D_80120666; right = &D_801206EE; old_right = *right; D_80120666 = old_right; D_801206EE = old_left;
  D_80120668 = 0;
  D_801206F0 = 0;
  D_80120658 = 0;
}

s32 func_8008667C(s32 a) {
    /* FAKE: the (u8) cast reserves the extra temp the original has (same as GYOZI func_80140D7C); real return type unknown. T-4060 */
    return (u8)get_g_zyotai_s(a) & 0x7F;
}

void func_800866A0(void) {
    if (D_800E6280.unk_110D == 0) {
        func_80062CD0();
        D_800E6280.unk_110D += 1;
        return;
    }
    if (cd_read_sync_status() & 1) {
        if (*(s32 *)D_800CA120 != 0x801820AC || *(s32 *)(D_800CA120 + 4) != 0x801860AC || *(s32 *)(D_800CA120 + 8) != 0x8018A0AC || *(s32 *)(D_800CA120 + 0xC) != 0x8018E0AC || *(s32 *)(D_800CA120 + 0x10) != 0x801920AC || *(s32 *)D_800CA128 != 0x8018005C || *(s32 *)(D_800CA128 + 4) != 0x80180070 || *(s32 *)D_800CA124 != 0x80180000) {
            D_800E6280.unk_110D = 0;
            return;
        }
        if (file_checksum_ok(0x2D, 0x80180000) == 0) {
            D_800E6280.unk_110D = 0;
            return;
        }
        func_8004284C();
    }
}

void func_800867CC(void) {
    if (D_80122EA0 == 0) {
        if (func_80044E8C() == 1) {
            D_80122EA0 = 1;
        }
    }
}
