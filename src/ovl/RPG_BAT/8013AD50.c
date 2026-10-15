#include "common.h"
#include "ovl/RPG_BAT.h"

void func_8013AD50(void) {
    if (!(D_8015EDF0 & 0xFF)) {
        /* FAKE: absolute address instead of D_8015ED98: the original's lui and lw use different registers
         * (as in func_8013D030, T-4010). T-16210 */
        if (*(s32 *)0x8015ED98 & 0x100) {
            func_8013E7C0(0x30, 3, 1, 1);
        } else {
            func_8013D030();
        }
    }
    /* FAKE: D_8015EDB4 reached through D_8015EDA8 (one base symbol keeps the switch load after the store,
     * T-4070); the real aggregate over D_8015EDA8..D_8015EDB8 is not declared yet. T-16210 */
    D_8015EDA8[3] += 1;
    switch (D_8015EDA8[1]) {
    case 0:
        if (!(D_8015EDC4 & 0xFF) && !(D_8015EDF0 & 0xFF)) {
            if (D_8015EDB4 >= 0x97) {
                D_8015ED98 = 0x104;
                /* FAKE: D_8015EDA0 read through D_8015ED98 so that as1 keeps the load after the store; the
                 * original probably had one aggregate over these words. T-16210 */
                D_8015EDA4 = (&D_8015ED98)[2] / 20;
                if (D_8015EDA4 == 0) {
                    D_8015EDA4 = 1;
                }
                func_800AE0F0(D_8015E74C, D_800E6280.unk_0D4);
                func_800AE100(D_8015E74C, "の体力が５％回復した");
                func_8014B738(&D_8015E814, 1, 0x3C);
                func_8014EBA8();
            }
        }
        break;
    case 1:
        func_8014EE3C(1);
        break;
    case 2:
        func_8013EA60(0, 0x21, D_8015EDA4, 0);
        func_8013F15C(0x502, 1, 0);
        func_8014EBA8();
        break;
    case 3:
        func_8014ED44(0x14);
        break;
    case 4:
        func_8014EBEC();
        D_8015ED9C = func_801445E8(1);
        break;
    }
}

void func_8013AF1C(void) {
    switch (D_8015EDAC) {
    case 0:
        func_80144480();
        func_8013EFD0(D_8015EBAC);
        func_80148F70();
        func_8013E7C0(0x30, 1, 1, 1);
        func_8014EBA8();
        return;
    case 1:
        func_8013E97C(0x30, -D_8015EDB4, 0);
        D_8015EDB4 += 3;
        if (D_8015EDB4 >= 0x21) {
            func_8013E97C(0x30, -0x20, 0);
            func_8013E7C0(0x30, 0, 1, 1);
            func_8014EBA8();
            return;
        }
    default:
        return;
    case 2:
        func_800AE0F0(D_8015E74C, D_800E6280.unk_0D4);
        func_800AE100(D_8015E74C, "の攻撃");
        if (D_8015EBB0 == 0) {
            func_800AE0F0(D_8015E774, D_8015E74C);
            func_800AE100(D_8015E774, "は失敗した");
        } else {
            func_8014D11C(D_8015E774, D_800E6280.unk_0D4, "は", D_8015EBB0, "のダメージを与えた");
        }
        func_8014B738(&D_8015E814, 2, 0x3C);
        D_8015EE48 = 3;
        func_8013E97C(0x31, D_8015EC28, D_8015EC2C);
        func_8014EBA8();
        return;
    case 3:
        func_8014ED44(0x19);
        return;
    case 4:
        if (D_8015EBB0 < 0xC8) {
            D_8015EC74 = 1;
        } else if (D_8015EBB0 < 0x1F4) {
            D_8015EC74 = 2;
        } else if (D_8015EBB0 < 0x320) {
            D_8015EC74 = 3;
        } else if (D_8015EBB0 < 0x44C) {
            D_8015EC74 = 4;
        } else if (D_8015EBB0 < 0x514) {
            D_8015EC74 = 5;
        } else if (D_8015EBB0 == 0x270F) {
            D_8015EC74 = 0xA;
        } else {
            D_8015EC74 = 6;
        }
        D_80121316 = 0x82;
        D_8012135A = 0x82;
        func_8014EBA8();
        return;
    case 5:
        if (D_8015EBB0 == 0) {
            func_8013B534();
            return;
        }
        func_8013B2F0();
        return;
    case 6:
        D_8015EE48 = 1;
        D_80121316 = 1;
        D_8012135A = 1;
        func_8014EE3C(1);
        return;
    case 7:
        func_8013EA60(0, D_8015EBAC, D_8015EBB0, 1);
        func_8013E7C0(0x30, 2, 1, 1);
        func_8014EBA8();
        return;
    case 8:
        func_8013E97C(0x30, D_8015EDB4 - 0x20, 0);
        D_8015EDB4 += 3;
        if (D_8015EDB4 >= 0x21) {
            func_8013E97C(0x30, 0, 0);
            func_8013E7C0(0x30, 0, 1, 1);
            func_8014EBA8();
            return;
        }
        break;
    case 9:
        func_8014EE3C(0);
        return;
    case 10:
        func_8014ED44(0x14);
        return;
    case 11:
        func_8014EBEC();
        break;
    }
}

s32 func_8013B2F0(void) {

    if (!(D_8015EC74 & 1)) {
        switch (D_8015EDB0.unk_00) {
        case 0:
            func_8013E7C0(0x30, 6, 5, 1);
            func_8014EBD0();
            return;
        case 1:
            /* FAKE: s32 view and dead test keep counter in v1; source unknown. T-9730. */
            if (!D_8015EC74) {
            }
            func_8014ED80(4);
            return;
        case 2:
            func_8013E810(0x31, 0xB, 5, 1);
            func_8013F15C(0x501, 1, 0);
            func_8014EBD0();
            return;
        case 3:
            if (func_8013E90C(0x31) != 0) {
                func_8013E810(0x31, 0xFF, 1, 0);
            }
            if (func_8013E90C(0x30) != 0) {
                func_8013E810(0x31, 0xFF, 1, 0);
                if (--D_8015EC74 == 0) {
                    func_8014EBA8();
                    return;
                }
                D_8015EDB0.unk_00 = 0;
            }
            break;
        }
    } else {
        switch (D_8015EDB0.unk_00) {
        case 0:
            func_8013E7C0(0x30, 6, 5, 1);
            func_8014EBD0();
            return;
        case 1:
            func_8014ED80(4);
            return;
        case 2:
            func_8013E810(0x31, 0xC, 5, 1);
            func_8013F15C(0x501, 1, 0);
            func_8014EBD0();
            return;
        case 3:
            if (func_8013E90C(0x31) != 0) {
                func_8013E810(0x31, 0xFF, 1, 0);
            }
            if (func_8013E90C(0x30) != 0) {
                func_8013E810(0x31, 0xFF, 1, 0);
                if (--D_8015EC74 == 0) {
                    func_8014EBA8();
                    return;
                }
                D_8015EDB0.unk_00 = 0;
            } else {
                return;
            }
            break;
        }
    }
}

void func_8013B534(void) {
    switch (D_8015EDB0.unk_00) {
    case 0:
        func_8013E7C0(0x30, 6, 5, 1);
        func_8014EBD0();
        break;
    case 1:
        if (func_8013E90C(0x30) != 0) {
            func_8014EBA8();
        }
        break;
    }
}
