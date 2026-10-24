#include "common.h"
#include "ovl/EN_NICHI.h"

typedef struct EnWork1C20 {
    /* 0x0000 */ u8 pad0000[0x1A2E];
    /* 0x1A2E */ s16 unk_1A2E;
    /* 0x1A30 */ u8 pad1A30[2];
    /* 0x1A32 */ s16 unk_1A32;
    /* 0x1A34 */ u8 pad1A34[0x1C8];
    /* 0x1BFC */ s16 unk_1BFC;
    /* 0x1BFE */ u8 pad1BFE[0x1E];
    /* 0x1C1C */ s32 unk_1C1C;
} EnWork1C20; /* size 0x1C20 */
typedef struct {
    void (*f[3])();
} FnTbl3; /* size 0xC */

/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80136B10.data", D_80139C30);
u8 D_80139C34[8] = { 0 };
u8 D_80139C3C[8] = { 0 };
u8 D_80139C44[8] = { 0 };
s16 D_80139C4C[6] = { 0 };
u16 D_80139C58 = 0;
/* Scene table, indexed by D_800E6280.unk_1109 in func_80136B10. Defining it here as the
 * original did matters for code: as1 treats every function named in this file's data as an
 * address-taken label, so the jump-table dispatch inside func_80136DEC gets an edge back to
 * that function's entry, and the resulting loop decides its delay-slot fills (T-25201). */
FnTbl3 D_80139C5C = { { func_80136B74, func_80136DEC, func_8013735C } };
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80136B10.data", D_80139C68);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80136B10.data", D_80139C94);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80136B10.data", D_80139CC0);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80136B10.data", D_80139CEC);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80136B10.data", D_80139D04);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80136B10.data", D_80139D1C);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80136B10.data", D_80139DA0);
INCLUDE_RODATA("asm/ovl/EN_NICHI/data/EN_NICHI/80136B10.data", D_80139E20);

void func_80136B10(void) {
    s32 pad; /* FAKE: unused local, takes the 4 bytes above the table (T-3330 layout); real source unknown. T-4010 */
    FnTbl3 tbl;

    tbl = D_80139C5C;
    tbl.f[D_800E6280.unk_1109]();
}

void func_80136B74(void) {
    s32 i;

    switch (D_800E6280.unk_110A) {
    case 0:
        func_80085E30(1, 0);
        func_8006BD6C(0);
        hizuke_disp_switch(0);
        message_disp_switch(0);
        srand(D_800E6280.unk_10F4);
        func_80044750(0xCF);
        func_8004284C();
        return;
    case 1:
        cd_read_sectors(0x6C, 0x801B0000, 0x7D0B);
        func_8004284C();
        return;
    case 2:
        if (cd_read_sync_status() & 1) {
            D_800E6280.unk_03A = 0;
            func_80132040();
            for (i = 0; i != 2; i++) {
                load_palette(D_801391F4, (i + 0x10) & 0xFF, 1, 1, i);
            }
            func_8004E44C(0, (u32 *) D_801391FC, D_801391E0);
            srn_init(0, (u32 *) D_801391F8, 0);
            srn_vram_set(0, 5, 0x580, 0xF0);
            Fade_In_Init();
            func_8004284C();
        }
        return;
    case 3:
        func_80044750(0x203);
        func_8004284C();
        return;
    case 4:
        Fade_In_Main();
        if (D_800E6280.unk_F88 & 0x60) {
            func_80044750(0x74);
            Fade_Out_Init();
            func_8004284C();
        }
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        return;
    case 5:
        if (Fade_Out_Main() != 0) {
            for (i = 0; i < 6; i++) {
                D_800E6280.unk_1228[i + 5] = -1;
                D_800E6280.unk_1228[i + 37] = -1;
            }
            func_80042808();
        }
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        return;
    default:
        func_80046500();
        break;
    }
}

/* The dispatch delay slots hold the next compare's li s0,2 and li at,3 only because D_80139C5C
 * (above) names this function: the inner jump-table jr then has an edge back to the entry, and
 * as1 schedules the compare chain and the case-3 head as one loop region without the case-1 and
 * case-2 heads (T-25201; history T-25109, drafts/T-25109/). */
