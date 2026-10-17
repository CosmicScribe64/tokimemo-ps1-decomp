#define RPG_BAT_OVERRIDE_func_8013F0F4 /* implicit int view: the call's $v0 keeps the compare-chain values in func_80149C5C off $v0 (T-20600) */
#include "common.h"
#include "ovl/RPG_BAT.h"

s32 func_8013F0F4();

void func_801498C0(void) {
    switch (D_8015EDAC) {
    case 0:
        func_800AE0F0(D_8015E74C, "　　　　　　　気分最高　　　　　　　");
        func_8014B738((s32) &D_8015E814, 1, 0x5A);
        func_8014EEF4();
        func_80144480();
        func_8013EFD0(D_8015EBAC);
        func_80148F70();
        if (D_8015EBAC == 1) {
            D_801215FD = 4;
            D_80121685 = 4;
        }
        switch (D_8015EC34) {
        default:
        case 0:
            D_8015EC74 = 5;
            break;
        case 1:
            D_8015EC74 = 6;
            break;
        case 2:
            D_8015EC74 = 7;
            break;
        case 3:
            D_8015EC74 = 8;
            break;
        case 4:
            D_8015EC74 = 9;
            break;
        case 5:
            D_8015EC74 = 0xA;
            break;
        }
        func_8014EBA8();
        func_8014EBA8();
        return;
    case 2:
        func_8013D418();
        return;
    case 3:
        func_8014A3DC();
        return;
    case 4:
        func_8014A16C();
        return;
    case 5:
        func_80149C5C();
        return;
    case 6:
        func_80149AA8();
        return;
    case 7:
        func_8013D3E4();
        func_8014EBA8();
        return;
    case 8:
        func_8013D59C();
        return;
    case 9:
        func_8014EBEC();
        D_8015EC34 = 0;
        /* fallthrough */
    default:
        return;
    }
}

void func_80149AA8(void) {
    switch (D_8015EDB0.unk_00) {
    case 0:
        func_8013E7C0(0x28, 0xFF, 1, 0);
        func_8013E810(0x29, 0xE, 5, 1);
        func_8014EBD0();
        break;
    case 1:
        if (D_8012114C + 3 == D_8015EC74) {
            func_8013EA30(0xFF);
            switch (D_8015EC34) {
            case 0:
            default:
                func_8013F0F4(0x505, 1, 0);
                break;
            case 1:
                func_8013F0F4(0x505, 1, 0);
                break;
            case 2:
                func_8013F0F4(0x506, 1, 0);
                break;
            case 3:
                func_8013F0F4(0x507, 1, 0);
                break;
            case 4:
                func_8013F0F4(0x508, 1, 0);
                break;
            case 5:
                func_8013F0F4(0x509, 1, 0);
                break;
            }
            func_8014EBD0();
        }
        break;
    case 2:
        D_8012114C = D_8015EC74 - 3;
        func_8014ED80(0xA);
        break;
    case 3:
        func_8013EA30(0x80);
        func_8013E810(0x29, 0xFF, 1, 0);
        func_8014EBA8();
        break;
    }
}

/* Implicit int with no return value: the call in func_801498C0 comes before this definition. */
s32 func_80149C5C(void) {
    s32 x;
    s32 y;

    x = (D_8015EC28 - 0xE0) * D_8015EDB0.unk_04 / 1000 + 0xE0;
    y = (D_8015EC2C - 0x58) * D_8015EDB0.unk_04 / 1000 + 0x58;
    if (D_8015EBAC == 0) {
        if (D_8015EDB0.unk_04 * 0x1800 / 1000 > 0) {
            func_8013F0F4(0x504, 0, 2);
        }
        if (D_8015EDB0.unk_04 * 0x1800 / 1000 > 0x400) {
            func_8013F0F4(0x504, 0, 3);
        }
        if (D_8015EDB0.unk_04 * 0x1800 / 1000 > 0x800) {
            func_8013F0F4(0x504, 0, 4);
        }
        if (D_8015EDB0.unk_04 * 0x1800 / 1000 > 0xC00) {
            func_8013F0F4(0x504, 0, 5);
        }
        if (D_8015EDB0.unk_04 * 0x1800 / 1000 > 0x1000) {
            func_8013F0F4(0x504, 0, 6);
        }
        if (D_8015EDB0.unk_04 * 0x1800 / 1000 > 0x1400) {
            func_8013F0F4(0x504, 0, 7);
        }
    } else {
        if (D_8015EDB0.unk_04 * 0x2000 / 1000 > 0) {
            func_8013F0F4(0x504, 0, 2);
        }
        if (D_8015EDB0.unk_04 * 0x2000 / 1000 > 0x400) {
            func_8013F0F4(0x504, 0, 3);
        }
        if (D_8015EDB0.unk_04 * 0x2000 / 1000 > 0x800) {
            func_8013F0F4(0x504, 0, 4);
        }
        if (D_8015EDB0.unk_04 * 0x2000 / 1000 > 0xC00) {
            func_8013F0F4(0x504, 0, 5);
        }
        if (D_8015EDB0.unk_04 * 0x2000 / 1000 > 0x1000) {
            func_8013F0F4(0x504, 0, 6);
        }
        if (D_8015EDB0.unk_04 * 0x2000 / 1000 > 0x1400) {
            func_8013F0F4(0x504, 0, 7);
        }
        if (D_8015EDB0.unk_04 * 0x2000 / 1000 > 0x1800) {
            func_8013F0F4(0x504, 0, 8);
        }
        if (D_8015EDB0.unk_04 * 0x2000 / 1000 > 0x1C00) {
            func_8013F0F4(0x504, 0, 9);
        }
    }
    if (D_8015EBAC == 0) {
        func_8013E97C(0x29, x, (func_800A0070(D_8015EDB0.unk_04 * 0x1800 / 1000) >> 8) + y);
        func_8013E97C(0x28, x, (func_800A0070(D_8015EDB0.unk_04 * 0x1800 / 1000) >> 8) + y);
    } else {
        func_8013E97C(0x29, x, (func_800A0070(D_8015EDB0.unk_04 * 0x2000 / 1000) >> 8) + y);
        func_8013E97C(0x28, x, (func_800A0070(D_8015EDB0.unk_04 * 0x2000 / 1000) >> 8) + y);
    }
    if (D_8015EBAC == 0) {
        D_8015EDB0.unk_04 += 0xC;
    } else {
        D_8015EDB0.unk_04 += 8;
    }
    if (D_8015EDB0.unk_04 > 0x3E8) {
        func_8014EBA8();
    }
}

