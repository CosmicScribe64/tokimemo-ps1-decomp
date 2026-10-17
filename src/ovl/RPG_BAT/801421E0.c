#include "common.h"
#include "ovl/RPG_BAT.h"

void func_801421E0(void) {
    s32 temp_v0;

    switch (D_8015EDAC) {
    case 0:
        func_800AE0F0(D_8015E74C, "　　　　　　世界征服ロボ　　　　　　");
        func_8014B738((s32) &D_8015E814, 1, 0x5A);
        func_8014EEF4();
        func_80144480();
        func_8013EFD0(D_8015EBAC);
        func_80148F70();
        func_8014EBA8();
        func_8014EBA8();
    default:
        break;
    case 2:
        func_8013D418();
        break;
    case 3:
        func_8013E7C0(0x34, 3, 5, 1);
        func_8014EBA8();
        break;
    case 4:
        if (!(D_80121422 & 1)) {
            func_8013E7C0(0x34, 3, 5, 1);
            temp_v0 = D_8015EDB0.unk_04 + 1;
            D_8015EDB0.unk_04 = temp_v0;
            if (temp_v0 >= 5) {
                func_8014EBA8();
            }
        }
        break;
    case 5:
        func_8013E7C0(0x34, 2, 1, 1);
        func_8013E7C0(0x2B, 0x11, 5, 1);
        func_8013F0F4(0x500, 1, 0);
        func_8014EBA8();
        break;
    case 6:
        func_8014EDBC(0x2B);
        break;
    case 7:
        func_80142544();
        break;
    case 8:
        func_8013D1D0();
        break;
    case 9:
        func_8014ED44(0x1E);
        break;
    case 10:
        func_801426B0();
        break;
    case 11:
        func_800AE0F0(D_8015E74C, "ロボ「服従パーンチ！");
        func_8014B738((s32) &D_8015E814, 1, 0x3C);
        func_8014EBA8();
        break;
    case 12:
        func_8014EE3C(1);
        break;
    case 13:
        func_801427E4();
        break;
    case 14:
        func_800AE0F0(D_8015E74C, "ロボ「独裁ミサイールッ！");
        func_8014B738((s32) &D_8015E814, 1, 0x3C);
        func_8014EBA8();
        break;
    case 15:
        func_8014EE3C(1);
        break;
    case 16:
        func_80142A30();
        break;
    case 17:
        func_8014ED44(0x1E);
        break;
    case 18:
        func_800AE0F0(D_8015E74C, "ロボ「世界征服ビィーム！");
        func_8014B738((s32) &D_8015E814, 1, 0x3C);
        func_8014EBA8();
        break;
    case 19:
        func_8014EE3C(0);
        break;
    case 20:
        func_80142CB8();
        break;
    case 21:
        func_8013D350();
        func_8014EBA8();
        break;
    case 22:
        func_8014ED44(0x28);
        break;
    case 23:
        func_80142DF4();
        break;
    case 24:
        func_8014ED44(0x1E);
        break;
    case 25:
        func_8013D290();
        break;
    case 26:
        func_8014ED44(0x14);
        break;
    case 27:
        func_8013D59C();
        break;
    case 28:
        func_8014EBEC();
        break;
    }
}