void func_80136DEC(void) {
    GameState *gs;
    s32 i;
    EnHalf22 frames;
    EnHalf22 xs;
    EnHalf22 ys;
    EnBytes22 kinds;

    frames = D_80139C68;
    xs = D_80139C94;
    ys = D_80139CC0;
    kinds = D_80139CEC;
    gs = &D_800E6280;
    switch (gs->unk_110A) {
    case 0:
        load_tpage_buf_lock(5, ((s32 *)D_801391E4)[7], 1);
        for (i = 0; i < 7; i++) {
            search_load_tpage_buf_lock(((s32 *)D_801391E4)[i], 1);
        }
        for (i = 8; i < 10; i++) {
            search_load_tpage_buf_lock(((s32 *)D_801391E4)[i], 1);
        }
        func_8004284C();
        return;
    case 1:
        for (i = 0; i < 22; i++) {
            func_80048F64(i + 96);
            ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_01 = kinds.value[i];
            ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_38 = 0x01000000;
            ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_07 = 0;
            ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_02 = 0x40;
            ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_03 = 4;
            ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_0C = (u8 *)D_801391E8;
            ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_10 = (u8 *)D_801391EC;
            ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_34 = (u8 *)D_801391E4;
            ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_14 = D_801391F0;
            ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_04 = 0;
            ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_05 = 0x80;
            ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_1A = 0x1000;
            ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_1C = 0x1000;
            ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_06 = 0;
            ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_18 = frames.value[i];
            ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_26 = xs.value[i];
            ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_2A = ys.value[i];
        }
        for (i = 0; i < 21; i++) {
            ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_03 |= 0x80;
        }
        func_8004284C();
        return;
    case 2:
        if (((DisplayActor44 *)D_8011ECD0)[96].unk_07 >= 0x80U) {
            gs->unk_03A = 0x80;
            ((DisplayActor44 *)D_8011ECD0)[96].unk_07 = 0x80;
            func_8004284C();
        } else {
            ((DisplayActor44 *)D_8011ECD0)[96].unk_07 += 4;
        }
        for (i = 1; i < 22; i++) {
            ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_07 = ((DisplayActor44 *)D_8011ECD0)[96].unk_07;
        }
        return;
    case 3:
        switch (gs->unk_110D) {
        case 0:
            func_80046290(0x07000002, 0x03000008, 0xE);
            func_80044750(0x300);
            gs->unk_110D += 1;
            return;
        case 1:
        case 3:
            if (func_800460EC() == 4) {
                gs->unk_110D += 1;
                return;
            }
        default:
            return;
        case 2:
            if (func_800460EC() == 2) {
                func_80046290(0x08000001, 0x02000004, 0xE);
                func_80044750(0x300);
                gs->unk_110D += 1;
                return;
            }
            break;
        case 4:
            if (func_800460EC() == 2) {
                D_80139C58 = 0;
                D_80139C30 = 5;
                for (i = 0; i < 5; i++) {
                    D_80139C34[i] = 0;
                    D_80139C3C[i] = 0;
                    D_80139C44[i] = 0;
                }
                ((DisplayActor44 *)D_8011ECD0)->unk_26 = 0;
                ((DisplayActor44 *)D_8011ECD0)->unk_2A = 0;
                func_80042808();
                return;
            }
            break;
        }
        break;
    default:
        func_80046500();
        break;
    }
}

