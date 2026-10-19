#include "ovl/RENSYU.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/RENSYU/data/RENSYU/80132040.data", D_801342C0);
u8 D_801342C4 = 0;
u8 D_801342C8[5] = { 0 };
INCLUDE_RODATA("asm/ovl/RENSYU/data/RENSYU/80132040.data", D_801342D0);
INCLUDE_RODATA("asm/ovl/RENSYU/data/RENSYU/80132040.data", D_801342D4);
INCLUDE_RODATA("asm/ovl/RENSYU/data/RENSYU/80132040.data", D_801342F4);
INCLUDE_RODATA("asm/ovl/RENSYU/data/RENSYU/80132040.data", D_80134328);
INCLUDE_RODATA("asm/ovl/RENSYU/data/RENSYU/80132040.data", D_8013435C);
RensyuTbl11 D_80134378 = { { 0x1E, 0x3C, 0x5A, 0x78, 0x96, 0xB4, 0xD2, 0xF0, 0x10E, 0x12C, 0x14A } };
RensyuTbl11 D_80134390 = { { 0x1E, 0x3C, 0x5A, 0x78, 0x96, 0xB4, 0xD2, 0xF0, 0x10E, 0x12C, 0x14A } };
INCLUDE_RODATA("asm/ovl/RENSYU/data/RENSYU/80132040.data", D_801343A8);
INCLUDE_RODATA("asm/ovl/RENSYU/data/RENSYU/80132040.data", D_801343D0);
INCLUDE_RODATA("asm/ovl/RENSYU/data/RENSYU/80132040.data", D_801343D4);
INCLUDE_RODATA("asm/ovl/RENSYU/data/RENSYU/80132040.data", D_801343D8);
INCLUDE_RODATA("asm/ovl/RENSYU/data/RENSYU/80132040.data", D_801343DC);
INCLUDE_RODATA("asm/ovl/RENSYU/data/RENSYU/80132040.data", D_801343E0);

typedef struct {
    void (*f[8])();
} FnTbl8; /* size 0x20 */
extern FnTbl8 D_801342D4;

void func_80132040(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl8 tbl;

    tbl = D_801342D4;
    idx = D_800E6280.unk_1109;
    tbl.f[idx]();
}

void func_801320E0(void) {
    u32 temp_t4;

    switch (D_800E6280.unk_110A) {
    case 0:
        func_80044750(0xBF);
        func_80044890(0, 0xBF98, 0xBF79, 0xC982, 0xC935, 0xC91B);
        func_80085E30(1, 0);
        func_8008585C();
        message_window_init();
        icon_disp_switch(0);
        func_8006764C(0);
        parameter_disp_switch(0);
        hizuke_init();
        k_reset(0);
        func_8006612C((s32) "練習試合");
        func_80065F34(0);
        dec_bg_reset();
        set_dec_bri(0);
        func_800AE120(D_800E6280.unk_10F4);
        func_8004284C();
        break;
    case 1:
        if (func_80044E8C() == 1) {
            func_80044750(0x206);
            func_8004284C();
        }
        break;
    case 2:
        cd_read_sectors(7U, 0x801E8000, 0xB037);
        func_8004284C();
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            func_800847B8(0U);
            func_80084E4C();
            func_80132000();
            D_800CA160 = D_801342B0;
            D_800CA164 = D_801342B4;
            temp_t4 = (u16) D_800E6280.unk_0F4.h >> 0xC;
            D_800CA168 = D_801342B8;
            switch (temp_t4) {
            case 6:
                D_800CA148 = 0;
                break;
            case 7:
                D_800CA148 = 1;
                break;
            case 8:
                D_800CA148 = 2;
                break;
            case 9:
                D_800CA148 = 3;
                break;
            case 10:
                D_800CA148 = 4;
                break;
            default:
                func_80046500();
                break;
            }
            D_800CA14C = 0;
            func_8004284C();
        }
        break;
    case 4:
    case 6:
        normal_date_speak();
        break;
    case 5:
        /* The target reads the adjacent record bytes +2/+3 as one halfword. */
        D_800CA148 = (*(u16 *)&D_800E6280.unk_66C[(u16) D_800E6280.unk_0F4.h >> 0xC].unk_02 & 0xF) + 5;
        D_800CA14C = 0;
        func_8004284C();
        break;
    case 7:
        func_80042808();
        break;
    default:
        func_80046500();
        break;
    }
    Default_Disp();
}

