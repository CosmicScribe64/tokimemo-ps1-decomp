#define MAIN_API_OVERRIDE_func_80085E30 /* called as an implicit int function (definition: void): the call's $v0 result keeps the loop pointers off $v0 in func_80142400, T-22004 */
#include "common.h"
#include "ovl/TAIIKU.h"
#define DISP(n) (((DisplayActor44 *)D_8011ECD0)[n])
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A110);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A118);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A11C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A120);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A124);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A128);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A12C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A130);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A134);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A138);
TaiRunnerB D_8014A13C[4] = { 0 };
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A1CC);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A1D0);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A1D4);
/* Defined in the unit (not island pieces) so that the first two stores share one lui at; T-16551, func_80133CFC, T-22004, func_80142400. */
s16 D_8014A1E8[10] = { 0 };
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A1FC);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A208);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A228);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A254);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A27C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A290);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A2A4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A2C4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A2F0);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A304);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A318);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A320);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A328);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A334);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A340);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A34C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A358);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A360);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A370);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A374);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A378);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A37C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A380);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A384);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A388);

typedef struct {
    void (*f[10])();
} FnTbl10; /* size 0x28 */
extern FnTbl10 D_8014A254;

void func_80142240(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl10 tbl;

    tbl = D_8014A254;
    idx = D_800E6280.unk_1109;
    tbl.f[idx]();
}

void func_801422BC(void) {
    if (D_800E6280.unk_110A == 0) {
        if ((func_8008667C(1) < 2) && (((u32) (D_800E6280.unk_1BC[1].unk_0C.w << 0x1E) >> 0x1F) == 1) && (D_800E6280.unk_56C[13] == 0)) {
            D_800E6280.unk_56C[13] = 1;
            D_8014A110 = 1;
            func_80044890(0, 0xBF98, 0xBF79, 0xD712, 0xD6C3, 0xD6B7);
            func_8004284C();
        } else {
            D_8014A110 = 0;
            func_80042908(3);
        }
        Default_Disp();
        return;
    }
    if (func_80044E8C() == 1) {
        func_80042808();
    }
    Default_Disp();
}

void func_801423A4(void) {
    s32 temp_v0;

    temp_v0 = func_80140A90();
    switch (temp_v0) {
    case 1:
        func_80042808();
        return;
    case 2:
        D_8014A110 = 0;
        func_80042808();
        /* fallthrough */
    case 0:
        return;
    }
}

/* NON_MATCHING: T-22004, 26 rows; only the saved registers of case 3 are left: the original keeps the tpage
 * pointer in $s0, its end in $s1, then the palette counter in $s1 and the bound 16 in $s0; the draft has all four
 * swapped (regdiff: the pointer range, priority 20.5, is coloured before the end range, priority 5, and takes $s1).
 * The pointer needs the counter `j` shared with the palette loop to keep the `sltu` form; a separate counter, a
 * pointer local (a stack word, frame 200), `<=`/`!=`/unsigned bounds, u32/s16 `j` and autotweak (513 edits) did not
 * move it. Needs MAIN_API_OVERRIDE_func_80085E30 (int call: 74 to 52 rows) and the unit-defined D_8014A1E8 (one
 * shared `lui at`). Forms to keep: D_8014A1D4 and D_8014A1FC are reached through their first symbol as in the matched
 * users of this file (rodata islands, aggregates not recovered); D_8014A1E8 is peeled by two stores because IDO unrolls the
 * plain 0..10 loop by four (207 rows). */
#ifdef NON_MATCHING
void func_80142400(void) {
    s32 i;
    s32 j;
    TaiikuHalf10 sp_a;
    TaiikuHalf10 sp_b;
    TaiWords8 sp_c;
    TaiWords11 sp_d;

    sp_a = D_8014A27C;
    sp_b = D_8014A290;
    sp_c = D_8014A2A4;
    sp_d = D_8014A2C4;
    switch (D_800E6280.unk_110A) {
    case 0:
        if (D_8014A110 == 1) {
            func_80044750(0xBF);
        }
        func_80044890(0, 0xBF98, 0xBF79, 0xC20F, 0xC1EF, 0xD6B7);
        func_80085E30(1, 1);
        for (i = 0; i < 11; i++) {
            D_800E6280.unk_1228[i + 5] = 8;
        }
        for (i = 0; i < 3; i++) {
            D_800E6280.unk_1228[i + 0x25] = 8;
        }
        for (i = 0; i < 8; i++) {
            D_8014A208[i] = sp_c.v[i];
        }
        for (i = 0; i < 11; i++) {
            D_8014A228[i] = sp_d.value[i];
        }
        for (i = 0; i < 10; i++) {
            *(s16 *)&D_8014A1D4[i * 2] = sp_a.h[i];
        }
        D_8014A1E8[0] = sp_b.h[0];
        D_8014A1E8[1] = sp_b.h[1];
        for (i = 2; i < 10; i++) {
            D_8014A1E8[i] = sp_b.h[i];
        }
        func_80048EB8(0);
        func_8006BD6C(0);
        func_8004284C();
        break;
    case 1:
        if (func_80044E8C() == 1) {
            cd_read_sectors(0x1E, 0x80180000, 0xBAD0);
            func_8004284C();
        }
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            cd_read_sectors(0x4F, 0x8018F000, 0xB861);
            Fade_Out_Init();
            func_8004284C();
        }
        break;
    case 3:
        if ((Fade_Out_Main() != 0) && (cd_read_sync_status() & 1)) {
            func_80140810();
            func_80145190();
            for (j = 0; j < 3; j++) {
                search_load_tpage_buf_lock(((s32 *)&D_8014A1FC)[j], 1);
            }
            for (j = 0; j < 0x10; j++) {
                load_palette(D_8014A378, (j + 0x10) & 0xFF, 1, 1, j);
            }
            func_8014357C();
            func_80143398();
            func_80143470();
            cd_read_sectors(0xA1, 0x8018F000, 0xB7C0);
            func_80042808();
        }
        break;
    default:
        func_80046500();
        break;
    }
}
#else
INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80142240", func_80142400);
#endif