void func_8013735C(void) {
    s32 i;
    s32 n;

    switch (D_800E6280.unk_110A) {
    case 0:
        func_8006BA40();
        ((DisplayActor44 *)D_8011ECD0)[103].unk_26 = ((DisplayActor44 *)D_8011ECD0)[0].unk_26;
        ((DisplayActor44 *)D_8011ECD0)[103].unk_2A = ((DisplayActor44 *)D_8011ECD0)[0].unk_2A;
        if ((D_800E6280.unk_F88 & 0x20) && D_80139C30 != 0) {
            D_80139C30--;
            D_80139C34[D_80139C30] = 1;
            D_80139C3C[D_80139C30] = (D_80139C58 >> 4) + 1;
            /* Slots from record 98 indexed by the plain counter: the original scales it by 0x44 with
             * shifts here, while case 1's [n + 98] form multiplies by the hoisted 0x44 register. */
            ((DisplayActor44 *)D_8011ECD0 + 98)[D_80139C30].unk_26 = ((DisplayActor44 *)D_8011ECD0)[103].unk_26;
            ((DisplayActor44 *)D_8011ECD0 + 98)[D_80139C30].unk_2A = ((DisplayActor44 *)D_8011ECD0)[103].unk_2A;
            ((DisplayActor44 *)D_8011ECD0 + 98)[D_80139C30].unk_01 = 4;
            func_80044750(0x50C);
        }
        func_80137C58();
        func_80138CCC();
        if (D_80139C30 == 0 && D_80139C34[0] == 0 && D_80139C34[1] == 0 && D_80139C34[2] == 0 && D_80139C34[3] == 0 && D_80139C34[4] == 0) {
            func_8004284C();
        }
        break;
    case 1:
        ((DisplayActor44 *)D_8011ECD0)[117].unk_03 |= 0x80;
        n = 0;
        for (i = 0; i < 5; i++) {
            if (D_80139C4C[i] != 0xFF) {
                ((DisplayActor44 *)D_8011ECD0)[n + 98].unk_18 = D_80139C4C[i];
                n++;
            }
            ((DisplayActor44 *)D_8011ECD0)[i + 98].unk_01 = 1;
        }
        /* FAKE: T-25109, a never-true loop that ugen folds away (no code). Its blocks lengthen the
         * live range of the hoisted 0xFF, so uopt colours D_8011ECD0 first (t1) and 0xFF second (t2). */
        do {
        } while ((u32)n < 0 && (u32)i < 0);
        for (; n < 5; n++) {
            ((DisplayActor44 *)D_8011ECD0)[n + 98].unk_18 = 0xFF;
        }
        Vblnk_Timer_Init();
        D_80139C30 = 0;
        func_8004284C();
        break;
    case 2:
        if (Vblnk_Timer() >= 0x2E) {
            if (((DisplayActor44 *)D_8011ECD0 + 98)[D_80139C30].unk_18 == 0xFF || D_80139C30 >= 5) {
                if (D_80139C4C[0] == 0x1F && D_80139C4C[1] == 0x1F && D_80139C4C[2] == 0x1F && D_80139C4C[3] == 0x1F && D_80139C4C[4] == 0x1F) {
                    func_80042940(5);
                } else if (D_80139C4C[0] == 0x20 && D_80139C4C[1] == 0x20 && D_80139C4C[2] == 0x20 && D_80139C4C[3] == 0x20 && D_80139C4C[4] == 0x20) {
                    func_80042940(6);
                } else {
                    Vblnk_Timer_Init();
                    func_8004284C();
                }
                func_80044750(0x50B);
            } else {
                func_80044750(0x50A);
                ((DisplayActor44 *)D_8011ECD0 + 98)[D_80139C30].unk_03 = 0x84;
                ((DisplayActor44 *)D_8011ECD0 + 98)[D_80139C30].unk_26 = (D_80139C30 << 6) - 0x80;
                ((DisplayActor44 *)D_8011ECD0 + 98)[D_80139C30].unk_2A = 0x38;
                D_80139C30++;
                Vblnk_Timer_Init();
            }
        }
        break;
    case 3:
        if (Vblnk_Timer() >= 0x79) {
            if (((DisplayActor44 *)D_8011ECD0)[96].unk_07 < 4U) {
                func_80048EB8(0);
                func_8004284C();
            } else {
                ((DisplayActor44 *)D_8011ECD0)[96].unk_07 -= 4;
            }
            for (i = 1; i < 22; i++) {
                ((DisplayActor44 *)D_8011ECD0)[96 + i].unk_07 = ((DisplayActor44 *)D_8011ECD0)[96].unk_07;
            }
        }
        break;
    case 4:
        if (D_80139C4C[0] < 0x10 && D_80139C4C[1] < 0x10 && D_80139C4C[2] < 0x10 && D_80139C4C[3] < 0x10 && D_80139C4C[4] < 0x10) {
        } else if (D_80139C4C[0] == 0xFF && D_80139C4C[1] == 0xFF) {
            /* FAKE: T-25109, a loop that uopt keeps and ugen folds away; it keeps the empty arm's two
             * 0xFF tests that the original has (an empty or dead-store arm is removed whole). */
            n = 0;
            do {
            } while ((u32)n < 0);
        }
        if (D_80139C4C[0] == 0x10 && D_80139C4C[1] == 0x10 && D_80139C4C[2] == 0x10 && D_80139C4C[3] == 0x10 && D_80139C4C[4] == 0x10 && D_800E6280.unk_F5F == 2) {
            /* FAKE: T-25109, a second use so uopt keeps unk_F5F of this test in v1 (register-cheatsheet). */
            if ((s32)D_800E6280.unk_F5F) {
            }
            D_80122E9C = 3;
        } else if (D_800E6280.unk_F5F + 0x14 == D_80139C4C[0] && D_800E6280.unk_F5F + 0x14 == D_80139C4C[1] && D_800E6280.unk_F5F + 0x14 == D_80139C4C[2] && D_800E6280.unk_F5F + 0x14 == D_80139C4C[3] && D_800E6280.unk_F5F + 0x14 == D_80139C4C[4] && D_800E6280.unk_F5F != 2) {
            D_80122E9C = 3;
        } else if (D_80139C4C[0] < 0x21 && D_80139C4C[1] < 0x21 && D_80139C4C[2] < 0x21 && D_80139C4C[3] < 0x21 && D_80139C4C[4] < 0x21) {
            D_80122E9C = 1;
        } else {
            D_80122E9C = 4;
        }
        main_state_set(D_800E6280.unk_720);
        func_80042908(D_800E6280.unk_721);
        func_80042940(D_800E6280.unk_722);
        break;
    case 5:
        switch (D_800E6280.unk_110D) {
        case 0:
            func_80046290(0x1A0000B1, 0x170000B7, 0xE);
            func_80044750(0x300);
            D_800E6280.unk_110D += 1;
            break;
        case 1:
            if (func_800460EC() == 4) {
                D_800E6280.unk_110D += 1;
            }
            break;
        case 2:
            if (func_800460EC() == 2) {
                func_80044750(0x74);
                D_800E6280.unk_110D += 1;
            }
            break;
        case 3:
            Vblnk_Timer_Init();
            D_800E6280.unk_110D += 1;
            break;
        case 4:
            if (Vblnk_Timer() >= 0x79) {
                Vblnk_Timer_Init();
                func_80042940(3);
            }
            break;
        default:
            func_80046500();
            break;
        }
        break;
    case 6:
        switch (D_800E6280.unk_110D) {
        case 0:
            func_80046290(0x03000000, 0x01000007, 0xE);
            func_80044750(0x300);
            D_800E6280.unk_110D += 1;
            break;
        case 1:
            if (func_800460EC() == 4) {
                D_800E6280.unk_110D += 1;
            }
            break;
        case 2:
            if (func_800460EC() == 2) {
                func_80044750(0x74);
                D_800E6280.unk_110D += 1;
            }
            break;
        case 3:
            Vblnk_Timer_Init();
            D_800E6280.unk_110D += 1;
            break;
        case 4:
            if (Vblnk_Timer() >= 0x79) {
                Vblnk_Timer_Init();
                func_80042940(3);
            }
            break;
        default:
            func_80046500();
            break;
        }
        break;
    default:
        func_80046500();
        break;
    }
}