/* FAKE: s32 fall-through keeps the selector in v1 and division result in v0. T-9780. */
s32 func_80142544(void) {
    s32 value;

    func_8013F0F4(0x509, 0, 1);
    func_8013E7C0(0x2B, 0x12, 1, 1);
    func_8013E7C0(0x2C, 0x13, 1, 1);
    if (D_8015EDB0.unk_04 < 200) {
        value = (D_8015EDB0.unk_04 << 12) / 30;
        ((RpgObjectBlock *)D_8011ECD0)->group[43].unk_1A = value;
        ((RpgObjectBlock *)D_8011ECD0)->group[43].unk_1C = value;
        ((RpgObjectBlock *)D_8011ECD0)->group[44].unk_1A = value;
        ((RpgObjectBlock *)D_8011ECD0)->group[44].unk_1C = value;
    }
    if (D_8015EDB0.unk_04 >= 10) {
        func_8013E97C(0x2B, 0, ((10 - D_8015EDB0.unk_04) * D_8015EDB0.unk_04) / 100);
        func_8013E97C(0x2C, 0, D_8015EDB0.unk_04 - 10);
    }
    D_8015EDB0.unk_04++;
    if (D_8015EDB0.unk_04 >= 129) {
        func_8013E7C0(0x2B, 0xFF, 1, 0);
        func_8013E7C0(0x2C, 0xFF, 1, 0);
        ((RpgObjectBlock *)D_8011ECD0)->group[43].unk_1A = 0x1000;
        ((RpgObjectBlock *)D_8011ECD0)->group[43].unk_1C = 0x1000;
        ((RpgObjectBlock *)D_8011ECD0)->group[44].unk_1A = 0x1000;
        ((RpgObjectBlock *)D_8011ECD0)->group[44].unk_1C = 0x1000;
        func_8014EBA8();
    }
}

void func_801426B0(void) {
    s32 t;

    if (D_8015EDB0.unk_04 == 0) {
        func_8013E810(0x2B, 4, 1, 1);
        func_8013E810(0x2C, 0x13, 1, 1);
        func_8013E97C(0x2C, 0x110, 0x90);
    }
    func_8013E97C(0x2B, 0x110, D_8015EDB0.unk_04 * 4 - 0xC0);
    t = (D_8015EDB0.unk_04 << 14) / 292;
    D_8012121A = t;
    D_8012121C = t;
    if (D_8015EDB0.unk_04 * 4 - 0xC0 >= 0x55) {
        func_8013E810(0x2B, 0x15, 1, 1);
    }
    D_8015EDB0.unk_04 += 1;
    if (D_8015EDB0.unk_04 * 4 - 0xC0 >= 0x65) {
        func_8013E7C0(0x2B, 6, 1, 1);
        func_8013E7C0(0x2C, 0xFF, 1, 0);
        func_8013E97C(0x2B, 0, 0);
        func_8014EBA8();
    }
}

s32 func_801427E4(void) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;

    switch (D_8015EDB0.unk_00) {
    case 0:
        func_8013E7C0(0x2B, 8, 5, 1);
        func_8014EBD0();
    default:
        break;
    case 1:
        if (!(D_801211BE & 1)) {
            func_8014EBD0();
        }
        break;
    case 2:
        func_8013E7C0(0x2B, 7, 1, 1);
        func_8013E97C(0x2A, 0, 0);
        func_8013E7C0(0x2A, 9, 5, 1);
        func_8014EBD0();
        break;
    case 3:
        if (((DisplayActor44 *)D_8011ECD0)[138].unk_18 == 7) {
            func_8014EBD0();
        }
        break;
    case 4:
        func_8013E97C(0x2A, 0xD2, 0x48);
        func_8013E810(0x2A, 0xA, 1, 1);
        func_8013F0F4(0x504, 1, 4);
        func_8014EBD0();
        break;
    case 5:
        temp_v0 = ++D_8015EDB0.unk_04;
        func_8013E97C(0x2A, 0xD2 - (temp_v0 * 8), 0x48);
        if (D_8015EDB0.unk_04 >= 0x31) {
            func_8013E810(0x2A, 0x16, 1, 1);
            func_8014EBD0();
        }
        break;
    case 6:
        temp_v0_2 = --D_8015EDB0.unk_04;
        func_8013E97C(0x2A, 0xD2 - (temp_v0_2 * 8), 0x48);
        if (D_8015EDB0.unk_04 < -4) {
            func_8013E7C0(0x2B, 8, 1, 1);
            func_8013E810(0x2A, 0xFF, 1, 0);
            func_8014EBD0();
        }
        break;
    case 7:
        temp_v0_3 = D_8015EDB0.unk_08 + 1;
        D_8015EDB0.unk_08 = temp_v0_3;
        if (temp_v0_3 >= 9) {
            func_8013E7C0(0x2B, 6, 1, 1);
            func_8014EBA8();
        }
        break;
    }
}

