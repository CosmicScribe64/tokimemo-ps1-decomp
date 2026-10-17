#include "common.h"
#include "ovl/RPG_BAT.h"

void func_801484E0(void) {
    switch (D_8015EDAC) {
    case 0:
        func_800AE0F0(D_8015E74C, "　　　　　レッドカード乱舞　　　　　");
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
        func_80148654();
        return;
    case 4:
        func_8013E7C0(0x34, 3, 1, 1);
        func_8014EBA8();
        return;
    case 5:
        func_8014ED44(0x20);
        return;
    case 6:
        func_8013E7C0(0x34, 5, 5, 1);
        func_8014EBA8();
        return;
    case 7:
        func_8014EDBC(0x34);
        return;
    case 8:
        func_80148714();
        return;
    case 9:
        func_80148B08();
        return;
    case 10:
        func_8014ED44(0x14);
        return;
    case 11:
        func_8013D59C();
        return;
    case 12:
        func_8014EBEC();
        /* fallthrough */
    default:
        return;
    }
}

void func_80148654(void) {
    switch (D_8015EDB0.unk_00) {
    case 0:
        func_8013E7C0(0x34, 4, 5, 1);
        func_8013F0F4(0x506, 1, 0);
        func_8014EBD0();
        break;
    case 1:
        if (!(D_80121422 & 1)) {
            func_8013E7C0(0x34, 4, 5, 1);
            D_8015EDB0.unk_08 += 1;
        }
        if (D_8015EDB0.unk_08 >= 0xE) {
            func_8014EBA8();
        }
        break;
    }
}

/* FAKE: the D_8015EBAC and D_8015EC70 tables are reached through their first symbol (aggregates not typed yet); m views the D_8015EC50 words as RpgMotion20, as func_80148B08 does. T-17113, T-22013 */
void func_80148714(void) {
    s32 i;
    s32 start;
    s32 *r;
    s32 x;
    s32 y;
    RpgMotion20 *m = (RpgMotion20 *)&D_8015EC50;

    switch (D_8015EDB0.unk_00) {
    case 0:
        /* FAKE: T-22013, D_8015EC28/2C/50/54 are words 31, 32, 41, 42 of the table at D_8015EBAC; as1 keeps the loads behind the first store only when all four are one symbol (aggregate not typed yet) */
        (&D_8015EBAC)[41] = (&D_8015EBAC)[31] - 0xB0;
        (&D_8015EBAC)[42] = (&D_8015EBAC)[32] - 0x58;
        if (D_8015EBAC == 1) {
            D_801215FD = 4;
            D_80121685 = 4;
        }
        if (D_8015EBAC == 0) {
            D_80121685 = 4;
        }
        func_8013F0F4(0x507, 1, 0);
        func_8014EBD0();
        return;
    case 1:
        start = 0;
        if (D_8015EDB0.unk_04 >= 0x3E8) {
            start = 1;
        } else {
            /* FAKE: redundant else; the original has the branch over an empty else (T-17113) */
            start = 0;
        }
        i = start;
        if (start < 6) {
            r = &(&D_8015EBAC)[start * 2];
            do {
                m->rect.unk_10 = r[50] * m->dx / 1000;
                m->rect.unk_14 = r[50] * m->dy / 1000;
                func_8013E97C(i + 0x32, m->rect.unk_10, m->rect.unk_14);
                func_8013E7C0(i + 0x32, 6, 1, 1);
                i++;
                r += 2;
            } while (i != 6);
        }
        if (D_8015EDB0.unk_04 < 0x3E8) {
            if (D_8015EBAC == 0) {
                D_8015EDB0.unk_04 += 0x3C;
            } else {
                D_8015EDB0.unk_04 += 0x28;
            }
            D_8015EC74 = D_8015EDB0.unk_04;
        }
        for (i = 0; i < 0x31; i++) {
            (&D_8015ED38)[-i] = (&D_8015ED34)[-i];
        }
        if (D_8015ECC4 == D_8015EC74) {
            for (i = 1; i < 6; i++) {
                func_8013E7C0(i + 0x32, 6, 1, 0);
            }
        }
        if (D_8015EDB0.unk_04 >= 0x3E8 && D_8015EDB0.unk_04 < 0x7D0) {
            func_8013E97C(0x32, D_8015EBFC.unk_2C, D_8015EBFC.unk_30);
            func_8013E810(0x32, 7, 5, 1);
            D_8015EDB0.unk_04 = 0x7D1;
        } else if (!(D_8012139A & 1)) {
            D_8015EDB0.unk_04 = 0x3E8;
            D_8015EDB0.unk_08 += 1;
        }
        if (D_8015EDB0.unk_04 >= 0x3E8) {
            func_8013F0F4(0x508, 0xA, 1);
            for (i = 0; i < 4; i++) {
                if (func_8013E9A8(0x14) == 0) {
                    x = func_8013E9A8(0x20);
                    y = func_8013E9A8(0x20);
                    func_8013E97C(i + 0x28, x + D_8015EBFC.unk_2C - 0x10, y + D_8015EBFC.unk_30 - 0x10);
                    func_8013E810(i + 0x28, 8, 1, 1);
                }
            }
        }
        if (D_8015EDB0.unk_08 >= 8) {
            for (i = 0; i < 4; i++) {
                func_8013E810(i + 0x28, 0xFF, 1, 0);
            }
            func_8014EBA8();
        }
        return;
    }
}