/* FAKE: the u8 locals `one` and `two` put the hoisted constants first in both bne compares (T-9410 idiom), T-11300 */
void func_80137C58(void) {
    s32 i;
    u8 *p;
    u8 *r;
    u8 v;
    u8 one;
    u8 two;

    one = 1;
    two = 2;
    D_80139C58 = D_80139C58 + 4;
    if (D_80139C58 >= 0x71) {
        D_80139C58 = 0;
    }
    func_80137E2C();
    for (i = 0, p = D_80139C34; i < 5; i++, p++) {
        v = *p;
        if (one == v) {
            r = D_8011ECD0 + i * 0x44;
            *(s16 *)(r + 0x1A22) -= D_80139C3C[i] * 8;
            *(s16 *)(r + 0x1A24) -= D_80139C3C[i] * 8;
            if (*(s16 *)(r + 0x1A22) < 0x401) {
                if (func_80137FEC(i) != 0) {
                    *(s16 *)(r + 0x1A20) = 0x27;
                    if ((D_80139C4C[i] < 0x10) || (D_80139C4C[i] >= 0x14)) {
                        func_80044750(0x508);
                    } else {
                        func_80044750(0x509);
                    }
                } else {
                    func_80044750(0x507);
                    *(s16 *)(r + 0x1A20) = 0x26;
                }
                *(s16 *)(r + 0x1A22) = 0x1000;
                *(s16 *)(r + 0x1A24) = 0x1000;
                *p = 2;
                v = 2;
            }
        }
        if (two == v) {
            D_80139C44[i] += 1;
            if (D_80139C44[i] >= 0x3D) {
                ((DisplayActor44 *)D_8011ECD0)[i + 98].unk_03 = 0;
                *p = 0;
            }
        }
    }
}