/* FAKE: `pad` reserves the top local word: the original keeps the two loop spills of the
 * tpage loops at 0x38 and 0x30 (sp). The u8 views of the flag D_8014A110 keep its lbu loads and
 * stop uopt from promoting it (as in func_80143F50; D_8014A110 is `u8` since T-19010). T-16554 */
void func_8014280C(void) {
    u8 pad[4];
    s32 i;

    switch (D_800E6280.unk_110A) {
    case 0:
        if (cd_read_sync_status() & 1) {
            for (i = 0; i < 8; i++) {
                D_800E6280.unk_1228[search_load_tpage_buf_lock(D_8014A208[i], 1)] = i + 0x80000100;
                D_8014A208[i] = i + 0x80000100;
            }
            for (i = 0; i != 0xB; i++) {
                D_800E6280.unk_1228[search_load_tpage_buf_lock(D_8014A228[i], 1)] = i + 0x80000200;
                D_8014A228[i] = i + 0x80000200;
            }
            func_8004284C();
        }
        break;
    case 1:
        Fade_In_Init();
        func_8004500C(1, 3);
        func_8004284C();
        break;
    case 2:
        if (Fade_In_Main() != 0) {
            func_8004284C();
        }
        if (D_800E6280.unk_F88 & 0xF0) {
            Fade_Out_Init();
            cd_read_sectors(0x87U, 0x8018F000, 0xB98F);
            func_80042940(5);
        }
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        break;
    case 3:
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        if (((DisplayActor44 *)D_8011ECD0)[96].unk_2A >= 0x61) {
            /* the 16.16 y position at 0x28 (its high half is unk_2A), through the fixed-point view. T-16554, T-20900 */
            ((DisplayFixActor44 *)D_8011ECD0)[96].unk_28.w -= 0x18000;
        } else {
            if (*(u8 *)&D_8014A110 == 0) {
                ((DisplayActor44 *)D_8011ECD0)[96].unk_16 = 1;
            } else {
                ((DisplayActor44 *)D_8011ECD0)[96].unk_16 = 3;
            }
            switch (D_800E6280.unk_110D) {
            case 0:
                if (*(u8 *)&D_8014A110 != 0) {
                    func_80046290(0x1E000050, 0x06000060, 1);
                } else {
                    func_80046290(0x1F000394, 0x440003A4, 0);
                }
                func_80044750(0x300);
                D_800E6280.unk_110D += 1;
                break;
            case 1:
            case 3:
            case 5:
                if (func_800460EC() == 4) {
                    D_800E6280.unk_110D += 1;
                }
                break;
            case 2:
                if (func_800460EC() == 2) {
                    if (*(u8 *)&D_8014A110 != 0) {
                        func_80046290(0x1F00009F, 0x140000A9, 1);
                    } else {
                        func_80046290(0x1F0003A5, 0x450003B1, 0);
                    }
                    func_80044750(0x300);
                    D_800E6280.unk_110D += 1;
                }
                break;
            case 4:
                if (func_800460EC() == 2) {
                    if (*(u8 *)&D_8014A110 != 0) {
                        func_80046290(0x1F0000AA, 0x150000B7, 1);
                    } else {
                        func_80046290(0x0A0003C3, 0x430003D0, 0);
                    }
                    func_80044750(0x300);
                    D_800E6280.unk_110D += 1;
                }
                break;
            case 6:
                if (func_800460EC() == 2) {
                    Vblnk_Timer_Init();
                    D_800E6280.unk_110D += 1;
                }
                break;
            case 7:
                if (Vblnk_Timer() >= 0xB5) {
                    D_800E6280.unk_110D = 0;
                }
                break;
            }
        }
        if (D_800E6280.unk_F88 & 0xF0) {
            Fade_Out_Init();
            func_80044750(0x74);
            func_8004284C();
        }
        break;
    case 4:
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        if (func_800460EC() == 2) {
            cd_read_sectors(0x87U, 0x8018F000, 0xB98F);
            func_8004284C();
        }
        break;
    case 5:
        Scroll(0, 0xF, 0, 0xF0, 0x280);
        if ((Fade_Out_Main() != 0) && (cd_read_sync_status() & 1)) {
            func_80133C30();
            for (i = 0; i != 6; i++) {
                search_load_tpage_buf_lock(D_8014A070[i], 1);
            }
            func_8004284C();
        }
        break;
    case 6:
        func_80048F64(0x60);
        for (i = 0; i < 3; i++) {
            D_800E6280.unk_1228[0x15 + i] = -1;
        }
        func_801330D4();
        func_80132BD0();
        func_80143814();
        func_80044750(0x505);
        Fade_In_Init();
        func_80042808();
        break;
    default:
        func_80046500();
        break;
    }
}

void func_80142D90(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        if (func_80086370() != 0) {
            func_80085ED0();
            func_8004284C();
        }
        func_80133438();
        break;
    case 1:
        if (func_80085EEC() >= 0x5B) {
            func_80044750(0x502);
            if ((u8) D_8014A110 != 0) {
                D_80120778 = 0x39;
            } else {
                D_80120778 = 0x2D;
                func_80044750(0x800);
            }
            D_80120763 = 0xA4;
            D_80120762 = 0x40;
            func_8004284C();
        }
        func_80133438();
        break;
    case 2:
        func_80143AC8();
        func_80132E20((s32) (s16) ((s16) D_8014A1CE / 2), 4);
        func_80143C04();
        func_80144768();
        func_8014430C();
        if (func_80144464() != 0) {
            func_80044750(0x504);
            func_800862D0();
            func_8004284C();
        }
        break;
    case 3:
        func_80085F0C((s32) ((s16) D_8014A1CE / 4) % 640, 0xF, 0, 0x70, 0x280);
        func_80085F0C((s32) ((s16) D_8014A1CE / 2) % 128, 0xF, 0x70, 0x80, 0x80);
        if (func_800862F0() != 0) {
            func_80144E54();
            func_80133258();
            func_80042808();
        }
        break;
    default:
        func_80046500();
        break;
    }
}

