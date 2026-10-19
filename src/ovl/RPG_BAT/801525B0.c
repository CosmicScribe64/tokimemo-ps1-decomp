#define MAIN_API_OVERRIDE_func_80049A40 /* unprototyped view: keeps 16-bit widening off the negative x/y arguments in func_80152F30 (T-24124) */
#include "common.h"
#include "ovl/RPG_BAT.h"
#include "psx_types.h"

void func_80049A40();

void func_801525B0(void) {
    s32 sp28; /* FAKE (T-17100): dead copy of the column; without it the 0x28 spill of var_a3 and the frame change */
    s32 var_a3;

    D_800E6280.unk_03A = 0;
    D_8015EDBC = func_8013E934(0xB1, 0xB3, (s32) D_800E6280.unk_0D4) - 1;
    if (D_8015EB9C != 0xE) {
        sp28 = 0xD3;
        var_a3 = 0xD3;
        D_8015EE14 = func_8013E934(0xB1, 0xC3, (s32) girl_short_name_table[D_8015EB9C]) - 1;
    } else {
        var_a3 = 0xC3;
    }
    switch (D_8015EB98) {
    case 0x10000:
        D_8015EE40 = func_8013E934(0xB1, var_a3, (s32) "マングース") - 1;
        D_8015EDE8 = func_8013E934(0x13, 0xBB, (s32) "ハブ・ロード") - 1;
        break;
    case 0x20000:
        D_8015EE40 = func_8013E934(0xB1, var_a3, (s32) "虚無僧") - 1;
        D_8015EDE8 = func_8013E934(0x13, 0xBB, (s32) "鹿・グレート") - 1;
        break;
    case 0x40000:
        D_8015EE40 = func_8013E934(0xB1, var_a3, (s32) "マタギ") - 1;
        D_8015EDE8 = func_8013E934(0x13, 0xBB, (s32) "キラーヒグマ") - 1;
        break;
    case 0x100000:
        D_8015EE40 = func_8013E934(0x13, 0xBB, (s32) "番長") - 1;
        D_8015EDE8 = func_8013E934(0x13, 0xBB, (s32) "不良達") - 1;
        break;
    case 0x80000000:
        D_8015EDE8 = func_8013E934(0x13, 0xBB, (s32) "宇宙人") - 1;
        break;
    case 0x8000000:
        D_8015EE14 = func_8013E934(0x17, 0xBB, (s32) "紐緒") - 1;
        D_8015EE40 = func_8013E934(0x17, 0xBB, (s32) "真・世界征服") - 1;
        D_8015EDE8 = func_8013E934(0x17, 0xBB, (s32) "真・世界征服ロボ") - 1;
        break;
    }
    k_disp_start(D_8015EDE8 + 1);
    k_disp_switch(D_8015EDBC, 1);
    k_disp_switch(D_8015EDE8, 1);
    k_disp_switch(D_8015EE40, 0);
    if (D_8015EB98 == 0x08000000) {
        k_disp_switch(D_8015EE14, 1);
        k_disp_switch(D_8015EE40, 0);
        k_disp_switch(D_8015EDE8, 0);
    }
    if (D_8015EB98 == 0x80000000) {
        k_disp_switch(D_8015EDE8, 0);
    }
    if (D_8015EB9C != 0xE) {
        k_disp_switch(D_8015EE14, 1);
    }
    k_sub_reset_point_set();
}

void func_801528DC(void) {
    s32 i;
    u8 *p;

    p = (u8 *)&D_800E6280.unk_110C;
    if (D_800E6280.unk_1100 == 0) {
        D_800E6280.unk_1100 += 1;
        *p = 0x80;
    }
    i = 0;
    do {
        ((DisplayActor44 *)D_8011ECD0)[i + 64].unk_07 = *p;
        D_800E6280.unk_03A = *p;
        if (D_8015EB98 == 0x20000 && *p >= 0x61) {
            D_80120E93 = 0x60;
        }
        i++;
    } while (D_80120650 != &D_8011ECD0[i * 0x44]);
    *p -= 2;
    if (*p == 0) {
        func_8004E58C();
        D_800E6280.unk_03A = 0x80;
        func_80048EB8(0);
        func_8006BC28(0);
        func_800438F0(1);
        func_80042808();
    }
}

