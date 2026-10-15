#include "common.h"
#include "ovl/RPG_BAT.h"

/* bulk_m2c declarations (T-11190) */
void func_8014A16C(void);
void func_80149AA8(void);
/* end bulk_m2c declarations */
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

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801498C0", func_8014A3DC);