void func_80142FB8(void) {
    s32 i;
    s32 j;

    Scroll(0, 0xF, 0, 0xF0, 0x280);
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80086350();
        func_8004284C();
        break;
    case 1:
        if (func_80086370() != 0) {
            if (D_8014A120[0] == 1 || D_8014A120[1] == 1) {
                func_80044750(0x205);
            } else {
                func_80044750(0x206);
            }
            Vblnk_Timer_Init();
            func_8004284C();
        }
        break;
    case 2:
        if (Vblnk_Timer() >= 0x3D) {
            func_80145044();
            Vblnk_Timer_Init();
            func_8004284C();
        }
        break;
    case 3:
        if (Vblnk_Timer() >= 0x3D) {
            func_801450C0();
            if (D_8014A110 != 0) {
                for (i = 0; i < 4; i++) {
                    if (D_8014A120[i] == 1) {
                        func_8013384C(1, (u8) i);
                    }
                }
            } else {
                for (i = 0; i < 4; i++) {
                    if (D_8014A120[i] == 1) {
                        func_8013384C(0, (u8) i);
                    }
                }
            }
            Vblnk_Timer_Init();
            D_8014A11C = 0;
            func_8004284C();
        }
        break;
    case 4:
        D_8014A11C += 1;
        func_80145148();
        if (Vblnk_Timer() >= 0xF1) {
            Fade_Out_Init();
            func_8004284C();
        }
        break;
    case 5:
        if (Fade_Out_Main() != 0) {
            if (D_8014A120[0] == 1) {
                for (j = 0; j < 13; j++) {
                    D_800E6280.unk_1BC[j].unk_02 += 4;
                }
                if (D_8014A110 != 0) {
                    D_800E6280.unk_1BC[1].unk_02 += 2;
                    D_800E6280.unk_1BC[1].unk_06 += 0xA;
                } else {
                    D_800E6280.unk_1BC[0].unk_02 += 2;
                    D_800E6280.unk_1BC[0].unk_06 += 0xA;
                }
            } else if (D_8014A120[1] == 1) {
                for (j = 0; j < 13; j++) {
                    D_800E6280.unk_1BC[j].unk_02 += 2;
                }
                if (D_8014A110 != 0) {
                    D_800E6280.unk_1BC[1].unk_02 += 1;
                    D_800E6280.unk_1BC[1].unk_06 += 5;
                } else {
                    D_800E6280.unk_1BC[0].unk_02 += 1;
                    D_800E6280.unk_1BC[0].unk_06 += 5;
                }
            }
            func_80042808();
        }
        break;
    default:
        func_80046500();
        break;
    }
}

void func_80143398(void) {
    s32 i;

    if ((u8) D_8014A110 == 0) {
        func_8004E44C(0, D_8014A384, D_8014A370);
        srn_init(0, D_8014A37C, 0);
    } else {
        func_8004E44C(0, D_8014A388, D_8014A374);
        srn_init(0, D_8014A380, 0);
    }
    srn_vram_set(0, 5, 0x580, 0xF0);
    for (i = 0; i < 3; i++) {
        D_800E6280.unk_1228[i + 5] = 8;
    }
    for (i = 0; i < 3; i++) {
        D_800E6280.unk_1228[i + 0x25] = 8;
    }
}

void func_80143470(void) {
    func_80048F64(0x60);
    D_80120651 = 3;
    D_80120688 = 0x01000000;
    D_80120652 = 1;
    D_80120653 = 0xA4;
    D_80120657 = 0x80;
    D_8012065C = (u8 *) D_80149FC0;
    D_80120660 = (u8 *) D_80149FF8;
    D_80120684 = &D_8014A1FC;
    D_80120664 = D_8014A030;
    D_80120654 = 0;
    D_80120655 = 0x80;
    if ((u8) D_8014A110 == 0) {
        D_80120666 = 0;
    } else {
        D_80120666 = 2;
    }
    D_80120676 = 0x50;
    D_8012067A = 0xB0;
    D_8012066A = 0x1000;
    D_8012066C = 0x1000;
    D_80120656 = 2;
}

/* NON_MATCHING: T-19010, 34 differing lines (T-16554: 38; statement order from tools/order_search.py), one window: the original keeps the
 * masked flags byte in $v1 (a uopt variable), loads the s16 rating straight into $v0 and loads the three
 * constants 0x7E0000/0x7F0000/0x7D0000 together before three stores that share one `lui at`; IDO leaves the
 * flags in a ugen temporary, promotes the rating field into $a1 with a move and emits each constant
 * next to its store. Same family as TAIIKU func_8013D2B4 (wiki/data/t0018-cases.md). T-19010 also tried, no
 * gain: a raw byte local, separate rating and speed locals, direct access instead of `gs`, the doubled
 * flags expression instead of `temp_v1`. T-22004 (34 rows, unchanged): `temp_v1` as u8/u16/u32/s8, the flags and rating
 * reads in the natural order, `temp_v1 & 3`, a doubled flags expression and viewsweep (6 callee views) gave no
 * gain; autotweak skips it (its isolated compile has 8 instructions more than the target). */
