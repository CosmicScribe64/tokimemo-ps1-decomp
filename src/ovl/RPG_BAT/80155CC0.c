#define RPG_BAT_OVERRIDE_func_8013F0F4 /* implicit int view: the call's $v0 moves the D_8015EDB0 selector of func_80156064 to $v1 (T-17116) */
#include "common.h"
#include "ovl/RPG_BAT.h"

s32 func_8013F0F4();

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

/* s32 without a return: func_80155CC0 calls this function earlier as an implicit int, a void definition does not build. */
/* FAKE: the twelve words at D_8015EBAC + 4 * (i + 50) are reached through the first symbol (the loop pointer starts at D_8015EBBC); the
 * real work-block aggregate is unknown. The records are D_8011ECD0 elements i + 96, the rolled loops start at i = 4. T-17116 */
s32 func_80156064(void) {
    s32 i;
    s32 x;
    s32 y;
    s16 v;

    func_8013F0F4(0x50F, 0, 1);
    switch (D_8015EDB0.unk_00) {
    case 0:
        for (i = 4; i < 16; i++) {
            (&D_8015EBAC)[i + 50] = 0;
            ((RpgObjectRecord44 *)D_8011ECD0)[i + 96].unk_01 = 5;
        }
        func_8014EBD0();
        break;
    case 1:
        ((RpgObjectRecord44 *)D_8011ECD0)[147].unk_2A -= 2;
        v = (D_8015EDB0.unk_04 << 6) + 0x1000;
        ((RpgObjectRecord44 *)D_8011ECD0)[147].unk_1A = v;
        ((RpgObjectRecord44 *)D_8011ECD0)[147].unk_1C = v;
        D_8015EDB0.unk_04 += 1;
        for (i = 4; i < 16; i++) {
            if ((&D_8015EBAC)[i + 50] == 0 && func_8013E9A8(0x2D) == 0) {
                (&D_8015EBAC)[i + 50] = 1;
                x = ((RpgObjectRecord44 *)D_8011ECD0)[147].unk_26 + func_8013E9A8(0x80) - 0x40;
                y = ((RpgObjectRecord44 *)D_8011ECD0)[147].unk_2A - func_8013E9A8(0x28);
                func_8013E97C(i, x + 0xA0, y + 0x78);
                func_8013E810(i, func_8013E9A8(2) + 8, 1, 1);
            }
            if ((&D_8015EBAC)[i + 50] != 0) {
                (&D_8015EBAC)[i + 50] += 1;
                if ((&D_8015EBAC)[i + 50] >= 0x1F) {
                    switch ((&D_8015EBAC)[i + 50]) {
                    case 31:
                        func_8013E810(i, 0xFF, 1, 0);
                        break;
                    case 32:
                        func_8013E810(i, 0xFF, 1, 1);
                        break;
                    case 33:
                        func_8013E810(i, 0xFF, 1, 0);
                        break;
                    case 34:
                        func_8013E810(i, 0xFF, 1, 1);
                        break;
                    case 35:
                        func_8013E810(i, 0xFF, 1, 0);
                        break;
                    case 36:
                        func_8013E810(i, 0xFF, 1, 1);
                        break;
                    case 37:
                        func_8013E810(i, 0xFF, 1, 0);
                        break;
                    }
                }
            }
            ((RpgObjectRecord44 *)D_8011ECD0)[i + 96].unk_2A += 1;
            v = (D_8015EDB0.unk_04 << 6) + 0x1000;
            ((RpgObjectRecord44 *)D_8011ECD0)[i + 96].unk_1A = v;
            ((RpgObjectRecord44 *)D_8011ECD0)[i + 96].unk_1C = v;
        }
        if (((RpgObjectRecord44 *)D_8011ECD0)[147].unk_2A < -0xC0) {
            func_8013E810(0x33, 0xFF, 1, 0);
            for (i = 4; i < 16; i++) {
                ((RpgObjectRecord44 *)D_8011ECD0)[i + 96].unk_1A = 0x1000;
                ((RpgObjectRecord44 *)D_8011ECD0)[i + 96].unk_1C = 0x1000;
            }
            ((RpgObjectRecord44 *)D_8011ECD0)[147].unk_1A = 0x1000;
            ((RpgObjectRecord44 *)D_8011ECD0)[147].unk_1C = 0x1000;
            func_8014EBA8();
        }
        break;
    }
}
