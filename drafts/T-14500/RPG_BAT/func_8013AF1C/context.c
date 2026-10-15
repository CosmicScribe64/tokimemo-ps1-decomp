#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8013AD50", func_8013AD50);

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E6354;
extern s8 D_80121316;
extern s8 D_8012135A;
s32 func_800AE100();
void func_8013B534(void);
s32 func_8013B2F0(void);
/* end bulk_m2c declarations */
void func_8013AF1C(void) {
    s32 temp_v0;
    s32 temp_v0_2;

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
        temp_v0 = D_8015EDB4 + 3;
        D_8015EDB4 = temp_v0;
        if (temp_v0 >= 0x21) {
            func_8013E97C(0x30, -0x20, 0);
            func_8013E7C0(0x30, 0, 1, 1);
            func_8014EBA8();
            return;
        }
    default:
        return;
    case 2:
        func_800AE0F0(D_8015E74C, &D_800E6354);
        func_800AE100(D_8015E74C, "の攻撃");
        if (D_8015EBB0 == 0) {
            func_800AE0F0(D_8015E774, D_8015E74C);
            func_800AE100(D_8015E774, "は失敗した");
        } else {
            func_8014D11C((s32) D_8015E774, (s32) &D_800E6354, (s32) "は", D_8015EBB0, (s32) "のダメージを与えた");
        }
        func_8014B738((s32) &D_8015E814, 2, 0x3C);
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
        temp_v0_2 = D_8015EDB4 + 3;
        D_8015EDB4 = temp_v0_2;
        if (temp_v0_2 >= 0x21) {
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