INCLUDE_ASM("asm/ovl/RENSYU/nonmatchings/RENSYU/80132040", func_80132390);

INCLUDE_ASM("asm/ovl/RENSYU/nonmatchings/RENSYU/80132040", func_80132A28);

void func_80132F10(void) {
    s32 var_s0;

    switch (D_800E6280.unk_110A) {
    case 0:
        switch ((u16) D_800E6280.unk_0F4.h >> 0xC) {
        case 6:
            func_8005742C(0x417A, 1);
            break;
        case 7:
            func_8005742C(0x4183, 1);
            break;
        case 8:
            func_8005742C(0x419E, 1);
            break;
        case 9:
            func_8005742C(0x4195, 1);
            break;
        case 10:
            func_8005742C(0x418D, 1);
            break;
        default:
            func_80046500();
            break;
        }
        func_8004284C();
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            func_80057418(1, func_8005751C(1));
            switch ((u16) D_800E6280.unk_0F4.h >> 0xC) {
            case 6:
                cd_read_sectors(0x1BU, 0x80197000, 0xB03E);
                break;
            case 7:
                cd_read_sectors(0x1AU, 0x80197000, 0xAFE7);
                break;
            case 8:
                cd_read_sectors(0x1BU, 0x80197000, 0xB01C);
                break;
            case 9:
                cd_read_sectors(0x1BU, 0x80197000, 0xB001);
                break;
            case 10:
                cd_read_sectors(0x1BU, 0x80197000, 0xAFCC);
                break;
            default:
                func_80046500();
                break;
            }
            func_80065F34(1);
            func_8004284C();
        }
        break;
    case 2:
        func_8007E81C();
        break;
    case 3:
        func_8004E9F4(1);
        func_8004E788(-0x80, 0x32, 0, (s32) "試合中", 0);
        func_8004E884(1);
        func_8004284C();
        break;
    case 4:
        if (cd_read_sync_status() & 1) {
            switch ((u16) D_800E6280.unk_0F4.h >> 0xC) {
            case 6:
                func_801340B0();
                break;
            case 7:
                func_80133FB4();
                break;
            case 8:
                func_8013405C();
                break;
            case 9:
                func_80134008();
                break;
            case 10:
                func_80133F60();
                break;
            default:
                func_80046500();
                break;
            }
            var_s0 = 0;
            do {
                func_80043914(D_801343E0, (var_s0 + 0x10) & 0xFF, 1, 1, var_s0);
                var_s0 += 1;
            } while (var_s0 != 5);
            func_80042808();
        }
        break;
    default:
        func_80046500();
        break;
    }
    func_80086424();
}

/* FAKE: scalar byte views on unk_110A and D_801342C4 preserve the target live ranges and
 * swap_count store placement; unused local `a` and reusing `value` as the second shuffle
 * index place `tbl` at 0x40(sp) and `swap_count` at 0x64(sp) (T-19020, T-24102). */
