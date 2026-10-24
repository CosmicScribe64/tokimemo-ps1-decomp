#define MAIN_API_OVERRIDE_D_80120656 /* 0x82 must not sign-extend (main_api.h: s8) */
#include "common.h"
#include "ovl/BUNKA_SD.h"

extern u8 D_80120656;

s32 func_80134540(void) {
    if (D_800E6280.substep == 0) {
        cd_read_sectors(0x85, 0x80180000, 0x4BD4);
        D_800E6280.substep += 1;
    } else if ((D_800E6280.substep == 1) && (cd_read_sync_status() & 1)) {
        func_8004284C();
    }
}

/* FAKE: the D_8012069A store shares a source line with the loop head; as1 schedules by line and this gives the original's order of the loop setup (T-7020, T-9380) */
void func_801345BC(void) {
    s32 i;

    if ((D_800E6280.step >= 3) && (D_800E6280.step < 7)) {
        if (D_800E6280.pad_held & PAD_CIRCLE) {
            func_801346D4();
            D_80120656 = 0x82;
            D_8012069A = 0x82; for (i = 0; i < 2; i++) {
                D_8011ED58[(i + 1) * 0x110 + 0x18BA] = 0x82;
                D_8011ED58[(i + 1) * 0x110 + 0x18FE] = 0x82;
                D_8011ED58[(i + 1) * 0x110 + 0x1942] = 0x82;
                D_8011ED58[(i + 1) * 0x110 + 0x1876] = 0x82;
            }
        } else {
            D_80120656 = 1;
            D_8012069A = 1; for (i = 0; i < 2; i++) {
                D_8011ED58[(i + 1) * 0x110 + 0x18BA] = 1;
                D_8011ED58[(i + 1) * 0x110 + 0x18FE] = 1;
                D_8011ED58[(i + 1) * 0x110 + 0x1942] = 1;
                D_8011ED58[(i + 1) * 0x110 + 0x1876] = 1;
            }
        }
    }
    if ((D_800E6280.step >= 3) && (D_800E6280.step < 6) && (D_800E6280.pad_held & PAD_CROSS)) {
        func_80042940(5);
    }
    func_801346D4();
}

void func_801346D4(void) {
    func_80066C08(2);
    message_window_show();
    func_80066334();
    switch (D_800E6280.step) {
    case 0:
        func_80134260();
        return;
    case 1:
        func_80134540();
        return;
    case 2:
        func_801347CC();
        return;
    case 3:
        func_80134418();
        return;
    case 4:
        func_80134800();
        return;
    case 5:
        func_80044750(0xC1);
        k_reset(1);
        func_8004284C();
        return;
    case 6:
        func_80134850();
        return;
    case 7:
        func_80134314();
        return;
    default:
        func_80046500();
        return;
    }
}

void func_801347CC(void) {
    func_8004500C(1, 0x201);
    func_80134938();
    func_8004284C();
}

void func_80134800(void) {
    u32 t;

    func_80134B88();
    func_80134D94();
    t = D_800E6280.step_timer.u + 1;
    D_800E6280.step_timer.u = t;
    if (t >= 0x818) {
        func_8004284C();
    }
}

/* FAKE: T-22021. The bytes D_800E6280.mode_scratch and unk_110C are read and written only as u8 views, so uopt promotes each to one register (see func_8013987C in 801388D0.c); `*= 0` keeps the `move v1,zero` that a plain `= 0` folds into `sb zero`; `s32` without a return matches the implicit-int call earlier in this file (no prototype); the original source is unknown. */
s32 func_80134850(void) {
    s32 i;
    u32 q;

    if (D_80122EAC == 3) {
        D_80120762 = 0x40;
        q = *(u8 *)&D_800E6280.stage_scratch / 14;
        if (q >= 2) {
            *(u8 *)&D_800E6280.stage_scratch = 0;
            q = 0;
        }
        D_80120778 = q + 0xA2;
        *(u8 *)&D_800E6280.stage_scratch += 1;
    }
    for (i = 0; i < 5; i++) {
        ((Actor *)D_8011ECD0)[96 + i].brightness = *(u8 *)&D_800E6280.mode_scratch;
    }
    *(u8 *)&D_800E6280.mode_scratch -= 2;
    if (*(u8 *)&D_800E6280.mode_scratch == 0) {
        *(u8 *)&D_800E6280.mode_scratch *= 0;
        for (i = 0; i < 0x40; i++) {
            ((Actor *)D_80120650)[i].brightness = *(u8 *)&D_800E6280.mode_scratch;
        }
        func_8004284C();
    }
}

