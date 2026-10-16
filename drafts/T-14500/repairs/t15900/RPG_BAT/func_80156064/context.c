#include "common.h"
#include "ovl/RPG_BAT.h"

void func_80155CC0(void) {
    switch (D_8015EDAC) {
    case 0:
        func_800AE0F0(D_8015E74C, "　　　　　　火の鳥サーブ　　　　　　");
        func_8014B738((s32) &D_8015E814, 1, 0x5A);
        func_80144480();
        func_8013EEAC(D_8015EBAC);
        func_80148F70();
        func_8013E54C(3);
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
        func_8014ED44(0x29);
        return;
    case 5:
        func_80155E80();
        return;
    case 6:
        func_8013E810(0x33, 6, 5, 1);
        func_8013F0F4(0x510, 0, 2);
        func_8014EBA8();
        return;
    case 7:
        func_8014EDBC(0x33);
        return;
    case 8:
        func_80156064();
        return;
    case 9:
        func_8013E54C(0xFF);
        func_8013D3E4();
        func_8014EBA8();
        return;
    case 10:
        func_8013E7C0(0x34, 2, 1, 1);
        func_8014EBA8();
        return;
    case 11:
        func_8014ED44(0x14);
        return;
    case 12:
        func_8013D59C();
        return;
    case 13:
        func_8014EBEC();
        /* fallthrough */
    default:
        return;
    }
}

void func_80155E80(void) {
    func_8013F0F4(0x50E, 0, 3);
    switch (D_8015EDB0.unk_00) {
    case 0:
        switch (*(u32 *)&D_8015EBAC) {
        case 0:
            D_80121685 = 4;
            break;
        case 1:
            D_801215FD = 4;
            D_80121685 = 4;
            break;
        }
        if (D_8015EDC4 & 0x100000) {
            /* FAKE: D_8015EC50/54 reached through D_8015EC28 (one object for as1) keep the second load after the first store; the real aggregate is unknown. T-9320 */
            (&D_8015EC28)[10] = (&D_8015EC28)[0] - 0xEC;
            (&D_8015EC28)[11] = (&D_8015EC28)[1] - 0x17;
        } else {
            D_8015EC50 = -0xA4;
            D_8015EC54 = 0x74;
        }
        func_8013E97C(0x33, 0xEC, 0x2C);
        func_8013E810(0x33, 5, 1, 1);
        func_8014EBD0();
        break;
    case 1:
        func_8013E97C(0x33, D_8015EC50 * D_8015EDB0.unk_04 / 1000 + 0xEC, D_8015EC54 * D_8015EDB0.unk_04 / 1000 + 0x2C);
        D_8015EDB0.unk_04 = D_8015EDB0.unk_04 + 0x32;
        if (D_8015EDB0.unk_04 >= 0x3E9) {
            func_8014EBA8();
        }
        break;
    }
}