s32 func_80142A30(void) {
    switch (D_8015EDB0.unk_00) {
    case 0:
        func_8013E7C0(0x2B, 0xB, 5, 1);
        func_8013F0F4(0x502, 1, 2);
        /* FAKE: D_8015EBAC, D_8015EC28/2C/50/54 read as one object (loads stay behind the stores, as in the original); the real type is unknown. T-9310 */
        (&D_8015EBAC)[41] = (&D_8015EBAC)[31] - 0xF0;
        (&D_8015EBAC)[42] = (&D_8015EBAC)[32] - 0x58;
        if ((&D_8015EBAC)[0] == 1) {
            D_801215FD = 4;
        }
        func_8014EBD0();
        return;
    case 1:
        if (!(D_801211BE & 1)) {
            func_8013E97C(0x2A, 0xF0, 0x58);
            func_8013E810(0x2A, 0xC, 1, 1);
            func_8013E7C0(0x2B, 0x14, 5, 1);
            func_8013F0F4(0x506, 1, 6);
            func_8014EBD0();
            return;
        }
        return;
    case 2:
        if (!(D_801211BE & 1)) {
            func_8013E7C0(0x2B, 6, 1, 1);
        }
        func_8013E97C(0x2A, D_8015EC50 * D_8015EDB0.unk_04 / 100 + 0xF0, D_8015EC54 * D_8015EDB0.unk_04 / 100 + 0x58);
        D_8015EDB0.unk_04 += 3;
        if (D_8015EDB0.unk_04 >= 0x65) {
            func_8013E810(0x2A, 0xE, 5, 1);
            func_8013F0F4(0x503, 1, 3);
            func_8014EBD0();
            return;
        }
        break;
    case 3:
        if (!(D_8012117A & 1)) {
            func_8013E7C0(0x2B, 6, 1, 1);
            func_8013E810(0x2A, 0xFF, 1, 0);
            func_8014EBA8();
        }
        break;
    }
}

void func_80142CB8(void) {
    switch (D_8015EDB0.unk_00) {
    case 0:
        func_8013E97C(0x2A, 0, 0);
        func_8013E7C0(0x2A, 0xF, 5, 1);
        func_8014EBD0();
        return;
    case 1:
        if (!(D_8012117A & 1)) {
            func_8013F0F4(0x505, 0xF, 5);
            func_8013E7C0(0x2A, 0x10, 5, 1);
            D_8015EDB0.unk_04 += 1;
            func_8014EBD0();
            return;
        }
        return;
    case 2:
        if (D_8015EDB0.unk_04 < 0x32) {
            func_8013F0F4(0x505, 0xF, 5);
            if (!(D_8012117A & 1)) {
                func_8013E7C0(0x2A, 0x10, 5, 1);
                D_8015EDB0.unk_04 += 1;
                return;
            }
        } else {
            func_8013E7C0(0x2A, 0xFF, 1, 0);
            func_8014EBA8();
        }
        break;
    }
}

void func_80142DF4(void) {
    s32 t;
    s32 u;
    s32 d;
    s32 y;

    if (D_8015EDB0.unk_04 == 0) {
        func_8013E810(0x2B, 0x15, 1, 1);
        func_8013E810(0x2C, 0x13, 1, 1);
        func_8013E97C(0x2C, 0x110, 0x90);
        func_8013F0F4(0x507, 1, 7);
    }
    func_8013E97C(0x2B, 0x110, 0x64 - D_8015EDB0.unk_04 * 4);
    d = 0x124;
    u = D_8015EDB0.unk_04 * 4;
    t = ((d - u) << 12) / d;
    D_8012121A = t;
    D_8012121C = t;
    y = 0x64 - u;
    if (y < 0x54 && y >= 0x4D) {
        func_8013E810(0x2B, 4, 1, 1);
    }
    D_8015EDB0.unk_04 += 1;
    if (0x64 - D_8015EDB0.unk_04 * 4 < -0xC0) {
        func_8013E7C0(0x2B, 6, 1, 0);
        func_8013E7C0(0x2C, 0xFF, 1, 0);
        func_8014EBA8();
    }
}