void func_80134938(void) {
    s32 i;

    for (i = 0; i < 0x10; i++) {
        load_palette(D_8013C71C, (i + 0x10) & 0xFF, 1, 1, i);
    }
    func_80048F64(0x60);
    ((Actor *)D_8011ECD0)[96].unk_01 = 5;
    ((Actor *)D_8011ECD0)[96].quad_flags = 0x01000000;
    ((Actor *)D_8011ECD0)[96].draw_flags = 0x80;
    ((Actor *)D_8011ECD0)[96].frame_table = D_8013C714;
    ((Actor *)D_8011ECD0)[96].sequence_table = D_8013C718;
    ((Actor *)D_8011ECD0)[96].tpage_table = (u8 *)D_8013C710;
    ((Actor *)D_8011ECD0)[96].sequence_slot = 0xA;
    ((Actor *)D_8011ECD0)[96].brightness = 0;
    ((Actor *)D_8011ECD0)[96].x = -0xA0;
    ((Actor *)D_8011ECD0)[96].y = -0x78;
    ((Actor *)D_8011ECD0)[96].unk_05 = 0x80;
    ((Actor *)D_8011ECD0)[96].unk_43 = 0;
    ((Actor *)D_8011ECD0)[96].unk_04 = 0;
    for (i = 1; i < 5; i++) {
        func_80048F64(i + 0x60);
        ((Actor *)D_8011ECD0)[96 + i].unk_01 = 4;
        ((Actor *)D_8011ECD0)[96 + i].quad_flags = 0x01000000;
        ((Actor *)D_8011ECD0)[96 + i].draw_flags = 0;
        ((Actor *)D_8011ECD0)[96 + i].frame_table = D_8013C714;
        ((Actor *)D_8011ECD0)[96 + i].sequence_table = D_8013C718;
        ((Actor *)D_8011ECD0)[96 + i].tpage_table = (u8 *)D_8013C710;
        ((Actor *)D_8011ECD0)[96 + i].sequence_slot = i - 1;
        ((Actor *)D_8011ECD0)[96 + i].blink_period = 1;
        ((Actor *)D_8011ECD0)[96 + i].brightness = 0;
        ((Actor *)D_8011ECD0)[96 + i].x = -0xA0;
        ((Actor *)D_8011ECD0)[96 + i].y = -0x78;
        ((Actor *)D_8011ECD0)[96 + i].unk_05 = 0x80;
        ((Actor *)D_8011ECD0)[96 + i].unk_43 = 0;
        ((Actor *)D_8011ECD0)[96 + i].unk_04 = 0;
    }
    ((Actor *)D_8011ECD0)[99].sequence_slot = D_80122EAC * 2 + 2;
    ((Actor *)D_8011ECD0)[99].draw_flags = 0x80;
    ((Actor *)D_8011ECD0)[100].sequence_slot = D_80122EAC * 2 + 3;
    search_load_tpage_buf_lock(D_8013C710[11], 1);
    search_load_tpage_buf_lock(D_8013C710[12], 1);
    search_load_tpage_buf_lock(D_8013C710[13], 1);
    search_load_tpage_buf_lock(D_8013C710[14], 1);
}

void func_80134B88(void) {
    /* u32 selector: sltiu compares and the selector stays in $v0 (T-8020) */
    switch ((u32)D_800E6280.step_timer.w) {                     /* irregular */
    case 0x4F2:
        func_80044750(0x509);
        break;
    case 0x542:
        func_80044750(0x509);
        break;
    case 0x592:
        func_80044750(0x509);
        break;
    case 0x606:
        func_80044750(0x500);
        break;
    case 0xF0:
        func_80044750(0x50A);
        break;
    case 0x150:
        func_80044750(0x50A);
        break;
    case 0x1B0:
        func_80044750(0x50A);
        break;
    case 0x210:
        func_80044750(0x50A);
        break;
    case 0x270:
        func_80044750(0x50A);
        break;
    case 0x2D0:
        func_80044750(0x50A);
        break;
    case 0x330:
        func_80044750(0x50A);
        break;
    case 0x390:
        func_80044750(0x50A);
        break;
    case 0x3F0:
        func_80044750(0x50A);
        break;
    case 0x450:
        func_80044750(0x50A);
        break;
    case 0x610:
        func_80044750(0x502);
        break;
    case 0x4B0:
        if (D_80122EAC == 3) {
            func_80044750(0x50A);
        }
        break;
    case 0x520:
        if (D_80122EAC == 3) {
            func_80044750(0x50A);
        }
        break;
    }
}

/* An implicit-int function: the last block returns its quotient in v0 (declared `s32` in the header). Every
 * GameState read goes through `g`: uopt does not copy-propagate an expression that holds a load through a
 * pointer, so the fade colour stays one variable (as in ETC func_80144224, T-25146) and the first counter is
 * loaded straight into its own register. Grey level is (0x68E - frame) * 0x10201. T-25142, T-25306 */