/* FAKE: the empty do-while after the load of D_80139C58 puts the load and its uses in the original's registers (a2, v0); found with the permuter, T-9380 */
void func_80137E2C(void) {
    u8 *p;
    u16 w;
    s32 a;

    p = GetWorkBase(0x24, D_8011ECA0);
    func_8009F054(p);
    SetSemiTrans(p, 1);
    SetShadeTex(p, 0);
    w = D_80139C58;
    do {
    } while (0);
    a = w * 2 + 0x1F;
    p[6] = 0xFF;
    p[4] = 0x1F;
    p[5] = 0x1F;
    p[0xC] = a;
    p[0xE] = 0xFF;
    p[0xD] = 0x1F;
    p[0x16] = 0xFF;
    p[0x14] = 0x1F;
    p[0x15] = 0x1F;
    p[0x1C] = a;
    p[0x1E] = 0xFF;
    p[0x1D] = 0x1F;
    *(s16 *)(p + 8) = D_800E6280.unk_014[D_8011ECA0] + 0x20;
    *(s16 *)(p + 0xA) = D_800E6280.unk_018[D_8011ECA0] + 0x50;
    *(s16 *)(p + 0x10) = D_800E6280.unk_014[D_8011ECA0] + w + 0x20;
    *(s16 *)(p + 0x12) = D_800E6280.unk_018[D_8011ECA0] + 0x50;
    *(s16 *)(p + 0x18) = D_800E6280.unk_014[D_8011ECA0] + 0x20;
    *(s16 *)(p + 0x1A) = D_800E6280.unk_018[D_8011ECA0] + 0x60;
    *(s16 *)(p + 0x20) = D_800E6280.unk_014[D_8011ECA0] + w + 0x20;
    *(s16 *)(p + 0x22) = D_800E6280.unk_018[D_8011ECA0] + 0x60;
    AddPrim((D_8011ECA0 << 10) + 0xC + D_800E8CA0, p, w);
}

/* FAKE: local reuse and the callback comma expression retain the table slots
 * and post-call decrement scheduling. T-9720. */
s32 func_80137FEC(s32 arg0) {
    s32 i;
    s32 value;
    s32 outside;
    EnHalf12 limits;
    EnFn33 calls;
    EnWork1C20 *record;
    s16 limit;

    limits = D_80139D04;
    calls = D_80139D1C;
    outside = 0;
    record = (EnWork1C20 *)((u8 *)D_8011ECD0 + arg0 * 0x44);
    i = 0;
    value = -0x78;
    do {
        if (record->unk_1A32 < value && ((limit = limits.value[i], record->unk_1A2E < -limit) || limit < D_801206FE)) {
            outside = 1;
        }
        i++;
        value += 8;
    } while (i < 12);
    i = 11;
    if (outside != 0) {
        D_80139C4C[arg0] = 0xFF;
        return 0;
    }
    record = (EnWork1C20 *)((u8 *)D_8011ECD0 + 11 * 0x44);
loop:
    if ((outside = calls.f[record->unk_1BFC](arg0, i), i--, outside != 0)) {
        value = record->unk_1C1C;
        if (!(value & 0x40000000)) {
            record->unk_1C1C = value | 0x40000000;
            D_80139C4C[arg0] = record->unk_1BFC;
            return 1;
        }
    }
    record = (EnWork1C20 *)((u8 *)record - 0x44);
    if (i < 0) {
        D_80139C4C[arg0] = 0xFF;
        return 0;
    }
    goto loop;
}