#ifdef NON_MATCHING
void func_8014357C(void) {
    GameState *gs;
    s32 var_v0;
    s32 temp_v1;

    gs = &D_800E6280;
    D_8014A120[0] = 0;
    D_8014A13C[0].unk_01 = 0;
    D_8014A13C[0].unk_00 = 0;
    D_8014A13C[0].unk_04 = 0;
    D_8014A13C[0].unk_08 = 0;
    D_8014A13C[0].unk_0C = 0;
    D_8014A13C[0].unk_10 = 0;
    D_8014A13C[0].unk_14.halves.low = 0;
    D_8014A13C[0].unk_1C = 1;
    D_8014A13C[0].unk_20 = 0;
    D_8014A13C[0].unk_1A = 0;
    D_8014A13C[0].unk_18 = 0;
    D_8014A13C[0].unk_02 = 0;
    D_8014A120[1] = 0;
    D_8014A13C[1].unk_01 = 0;
    D_8014A13C[1].unk_00 = 0;
    D_8014A13C[1].unk_04 = 0;
    D_8014A13C[1].unk_08 = 0;
    D_8014A13C[1].unk_0C = 0;
    D_8014A13C[1].unk_14.halves.high = 8; D_8014A13C[1].unk_1C = 1;
    D_8014A13C[1].unk_10 = 0;
    D_8014A13C[1].unk_14.halves.low = 0;
    D_8014A13C[1].unk_20 = 0;
    D_8014A13C[1].unk_1A = 0;
    D_8014A13C[1].unk_18 = 0;
    D_8014A13C[1].unk_02 = 0;
    D_8014A120[2] = 0;
    D_8014A13C[2].unk_01 = 0;
    D_8014A13C[2].unk_00 = 0;
    D_8014A13C[2].unk_04 = 0;
    D_8014A13C[2].unk_08 = 0;
    D_8014A13C[2].unk_14.halves.high = 0x10;
    D_8014A13C[2].unk_0C = 0;
    D_8014A13C[2].unk_10 = 0;
    D_8014A13C[2].unk_14.halves.low = 0;
    D_8014A13C[2].unk_1C = 1;
    D_8014A13C[2].unk_20 = 0;
    D_8014A13C[2].unk_1A = 0;
    D_8014A13C[2].unk_18 = 0;
    D_8014A13C[2].unk_02 = 0;
    D_8014A120[3] = 0;
    D_8014A13C[3].unk_01 = 0;
    D_8014A13C[3].unk_00 = 0;
    D_8014A13C[3].unk_04 = 0;
    D_8014A13C[3].unk_14.halves.high = 0x18;
    D_8014A13C[3].unk_08 = 0;
    D_8014A13C[3].unk_0C = 0;
    D_8014A13C[3].unk_10 = 0;
    D_8014A13C[3].unk_14.halves.low = 0;
    D_8014A13C[3].unk_1C = 1;
    D_8014A13C[3].unk_20 = 0;
    D_8014A13C[3].unk_1A = 0;
    D_8014A13C[3].unk_18 = 0;
    D_8014A13C[3].unk_02 = 0;
    D_8014A13C[0].unk_14.halves.high = -0xA0;
    D_8014A1D0 = 0;
    D_8014A1CC.w = 0;
    D_8014A118 = 0;
    D_8014A130[0] = 0x7E0000;
    D_8014A130[1] = 0x7F0000;
    var_v0 = gs->unk_10C.unk_02;
    D_8014A130[2] = 0x7D0000;
    temp_v1 = gs->unk_0F4.b[1] & 0xF;
    if ((temp_v1 & 1) || (temp_v1 & 2)) {
        var_v0 = (s16) (var_v0 >> 1);
    }
    if (var_v0 >= 0xB4) {
        D_8014A124 = 6;
        var_v0 = 0x20000;
    } else if (var_v0 >= 0x64) {
        D_8014A124 = 7;
        var_v0 = 0x1E000;
    } else if (var_v0 >= 0x41) {
        D_8014A124 = 8;
        var_v0 = 0x1C000;
    } else if (var_v0 >= 0x1E) {
        D_8014A124 = 9;
        var_v0 = 0x1A000;
    } else {
        D_8014A124 = 0xA;
        var_v0 = 0x18000;
    }
    if ((u8) D_8014A110 != 0) {
        var_v0 -= 0x2000;
    }
    D_8014A128 = var_v0 >> 3;
    D_8014A12C = var_v0;
    func_800AE120(gs->unk_10F4);
}
#else
INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80142240", func_8014357C);
#endif

/* FAKE: the dead `i = 0;` at the end keeps `i` live after the loop, so IDO tests the loop with
 * `slti` like the original and not as `i != 9`. T-19010 */
void func_80143814(void) {
    s32 i;
    TaiShorts12 a;
    TaiShorts12 b;

    a = D_8014A2F0;
    b = D_8014A304;
    for (i = 0; i < 9; i++) {
        func_80048F64(i + 0x60);
        DISP(96 + i).unk_38 = 0x01000000;
        DISP(96 + i).unk_07 = 0x80;
        DISP(96 + i).unk_0C = (u8 *)D_80149FC4;
        DISP(96 + i).unk_10 = (u8 *)D_80149FFC;
        DISP(96 + i).unk_34 = (u8 *)D_8014A208;
        DISP(96 + i).unk_14 = D_8014A034;
        DISP(96 + i).unk_04 = 0;
        DISP(96 + i).unk_05 = 0x80;
        DISP(96 + i).unk_1A = 0x1000;
        DISP(96 + i).unk_1C = 0x1000;
        DISP(96 + i).unk_06 = 2;
        DISP(96 + i).unk_2A = a.value[i];
        DISP(96 + i).unk_18 = b.value[i];
    }
    if (D_8014A110 != 0) {
        DISP(96).unk_18 = 0xE;
    }
    DISP(96).unk_01 = 2;
    DISP(96).unk_26 = -0xA0;
    DISP(96).unk_02 = 0x40;
    DISP(96).unk_03 = 0xA4;
    DISP(97).unk_01 = 3;
    DISP(97).unk_26 = -0x98;
    DISP(97).unk_02 = 0x40;
    DISP(97).unk_03 = 0xA4;
    DISP(98).unk_01 = 4;
    DISP(98).unk_26 = -0x90;
    DISP(98).unk_02 = 0x40;
    DISP(98).unk_03 = 0xA4;
    DISP(99).unk_01 = 5;
    DISP(99).unk_26 = -0x88;
    DISP(99).unk_02 = 0x40;
    DISP(99).unk_03 = 0xA4;
    DISP(100).unk_01 = 1;
    DISP(100).unk_26 = DISP(96).unk_26;
    DISP(100).unk_2A = DISP(96).unk_2A;
    DISP(101).unk_01 = 7;
    DISP(102).unk_01 = 5;
    DISP(103).unk_01 = 6;
    DISP(104).unk_01 = 7;
    DISP(104).unk_26 = -0x30;
    DISP(104).unk_02 = 0x40;
    DISP(104).unk_03 = 0xA4;
    i = 0;
}

void func_80143AC8(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        D_8014A13C[i].unk_04 += 1;
        D_8014A13C[i].unk_10 += 1;
        D_8014A13C[i].unk_1A += 1;
        D_8014A13C[i].unk_18 += 1;
    }
    D_8014A118 += 1;
}

