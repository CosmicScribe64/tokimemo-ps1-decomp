#include "common.h"
#include "ovl/RPG_BAT.h"

/* NON_MATCHING: T-16210, 20 differing lines; the two D_8015EDB8 counters (cases 7 and 16) get $v0, the original
 * $v1 (regdiff: an interfering range holds $v0 there in the original; s32 return, temporaries, struct views tried). */
#ifdef NON_MATCHING
void func_801454F0(void) {
    if (D_8015EDF0 == 0x40000000) {
        switch (D_8015EDAC) {
        case 6:
            func_80150DE0();
            func_8013E97C(0x1D, D_8015EC58.unk_08 + 0x60, D_8015EC58.unk_0C);
            func_8013E97C(0x32, D_8015EC58.unk_10 + D_8015EC58.unk_08 + 0x60, D_8015EC58.unk_14 + D_8015EC58.unk_0C);
            func_8013E810(0x1D, 0, 1, 1);
            func_8013E810(0x32, 7, 5, 1);
            D_801213B0 = 1;
            D_80120E05 = 4;
            D_80121399 = 3;
            func_8014EBA8();
            break;
        case 7:
            func_8013E97C(0x1D, (D_8015EC58.unk_08 - D_8015EDB8) + 0x60, D_8015EC58.unk_0C);
            func_8013E97C(0x32, ((D_8015EC58.unk_10 + D_8015EC58.unk_08) - D_8015EDB8) + 0x60, D_8015EC58.unk_14 + D_8015EC58.unk_0C);
            D_8015EDB8 += 2;
            if (D_8015EDB8 >= 0x61) {
                func_8014EBA8();
            }
            break;
        case 8:
            func_800AE0F0(D_8015E74C, "美樹原「いいかげんに…");
            func_800AE0F0(D_8015E774, "い、いえ…やっぱりいいです…");
            func_8014B738(&D_8015E814, 2, 0xA);
            func_8013F1C4(0x86, 0x1300008B, 0);
            D_8015EE48 = 3;
            func_8013E810(0x1D, 8, 1, 1);
            func_8014EBA8();
            break;
        case 9:
            func_8014ED44(0x14);
            break;
        case 10:
            func_8013E810(0x32, 0xD, 1, 1);
            func_8014EBA8();
            break;
        case 11:
            D_8015EDB4 += 1;
            if (D_8015EDB4 >= 0x12D) {
                func_8013E810(0x32, 0xD, 1, 0);
                D_8015EE48 = 1;
                func_8014EBA8();
            }
            if (func_800460EC() == 4) {
                D_8015ED38 = 1;
            }
            if (D_8015ED38 == 1) {
                if (func_800460EC() == 2) {
                    func_8013E810(0x32, 0xD, 1, 0);
                    D_8015EE48 = 1;
                    D_8015ED38 = 0;
                    func_8014EBA8();
                }
            }
            break;
        case 12:
            func_8013F1C4(0x8C, 0x14000099, 1);
            D_8015EE48 = 3;
            func_8014EBA8();
            break;
        case 13:
            func_8014ED44(5);
            break;
        case 14:
            D_8015EDB4 += 1;
            if (D_8015EDB4 >= 0x12D) {
                D_8015EE48 = 1;
                D_8015EE4C = D_8015E744 + 1;
                func_8013E810(0x1D, 0, 1, 1);
                func_8014EBA8();
            }
            if (func_800460EC() == 4) {
                D_8015ED38 = 1;
            }
            if (D_8015ED38 == 1) {
                if (func_800460EC() == 2) {
                    D_8015EE48 = 1;
                    D_8015EE4C = D_8015E744 + 1;
                    func_8013E810(0x1D, 0, 1, 1);
                    D_8015ED38 = 0;
                    func_8014EBA8();
                }
            }
            break;
        case 15:
            func_8014ED44(0x1E);
            break;
        case 16:
            func_8013E97C(0x1D, D_8015EC58.unk_08 + D_8015EDB8, D_8015EC58.unk_0C);
            func_8013E97C(0x32, D_8015EC58.unk_10 + D_8015EC58.unk_08 + D_8015EDB8, D_8015EC58.unk_14 + D_8015EC58.unk_0C);
            D_8015EDB8 += 2;
            if (D_8015EDB8 >= 0x61) {
                func_8014EBA8();
            }
            break;
        case 17:
            D_8015EDAC = 0x64;
            break;
        }
    } else {
        switch (D_8015EDAC) {
        case 6:
            func_801516E4();
        default:
            break;
        case 7:
            func_800AE0F0(D_8015E74C, "美樹原「いいかげんに…");
            func_800AE0F0(D_8015E774, "い、いえ…やっぱりいいです…");
            func_8014B738(&D_8015E814, 2, 0xA);
            func_8013F1C4(0x86, 0x1300008B, 0);
            D_8015EE48 = 3;
            func_8013E810(0x1D, 8, 1, 1);
            func_8014EBA8();
            break;
        case 8:
            func_8014ED44(0x14);
            break;
        case 9:
            func_8013E810(0x32, 0xD, 1, 1);
            func_8014EBA8();
            break;
        case 10:
            D_8015EDB4 += 1;
            if (D_8015EDB4 >= 0x12D) {
                func_8013E810(0x32, 0xD, 1, 0);
                D_8015EE48 = 1;
                func_8014EBA8();
            }
            if (func_800460EC() == 4) {
                D_8015ED38 = 1;
            }
            if (D_8015ED38 == 1) {
                if (func_800460EC() == 2) {
                    func_8013E810(0x32, 0xD, 1, 0);
                    D_8015EE48 = 1;
                    D_8015ED38 = 0;
                    func_8014EBA8();
                }
            }
            break;
        case 11:
            func_8013F1C4(0x8C, 0x14000099, 1);
            D_8015EE48 = 3;
            func_8014EBA8();
            break;
        case 12:
            func_8014ED44(5);
            break;
        case 13:
            D_8015EDB4 += 1;
            if (D_8015EDB4 >= 0x12D) {
                D_8015EE48 = 1;
                D_8015EE4C = D_8015E744 + 1;
                func_8013E810(0x1D, 0, 1, 1);
                func_8014EBA8();
            }
            if (func_800460EC() == 4) {
                D_8015ED38 = 1;
            }
            if (D_8015ED38 == 1) {
                if (func_800460EC() == 2) {
                    D_8015EE48 = 1;
                    D_8015EE4C = D_8015E744 + 1;
                    func_8013E810(0x1D, 0, 1, 1);
                    D_8015ED38 = 0;
                    func_8014EBA8();
                }
            }
            break;
        case 14:
            func_8014ED44(0x1E);
            break;
        case 15:
            func_80151984();
            break;
        case 16:
            D_8015EDAC = 0x64;
            break;
        }
    }
}
#else
INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801454F0", func_801454F0);
#endif