/* bulk_m2c declarations (T-11190) */
extern s16 D_801213F6;
extern s16 D_801213F8;
extern s16 D_80121402;
extern s16 D_80121406;
extern s32 D_8011F110;
/* end bulk_m2c declarations */
s32 func_80156064(void) {
    s16 temp_v0;
    s16 temp_v0_3;
    s32 *var_s1;
    s32 *var_s1_2;
    s32 temp_s3;
    s32 temp_t4;
    s32 temp_v0_2;
    s32 var_s2;
    u8 *var_s0;
    u8 *var_s0_2;
    u8 *var_s0_3;

    func_8013F0F4(0x50F, 0, 1);
    var_s1 = &D_8015EBBC;
    switch (D_8015EDB0.unk_00) {
    case 0:
        var_s0 = D_8011EDE0;
        do {
            var_s0 += 0x110;
            *(s32 *)((unsigned char *)var_s1 + (204)) = 0;
            *(s8 *)((unsigned char *)var_s0 + (6325)) = 5;
            *(s32 *)((unsigned char *)var_s1 + (208)) = 0;
            *(s8 *)((unsigned char *)var_s0 + (6393)) = 5;
            *(s32 *)((unsigned char *)var_s1 + (212)) = 0;
            *(s8 *)((unsigned char *)var_s0 + (6461)) = 5;
            var_s1 += 0x10;
            *(s32 *)((unsigned char *)var_s1 + (184)) = 0;
            *(s8 *)((unsigned char *)var_s0 + (6257)) = 5;
        } while (var_s0 != &D_8011F110);
        func_8014EBD0();
        return;
    case 1:
        D_80121406 -= 2;
        temp_v0 = (D_8015EDB4 << 6) + 0x1000;
        D_801213F6 = temp_v0;
        D_801213F8 = temp_v0;
        D_8015EDB4 += 1;
        var_s0_2 = D_8011EDE0;
        var_s1_2 = &D_8015EBBC;
        var_s2 = 4;
        do {
            if ((*(s32 *)((unsigned char *)var_s1_2 + (200)) == 0) && (func_8013E9A8(0x2D) == 0)) {
                *(s32 *)((unsigned char *)var_s1_2 + (200)) = 1;
                temp_s3 = (func_8013E9A8(0x80) + D_80121402) - 0x40;
                func_8013E97C(var_s2, temp_s3 + 0xA0, (D_80121406 - func_8013E9A8(0x28)) + 0x78);
                func_8013E810(var_s2, func_8013E9A8(2) + 8, 1, 1);
            }
            temp_v0_2 = *(s32 *)((unsigned char *)var_s1_2 + (200));
            temp_t4 = temp_v0_2 + 1;
            if (temp_v0_2 != 0) {
                *(s32 *)((unsigned char *)var_s1_2 + (200)) = temp_t4;
                if (temp_t4 >= 0x1F) {
                    switch (temp_t4) {
                    case 31:
                        func_8013E810(var_s2, 0xFF, 1, 0);
                        break;
                    case 32:
                        func_8013E810(var_s2, 0xFF, 1, 1);
                        break;
                    case 33:
                        func_8013E810(var_s2, 0xFF, 1, 0);
                        break;
                    case 34:
                        func_8013E810(var_s2, 0xFF, 1, 1);
                        break;
                    case 35:
                        func_8013E810(var_s2, 0xFF, 1, 0);
                        break;
                    case 36:
                        func_8013E810(var_s2, 0xFF, 1, 1);
                        break;
                    case 37:
                        func_8013E810(var_s2, 0xFF, 1, 0);
                        break;
                    }
                }
            }
            *(s16 *)((unsigned char *)var_s0_2 + (6570)) = (s16) (*(s16 *)((unsigned char *)var_s0_2 + (6570)) + 1);
            var_s2 += 1;
            temp_v0_3 = (D_8015EDB4 << 6) + 0x1000;
            var_s1_2 += 4;
            var_s0_2 += 0x44;
            *(s16 *)((unsigned char *)var_s0_2 + (6486)) = temp_v0_3;
            *(s16 *)((unsigned char *)var_s0_2 + (6488)) = temp_v0_3;
        } while (var_s2 < 0x10);
        if (D_80121406 < -0xC0) {
            func_8013E810(0x33, 0xFF, 1, 0);
            var_s0_3 = D_8011EDE0;
            do {
                var_s0_3 += 0x110;
                *(s16 *)((unsigned char *)var_s0_3 + (6350)) = 0x1000;
                *(s16 *)((unsigned char *)var_s0_3 + (6352)) = 0x1000;
                *(s16 *)((unsigned char *)var_s0_3 + (6418)) = 0x1000;
                *(s16 *)((unsigned char *)var_s0_3 + (6420)) = 0x1000;
                *(s16 *)((unsigned char *)var_s0_3 + (6486)) = 0x1000;
                *(s16 *)((unsigned char *)var_s0_3 + (6488)) = 0x1000;
                *(s16 *)((unsigned char *)var_s0_3 + (6282)) = 0x1000;
                *(s16 *)((unsigned char *)var_s0_3 + (6284)) = 0x1000;
            } while (var_s0_3 != &D_8011F110);
            D_801213F6 = 0x1000;
            D_801213F8 = 0x1000;
            func_8014EBA8();
        }
        return;
    }
}