void func_80143C04(void) {
    if ((D_80120778 != 0x2D) && (D_80120778 != 0x39)) {
        func_80143C84();
        func_80143CC0();
        func_80143D24();
        func_80143DD0();
        func_80143F50();
        return;
    }
    if ((u32)D_8014A13C[0].unk_18 >= 0x32) {
        D_80120778 = 0x2F;
    }
}

void func_80143C84(void) {
    if (D_8014A13C[0].unk_1C != 0) {
        D_8014A13C[0].unk_08 = D_8014A128;
        return;
    }
    D_8014A13C[0].unk_08 = -D_8014A128;
}

void func_80143CC0(void) {
    if ((u32)D_8014A13C[0].unk_10 >= 0xB) {
        D_8014A13C[0].unk_0C += D_8014A13C[0].unk_08;
        if (D_8014A13C[0].unk_0C < -0x4000) {
            D_8014A13C[0].unk_0C = -0x4000;
        }
        if (D_8014A12C < D_8014A13C[0].unk_0C) {
            D_8014A13C[0].unk_0C = D_8014A12C;
        }
        D_8014A13C[0].unk_10 = 0;
    }
}

/* D_8014A13C has to be an array here (not a struct) so that the base stays in a register; see TaiRunnerB. T-16550 */
void func_80143D24(void) {
    if (D_8014A13C[0].unk_08 < 0 || D_8014A1CE == 0) {
        if (D_8014A1D0 != 0) {
            D_8014A13C[0].unk_14.word -= D_8014A1D0;
        }
    } else {
        D_8014A13C[0].unk_14.word += (s32)D_8014A12C >> 2;
    }
    if (D_8014A13C[0].unk_14.halves.high >= -0x1F) {
        D_8014A13C[0].unk_14.halves.high = -0x20;
    }
    if (D_8014A13C[0].unk_14.halves.high < -0x9F) {
        D_8014A13C[0].unk_14.halves.high = -0xA0;
    }
    D_80120676 = D_8014A13C[0].unk_14.halves.high;
}

void func_80143DD0(void) {
    if (((u32) D_8014A124 < (u32) D_8014A13C[0].unk_04) && (D_8014A13C[0].unk_1C != 0)) {
        if ((s32) D_8014A13C[0].unk_00 >= 9) {
            D_8014A13C[0].unk_00 = 2;
            D_8014A13C[0].unk_04 = 0;
        } else {
            D_8014A13C[0].unk_00 = D_8014A13C[0].unk_00 + 1;
            D_8014A13C[0].unk_04 = 0;
        }
        if (D_8014A13C[0].unk_00 == 6) {
            ((DisplayActor44 *)D_8011ECD0)[100].unk_18 = 0x2E;
            if ((u8) D_8014A110 != 0) {
                func_80044750(0x804);
            } else {
                func_80044750(0x801);
            }
            D_8014A13C[0].unk_02 = 1;
        }
        if (D_8014A13C[0].unk_00 == 2) {
            ((DisplayActor44 *)D_8011ECD0)[100].unk_18 = 0x2F;
            if ((u8) D_8014A110 != 0) {
                func_80044750(0x805);
            } else {
                func_80044750(0x802);
            }
        }
    }
    if (D_8014A13C[0].unk_1C != 0) {
        if ((u8) D_8014A110 != 0) {
            D_80120668 = D_8014A1E8[D_8014A13C[0].unk_00];
        } else {
            D_80120668 = *(s16 *)&D_8014A1D4[D_8014A13C[0].unk_00 * 2];
        }
    }
    ((DisplayActor44 *)D_8011ECD0)[100].unk_26 = ((DisplayActor44 *)D_8011ECD0)[96].unk_26;
    ((DisplayActor44 *)D_8011ECD0)[100].unk_2A = ((DisplayActor44 *)D_8011ECD0)[96].unk_2A;
}

/* FAKE: the u8 view of the flag D_8014A110 keeps the lbu loads and the register shape of the
 * original; a plain read (promoted) changes the allocation. T-16554
 *
 * `bits` (GameState unk_F88) is read per region into a local: reading the field
 * directly lets the hoisted `& 5` test outrank it and take $v0. */