void func_801529B4(void) {
    func_8014B79C();
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8004500C(1, 0x200);
        switch (D_8015EB98) {
        case 0x10000:
            func_800AE0F0(D_8015E9E0, "ハブ・ロードが襲ってきた！");
            break;
        case 0x20000:
            func_800AE0F0(D_8015E9E0, "鹿・グレートが襲ってきた！");
            break;
        case 0x40000:
            func_800AE0F0(D_8015E9E0, "キラーヒグマが襲ってきた！");
            break;
        case 0x100000:
            func_800AE0F0(D_8015E9E0, "不良達が襲ってきた！");
            break;
        }
        if (D_8015EB98 != 0x08000000) {
            func_8014B738((s32) D_8015EB70, 1, 0x5A);
        }
        func_8013E810(0x30, 1, 1, 1);
        switch (D_8015EB98) {
        case 0x10000:
        case 0x20000:
        case 0x40000:
        case 0x08000000:
            func_8013E810(0x3B, 0, 1, 1);
            break;
        case 0x100000:
            func_8013E810(0x3B, 0, 1, 1);
            func_8013E810(0x3C, 0, 1, 1);
            func_8013E810(0x3D, 0, 1, 1);
            break;
        }
        if (D_8015EB9C != 0xE) {
            switch (D_8015EB9C) {
            case 0:
                func_8013E810(0x1D, 2, 1, 1);
                break;
            case 1:
                func_8013E810(0x1D, 2, 1, 1);
                break;
            case 2:
                func_8013E810(0x1D, 9, 1, 1);
                break;
            case 3:
                func_8013E810(0x1D, 2, 1, 1);
                break;
            case 4:
                func_8013E810(0x1D, 1, 1, 1);
                break;
            case 5:
                func_8013E810(0x1D, 0, 1, 1);
                break;
            case 6:
                func_8013E810(0x1D, 1, 1, 1);
                break;
            case 7:
                func_8013E810(0x1D, 1, 1, 1);
                break;
            case 8:
                func_8013E810(0x1D, 1, 1, 1);
                break;
            case 9:
                func_8013E810(0x1D, 1, 1, 1);
                break;
            case 10:
                func_8013E810(0x1D, 1, 1, 1);
                break;
            case 13:
                func_8013E810(0x1D, 1, 1, 1);
                break;
            }
        }
        func_8013E97C(0x30, 0x160, 0x58);
        func_8013E97C(0x1D, 0x168, 0x88);
        func_8013E97C(0x3B, -0x58, 0x60);
        if (D_8015EB98 == 0x100000) {
            func_8013E97C(0x3B, -0x28, 0x68);
            func_8013E97C(0x3C, -0x60, 0x58);
            func_8013E97C(0x3D, -0x68, 0x88);
        }
        if (D_8015EB98 == 0x08000000) {
            func_8013E97C(0x3B, -0x58, 0x70);
        }
        func_8004284C();
        break;
    case 1:
        func_8013E97C(0x30, 0x160 - D_800E6280.unk_1104.u, 0x58);
        func_8013E97C(0x1D, 0x168 - D_800E6280.unk_1104.u, 0x88);
        func_8013E97C(0x3B, (D_800E6280.unk_1104.u * 2) - 0x58, 0x60);
        if (D_8015EB98 == 0x100000) {
            func_8013E97C(0x3B, (D_800E6280.unk_1104.u * 2) - 0x28, 0x68);
            func_8013E97C(0x3C, (D_800E6280.unk_1104.u * 2) - 0x60, 0x58);
            func_8013E97C(0x3D, (D_800E6280.unk_1104.u * 2) - 0x68, 0x88);
        }
        if (D_8015EB98 == 0x08000000) {
            func_8013E97C(0x3B, (D_800E6280.unk_1104.u * 2) - 0x58, 0x70);
        }
        D_800E6280.unk_1104.u += 4;
        if (D_800E6280.unk_1104.u >= 0x51) {
            func_8013E97C(0x30, 0, 0);
            func_8013E7C0(0x30, 0, 1, 1);
            func_8013E97C(0x1D, 0, 0);
            if (D_8015EB9C != 0xE) {
                func_8013E7C0(0x1D, 0, 1, 1);
            }
            func_8004284C();
        }
        break;
    case 2:
        D_800E6280.unk_1104.u += 1;
        if (D_800E6280.unk_1104.u >= 0x3D) {
            func_80042808();
        }
        break;
    }
}

