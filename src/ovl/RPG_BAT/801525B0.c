#include "common.h"
#include "ovl/RPG_BAT.h"

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
        D_8011ECD0[i * 0x44 + 0x1107] = *p;
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

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801525B0", func_80152F30);

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

/* NON_MATCHING: T-17112, 21 differing lines (8 are branch offsets); everything else matches. Case 1: the original loads D_8015ED28/D_8015ED38 before the D_8015EC38 store and D_8015ED30 right before the compare (an extra nop); case 3: its sw to D_8015EC78 comes after the loop pointer setup. The (&D_8015EC38)[2] read keeps the D_8015EC40 load behind the store as in the original. */
#ifdef NON_MATCHING
s32 func_80153784(void) {
    s32 pad[3]; /* FAKE: unused words above the RECTs, the original frame has them (real source unknown). T-17112 */
    RECT r0;
    RECT r1;
    RECT r2;
    RECT r3;
    s32 i;

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
        D_8015ED28 = 0;
        func_8004284C();
        break;
    case 1:
        D_8015EC38 += D_8015EC48;
        D_8015ED28 += D_8015ED38;
        if (D_8015EC38 >= (&D_8015EC38)[2]) {
            D_8015EC38 = 0;
        }
        if (D_8015ED28 >= D_8015ED30) {
            D_8015ED28 = 0;
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
        D_8015EC78 = 0;
        for (i = 0; i < 48; i++) (&D_8015EC7C)[i] = 0;
        func_80042808();
        break;
    }
}
#else
INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801525B0", func_80153784);
#endif

void func_80153A60(void) {
    int dead; /* FAKE: dead copy of *p; only reserves $v0 so the table pointer lands in $v1. Found with decomp-permuter. T-10910 */
    s32 i;
    s32 *p;

    i = 0;
    p = &D_8015EC74;
    do {
        D_8011ECD0[i * 0x44 + 0x1107] = *p;
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
    u8 *p;
    s32 y;

    p = (u8 *)GetWorkBase(0x28, D_8011ECA0);
    func_8009F040(p);
    SetShadeTex(p, 0);
    if (arg3 != 0) {
        SetSemiTrans(p, 1);
    } else {
        SetSemiTrans(p, 0);
    }
    p[4] = arg2;
    p[5] = arg2;
    p[6] = arg2;
    if (arg3 != 0) {
        *(s16 *)(p + 0xE) = func_8009ED74(0, 0x1F7);
    } else {
        *(s16 *)(p + 0xE) = func_8009ED74(0, 0x1F5);
    }
    *(s16 *)(p + 0x16) = func_8009ECB0(1, arg3 + 4, (arg4 << 6) + 0x140, 0x100);
    y = -D_8011ECA0 * 0xF0 + 0xF0;
    *(s16 *)(p + 8) = arg0[0];
    *(s16 *)(p + 0xA) = arg0[1] + y;
    *(s16 *)(p + 0x10) = arg0[2] + 1;
    *(s16 *)(p + 0x12) = arg0[3] + y;
    *(s16 *)(p + 0x18) = arg0[4];
    *(s16 *)(p + 0x1A) = arg0[5] + y + 1;
    *(s16 *)(p + 0x20) = arg0[6] + 1;
    *(s16 *)(p + 0x22) = arg0[7] + y + 1;
    p[0xC] = arg1[0];
    p[0xD] = arg1[1];
    p[0x14] = arg1[2] + 1;
    p[0x15] = arg1[3];
    p[0x1C] = arg1[4];
    p[0x1D] = arg1[5] + 1;
    p[0x24] = arg1[6] + 1;
    p[0x25] = arg1[7] + 1;
    AddPrim((D_8011ECA0 << 0xA) + 0x20 + D_800E8CA0, (s32)p);
}