void func_8014A16C(void) {
    switch (D_8015EDB0.unk_00) {
    case 0:
        func_8013E7C0(0x2A, 0xFF, 1, 0);
        func_8013E7C0(0x2B, 0xFF, 1, 0);
        func_8013E7C0(0x2C, 0xFF, 1, 0);
        func_8013E7C0(0x2D, 0xFF, 1, 0);
        func_8013E7C0(0x2E, 0xFF, 1, 0);
        func_8013E7C0(0x2F, 0xFF, 1, 0);
        func_8013E7C0(0x34, 5, 5, 1);
        func_8013E97C(0x29, 0xFB, 0x3D);
        func_8013E97C(0x28, 0xFB, 0x3D);
        func_8014EBD0();
        break;
    case 1:
        func_8014ED80(3);
        break;
    case 2:
        func_8013E97C(0x29, 0x102, 0x45);
        func_8013E97C(0x28, 0x102, 0x45);
        func_8014EBD0();
        break;
    case 3:
        func_8014ED80(3);
        break;
    case 4:
        func_8013E97C(0x29, 0x107, 0x4C);
        func_8013E97C(0x28, 0x107, 0x4C);
        func_8014EBD0();
        break;
    case 5:
        func_8014ED80(5);
        break;
    case 6:
        func_8013E97C(0x29, 0x109, 0x53);
        func_8013E97C(0x28, 0x109, 0x53);
        func_8014EBD0();
        break;
    case 7:
        func_8014ED80(5);
        break;
    case 8:
        func_8013E97C(0x29, 0x108, 0x58);
        func_8013E97C(0x28, 0x108, 0x58);
        func_8014EBD0();
        break;
    case 9:
        func_8014ED80(7);
        break;
    case 10:
        func_8013E97C(0x29, 0x108 - D_8015EDB8, 0x58);
        func_8013E97C(0x28, 0x108 - D_8015EDB8, 0x58);
        D_8015EDB8 += 2;
        if (D_8015EDB8 >= 0x28) {
            func_8014EBA8();
        }
        break;
    }
}

/* int-returning (implicit int in the original): v0 is live out, so as1 keeps the lui of the promoted
 * D_8015EDA4 behind the case 4 branch; with a void return it moves it above (T-17115) */
s32 func_8014A3DC(void) {
    if (D_8015EDB0.unk_00 < 5) {
        func_8013F0F4(0x503, 0x1E, 1);
    }
    switch (D_8015EDB0.unk_00) {
    case 0:
        func_8013E7C0(0x34, 3, 1, 1);
        func_8013E7C0(0x2F, 0xD, 5, 1);
        func_8014EBD0();
        return;
    case 1:
        func_8014EDFC(0x2F);
        return;
    case 2:
        func_8013E7C0(0x2F, 7, 1, 1);
        func_8013E7C0(0x2A, 8, 1, 1);
        func_8013E7C0(0x2B, 9, 1, 1);
        func_8013E7C0(0x2C, 0xA, 1, 1);
        func_8013E7C0(0x2D, 0xB, 1, 1);
        func_8013E7C0(0x2E, 0xC, 1, 1);
        func_8014EBD0();
        return;
    case 3:
        func_8013E97C(0x29, 0xF0, 0x38);
        func_8013E97C(0x28, 0xF0, 0x38);
        func_8013E810(0x29, 0xF, 1, 1);
        func_8014EBD0();
        return;
    case 4:
        if (D_8015EDB8 >= 0x1F) {
            D_8015EDB8 = 0;
            if (++D_8015EDA4 == D_8015EC74) {
                func_8014EBD0();
            } else {
                func_8013E810(0x29, D_8015EDA4 + 0xF, 1, 1);
                if ((D_8015EDA4 == 6) || (D_8015EDA4 == 7)) {
                    func_8013E810(0x28, 6, 1, 1);
                }
                if ((D_8015EDA4 == 8) || (D_8015EDA4 == 9)) {
                    func_8013E810(0x28, 0x19, 1, 1);
                }
            }
        }
        D_8015EDB8 += 1;
        return;
    case 5:
        func_8014ED80(0x1E);
        return;
    case 6:
        func_8014EBA8();
        /* fallthrough */
    default:
        return;
    }
}