/* FAKE: x and y are unused locals that place color at sp+0x3C in the 0x48 frame; *(u8 *)&D_800E6280.unk_110A bypasses cvt_pass.py
 * widening so the four loads of unk_110A stay in ugen temporaries ($t7..$t9); in case 3, the *(s32 *)& reads of D_8015EC38/3C/40/44/4C
 * keep D_8015EC38 and D_8015EC3C promoted to $a1/$a2 across the function and allocate $v0, $v1, $a3 to D_8015EC44, D_8015EC40,
 * D_8015EC4C so uopt flushes the six stores in the original order. T-24124 */
void func_80152F30(void) {
    s32 x;
    s32 y;
    RpgColorWord color;
    u32 selector;

    selector = func_8013E9A8(8);
    switch (selector) {
    case 0:
        color.word = 0x9F9F9F;
        break;
    case 1:
        color.word = 0x6F6F6F;
        break;
    case 2:
        color.word = 0xBF0000;
        break;
    case 3:
        color.word = 0xBF00;
        break;
    case 4:
        color.word = 0xBF;
        break;
    case 5:
        color.word = 0xAFAF00;
        break;
    case 6:
        color.word = 0xAF00AF;
        break;
    case 7:
        color.word = 0xAFAF;
        break;
    }
    func_8014B79C();
    switch (*(u8 *)&D_800E6280.unk_110A) {
    case 0:
        D_800E6280.unk_F5F = 0xE;
        func_80046290(0x1C0000A8, 0x0F0000B7, 0xE);
        func_80044750(0x300);
        func_8004284C();
        break;
    case 1:
        func_80044750(0xE4);
        func_8004284C();
        break;
    case 2:
        if (func_800460EC() & 4) {
            func_8004284C();
        }
        break;
    case 3:
        func_8013E810(0x30, 1, 1, 1);
        func_8013E810(0x1D, 0x17, 1, 1);
        func_8013E97C(0x30, 0x160, 0x58);
        func_8013E97C(0x1D, 0x168, 0x88);
        D_8015EC48 = 0;
        D_8015EC44 = 0;
        D_8015EC40 = 0;
        D_8015EC4C = 0;
        D_8015EC44 = *(s32 *)&D_8015EC44;
        D_8015EC40 = *(s32 *)&D_8015EC40;
        D_8015EC4C = *(s32 *)&D_8015EC4C;
        D_8015EC38 = 0;
        D_8015EC3C = -0x1E;
        func_8013E97C(0x2F, *(s32 *)&D_8015EC38, *(s32 *)&D_8015EC3C);
        func_8013E810(0x2F, 2, 1, 1);
        func_8013E97C(0x2E, D_8015EC38, D_8015EC3C);
        func_8013E810(0x2E, 6, 1, 1);
        func_8004284C();
        break;
    case 4:
        func_8013E97C(0x30, 0x160 - D_800E6280.unk_1104.u, 0x58);
        func_8013E97C(0x1D, 0x168 - D_800E6280.unk_1104.u, 0x88);
        D_800E6280.unk_1104.u += 4;
        if (D_800E6280.unk_1104.u >= 0x51U) {
            func_8013E97C(0x30, 0, 0);
            func_8013E7C0(0x30, 0, 1, 1);
            func_8013E97C(0x1D, 0, 0);
            func_8013E7C0(0x1D, 0x18, 1, 1);
            func_8013E97C(0x38, 0, 0);
            func_8013E7C0(0x38, 9, 1, 1);
            ((DisplayActor44 *)D_8011ECD0)[152].unk_01 = 2;
            func_8004284C();
        }
        break;
    case 5:
        func_80049A40(-0x5C - (D_800E6280.unk_1104.u >> 1), -0x78, (D_800E6280.unk_1104.u >> 1) * 2, 0xA8, 3, color.word, 0x81);
        func_800494BC(0, 0, 3, 1, 0);
        D_800E6280.unk_1104.u += 1;
        if (D_800E6280.unk_1104.u >= 0x41U) {
            func_8004284C();
        }
        break;
    case 6:
        func_80049A40(-0x7C, -0x78, 0x40, 0xA8, 3, color.word, 0x81);
        func_800494BC(0, 0, 3, 1, 0);
        func_8013E97C(0x3B, 0x48, D_800E6280.unk_1104.u - 0x80);
        func_8013E810(0x3B, 8, 1, 1);
        D_800E6280.unk_1104.u += 1;
        if (D_800E6280.unk_1104.u >= 0xE9U) {
            k_disp_switch(D_8015EDE8, 1);
            func_8004284C();
        }
        break;
    case 7:
        func_80049A40(-0x7C, -0x78, 0x40, 0xA8, 3, color.word, 0x81);
        D_800E6280.unk_1104.u += 1;
        func_800494BC(0, 0, 3, 1, 0);
        if (D_800E6280.unk_1104.u >= 0x79U) {
            func_8004284C();
        }
        break;
    case 8:
        func_80049A40(D_800E6280.unk_1104.u - 0x7C, -0x78, 0x40 - (D_800E6280.unk_1104.u * 2), 0xA8, 3, color.word, 0x81);
        func_800494BC(0, 0, 3, 1, 0);
        D_800E6280.unk_1104.u += 1;
        if (D_800E6280.unk_1104.u >= 0x21U) {
            func_8004284C();
        }
        break;
    case 9:
        D_8015EC38 = 0x40;
        D_8015EC3C = D_800E6280.unk_1104.u - 0x40;
        D_800E6280.unk_1104.u += 1;
        if (D_800E6280.unk_1104.u >= 0x59U) {
            func_8004284C();
        }
        break;
    case 10:
        func_800AE0F0(D_8015E9E0, "宇宙人が襲ってきた！");
        func_8014B738((s32)D_8015EB70, 1, 0x5A);
        func_8013E7C0(0x38, 0xFF, 1, 0);
        func_8013E97C(0x3B, 0, 0);
        func_8013E7C0(0x3B, 0, 1, 1);
        func_8004284C();
        break;
    case 11:
        if (D_8015EE48 == 0) {
            func_8004284C();
        }
        break;
    case 12:
        func_8013E97C(0x1E, 0, 0);
        func_8013E7C0(0x1E, 0, 5, 1);
        func_8004284C();
        break;
    case 13:
        func_801535E4();
        break;
    case 14:
        D_8015EDF0 = 0x10001000;
        func_80042808();
        break;
    }
    if (*(u8 *)&D_800E6280.unk_110A >= 9) {
        func_8015785C();
    }
    if ((*(u8 *)&D_800E6280.unk_110A >= 5) && (*(u8 *)&D_800E6280.unk_110A < 8)) {
        func_8013F15C(0x505, 0x6E, 7);
    }
}

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 func_801535E4(void) {
    if (D_800E6280.unk_F88 & 0x40) {
        func_80044750(0x74);
        if (D_8015EE44.unk_00 >= 3 && D_8015EE44.unk_08 >= 3) {
            D_8015EE44.unk_08 = D_8015E744 + 1;
        }
    }
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E9E0, "紐緒「私が準備するまで、");
        func_800AE0F0(D_8015EA08, "持ちこたえなさい。");
        func_8013E7C0(0x1D, 0x19, 1, 1);
        func_8014B738(D_8015EB70, 2, 0x96);
        D_8015ED8C[0] += 1;
        return;
    case 1:
        func_8013F1C4(0x2E, 0x07000033, 0);
        D_8015ED8C[0] += 1;
        return;
    case 2:
        func_8013F250(1);
        return;
    case 3:
        func_8013F1C4(0x34, 0x08000037, 1);
        D_8015ED8C[0] += 1;
        return;
    case 4:
        func_8013F250(0);
        return;
    case 5:
        func_8013E7C0(0x1D, 0x18, 1, 1);
        func_8013F220();
        func_8004284C();
        return;
    }
}