void func_80143F50(void) {
    s32 bits;

    if ((D_8014A13C[0].unk_00 >= 2U) && (D_8014A13C[0].unk_00 < 6U)) {
        bits = D_800E6280.unk_F88;
        if (bits & 5) {
            D_8014A13C[0].unk_20 = 1;
        }
        if ((bits & 0xA) && (D_8014A13C[0].unk_1C != 0)) {
            D_8014A13C[0].unk_1C = 0;
            D_8014A13C[0].unk_1A = 0;
            D_8014A13C[0].unk_02 = 0;
            D_8014A13C[0].unk_0C = 0;
            if (*(u8 *)&D_8014A110 != 0) {
                if (D_8014A13C[0].unk_20 != 0) {
                    ((DisplayActor44 *)D_8011ECD0)[96].unk_18 = 0x11;
                } else {
                    func_80044750(0x1805);
                    func_80044750(0x1804);
                    func_80044750(0x806);
                    ((DisplayActor44 *)D_8011ECD0)[96].unk_18 = 0x10;
                }
            } else if (D_8014A13C[0].unk_20 != 0) {
                ((DisplayActor44 *)D_8011ECD0)[96].unk_18 = 8;
            } else {
                func_80044750(0x1802);
                func_80044750(0x1801);
                func_80044750(0x803);
                ((DisplayActor44 *)D_8011ECD0)[96].unk_18 = 7;
            }
            D_8014A13C[0].unk_20 = 0;
            ((DisplayActor44 *)D_8011ECD0)[100].unk_03 = 0;
            func_80044750(0x50A);
        }
    }
    if ((D_8014A13C[0].unk_00 >= 6U) && (D_8014A13C[0].unk_00 < 0xAU)) {
        bits = D_800E6280.unk_F88;
        if (bits & 0xA) {
            D_8014A13C[0].unk_20 = 1;
        }
        if ((bits & 5) && (D_8014A13C[0].unk_1C != 0)) {
            D_8014A13C[0].unk_1C = 0;
            D_8014A13C[0].unk_1A = 0;
            D_8014A13C[0].unk_02 = 0;
            D_8014A13C[0].unk_0C = 0;
            if (*(u8 *)&D_8014A110 != 0) {
                if (D_8014A13C[0].unk_20 != 0) {
                    ((DisplayActor44 *)D_8011ECD0)[96].unk_18 = 0x11;
                } else {
                    func_80044750(0x1805);
                    func_80044750(0x1804);
                    func_80044750(0x806);
                    ((DisplayActor44 *)D_8011ECD0)[96].unk_18 = 0x10;
                }
            } else if (D_8014A13C[0].unk_20 != 0) {
                ((DisplayActor44 *)D_8011ECD0)[96].unk_18 = 8;
            } else {
                func_80044750(0x1802);
                func_80044750(0x1801);
                func_80044750(0x803);
                ((DisplayActor44 *)D_8011ECD0)[96].unk_18 = 7;
            }
            D_8014A13C[0].unk_20 = 0;
            ((DisplayActor44 *)D_8011ECD0)[100].unk_03 = 0;
            func_80044750(0x50A);
        }
    }
    if ((D_8014A13C[0].unk_04 == 0) && ((D_8014A13C[0].unk_00 == 5) || (D_8014A13C[0].unk_00 == 9)) && (D_8014A13C[0].unk_02 == 1)) {
        if (D_8014A13C[0].unk_20 != 0) {
            D_8014A13C[0].unk_20 = 0;
        } else if (D_8014A13C[0].unk_1C != 0) {
            D_8014A13C[0].unk_20 = 0;
            D_8014A13C[0].unk_1C = 0;
            D_8014A13C[0].unk_1A = 0;
            D_8014A13C[0].unk_02 = 0;
            D_8014A13C[0].unk_0C = 0;
            if (*(u8 *)&D_8014A110 != 0) {
                func_80044750(0x1805);
                func_80044750(0x1804);
                func_80044750(0x806);
                ((DisplayActor44 *)D_8011ECD0)[96].unk_18 = 0xF;
            } else {
                func_80044750(0x1802);
                func_80044750(0x1801);
                func_80044750(0x803);
                ((DisplayActor44 *)D_8011ECD0)[96].unk_18 = 6;
            }
            ((DisplayActor44 *)D_8011ECD0)[100].unk_03 = 0;
            func_80044750(0x50A);
        }
    }
    if (D_8014A13C[0].unk_1C == 0) {
        if (D_8014A13C[0].unk_1A >= 0x1EU) {
            D_8014A13C[0].unk_20 = 0;
            D_8014A13C[0].unk_1C = 1;
            D_8014A13C[0].unk_18 = 0;
            D_8014A13C[0].unk_00 = 0;
            if (*(u8 *)&D_8014A110 != 0) {
                ((DisplayActor44 *)D_8011ECD0)[96].unk_18 = 0xE;
            } else {
                ((DisplayActor44 *)D_8011ECD0)[96].unk_18 = 5;
            }
            ((DisplayActor44 *)D_8011ECD0)[100].unk_03 = 0xA4;
            if (*(u8 *)&D_8014A110 == 0) {
                ((DisplayActor44 *)D_8011ECD0)[100].unk_18 = 0x2D;
                func_80044750(0x800);
            } else {
                ((DisplayActor44 *)D_8011ECD0)[100].unk_18 = 0x39;
            }
        }
    }
}

void func_8014430C(void) {

    if ((u8) D_8014A118 >= 0xBU) {
        D_8014A1D0 += (D_8014A13C[0].unk_08);
        D_8014A118 = 0;
    }
    if (D_8014A12C < D_8014A1D0) {
        D_8014A1D0 = D_8014A12C;
    }
    if (D_8014A1D0 < 0) {
        D_8014A1D0 = 0;
    }
    if ((D_80120778 == 0x2D) || (D_80120778 == 0x39)) {
        D_8014A1D0 = 0;
    }
    D_8014A1CC.w += D_8014A1D0;
    Scroll((s32) ((s16) D_8014A1CC.h[1] / 4) % 640, 0xF, 0, 0x70, 0x280);
    Scroll((s32) ((s16) D_8014A1CC.h[1] / 2) % 128, 0xF, 0x70, 0x80, 0x80);
    func_8004B19C(D_8014A1D0, 0, -0x3C);
}

s32 func_80144464(void) {
    s32 i;
    s32 j;
    TkS16x4 t;

    t = D_8014A318;
    for (i = 0; i < 3; i++) {
        DISP(101 + i).unk_02 = 0x40;
        DISP(101 + i).unk_03 = 0xA4;
    }
    for (i = 0; i < 3; i++) {
        if (D_8014A13C[i + 1].unk_01 == 1 && DISP(101).unk_26 + 0x30 < DISP(97 + i).unk_26 + 0x40) {
            DISP(102).unk_03 = 0;
            DISP(102).unk_02 = 0;
        }
    }
    for (i = 0; i < 4; i++) {
        DISP(101 + i).unk_26 = t.v[i] - D_8014A1CE - 0xA0;
    }
    if (DISP(102).unk_18 == 0x32) {
        DISP(102).unk_03 |= 0x40;
        DISP(102).unk_26 = DISP(101).unk_26 + 0x20;
    }
    if (DISP(104).unk_26 < -0xDC && DISP(104).unk_03 != 0) {
        DISP(104).unk_03 = 0;
    }
    if (DISP(96).unk_26 + 0x40 >= DISP(101).unk_26) {
        if (D_8014A120[0] == 0) {
            DISP(102).unk_03 = 0xE4;
            DISP(102).unk_18 = 0x32;
            DISP(102).unk_26 = DISP(101).unk_26 + 0x20;
        }
        i = 0;
        if (D_8014A120[0] != 0) {
            do {
                i++;
            } while (D_8014A120[i] != 0);
        }
        D_8014A120[i] = 1;
        for (j = i + 1; j < 4; j++) {
            D_8014A120[j] = func_801446A0();
        }
        return 1;
    }
    return 0;
}