void func_80148B08(void) {
    RpgMotion20 *motion = (RpgMotion20 *)&D_8015EC50;

    switch (D_8015EDB0.unk_00) {
    case 0:
        switch (D_8015EBAC) {
        case 0:
            motion->rect.unk_10 = 0x98;
            motion->rect.unk_14 = 0x68;
            break;
        case 1:
            motion->rect.unk_10 = 0x60;
            motion->rect.unk_14 = 0x58;
            break;
        case 2:
            motion->rect.unk_10 = 0x58;
            motion->rect.unk_14 = 0x88;
            break;
        case 3:
        case 4:
            motion->rect.unk_10 = 0x80;
            motion->rect.unk_14 = 0x80;
            break;
        }
        func_8013E97C(0x32, motion->rect.unk_10, motion->rect.unk_14);
        func_8013E810(0x32, 9, 1, 1);
        func_8014EBD0();
    default:
        break;
    case 1:
        func_8014ED80(0xA);
        break;
    case 2:
        if (D_8015EDC4 & 0x100000) {
            func_8013E810(0x32, 0xB, 5, 1);
        } else {
            func_8013E810(0x32, 0xA, 5, 1);
        }
        func_8013F0F4(0x507, 1, 0);
        func_8014EBD0();
        break;
    case 3:
        if (D_8015EDC4 & 0x100000) {
            if (D_801213B0 == 9) {
                func_8013F0F4(0x509, 0, 5);
                func_8013E97C(0x28, motion->rect.unk_10 - 0x23, motion->rect.unk_14 - 0x14);
                func_8013E810(0x28, 8, 5, 1);
            }
        } else if (D_801213B0 == 0xE) {
            func_8013F0F4(0x509, 0, 5);
            if (D_8015EDC4 & 0x08000000) {
                func_8013E97C(0x28, 0x48, 0x38);
            } else {
                func_8013E97C(0x28, 0x4D, 0x24);
            }
            func_8013E810(0x28, 8, 5, 1);
        }
        if (!(D_801210F2 & 1)) {
            func_8013E810(0x28, 8, 1, 0);
        }
        if (!(D_8012139A & 1)) {
            func_8013D3E4();
            func_8014EBD0();
        }
        break;
    case 4:
        motion->dx = 0x105 - motion->rect.unk_10;
        motion->dy = 0x56 - motion->rect.unk_14;
        func_8013E97C(0x32, ((s32) (motion->dx * D_8015EDB8) / 1000) + motion->rect.unk_10, (motion->rect.unk_14 + ((s32) (motion->dy * D_8015EDB8) / 1000)) - (func_800A0070((s32) (D_8015EDB8 << 0xB) / 1000) >> 7));
        D_8015EDB8 += 0x14;
        func_8013E810(0x32, 0xC, 1, 1);
        if (D_8015EDB8 >= 0x3E9) {
            func_8013F0F4(0x502, 1, 0);
            func_8014EBD0();
        }
        break;
    case 5:
        func_8013E810(0x32, 0xFF, 1, 0);
        func_8013E97C(0x34, 0, 0);
        func_8013E7C0(0x34, 2, 1, 1);
        func_8014EBA8();
        break;
    }
}