s32 func_80153784(void) {
    s32 pad[3]; /* FAKE: unused words above the RECTs, the original frame has them (real source unknown). T-17112 */
    RECT r0;
    RECT r1;
    RECT r2;
    RECT r3;
    s32 i;

    /* FAKE: x/y and w/h of each RECT share a source line; as1 then stores y, x, h, w as the original does. T-17112 */
    switch (D_800E6280.unk_110A) {
    case 0:
        r0.x = 0x240; r0.y = 0x100;
        r0.w = 0x40; r0.h = 0x28;
        func_8009C93C(&r0, 0x180, 0x180);
        r1.x = 0x240; r1.y = 0x128;
        r1.w = 0x40; r1.h = 0x28;
        func_8009C93C(&r1, 0x1C0, 0x180);
        r2.x = 0x220; r2.y = 0x100;
        r2.w = 0x40; r2.h = 0x28;
        func_8009C93C(&r2, 0x200, 0x180);
        r3.x = 0x21F; r3.y = 0x100;
        r3.w = 1; r3.h = 0x80;
        func_8009C93C(&r3, 0x220, 0x100);
        D_8015EC40 = 0x20;
        D_8015EC44 = 0x40;
        D_8015EC48 = 1;
        D_8015ED2C = -0x80;
        D_8015ED30 = 0x20;
        D_8015ED34 = 0x40;
        D_8015ED38 = 1;
        D_8015EC38 = 0;
        D_8015EC3C = 0x80;
        /* FAKE: D_8015ED28 through the D_8015EC38 symbol as in case 1; under its own name uopt moves this store above the D_8015EC38 store. T-17112 */
        (&D_8015EC38)[60] = 0;
        func_8004284C();
        break;
    case 1:
        /* FAKE: D_8015EC40 and D_8015ED28..D_8015ED38 (+0x08, +0xF0..+0x100) are reached through the D_8015EC38 symbol; the original
         * relocations probably name one aggregate over these words that is not recovered yet. as1 keeps a load and a store of one
         * symbol in source order: the D_8015ED28/D_8015ED38 loads stay above the D_8015EC38 store, the D_8015EC40 and D_8015ED30
         * loads below the stores, and the D_8015ED30 lui fills the load delay of D_8015EC40. T-17112 */
        D_8015EC38 += D_8015EC48;
        (&D_8015EC38)[60] += (&D_8015EC38)[64];
        if (D_8015EC38 >= (&D_8015EC38)[2]) {
            D_8015EC38 = 0;
        }
        if ((&D_8015EC38)[60] >= (&D_8015EC38)[62]) {
            (&D_8015EC38)[60] = 0;
        }
        func_80153B08();
        func_80153D20();
        func_80153A60();
        D_8015ED2C += 1;
        D_8015EC3C -= 1;
        if (D_8015EC3C == 0) {
            func_8004284C();
        }
        break;
    case 2:
        r2.x = 0x200; r2.y = 0x180;
        r2.w = 1; r2.h = 0x28;
        func_8009C93C(&r2, 0x220, 0x100);
        func_8013E7C0(0x1F, 0, 1, 1);
        func_8004284C();
        break;
    case 3:
        D_8015EC74 = 0;
        /* FAKE: the 48 words from D_8015EC7C through the first symbol, with D_8015EC78 on the loop's line to keep its store behind the loop setup. T-17112 */
        D_8015EC78 = 0; for (i = 0; i < 48; i++) (&D_8015EC7C)[i] = 0;
        func_80042808();
        break;
    }
}

