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

s32 func_80156064(void) {
    DisplayActor44 *var_s0;
    DisplayActor44 *var_s0_2;
    DisplayActor44 *var_s0_3;
    s16 temp_v0;
    s16 temp_v0_3;
    s32 *var_s1;
    s32 *var_s1_2;
    s32 temp_s3;
    s32 temp_t4;
    s32 temp_v0_2;
    s32 var_s2;

    func_8013F0F4(0x50F, 0, 1);
    var_s1 = &D_8015EBBC;
    switch (D_8015EDB0.unk_00) {
    case 0:
        var_s0 = &((DisplayActor44 *)D_8011ECD0)[4];
        do {
            var_s0 += 4;
            *(s32 *)((unsigned char *)var_s1 + (204)) = 0;
            var_s0[93].unk_01 = 5;
            *(s32 *)((unsigned char *)var_s1 + (208)) = 0;
            var_s0[94].unk_01 = 5;
            *(s32 *)((unsigned char *)var_s1 + (212)) = 0;
            var_s0[95].unk_01 = 5;
            var_s1 += 4;
            *(s32 *)((unsigned char *)var_s1 + (184)) = 0;
            var_s0[92].unk_01 = 5;
        } while (var_s0 != (&((DisplayActor44 *)D_8011ECD0)[16]));
        func_8014EBD0();
        return;
    case 1:
        ((DisplayActor44 *)D_8011ECD0)[147].unk_2A = (s16) ((*(s32 *)D_8011ECD0) - 2);
        temp_v0 = (D_8015EDB0.unk_04 << 6) + 0x1000;
        ((DisplayActor44 *)D_8011ECD0)[147].unk_1A = temp_v0;
        *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (10024)) = temp_v0;
        D_8015EDB0.unk_04 += 1;
        var_s0_2 = &((DisplayActor44 *)D_8011ECD0)[4];
        var_s1_2 = &D_8015EBBC;
        var_s2 = 4;
        do {
            if ((*(s32 *)((unsigned char *)var_s1_2 + (200)) == 0) && (func_8013E9A8(0x2D) == 0)) {
                *(s32 *)((unsigned char *)var_s1_2 + (200)) = 1;
                temp_s3 = (func_8013E9A8(0x80) + ((DisplayActor44 *)D_8011ECD0)[147].unk_26) - 0x40;
                func_8013E97C(var_s2, temp_s3 + 0xA0, (((DisplayActor44 *)D_8011ECD0)[147].unk_2A - func_8013E9A8(0x28)) + 0x78);
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
            var_s0_2[96].unk_2A = (s16) (var_s0_2[96].unk_2A + 1);
            var_s2 += 1;
            temp_v0_3 = (D_8015EDB0.unk_04 << 6) + 0x1000;
            var_s1_2 += 1;
            var_s0_2 += 1;
            var_s0_2[95].unk_1A = temp_v0_3;
            *(s16 *)((unsigned char *)var_s0_2 + (6488)) = temp_v0_3;
        } while (var_s2 < 0x10);
        if (((DisplayActor44 *)D_8011ECD0)[147].unk_2A < -0xC0) {
            func_8013E810(0x33, 0xFF, 1, 0);
            var_s0_3 = &((DisplayActor44 *)D_8011ECD0)[4];
            do {
                var_s0_3 += 4;
                var_s0_3[93].unk_1A = 0x1000;
                *(s16 *)((unsigned char *)var_s0_3 + (6352)) = 0x1000;
                var_s0_3[94].unk_1A = 0x1000;
                *(s16 *)((unsigned char *)var_s0_3 + (6420)) = 0x1000;
                var_s0_3[95].unk_1A = 0x1000;
                *(s16 *)((unsigned char *)var_s0_3 + (6488)) = 0x1000;
                var_s0_3[92].unk_1A = 0x1000;
                *(s16 *)((unsigned char *)var_s0_3 + (6284)) = 0x1000;
            } while (var_s0_3 != (&((DisplayActor44 *)D_8011ECD0)[16]));
            ((DisplayActor44 *)D_8011ECD0)[147].unk_1A = 0x1000;
            *(s16 *)((unsigned char *)((DisplayActor44 *)D_8011ECD0) + (10024)) = 0x1000;
            func_8014EBA8();
        }
        return;
    }
}