void func_8013325C(void) {
    u8 swap;
    u8 other;
    s32 i;
    s32 swap_count;
    u8 *a;
    s32 first;
    s32 value;
    RensyuTbl11 tbl;

    tbl = D_80134378;
    switch (*(u8 *)&D_800E6280.unk_110A) {
    case 0:
        value = (s16)(D_800E6280.unk_66C[(u16) D_800E6280.unk_0F4.h >> 0xC].unk_00 - tbl.v[((RensyuClubCount *)&D_800E6280.unk_66C[(u16) D_800E6280.unk_0F4.h >> 0xC])->count]);
        if (value < -0x1E) {
            (*(u8 *)&D_801342C4) = 0;
        } else if (value < -0x14) {
            (*(u8 *)&D_801342C4) = 1;
        } else if (value < -0xA) {
            (*(u8 *)&D_801342C4) = 1;
        } else if (value < 0) {
            (*(u8 *)&D_801342C4) = 2;
        } else if (value >= 0x1E) {
            (*(u8 *)&D_801342C4) = 5;
        } else if (value >= 0x14) {
            (*(u8 *)&D_801342C4) = 4;
        } else if (value >= 0xA) {
            (*(u8 *)&D_801342C4) = 4;
        } else if (value >= 0) {
            (*(u8 *)&D_801342C4) = 3;
        }
        func_8004284C();
        break;
    case 1:
        for (i = 0; i < 5; i++) {
            D_801342C8[i] = 0;
        }
        for (i = 0; i < (u8)(*(u8 *)&D_801342C4); i++) {
            D_801342C8[i] = 1;
        }
        i = 0;
        swap_count = func_800AE0D0() & 0xF;
        if ((u8)(*(u8 *)&D_801342C4) >= 3U) {
            swap = D_801342C8[0];
            other = D_801342C8[4];
            D_801342C8[4] = swap;
            D_801342C8[0] = other;
        }
        if (swap_count > 0) {
            do {
                first = func_800AE0D0() & 3;
                value = func_800AE0D0() & 3;
                swap = D_801342C8[first];
                i++;
                other = D_801342C8[value];
                D_801342C8[first] = other;
                D_801342C8[value] = swap;
            } while (i != swap_count);
        }
        if (((u16) D_800E6280.unk_0F4.h >> 0xC) == 9) {
            if ((u8)(*(u8 *)&D_801342C4) >= 3U) {
                D_801342C8[0] = 1;
            } else {
                D_801342C8[0] = 0;
            }
        }
        func_8004284C();
        break;
    case 2:
        func_80048F64(0x60);
        ((DisplayActor44 *)D_8011ECD0)[96].unk_01 = 3;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_38 = 0x01000000;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_07 = 0x80;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_05 = 0x80;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_1A = 0x1000;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_1C = 0x1000;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_06 = 1;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_43 = ((RensyuClubCount *)&D_800E6280.unk_66C[(u16) D_800E6280.unk_0F4.h >> 0xC])->count % 5U + 0x10;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_26 = -0x60;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_2A = 0x20;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_0C = (u8 *) D_801343D4;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_10 = (u8 *) D_801343D8;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_34 = (u8 *) D_801343D0;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_14 = D_801343DC;
        D_801342C0 = 0;
        func_80042808();
        break;
    default:
        func_80046500();
        break;
    }
    func_80086424();
}

#ifdef NON_MATCHING
/* NON_MATCHING: T-22030, T-24082, 4 rows (2 pairs); frame, registers, control flow and cases 1 to 6 match. In case 0
 * the original has `lui v0,%hi(D_801342C0)` before the bnez on D_80120652 (with a nop in the branch slot); the draft
 * puts the lui into the branch slot (nop before the bnez). Root cause (T-24082): uopt promotes &tbl to $a0 (RLDA) in
 * BB0 and emits UNAL(1,$4,$gp) before `tbl = D_80134390` and UNAL(0,$4,$gp) at the end of BB0 after XJP/CLAB/UJP; ugen
 * emits `.alias $4,$gp` (I_IALIAS) between the jump-table `.text` switch and the case-0 label, which makes IDO 5.3 as1
 * schedule `lui v0` into the bnez slot instead of the lbu load-delay slot (moving or dropping that single I_IALIAS
 * record in binasm gives score=0). */