void func_80153A60(void) {
    int dead; /* FAKE: dead copy of *p; only reserves $v0 so the table pointer lands in $v1. Found with decomp-permuter. T-10910 */
    s32 i;
    s32 *p;

    i = 0;
    p = &D_8015EC74;
    do {
        ((DisplayActor44 *)D_8011ECD0)[i + 64].unk_07 = *p;
        *(s8 *)&D_800E6280.unk_3A = *p;
        if (D_8015EB98 == 0x20000 && (dead = *p) >= 0x61) {
            D_80120E93 = 0x60;
        }
        if (0) { /* FAKE: empty dead branch steers the allocation. T-10910 */
        }
        i++;
    } while (D_80120650 != &D_8011ECD0[i * 0x44]);
    func_8013E7C0(0x1F, 0, 1, 0);
    D_8015EC74 += 2;
    if (*p > 0x80) {
        D_8015EC74 = 0x80;
    }
}

void func_80153B08(void) {
    s32 temp_s1;
    s32 temp_s3;
    s32 var_s0;

    var_s0 = 0;
    do {
        temp_s1 = var_s0 + 4;
        temp_s3 = (s32) (func_800A0070(((s32) ((var_s0 % (s32) D_8015EC44) << 0xC) / (s32) D_8015EC44) + ((s32) (D_8015EC38 << 0xC) / (s32) D_8015EC40)) * D_8015EC3C) >> 0xD;
        func_80153F3C(temp_s3, (s32) (func_800A0070(((s32) ((temp_s1 % (s32) D_8015EC44) << 0xC) / (s32) D_8015EC44) + ((s32) (D_8015EC38 << 0xC) / (s32) D_8015EC40)) * D_8015EC3C) >> 0xD, var_s0, temp_s1, 0, 5);
        var_s0 += 5;
    } while (var_s0 != 0xAA);
}

