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

/* FAKE: the slot table at D_8015EBAC (flag words at +0xC8, x/y pairs at +0xDC,
 * see func_8014AC74) is not recovered and other files use its words as
 * scalars, so the stores go through the first symbol. Element indexes keep the
 * reload of arg0 in the ugen temporary $t9 (byte-offset casts gave $a3).
 * T-17108. */
void func_8014AB84(s32 arg0, s32 arg1, s32 arg2) {
    func_8013F0F4(0x500, 1, 0);
    (&D_8015EBAC)[arg0 + 0x32] = 1;
    (&D_8015EBAC)[arg0 * 2 + 55] = arg1;
    (&D_8015EBAC)[arg0 * 2 + 56] = arg2;
    func_8013E97C(arg0 * 3 + 4, arg1, arg2);
    func_8013E97C(arg0 * 3 + 5, arg1, arg2);
    func_8013E97C(arg0 * 3 + 6, arg1, arg2);
    func_8013E810(arg0 * 3 + 4, 6, 5, 1);
    func_8013E810(arg0 * 3 + 5, 8, 1, 1);
    func_8013E810(arg0 * 3 + 6, 0xD, 5, 1);
}

/* FAKE: the slot table at D_8015EBAC (flag words at +0xC8, x/y pairs at +0xDC)
 * is not recovered and other files use its words as scalars, so it is read
 * through the first symbol, written inline at every use. The display records
 * use the typed DisplayActor44 view; an `ent` pointer local and byte-offset
 * record casts made the base address a ugen temporary (every later t-register
 * one too low). T-17108. */
void func_8014AC74(void) {
    s32 i;
    s32 t;
    s32 t5;
    s32 v;

    for (i = 0; i < 4; i++) {
        if ((&D_8015EBAC)[i + 0x32] == 1) {
            t = i * 3;
            t5 = t + 5;
            if (!(((DisplayActor44 *)D_8011ECD0)[t + 100].unk_02 & 1)) {
                func_8013E810(t + 4, 7, 1, 1);
            }
            v = ((DisplayActor44 *)D_8011ECD0)[t + 102].unk_18;
            if (v >= 0x10) {
                v = 0x1E - v;
            }
            func_8013E97C(t5, (&D_8015EBAC)[i * 2 + 55], (&D_8015EBAC)[i * 2 + 56] - v * 8);
            if (!(((DisplayActor44 *)D_8011ECD0)[t + 102].unk_02 & 1)) {
                func_8013E810(t + 4, 0xFF, 1, 0);
                func_8013E810(t5, 0xFF, 1, 0);
                func_8013E810(t + 6, 0xFF, 1, 0);
            }
        }
    }
}
