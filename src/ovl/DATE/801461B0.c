#include "common.h"
#include "ovl/DATE.h"

void func_801461B0(void) {
    D_8015CF80 = 0x801C6000;
    D_8015CF84 = 0x801AE000;
    D_8015CF88 = 0x801B2000;
    D_8015CF8C = 0x801B6000;
    D_8015CF90 = 0x801BA000;
    D_8015CF94 = 0x801BE000;
    D_8015CF98 = 0x801C2000;
}

void func_80146224(void) {
    /* T-10040: this dispatcher reads the signed-declared byte with lbu. */
    switch (*(u8 *)&D_8015B68C) {
    case 0:
        func_80147958();
        return;
    case 1:
        func_801489D0();
        return;
    case 2:
        func_8014718C();
        return;
    case 3:
        func_801462B0();
        return;
    case 4:
        func_8014A1E4();
        /* fallthrough */
    default:
        return;
    }
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/801461B0", func_801462B0);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/801461B0", func_8014718C);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/801461B0", func_80147958);

s32 func_80148924(s16 a0) {
    D_801217D0[4].unk_06 += a0;
    D_801217D0[5].unk_06 += a0;
    D_801217D0[4].unk_0A -= a0;
    D_801217D0[5].unk_0A -= a0;
    D_801217D0[6].unk_0A += a0;
    D_801217D0[7].unk_0A += a0;
    D_801217D0[6].unk_0F -= a0;
    D_801217D0[7].unk_0F -= a0;
}