void func_80153D20(void) {
    s32 temp_s1;
    s32 temp_s3;
    s32 var_s0;

    var_s0 = 0;
    do {
        temp_s1 = var_s0 + 4;
        temp_s3 = (s32) (func_800A0070(((s32) ((var_s0 % (s32) D_8015ED34) << 0xC) / (s32) D_8015ED34) + ((s32) (D_8015ED28 << 0xC) / (s32) D_8015ED30)) * D_8015ED2C) >> 0xD;
        func_80153F3C(temp_s3, (s32) (func_800A0070(((s32) ((temp_s1 % (s32) D_8015ED34) << 0xC) / (s32) D_8015ED34) + ((s32) (D_8015ED28 << 0xC) / (s32) D_8015ED30)) * D_8015ED2C) >> 0xD, var_s0, temp_s1, 2, 5);
        var_s0 += 5;
    } while (var_s0 != 0xAA);
}

/* FAKE: the two x stores of each point pair share a source line; as1 schedules the stores by source line and only then reproduces the original order (T-17112). */
void func_80153F3C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    s32 xy[8];
    s32 uv[8];

    xy[0] = arg0; xy[2] = arg0 + 0x7F;
    xy[1] = arg2;
    xy[3] = arg2;
    xy[4] = arg1; xy[6] = arg1 + 0x7F;
    xy[5] = arg3;
    xy[7] = arg3;
    uv[0] = 0; uv[2] = 0x7F;
    uv[1] = arg2;
    uv[3] = arg2;
    uv[4] = 0; uv[6] = 0x7F;
    uv[5] = arg3;
    uv[7] = arg3;
    func_80154220(xy, uv, (D_80120E93 + arg4) >> 1, arg5, 1);
    xy[0] = arg0 + 0x80; xy[2] = arg0 + 0xFF;
    xy[1] = arg2;
    xy[3] = arg2;
    xy[4] = arg1 + 0x80; xy[6] = arg1 + 0xFF;
    xy[5] = arg3;
    xy[7] = arg3;
    uv[0] = 0; uv[2] = 0x7F;
    uv[1] = arg2;
    uv[3] = arg2;
    uv[4] = 0; uv[6] = 0x7F;
    uv[5] = arg3;
    uv[7] = arg3;
    func_80154220(xy, uv, (D_80120E93 + arg4) >> 1, arg5, 2);
    xy[0] = arg0 + 0x100; xy[2] = arg0 + 0x13F;
    xy[1] = arg2;
    xy[3] = arg2;
    xy[4] = arg1 + 0x100; xy[6] = arg1 + 0x13F;
    xy[5] = arg3;
    xy[7] = arg3;
    uv[0] = 0; uv[2] = 0x3F;
    uv[1] = arg2;
    uv[3] = arg2;
    uv[4] = 0; uv[6] = 0x3F;
    uv[5] = arg3;
    uv[7] = arg3;
    func_80154220(xy, uv, (D_80120E93 + arg4) >> 1, arg5, 3);
    xy[0] = arg0; xy[2] = arg0 - 0x7F;
    xy[1] = arg2;
    xy[3] = arg2;
    xy[4] = arg1; xy[6] = arg1 - 0x7F;
    xy[5] = arg3;
    xy[7] = arg3;
    uv[0] = 0; uv[2] = 0x7F;
    uv[1] = arg2;
    uv[3] = arg2;
    uv[4] = 0; uv[6] = 0x7F;
    uv[5] = arg3;
    uv[7] = arg3;
    func_80154220(xy, uv, (D_80120E93 + arg4) >> 1, arg5, 1);
    xy[0] = arg0 + 0x17F; xy[2] = arg0 + 0x13F;
    xy[1] = arg2;
    xy[3] = arg2;
    xy[4] = arg1 + 0x17F; xy[6] = arg1 + 0x13F;
    xy[5] = arg3;
    xy[7] = arg3;
    uv[0] = 0; uv[2] = 0x3F;
    uv[1] = arg2;
    uv[3] = arg2;
    uv[4] = 0; uv[6] = 0x3F;
    uv[5] = arg3;
    uv[7] = arg3;
    func_80154220(xy, uv, (D_80120E93 + arg4) >> 1, arg5, 3);
}