void func_80133628(void) {
    s32 girl;
    s16 diff;
    RensyuTbl11 tbl;

    tbl = D_80134390;
    switch (D_800E6280.unk_110A) {
    case 0:
        if (((DisplayActor44 *)D_8011ECD0)[96].unk_02 == 0) {
            if ((D_801342C0 >= 5U) || (D_801342C0 && (((u16) D_800E6280.unk_0F4.h >> 0xC) == 9))) {
                func_8004284C();
            } else {
                ((DisplayActor44 *)D_8011ECD0)[96].unk_03 = 0x84;
                ((DisplayActor44 *)D_8011ECD0)[96].unk_02 = 5;
                ((DisplayActor44 *)D_8011ECD0)[96].unk_18 = 0;
                ((DisplayActor44 *)D_8011ECD0)[96].unk_08 = 0;
                if (D_801342C8[D_801342C0] == 1) {
                    ((DisplayActor44 *)D_8011ECD0)[96].unk_16 = 0;
                } else {
                    ((DisplayActor44 *)D_8011ECD0)[96].unk_16 = 1;
                }
                ++D_801342C0;
            }
        }
        break;
    case 1:
        if (D_801342C4 >= 3U) {
            func_80044750(0x204);
            D_800CA148 = 0x11;
        } else {
            func_80044750(0x205);
            D_800CA148 = ((RensyuClubCount *)&D_800E6280.unk_66C[(u16) D_800E6280.unk_0F4.h >> 0xC])->count + 0x12;
        }
        k_reset(1);
        func_8004284C();
        break;
    case 2:
        normal_date_speak();
        break;
    case 3:
        girl = (u16) D_800E6280.unk_0F4.h >> 0xC;
        diff = D_800E6280.unk_66C[girl].unk_00 - tbl.v[((RensyuClubCount *)&D_800E6280.unk_66C[girl])->count];
        if (diff < -0x1E) {
            D_800CA148 = 0x1D;
        } else if (diff < -0x14) {
            D_800CA148 = 0x1E;
        } else if (diff < -0xA) {
            D_800CA148 = 0x1F;
        } else if (diff < 0) {
            D_800CA148 = 0x20;
        } else if (diff >= 0x1E) {
            D_800CA148 = 0x24;
        } else if (diff >= 0x14) {
            D_800CA148 = 0x23;
        } else if (diff >= 0xA) {
            D_800CA148 = 0x22;
        } else if (diff >= 0) {
            D_800CA148 = 0x21;
        }
        func_8004284C();
        break;
    case 4:
        normal_date_speak();
        break;
    case 5:
        ((DisplayActor44 *)D_8011ECD0)[96].unk_07 -= 4;
        if (((DisplayActor44 *)D_8011ECD0)[96].unk_07 < 3U) {
            ((DisplayActor44 *)D_8011ECD0)[96].unk_07 = 0;
        }
        normal_date_bg_fadeout();
        break;
    case 6:
        func_80048F64(0x60);
        if ((D_801342D0 == 0xB) || (D_801342D0 == 0xFF)) {
            func_80042908(7);
        } else {
            func_80042808();
        }
        break;
    }
    Default_Disp();
}
#else
INCLUDE_ASM("asm/ovl/RENSYU/nonmatchings/RENSYU/80132040", func_80133628);
#endif

/* FAKE: T-24122. Unused `pad` and `a` place `tbl` at 0x30(sp); reading D_801342D0 before
 * func_80083808() puts it in cvt_pass's entry set so uopt promotes it to $a0 across the
 * switch; the empty raw-byte test on unk_1BC[4].unk_0C.b[2] retains separate live ranges
 * for the raw byte ($a1) and its high nibble ($v0), matching SHOUGATU:func_801341E4. */
void func_80133990(void) {
    s32 pad;
    s16 a;
    RensyuTbl13 tbl;

    tbl = D_801343A8;
    a = D_801342D0;
    func_80083808();
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8004500C(1, 0);
        func_80083474();
        func_800573F8(1);
        break;
    case 1:
        func_8007E884();
        break;
    case 2:
        if (D_801342C4 >= 3U) {
            D_800CA148 = tbl.v[D_801342D0] + 2;
            if (D_801342D0 == 4) {
                if (D_800E6280.unk_1BC[4].unk_0C.b[2]) {
                }
                if (((u16)D_800E6280.unk_0F4.h >> 12) != ((u8)D_800E6280.unk_1BC[4].unk_0C.b[2] >> 4)) {
                    if (((u8)D_800E6280.unk_1BC[4].unk_0C.b[2] >> 4) == 7) {
                        D_800CA148 += 1;
                    }
                    if (((u8)D_800E6280.unk_1BC[4].unk_0C.b[2] >> 4) == 6) {
                        D_800CA148 += 2;
                    }
                }
            }
            if (D_801342D0 == 6) {
                if (((u8)D_800E6280.unk_1BC[6].unk_0C.b[2] >> 4) != ((u16)D_800E6280.unk_0F4.h >> 12)) {
                    D_800CA148 += 1;
                }
            }
        } else {
            D_800CA148 = tbl.v[D_801342D0] + 3;
            if (D_801342D0 == 4) {
                D_800CA148 += 2;
            }
            if (D_801342D0 == 6) {
                D_800CA148 += 1;
            }
        }
        func_8004284C();
        break;
    case 3:
    case 6:
        normal_date_speak();
        break;
    case 4:
        if (D_801342C4 >= 3U) {
            if (D_801342D0 == 0xC) {
                D_800CA148 = 0x26;
            } else {
                D_800CA148 = 0x25;
            }
        } else {
            if (D_800E6280.unk_03E == 0x61 && D_800E6280.unk_03F == 0xC) {
                D_800CA148 = 0x28;
            } else {
                D_800CA148 = 0x27;
            }
        }
        func_8004284C();
        break;
    case 5:
        if (D_801342D0 == 0xC) {
            func_8007E5A0();
        } else {
            func_8004284C();
        }
        break;
    case 7:
        if (D_801342D0 == 0xC) {
            func_8007E934();
        } else {
            func_8007E99C();
        }
        break;
    case 8:
        func_80044750(0xC1);
        func_80042808();
        break;
    default:
        func_80046500();
        break;
    }
    Default_Disp();
}