s32 func_80134D94(void) {
    u32 q;
    s32 b;
    s32 col;
    s32 t;
    GameState *g;

    g = &D_800E6280;
    if (g->step_timer.u == 0) {
        D_8012071E = 5;
    }
    if (g->step_timer.u == 0xFE) {
        D_80120697 = 0x80;
        D_80120696 = 5;
        D_8012069B = 0x80;
    }
    if ((D_80120696 & 1) == 0) {
        D_80120697 = 0;
    }
    if (g->step_timer.u == 0x29C) {
        D_801206DB = 0x80;
        D_801206DA = 5;
        D_801206DF = 0x80;
    }
    if (g->step_timer.u >= 0xFF && g->step_timer.u < 0x5A3) {
        b = g->substep;
        D_8012071E = 0x40;
        /* FAKE: T-25306. A copy of the counter that uopt keeps as its own range and as1 folds back into the
         * divu; it holds v1 in this block, so q, b and the frame counter get a0, a1 and t0 as in the original. */
        t = (u32)b * 1;
        if (t < 0) {
        }
        q = (u32)t / 14;
        if (q >= 2) {
            b = 0;
            q = 0;
        }
        /* FAKE: T-25142. `q * 1` is not a plain copy, so copy propagation keeps a range for the arm value (`move v0,a0` at the switch head); the empty test keeps it before the switch (as `q * 2` in func_80132708). */
        t = q * 1;
        if (t < 0) {
        }
        switch (D_80122EAC) {
        case 0:
            D_80120734 = t + 0x5D;
            break;
        case 1:
            D_80120734 = t + 0x72;
            break;
        case 2:
            D_80120734 = t + 0x87;
            break;
        case 3:
        default:
            if (g->step_timer.u < 0x4C2) {
                D_80120734 = t + 0x9D;
            } else {
                D_80120734 = t + 0x9F;
            }
            break;
        }
        g->substep = b + 1;
    }
    if (g->step_timer.u == 0x4C2 && D_80122EAC != 3) {
        D_8012071F = 0;
        D_80120763 = 0x80;
        D_80120762 = 5;
        D_80120767 = 0x80;
    }
    if (g->step_timer.u == 0x58F && D_80122EAC == 3) {
        D_8012071F = 0;
        D_80120763 = 0x80;
        D_80120762 = 5;
        D_80120767 = 0x80;
    }
    if (g->step_timer.u == 0x606 || g->step_timer.u == 0x609 || g->step_timer.u == 0x60C) {
        func_80049A40(-0xA0, -0x78, 0x140, 0xA0, 2, 0xFFFFFF, 0x81);
        dtd_on_tpage(0, 0, 2, 0, 0);
    }
    if (g->step_timer.u == 0x606) {
        D_801206F0 = 0x48;
        D_801206E0 = 0;
    }
    if (g->step_timer.u >= 0x60F && g->step_timer.u < 0x68E) {
        col = 0x68E - g->step_timer.u;
        col += col << 8;
        col += col << 8;
        func_80049A40(-0x130, -0x78, 0x140, 0xA0, 2, col, 0x86);
        func_80049A40(0x10, -0x78, 0x140, 0xA0, 2, col, 0x85);
        dtd_on_tpage(0, 0, 2, 1, 0);
    }
    if (g->step_timer.u == 0x67E) {
        D_80120657 = 0x80;
        D_801206DF = 0x80;
        D_80120767 = 0x80;
    }
    if (g->step_timer.u >= 0x723 && D_80122EAC == 3) {
        u32 w;
        u32 n;

        /* FAKE: T-25306. u8 view of the s8 counter, read with lbu as in func_80134850 (T-22021). */
        n = *(u8 *)&g->stage_scratch;
        D_80120762 = 0x40;
        w = n / 14;
        if (w >= 2) {
            n = 0;
            w = 0;
        }
        D_80120778 = w + 0xA2;
        /* FAKE: T-25306. An extra use of w colours it before n (w v1, n a0). The selector read keeps the
         * selector live in this block, where w holds v1, so it takes a2 everywhere and its load in the
         * first block stays at the switch. */
        if (w) {}
        if (D_80122EAC) {}
        *(u8 *)&g->stage_scratch = n + 1;
/* FAKE: T-25306. as1 breaks the tie between this store and `move v0,v1` by source line, smaller first
 * (wiki/as1-scheduler-rule.md, T-25137); the original copies w before the store, so the return gets an
 * earlier line and the next `#line` restores the numbering. */
#line 352
        return w;
#line 360
    }
}