void func_80154220(s32 *arg0, s32 *arg1, s32 arg2, s32 arg3, s32 arg4) {
    POLY_FT4 *p;
    s32 y;

    p = (POLY_FT4 *)GetWorkBase(0x28, D_8011ECA0);
    func_8009F040(p);
    SetShadeTex(p, 0);
    if (arg3 != 0) {
        SetSemiTrans(p, 1);
    } else {
        SetSemiTrans(p, 0);
    }
    p->r0 = arg2;
    p->g0 = arg2;
    p->b0 = arg2;
    if (arg3 != 0) {
        p->clut = func_8009ED74(0, 0x1F7);
    } else {
        p->clut = func_8009ED74(0, 0x1F5);
    }
    p->tpage = func_8009ECB0(1, arg3 + 4, (arg4 << 6) + 0x140, 0x100);
    y = -D_8011ECA0 * 0xF0 + 0xF0;
    p->x0 = arg0[0];
    p->y0 = arg0[1] + y;
    p->x1 = arg0[2] + 1;
    p->y1 = arg0[3] + y;
    p->x2 = arg0[4];
    p->y2 = arg0[5] + y + 1;
    p->x3 = arg0[6] + 1;
    p->y3 = arg0[7] + y + 1;
    p->u0 = arg1[0];
    p->v0 = arg1[1];
    p->u1 = arg1[2] + 1;
    p->v1 = arg1[3];
    p->u2 = arg1[4];
    p->v2 = arg1[5] + 1;
    p->u3 = arg1[6] + 1;
    p->v3 = arg1[7] + 1;
    AddPrim((D_8011ECA0 << 0xA) + 0x20 + D_800E8CA0, (s32)p);
}