s32 func_801381C8(s32 arg0, s32 arg1) {
    u8 *a;
    u8 *b;
    s16 x;
    s16 y;
    s16 px;
    s16 py;
    s32 xh;

    a = D_8011ECD0 + arg1 * 0x44;
    x = *(s16 *)(a + 0x1C0E);
    xh = x + 0x10;
    b = D_8011ECD0 + arg0 * 0x44;
    px = *(s16 *)(b + 0x1A32);
    if (xh >= px && px >= x - 0x10) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 0x8 >= py && py >= y - 0x8) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    if (xh >= px && px >= x) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y - 0x8 >= py && py >= y - 0x10) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_801382B4(s32 arg0, s32 arg1) {
    u8 *a;
    u8 *b;
    s16 x;
    s16 y;
    s16 px;
    s16 py;

    a = D_8011ECD0 + arg1 * 0x44;
    x = *(s16 *)(a + 0x1C0E);
    b = D_8011ECD0 + arg0 * 0x44;
    px = *(s16 *)(b + 0x1A32);
    if (x + 0x10 >= px && px >= x) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 0x10 >= py && py >= y - 0x10) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    if (x >= px && px >= x - 0x10) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 0x8 >= py && py >= y - 0x8) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_801383A0(s32 arg0, s32 arg1) {
    u8 *a;
    u8 *b;
    s16 x;
    s16 y;
    s16 px;
    s16 py;

    a = D_8011ECD0 + arg1 * 0x44;
    x = *(s16 *)(a + 0x1C0E);
    b = D_8011ECD0 + arg0 * 0x44;
    px = *(s16 *)(b + 0x1A32);
    if (x + 0x10 >= px && px >= x) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 0x8 >= py && py >= y - 0x8) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_80138434(s32 arg0, s32 arg1) {
    u8 *a;
    u8 *b;
    s16 x;
    s16 y;
    s16 px;
    s16 py;
    s32 xs;

    a = D_8011ECD0 + arg1 * 0x44;
    x = *(s16 *)(a + 0x1C0E);
    xs = x + 8;
    b = D_8011ECD0 + arg0 * 0x44;
    px = *(s16 *)(b + 0x1A32);
    if (x + 0x10 >= px && px >= xs) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 0x10 >= py && py >= y - 0x10) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    if (xs >= px && px >= x) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 8 >= py && py >= y - 8) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_80138524(s32 arg0, s32 arg1) {
    u8 *a;
    u8 *b;
    s16 x;
    s16 y;
    s16 px;
    s16 py;
    s32 xs;

    a = D_8011ECD0 + arg1 * 0x44;
    x = *(s16 *)(a + 0x1C0E);
    xs = x + 8;
    b = D_8011ECD0 + arg0 * 0x44;
    px = *(s16 *)(b + 0x1A32);
    if (x + 0x10 >= px && px >= xs) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 0x10 >= py && py >= y - 0x10) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    if (xs >= px && px >= x) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 0x10 >= py && py >= y - 8) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_80138614(s32 arg0, s32 arg1) {
    u8 *a;
    u8 *b;
    s16 x;
    s16 y;
    s16 px;
    s16 py;
    s32 xs;

    a = D_8011ECD0 + arg1 * 0x44;
    x = *(s16 *)(a + 0x1C0E);
    xs = x + 8;
    b = D_8011ECD0 + arg0 * 0x44;
    px = *(s16 *)(b + 0x1A32);
    if (x + 0x10 >= px && px >= x - 0x10) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 8 >= py && py >= y - 8) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    if (xs >= px && px >= x - 8) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 0x10 >= py && py >= y - 0x10) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_80138708(s32 arg0, s32 arg1) {
    u8 *a;
    u8 *b;
    s16 x;
    s16 y;
    s16 px;
    s16 py;
    s32 xs;

    a = D_8011ECD0 + arg1 * 0x44;
    x = *(s16 *)(a + 0x1C0E);
    xs = x - 8;
    b = D_8011ECD0 + arg0 * 0x44;
    px = *(s16 *)(b + 0x1A32);
    if (x + 0x10 >= px && px >= xs) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 0x10 >= py && py >= y - 0x10) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    if (xs >= px && px >= x - 0x10) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 8 >= py && py >= y - 0x10) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_801387FC(s32 arg0, s32 arg1) {
    u8 *a;
    u8 *b;
    s16 x;
    s16 y;
    s16 px;
    s16 py;

    a = D_8011ECD0 + arg1 * 0x44;
    x = *(s16 *)(a + 0x1C0E);
    b = D_8011ECD0 + arg0 * 0x44;
    px = *(s16 *)(b + 0x1A32);
    if (x + 0x10 >= px && px >= x) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 0x10 >= py && py >= y - 0x10) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_80138890(s32 arg0, s32 arg1) {
    u8 *a;
    u8 *b;
    s16 x;
    s16 y;
    s16 px;
    s16 py;
    s32 xa;
    s32 xb;

    a = D_8011ECD0 + arg1 * 0x44;
    x = *(s16 *)(a + 0x1C0E);
    xa = x + 8;
    xb = x + 0x10;
    b = D_8011ECD0 + arg0 * 0x44;
    px = *(s16 *)(b + 0x1A32);
    if (xa >= px && px >= x - 8) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 0x10 >= py && py >= y - 0x10) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    if (xb >= px && px >= xa) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 8 >= py && py >= y - 8) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_80138980(s32 arg0, s32 arg1) {
    u8 *a;
    u8 *b;
    s16 x;
    s16 y;
    s16 px;
    s16 py;

    a = D_8011ECD0 + arg1 * 0x44;
    x = *(s16 *)(a + 0x1C0E);
    b = D_8011ECD0 + arg0 * 0x44;
    px = *(s16 *)(b + 0x1A32);
    if (x + 0x10 >= px && px >= x - 0x10) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 8 >= py && py >= y - 8) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_80138A18(s32 arg0, s32 arg1) {
    u8 *a;
    u8 *b;
    s16 x;
    s16 y;
    s16 px;
    s16 py;

    a = D_8011ECD0 + arg1 * 0x44;
    x = *(s16 *)(a + 0x1C0E);
    b = D_8011ECD0 + arg0 * 0x44;
    px = *(s16 *)(b + 0x1A32);
    if (x + 0x10 >= px && px >= x - 0x8) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 0x8 >= py && py >= y - 0x8) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_80138AB0(s32 arg0, s32 arg1) {
    u8 *a;
    u8 *b;
    s16 x;
    s16 y;
    s16 px;
    s16 py;
    s32 xh;

    a = D_8011ECD0 + arg1 * 0x44;
    x = *(s16 *)(a + 0x1C0E);
    xh = x + 0x10;
    b = D_8011ECD0 + arg0 * 0x44;
    px = *(s16 *)(b + 0x1A32);
    if (xh >= px && px >= x - 0x8) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 0x10 >= py && py >= y - 0x8) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    if (xh >= px && px >= x) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y - 0x8 >= py && py >= y - 0x10) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_80138B9C(s32 arg0, s32 arg1) {
    u8 *a;
    u8 *b;
    s16 x;
    s16 y;
    s16 px;
    s16 py;

    a = D_8011ECD0 + arg1 * 0x44;
    x = *(s16 *)(a + 0x1C0E);
    b = D_8011ECD0 + arg0 * 0x44;
    px = *(s16 *)(b + 0x1A32);
    if (x + 0x20 >= px && px >= x - 0x20) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 0x20 >= py && py >= y - 0x20) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_80138C34(s32 arg0, s32 arg1) {
    u8 *a;
    u8 *b;
    s16 x;
    s16 y;
    s16 px;
    s16 py;

    a = D_8011ECD0 + arg1 * 0x44;
    x = *(s16 *)(a + 0x1C0E);
    b = D_8011ECD0 + arg0 * 0x44;
    px = *(s16 *)(b + 0x1A32);
    if (x + 0x20 >= px && px >= x - 0x20) {
        y = *(s16 *)(a + 0x1C0A);
        py = *(s16 *)(b + 0x1A2E);
        if (y + 16 >= py && py >= y - 16) {
            if (a[0x1BE7] & 0x80) {
                return 1;
            }
        }
    }
    return 0;
}

