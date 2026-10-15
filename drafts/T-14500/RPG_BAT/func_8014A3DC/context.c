#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801498C0", func_801498C0);

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

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801498C0", func_80149C5C);

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

/* bulk_m2c declarations (T-11190) */
extern s32 D_8015EDA4;
/* end bulk_m2c declarations */
void func_8014A3DC(void) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;

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
            temp_v0 = D_8015EDA4 + 1;
            D_8015EDB8 = 0;
            if (temp_v0 == D_8015EC74) {
                D_8015EDA4 = temp_v0;
                func_8014EBD0();
            } else {
                D_8015EDA4 = temp_v0;
                func_8013E810(0x29, temp_v0 + 0xF, 1, 1);
                temp_v0_2 = D_8015EDA4;
                if ((temp_v0_2 == 6) || (D_8015EDA4 = temp_v0_2, (temp_v0_2 == 7))) {
                    func_8013E810(0x28, 6, 1, 1);
                }
                temp_v0_3 = D_8015EDA4;
                if ((temp_v0_3 == 8) || (D_8015EDA4 = temp_v0_3, (temp_v0_3 == 9))) {
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
