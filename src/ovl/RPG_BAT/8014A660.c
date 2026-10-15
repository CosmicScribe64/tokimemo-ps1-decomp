#include "common.h"
#include "ovl/RPG_BAT.h"

void func_8014A660(void) {
    switch (D_8015EDAC) {
    case 0:
        func_800AE0F0(D_8015E74C, "　　　　　　　水竜破　　　　　　　");
        func_8014B738((s32) &D_8015E814, 1, 0x5A);
        func_80144480();
        func_8013EEAC(D_8015EBAC);
        func_80148F70();
        func_8013E54C(3);
        D_8015EC74 = 0;
        D_8015EC78 = 0;
        D_8015EC7C = 0;
        D_8015EC80 = 0;
        D_8015ECEC = 0;
        D_8015ECF0 = 0;
        D_8015ECF4 = 0;
        D_8015ECF8 = 0;
        func_8014EBA8();
        func_8014EBA8();
        return;
    case 2:
        func_8013D418();
        return;
    case 3:
        func_8013E7C0(0x34, 3, 1, 1);
        func_8013E97C(0x33, 0xD0, 0x70);
        func_8013E810(0x33, 5, 1, 1);
        func_8014EBA8();
        return;
    case 4:
        func_8014ED44(0x6C);
        return;
    case 5:
        func_8014AB84(0, 0xD0, 0x70);
        func_8013E810(0x33, 5, 1, 0);
        func_8013E7C0(0x34, 4, 1, 1);
        switch (D_8015EBAC) {
        case 0:
            D_80121685 = 4;
            break;
        case 1:
            D_801215FD = 4;
            D_80121685 = 4;
            break;
        }
        func_8014EBA8();
        return;
    case 6:
        /* FAKE: scalar pointer views keep the promoted counter in v1 rather
         * than v0. Direct member access changes 31 instructions. T-12300 */
        (*(s32 *)&D_8015EDB0.unk_04)++;
        switch (D_8015EBAC) {
        case 0:
            switch (*(s32 *)&D_8015EDB0.unk_04) {
            case 9:
                func_8014AB84(1, 0xB3, 0x78);
                break;
            case 18:
                func_8014EBA8();
                break;
            }
            break;
        case 1:
            switch (*(s32 *)&D_8015EDB0.unk_04) {
            case 9:
                func_8014AB84(1, 0xB4, 0x71);
                break;
            case 18:
                func_8014AB84(2, 0x98, 0x72);
                break;
            case 27:
                func_8014AB84(3, 0x7C, 0x73);
                break;
            case 36:
                func_8014EBA8();
                break;
            }
            break;
        case 2:
            switch (*(s32 *)&D_8015EDB0.unk_04) {
            case 9:
                func_8014AB84(1, 0xB3, 0x7A);
                break;
            case 18:
                func_8014AB84(2, 0x96, 0x84);
                break;
            case 27:
                func_8014AB84(3, 0x79, 0x8D);
                break;
            case 36:
                func_8014EBA8();
                break;
            }
            break;
        default:
            switch (*(s32 *)&D_8015EDB0.unk_04) {
            case 9:
                func_8014AB84(1, 0xB4, 0x7A);
                break;
            case 18:
                func_8014AB84(2, 0x98, 0x84);
                break;
            case 27:
                func_8014AB84(3, 0x7C, 0x8D);
                break;
            case 36:
                func_8014EBA8();
                break;
            }
            break;
        }
        func_8014AC74();
        return;
    case 7:
        func_8014AC74();
        switch (D_8015EBAC) {
        case 0:
            func_8014ADD0(0x78, 0x82);
            return;
        case 1:
            func_8014ADD0(0x40, 0x72);
            return;
        case 2:
            func_8014ADD0(0x3C, 0xA2);
            return;
        default:
            func_8014ADD0(0x40, 0xA2);
            return;
        }
        break;
    case 8:
        func_8014EBA8();
        return;
    case 9:
        func_8013E54C(0xFF);
        func_8013D3E4();
        func_8014EBA8();
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

#ifdef NON_MATCHING
/* NON_MATCHING: T-10320, only the register of the reloaded arg0 differs ($a3 vs $t9), 6 lines */
void func_8014AB84(s32 arg0, s32 arg1, s32 arg2) {
    func_8013F0F4(0x500, 1, 0);
    *(s32 *)((u8 *)&D_8015EBAC + arg0 * 4 + 0xC8) = 1;
    *(s32 *)((u8 *)&D_8015EBAC + arg0 * 8 + 0xDC) = arg1;
    *(s32 *)((u8 *)&D_8015EBAC + arg0 * 8 + 0xE0) = arg2;
    func_8013E97C(arg0 * 3 + 4, arg1, arg2);
    func_8013E97C(arg0 * 3 + 5, arg1, arg2);
    func_8013E97C(arg0 * 3 + 6, arg1, arg2);
    func_8013E810(arg0 * 3 + 4, 6, 5, 1);
    func_8013E810(arg0 * 3 + 5, 8, 1, 1);
    func_8013E810(arg0 * 3 + 6, 0xD, 5, 1);
}
#else
INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014A660", func_8014AB84);
#endif

#ifdef NON_MATCHING
/* NON_MATCHING: T-10320, temp numbering only: the original builds the D_8015EBAC base in $t0 before the i*8 product, 16 lines */
void func_8014AC74(void) {
    s32 i;
    s32 *p;
    u8 *rec;
    s32 t;
    s32 t5;
    u8 *ent;
    s32 v;

    p = &D_8015EBAC;
    i = 0;
    do {
        if (p[0x32] == 1) {
            t = i * 3;
            rec = D_8011ECD0 + t * 0x44;
            t5 = t + 5;
            ent = (u8 *)&D_8015EBAC + i * 8;
            if (!(rec[0x1A92] & 1)) {
                func_8013E810(t + 4, 7, 1, 1);
            }
            v = *(s16 *)(rec + 0x1B30);
            if (v >= 0x10) {
                v = 0x1E - v;
            }
            func_8013E97C(t5, *(s32 *)(ent + 0xDC), *(s32 *)(ent + 0xE0) - v * 8);
            if (!(rec[0x1B1A] & 1)) {
                func_8013E810(t + 4, 0xFF, 1, 0);
                func_8013E810(t5, 0xFF, 1, 0);
                func_8013E810(t + 6, 0xFF, 1, 0);
            }
        }
        i++;
        p++;
    } while (i != 4);
}
#else
INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/8014A660", func_8014AC74);
#endif