void func_80133CB8(void) {
    s32 i;
    u32 temp_t8;

    switch (D_800E6280.unk_110A) {
    case 0:
        func_80044750(0xBF);
        func_800674B0();
        func_8004284C();
        break;
    case 1:
        if (func_80044E8C() == 1) {
            if (D_801342C4 >= 3U) {
                temp_t8 = (u16) D_800E6280.unk_0F4.h >> 0xC;
                ((RensyuClubCount *)&D_800E6280.unk_66C[temp_t8])->count++;
                /* FAKE: T-19020. The empty test puts a basic block boundary between the counter update and the
                 * loop setup; without it the loop pointer constant is defined in the counter's block and takes
                 * v0 from the counter address (original: both in v0). The loop sits on the line of the
                 * statement before it because as1 schedules by source line (the lui of the loop pointer then
                 * comes before the lui of the store). The real source is unknown. */
                i = 1;
                if (i) {
                }
                D_800E6280.unk_1BC[0].unk_02 += 2; for (i = 1; i < 13; i++) {
                    D_800E6280.unk_1BC[i].unk_02 += 2;
                }
            } else {
                D_800E6280.unk_118.unk_02 += 0xA;
                D_800E6280.unk_11C.unk_02 += 0xA;
            }
            if (D_801342D0 != 0xFF) {
                D_800E6280.unk_1BC[D_801342D0].unk_06 += 2;
            }
            func_8008585C();
            set_dec_bri(0);
            func_80072B5C(1);
        }
        break;
    default:
        func_80046500();
        break;
    }
    Default_Disp();
}

void func_80133E5C(void) {
    switch (D_801342D0) {
    case 0:
        switch ((u32)D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4) {
        case 6:
        case 7:
            func_80062CD0(0x54FE);
            break;
        case 8:
            func_80062CD0(0x552B);
            break;
        case 9:
            func_80062CD0(0x5558);
            break;
        case 10:
            func_80062CD0(0x5585);
            break;
        }
        break;
    case 4:
        func_80062CD0(0x66EC);
        break;
    case 5:
        func_80062CD0(0x5FE4);
        break;
    case 6:
        func_80062CD0(0x625A);
        break;
    case 10:
        func_80062CD0(0x6962);
        break;
    default:
        func_80046500();
        break;
    }
}

void func_80133F60(void) {
    D_801343D0 = 0x801A42D4;
    D_801343D4 = 0x801A42E0;
    D_801343D8 = 0x801A4330;
    D_801343DC = *(s16 *)0x801A4338;
    D_801343E0 = 0x80197000;
}

void func_80133FB4(void) {
    D_801343D0 = 0x801A3E3C;
    D_801343D4 = 0x801A3E48;
    D_801343D8 = 0x801A3E90;
    D_801343DC = *(s16 *)0x801A3E98;
    D_801343E0 = 0x80197000;
}

void func_80134008(void) {
    D_801343D0 = 0x801A42E4;
    D_801343D4 = 0x801A42F0;
    D_801343D8 = 0x801A4368;
    D_801343DC = *(s16 *)0x801A4370;
    D_801343E0 = 0x80197000;
}

void func_8013405C(void) {
    D_801343D0 = 0x801A432C;
    D_801343D4 = 0x801A4338;
    D_801343D8 = 0x801A4394;
    D_801343DC = *(s16 *)0x801A43A4;
    D_801343E0 = 0x80197000;
}

void func_801340B0(void) {
    D_801343D0 = 0x801A403C;
    D_801343D4 = 0x801A4048;
    D_801343D8 = 0x801A4098;
    D_801343DC = *(s16 *)0x801A40B0;
    D_801343E0 = 0x80197000;
}