/* FAKE: v[6] as in func_8013815C. T-0016 */
s32 func_801446A0(void) {
    s16 v[6];
    s32 i;

    for (i = 0; i < 3; i++) {
        if (D_8014A13C[i + 1].unk_01 == 0) {
            v[i] = *(s16 *)(D_8011ECD0 + i * 0x44 + 0x19EA);
        } else {
            v[i] = -0xF00;
        }
    }
    if (v[0] >= v[1] && v[0] >= v[2]) {
        D_8014A13C[1].unk_01 = 1;
        return 2;
    }
    if (v[1] >= v[2]) {
        D_8014A13C[2].unk_01 = 1;
        return 3;
    }
    D_8014A13C[3].unk_01 = 1;
    return 4;
}

typedef struct {
    s16 v[3];
} Tri3S; /* size 0x6 */
extern Tri3S D_8014A320;

void func_80144768(void) {
    s32 i; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    Tri3S tbl;

    tbl = D_8014A320;
    for (i = 0; i < 3; i++) {
        if (D_8014A13C[i + 1].unk_18 >= 0x33U) {
            if (i + 0x36 == ((DisplayActor44 *)D_8011ECD0)[i + 97].unk_18) {
                ((DisplayActor44 *)D_8011ECD0)[i + 97].unk_18 = tbl.v[i];
            }
        }
    }
    func_80144834();
    func_801448D4();
    func_801449A0();
    func_80144B40();
    func_80144C70();
}

typedef struct {
    s32 v[3];
} Tri3; /* size 0xC */
extern Tri3 D_8014A328;

void func_80144834(void) {
    s32 i; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    Tri3 tbl;

    tbl = D_8014A328;
    for (i = 0; i < 3; i++) {
        if (i + 0x36 != *(s16 *) (D_8011ECD0 + i * 0x44 + 0x19DC)) {
            if (D_8014A13C[i + 1].unk_1C != 0) {
                D_8014A13C[i + 1].unk_08 = tbl.v[i];
            } else {
                D_8014A13C[i + 1].unk_08 = -0x500;
            }
        }
    }
}

extern Tri3 D_8014A334;

/* FAKE: byte-offset walk over the TaiRunnerB records; with r = &D_8014A13C[i + 1] or the byte base plus 0x24 * i
 * cast to TaiRunnerB *, the fields give different code (checked in the T-16550 review). T-16550 */
void func_801448D4(void) {
    s32 i; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    Tri3 tbl;
    u8 *q;
    u8 *r;
    s32 v;

    tbl = D_8014A334;
    for (i = 0, q = D_8011ECD0; i < 3; i++, q += 0x44) {
        if (i + 0x36 != *(s16 *)(q + 0x19DC)) {
            r = (u8 *)D_8014A13C + i * 0x24;
            if (*(u32 *)(r + 0x34) >= 0xBU) {
                v = *(s32 *)(r + 0x30) += *(s32 *)(r + 0x2C);
                if (v < 0) {
                    *(s32 *)(r + 0x30) = 0;
                    v = 0;
                }
                if (tbl.v[i] < v) {
                    *(s32 *)(r + 0x30) = tbl.v[i];
                }
                *(s32 *)(r + 0x34) = 0;
            }
        }
    }
}

extern Tri3 D_8014A340;

/* FAKE: byte view of the TaiRunnerB records. Written as D_8014A13C[i + 1] fields the loop gets a
 * second induction register and a frame 8 bytes larger (T-16550). The factored flag-store stride
 * preserves the height reload. T-10010. */
void func_801449A0(void) {
    s32 i;
    s32 d;
    u8 *q; /* FAKE: unused word keeps tbl at stack +0x48. T-10010. */
    Tri3 tbl;
    s32 y;
    s32 n;

    tbl = D_8014A340;
    d = D_8014A1CE;
    for (i = 0; i < 3; i++) {
        *(s32 *)&((u8 *)D_8014A13C)[i * 0x24 + 0x38] += *(s32 *)&((u8 *)D_8014A13C)[i * 0x24 + 0x30];
        y = *(s16 *)&((u8 *)D_8014A13C)[i * 0x24 + 0x3A] - d;
        ((DisplayActor44 *)D_8011ECD0)[i + 97].unk_26 = y - 0xA0;
        if (i + 0x36 != ((DisplayActor44 *)D_8011ECD0)[i + 97].unk_18 && D_8014A130[i] > 0) {
            *(s32 *)&((u8 *)D_8014A13C)[i * 0x24 + 0x38] += tbl.v[i];
            D_8014A130[i] -= tbl.v[i];
            y = *(s16 *)&((u8 *)D_8014A13C)[i * 0x24 + 0x3A] - d;
        }
        if (((DisplayActor44 *)D_8011ECD0)[101].unk_26 + i * 8 + 8 < y - 0x60 && ((u8 *)D_8014A13C)[i * 0x24 + 0x25] == 0) {
            ((DisplayActor44 *)D_8011ECD0)[102].unk_18 = 0x32;
            ((DisplayActor44 *)D_8011ECD0)[102].unk_26 = ((DisplayActor44 *)D_8011ECD0)[101].unk_26 + 0x20;
            n = 0;
            while (D_8014A120[n] != 0) {
                n++;
            }
            D_8014A120[n] = i + 2;
            ((u8 *)D_8014A13C)[(i * 3) * 12 + 0x25] = 1;
            y = *(s16 *)&((u8 *)D_8014A13C)[i * 0x24 + 0x3A] - d;
        }
        if (y >= 0x1E1 || y < -0xA0) {
            ((DisplayActor44 *)D_8011ECD0)[i + 97].unk_03 = 0;
        } else {
            ((DisplayActor44 *)D_8011ECD0)[i + 97].unk_03 = 0xA4;
        }
    }
}

extern Tri3 D_8014A34C;