/* NON_MATCHING: T-10040, 13-line initialization/data-ownership residual. */
#ifdef NON_MATCHING
s32 func_801489D0(void) {
    s16 *temp_a0;
    s16 *temp_v0;
    s16 temp_a1;
    s16 temp_v0_2;
    s16 var_s3;
    s32 remainder;
    u32 temp_s1;
    u32 temp_s1_3;
    u8 temp_s0;
    u8 temp_s0_2;

    switch (D_800E6280.unk_1104.u) {
    case 0:
        D_80122D2C = 1;
        D_8015CFC4 = 0x64;
        func_800AE120((*(s32 *)&D_800E6280.unk_10F8));
        D_8015CF9C = 0;
        hizuke_disp_switch(0);
        func_801491E4(0x43BA);
        D_8015CFA4[0] = -0x50;
        D_8015CFA4[1] = 0;
        D_8015CFA4[2] = 0x64;
        D_8015CFAC[0] = -0x28;
        D_8015CFAC[1] = 0;
        D_8015CFAC[2] = -0x3C;
        D_800E6280.unk_1104.u += 1;
        D_8015CFB4[0] = (func_800AE0D0() % 3) - 1;
        D_8015CFB4[1] = (func_800AE0D0() % 3) - 1;
        D_8015CFB4[2] = (func_800AE0D0() % 3) - 1;
        D_8015CFBC[0] = (func_800AE0D0() % 3) - 1;
        D_8015CFBC[1] = (func_800AE0D0() % 3) - 1;
        D_8015CFBC[2] = (func_800AE0D0() % 3) - 1;
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            D_800E6280.unk_1104.u += 1;
            break;
        }
        break;
    case 2:
        if (((((u32 *)D_80125C04)[0] & 0xFFFF0000) != 0x38000000) || ((((u32 *)D_80125C04)[1] & 0xFF010000) != 0x10000)) {
            func_800AE0B0("DEC_BS header Error \n");
            ((RECT *)D_800B5920)[0].w = 0;
            ((RECT *)D_800B5920)[0].h = 0;
            D_800E6280.unk_1104.u -= 2;
            return 0;
        }
        func_8005751C(1);
        dec_bg_show_switch(1);
        func_80044750(0x203);
        D_800E6280.unk_1104.u += 1;
        break;
    case 3:
        temp_s0 = D_800E6280.unk_110A;
        temp_s1 = D_800E6280.unk_1104.u;
        normal_date_bg_fadein();
        D_800E6280.unk_1104.u = temp_s1;
        if (temp_s0 != D_800E6280.unk_110A) {
            D_800E6280.unk_1104.u = temp_s1 + 1;
        }
        D_800E6280.unk_110A = temp_s0;
        break;
    case 4:
        var_s3 = 0;
        do {
            if (((u32) (*(s32 *)&D_800E6280.unk_10F8) % 30U) == (func_800AE0D0() % 30)) {
                (D_8015CFB4)[var_s3] = (func_800AE0D0() % 3) - 1;
            }

            if (((u32) (*(s32 *)&D_800E6280.unk_10F8) % 30U) == (func_800AE0D0() % 30)) {
                (D_8015CFBC)[var_s3] = (func_800AE0D0() % 3) - 1;
            }

            if ((D_8015CFB4[var_s3] == 0) && (D_8015CFBC[var_s3] == 0)) {
                if ((remainder = func_800AE0D0() % 2)) {
                    D_8015CFB4[var_s3] = ((func_800AE0D0() % 2) * 2) - 1;
                } else {
                    D_8015CFBC[var_s3] = ((func_800AE0D0() % 2) * 2) - 1;
                }
            }
            temp_v0 = &(D_8015CFA4)[var_s3];
            *temp_v0 += D_8015CFB4[var_s3];
            temp_a0 = &(D_8015CFAC)[var_s3];
            *temp_a0 += D_8015CFBC[var_s3];
            temp_a1 = *temp_v0;
            if (temp_a1 < -0x95) {
                D_8015CFB4[var_s3] = 1;
            }
            if (temp_a1 >= 0x96) {
                D_8015CFB4[var_s3] = -1;
            }
            temp_v0_2 = *temp_a0;
            if (temp_v0_2 < -0x6D) {
                D_8015CFBC[var_s3] = 1;
            }
            if (temp_v0_2 >= 0x1E) {
                D_8015CFBC[var_s3] = -1;
            }
            var_s3++;
        } while (var_s3 < 3);
        func_8004A814((s32) D_8015CFA4[0], (s32) D_8015CFAC[0], 0xA, 2, 2, 0x808080, 3, 1);
        func_8004A814((s32) D_8015CFA4[1], (s32) D_8015CFAC[1], 0xA, 2, 2, 0x804040, 3, 1);
        func_8004A814((s32) D_8015CFA4[2], (s32) D_8015CFAC[2], 0xA, 2, 2, 0x800000, 3, 1);
        if (((u32) (*(s32 *)&D_800E6280.unk_10F8) % 50U) == 0) {
            if (!(func_800AE0D0() & 3)) {
                func_80044750(0x504);
            } else if ((func_800AE0D0() % 4) == 1) {
                func_80044750(0x505);
            } else if ((func_800AE0D0() % 4) == 2) {
                func_80044750(0x506);
            } else {
                func_80044750(0x507);
                D_8015CFC4 = 0;
            }
        }
        if (D_8015CFC4++ == 0x14) {
            func_80044750(0x507);
        }
        if (D_8015CFC4 >= 0x65) {
            D_8015CFC4 = 0x64;
        }
        if (D_8015CF9C++ == 0x12C) {
            D_800E6280.unk_1104.u += 1;
            break;
        }
        break;
    case 5:
        temp_s0_2 = D_800E6280.unk_110A;
        temp_s1_3 = D_800E6280.unk_1104.u;
        normal_date_bg_fadeout();
        D_800E6280.unk_1104.u = temp_s1_3;
        if (temp_s0_2 != D_800E6280.unk_110A) {
            D_800E6280.unk_1104.u = temp_s1_3 + 1;
        }
        D_800E6280.unk_110A = temp_s0_2;
        break;
    case 6:
        dec_bg_show_switch(1);
        D_800E6280.unk_1104.u += 1;
        break;
    default:
        func_80044750(0x200);
        D_80122D2C = 0;
        hizuke_disp_switch(1);
        func_8004284C();
        break;
    }
}
#else
INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/801461B0", func_801489D0);
#endif

void func_801491E4(s32 arg0) {
    s32 temp_v0;

    D_800B3D60 = 0;
    temp_v0 = func_8005742C(arg0, 1);
    if (temp_v0 == D_800B5939) {
        func_800AE0B0("same\n");
        D_800E6280.unk_1104.w += 3;
        return;
    }
    if (temp_v0 == (1 - D_800B5939)) {
        func_800AE0B0("another\n");
        D_800E6280.unk_1104.w += 2;
    }
}

/* FAKE: s32 fall-through keeps the record base in v0 (T-9780). */
s32 func_80149268(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    Rec24 *record;
    s16 i;

    func_80084E90(arg0, arg1, arg2, arg3, arg4, arg5);
    /* FAKE: shared setup/loop line preserves instruction scheduling (T-9780). */
    i = 0; do {
        record = &D_801217D0[i];
        record[4] = record[58];
        D_800E6280.unk_10A5[i + 4] = D_800E6280.unk_10A5[i + 58];
        i++;
    } while (i < 6);
}