/* Moves the three rows of four display records (105.., 109.., 113..) in 16.16 steps; a record that
 * leaves the screen re-enters behind its neighbour with a frame from tbl, whose entries 32..47 are
 * the values of girls picked at random from list (girls with flag bit 1, and 11, 12 when set).
 * The neighbour is read before the new frame is stored, as in the original. T-20920 */
void func_80138CCC(void) {
    s32 i;
    s32 r1;
    s32 r2;
    s32 r3;
    EnHalf64 tbl;
    EnHalf13 girls;
    u8 n;
    u8 list[15];

    tbl = D_80139DA0;
    girls = D_80139E20;
    n = 0;
    for (i = 0; i < 11; i++) {
        if (D_800E6280.unk_1BC[i].unk_0C.f.b1 == 1) {
            list[n] = i;
            n++;
        }
    }
    if (D_800E7D10.flags[11] != 0 || D_800E6280.unk_1BC[11].unk_0C.f.f9 != 0) {
        list[n] = 11;
        n++;
    }
    if (D_800E7D10.flags[12] != 0) {
        list[n] = 12;
        n++;
    }
    for (i = 32; i < 48; i++) {
        tbl.value[i] = girls.value[list[rand() % n]];
    }
    for (i = 0; i < 4; i++) {
        ((DisplayFixActor44 *)D_8011ECD0)[105 + i].unk_24.h[1]++;
        if (((DisplayFixActor44 *)D_8011ECD0)[105 + i].unk_24.h[1] >= 0xC1) {
            r1 = rand() & 0x3F;
            ((DisplayFixActor44 *)D_8011ECD0)[105 + i].unk_24.h[1] = ((DisplayFixActor44 *)D_8011ECD0)[105 + (i + 1) % 4].unk_24.h[1] - 0x60;
            ((DisplayFixActor44 *)D_8011ECD0)[105 + i].unk_18 = tbl.value[r1];
            if ((r1 & 1) && r1 < 0x20) {
                ((DisplayFixActor44 *)D_8011ECD0)[105 + i].unk_28.h[1] = -0x38;
            } else {
                ((DisplayFixActor44 *)D_8011ECD0)[105 + i].unk_28.h[1] = -0x48;
            }
            ((DisplayFixActor44 *)D_8011ECD0)[105 + i].unk_38 = 0x01000000;
        }
    }
    for (i = 0; i < 4; i++) {
        ((DisplayFixActor44 *)D_8011ECD0)[109 + i].unk_24.w += 0xFFFF2000;
        if (((DisplayFixActor44 *)D_8011ECD0)[109 + i].unk_24.h[1] < -0xC0) {
            r2 = rand() & 0x3F;
            ((DisplayFixActor44 *)D_8011ECD0)[109 + i].unk_24.h[1] = ((DisplayFixActor44 *)D_8011ECD0)[109 + (i + 3) % 4].unk_24.h[1] + 0x60;
            ((DisplayFixActor44 *)D_8011ECD0)[109 + i].unk_18 = tbl.value[r2];
            if ((r2 & 1) && r2 < 0x20) {
                ((DisplayFixActor44 *)D_8011ECD0)[109 + i].unk_28.h[1] = -0x10;
            } else {
                ((DisplayFixActor44 *)D_8011ECD0)[109 + i].unk_28.h[1] = -0x20;
            }
            ((DisplayFixActor44 *)D_8011ECD0)[109 + i].unk_38 = 0x01000000;
        }
    }
    for (i = 0; i < 4; i++) {
        ((DisplayFixActor44 *)D_8011ECD0)[113 + i].unk_24.w += 0xC000;
        if (((DisplayFixActor44 *)D_8011ECD0)[113 + i].unk_24.h[1] >= 0xC1) {
            r3 = rand() & 0x3F;
            ((DisplayFixActor44 *)D_8011ECD0)[113 + i].unk_24.h[1] = ((DisplayFixActor44 *)D_8011ECD0)[113 + (i + 1) % 4].unk_24.h[1] - 0x60;
            ((DisplayFixActor44 *)D_8011ECD0)[113 + i].unk_18 = tbl.value[r3];
            if ((r3 & 1) && r3 < 0x20) {
                ((DisplayFixActor44 *)D_8011ECD0)[113 + i].unk_28.h[1] = 0x18;
            } else {
                ((DisplayFixActor44 *)D_8011ECD0)[113 + i].unk_28.h[1] = 8;
            }
            ((DisplayFixActor44 *)D_8011ECD0)[113 + i].unk_38 = 0x01000000;
        }
    }
}
