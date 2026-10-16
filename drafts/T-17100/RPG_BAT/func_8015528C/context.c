#include "common.h"
#include "ovl/RPG_BAT.h"

void func_80154430(void) {
    s16 *state;

    switch (D_8015EDAC) {
    case 0:
        func_800AE0F0(D_8015E74C, "　　　　　　　色素分解　　　　　　　");
        func_8014B738((s32) &D_8015E814, 1, 0x5A);
        func_80144480();
        func_8013EEAC(D_8015EBAC);
        func_80148F70();
        func_8014EBA8();
        func_8014EBA8();
        return;
    case 2:
        func_8013D418();
        return;
    case 3:
        func_8013E7C0(0x34, 3, 5, 1);
        func_8014EBA8();
        return;
    case 4:
        state = &D_80121438;
        switch (*state) {
        case 1:
            func_8013F0F4(0x509, 0, 2);
            break;
        case 3:
            func_8013F0F4(0x509, 0, 3);
            break;
        case 5:
            func_8013F0F4(0x509, 0, 4);
            break;
        case 7:
            func_8013F0F4(0x509, 0, 5);
            break;
        }
        func_8014EDBC(0x34);
        return;
    case 5:
        func_8013E7C0(0x34, 4, 1, 1);
        func_8014EBA8();
        return;
    case 6:
        func_80154DD8();
        return;
    case 7:
        func_80154760();
        return;
    case 8:
        func_8015461C();
        return;
    case 9:
        func_8014ED44(0x14);
        return;
    case 10:
        func_8013D59C();
        return;
    case 11:
        func_8014EBEC();

    default:
        return;
    }
}

void func_8015461C(void) {
    s32 i;

    switch (D_8015EDB0.unk_00) {
    case 0:
        func_8013EA30(255);
        func_8014EBD0();
        return;
    case 1:
        func_8014ED80(5);
        return;
    case 2:
        func_8013EA30(128);
        for (i = 0; i < 6; i++) {
            ((RpgFlagRecord44 *)D_8011ECD0)[i + 136].unk_03 ^= 128;
        }
        if ((D_8015EDC4 & 0x100000) && D_8015EBB0 == 1000) {
            D_8015EBC0 = 0;
        } else {
            ((RpgFlagRecord44 *)D_8011ECD0)[D_8015EC70 + 96].unk_03 ^= 128;
            func_8013D3E4();
        }
        func_8014EBA8();
        return;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80154430", func_80154760);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80154430", func_80154DD8);

void func_8015528C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    s32 *temp_fp;
    s32 *temp_s7;
    s32 temp_a1;
    s32 temp_a2;
    s32 temp_lo;
    s32 temp_lo_2;
    s32 temp_t6;
    s32 temp_v0;
    s32 var_a0;
    s32 var_a3;
    s32 var_s1;
    s32 var_v1;

    temp_fp = &(&D_8015EBAC)[arg6];
    temp_v0 = *temp_fp;
    var_s1 = 0;
    if (arg3 < (temp_v0 / 10)) {
        temp_s7 = &(&D_8015EBAC)[arg7];
        *temp_fp = temp_v0 - (arg3 * 0xA);
        temp_t6 = *temp_s7 - 1;
        *temp_s7 = temp_t6;
        if (temp_t6 < 0) {
            *temp_s7 = 2;
        }
    }
    do {
        temp_lo = (s32) *temp_fp / 10;
        temp_a2 = (&D_8015EBAC)[arg7] + var_s1;
        temp_a1 = temp_lo + (var_s1 * arg3);
        temp_lo_2 = (var_s1 - 1) * arg3;
        if (temp_a1 < arg0) {
            var_a0 = temp_lo + temp_lo_2;
            var_a3 = temp_a1;
            var_v1 = temp_a2 % 3;
            if (var_a0 < 0) {
                goto block_9;
            }
        } else {
            var_a0 = temp_lo + temp_lo_2;
            var_a3 = arg0;
            var_s1 = 0x64;
            var_v1 = temp_a2 % 3;
            if (var_a0 < 0) {
block_9:
                var_a0 = 0;
            }
        }
        switch (var_v1) {
        case 0:
            func_80155868(var_a0, (s32) (arg1 * var_a0) / arg0, (s32) (arg2 * var_a0) / arg0, var_a3, (s32) (arg1 * var_a3) / arg0, (s32) (arg2 * var_a3) / arg0, arg8 << 0x10, arg8 << 8, 4);
            break;
        case 1:
            func_80155868(var_a0, (s32) (arg1 * var_a0) / arg0, (s32) (arg2 * var_a0) / arg0, var_a3, (s32) (arg1 * var_a3) / arg0, (s32) (arg2 * var_a3) / arg0, arg8 << 8, arg8, 4);
            break;
        case 2:
            func_80155868(var_a0, (s32) (arg1 * var_a0) / arg0, (s32) (arg2 * var_a0) / arg0, var_a3, (s32) (arg1 * var_a3) / arg0, (s32) (arg2 * var_a3) / arg0, arg8, arg8 << 0x10, 4);
            break;
        }
        var_s1 += 1;
    } while (var_s1 < 0x32);
    dtd_on_tpage(0, 0, 4, arg5 - 4, 0);
    *temp_fp += arg4;
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80154430", func_80155868);