void func_80144B40(void) {
    s32 i;
    Tri3 tbl;

    tbl = D_8014A34C;
    for (i = 0; i < 3; i++) {
        if (i + 0x36 != ((DisplayActor44 *)D_8011ECD0)[i + 97].unk_18) {
            if ((u32)tbl.v[i] < D_8014A13C[i + 1].unk_04 && D_8014A13C[i + 1].unk_1C == 1) {
                if ((s32)D_8014A13C[i + 1].unk_00 >= 9) {
                    D_8014A13C[i + 1].unk_00 = 2;
                    D_8014A13C[i + 1].unk_04 = 0;
                } else {
                    D_8014A13C[i + 1].unk_00 += 1;
                    D_8014A13C[i + 1].unk_04 = 0;
                }
                ((DisplayActor44 *)D_8011ECD0)[i + 97].unk_18 = *(s16 *)&D_8014A1D4[D_8014A13C[i + 1].unk_00 * 2] + i * 9 + 0x12;
            }
            if (D_8014A13C[i + 1].unk_00 == 6) {
                D_8014A13C[i + 1].unk_02 = 1;
            }
        }
    }
}

extern Tri3S D_8014A358;

/* FAKE: byte-offset walk over the TaiRunnerB records, same as func_801448D4 (the field form was checked in the
 * T-16550 review and differs). T-16550 */
void func_80144C70(void) {
    s32 i; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    Tri3S tbl;
    u8 *q;
    u8 *r;

    tbl = D_8014A358;
    for (i = 0, q = D_8011ECD0; i < 3; i++, q += 0x44) {
        if (i + 0x36 != *(s16 *)(q + 0x19DC)) {
            r = (u8 *)D_8014A13C + i * 0x24;
            if (*(s32 *)(r + 0x28) == 0 && (r[0x24] == 2 || r[0x24] == 6) && *(s32 *)(r + 0x40) == 1 &&
                tbl.v[i] < (func_800AE0D0() & 0xFF) && r[0x26] == 1) {
                *(s32 *)(r + 0x40) = 0;
                *(s16 *)(r + 0x3E) = 0;
                *(s32 *)(r + 0x30) = 0;
                *(s16 *)(q + 0x19DC) = func_800AE0D0() % 3 + i * 9 + 0x18;
            }
        }
    }
    for (i = 0, q = D_8011ECD0; i != 3; i++, q += 0x44) {
        if (i + 0x36 != *(s16 *)(q + 0x19DC)) {
            r = (u8 *)D_8014A13C + i * 0x24;
            if (*(s32 *)(r + 0x40) == 0 && *(u16 *)(r + 0x3E) >= 0x1EU) {
                *(s32 *)(r + 0x40) = 1;
                *(s16 *)(r + 0x3C) = 0;
                r[0x24] = 0;
                r[0x26] = 0;
                *(s16 *)(q + 0x19DC) = i + 0x36;
            }
        }
    }
}

/* FAKE: the dead `i = D_8014A110;` before the first call puts the flag in the entry set of
 * tools/cvt_pass.py, which then strips the widening of the flag test in the second loop (promoted
 * `lbu a1`, flag compared first); the dead `i = 0;` at the end keeps the second loop rolled with the
 * walk compare, as in func_80135830. D_8014A110 is `u8` (was `s8`; the matched uses are unchanged). T-19010 */
void func_80144E54(void) {
    s32 i;

    i = D_8014A110;
    func_80048E78();
    for (i = 0; i < 8; i++) {
        DISP(96 + i).unk_03 = 0xA4;
        DISP(96 + i).unk_02 = 0x40;
        DISP(96 + i).unk_05 = 0x80;
        DISP(96 + i).unk_01 = 3;
        DISP(96 + i).unk_38 = 0x01000000;
        DISP(96 + i).unk_07 = 0x80;
        DISP(96 + i).unk_0C = (u8 *)D_80149FC8;
        DISP(96 + i).unk_10 = (u8 *)D_8014A000;
        DISP(96 + i).unk_34 = (u8 *)D_8014A228;
        DISP(96 + i).unk_14 = D_8014A038;
        DISP(96 + i).unk_1A = 0x1000;
        DISP(96 + i).unk_1C = 0x1000;
        DISP(96 + i).unk_06 = 2;
    }
    for (i = 0; i < 4; i++) {
        DISP(96 + i).unk_26 = i * 0x40 - 0x78;
        DISP(96 + i).unk_18 = D_8014A120[i] * 8 + 0x1A;
        DISP(96 + i).unk_2A = -8;
        if (DISP(96 + i).unk_18 == 0x22 && D_8014A110 == 1) {
            DISP(96 + i).unk_18 = 0x42;
        }
    }
    DISP(100).unk_26 = -0x78;
    DISP(100).unk_2A = -8;
    DISP(100).unk_18 = D_8014A120[0] * 5 + 0xD;
    DISP(101).unk_26 = -0x38;
    DISP(101).unk_2A = -8;
    DISP(101).unk_18 = D_8014A120[1] * 5 + 0xD;
    DISP(102).unk_26 = 8;
    DISP(102).unk_2A = -8;
    DISP(102).unk_18 = D_8014A120[2] * 5 + 0xD;
    DISP(103).unk_26 = 0x48;
    DISP(103).unk_2A = -8;
    DISP(103).unk_18 = D_8014A120[3] * 5 + 0xD;
    i = 0;
}

void func_80145044(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        ((DisplayActor44 *)D_8011ECD0)[i + 96].unk_18 += 1;
    }
    for (i = 0; i < 4; i++) {
        ((DisplayActor44 *)D_8011ECD0)[i + 100].unk_18 -= 4 - i;
    }
}

void func_801450C0(void) {
    s32 i;
    TkS16x4 t;

    t = D_8014A360;
    for (i = 0; i < 4; i++) {
        ((DisplayActor44 *)D_8011ECD0)[i + 96].unk_18 += t.v[i];
    }
}

void func_80145148(void) {
    if ((u32)D_8014A11C >= 0xF) {
        *(s16 *)(D_8011ECD0 + 0x19DC) ^= 1;
        *(s16 *)(D_8011ECD0 + 0x1A20) ^= 1;
        D_8014A11C = 0;
    }
}

void func_80145190(void) {
    D_8014A370 = 0x801AD000;
    D_8014A374 = 0x801AE018;
    D_8014A378 = 0x8018F000;
    D_8014A37C = 0x801AF1B0;
    D_8014A380 = 0x801B29EC;
    D_8014A384 = 0x801AD008;
    D_8014A388 = 0x801AE020;
}
