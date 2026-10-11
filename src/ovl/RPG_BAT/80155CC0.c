#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80155CC0", func_80155CC0);

void func_80155E80(void) {
    func_8013F0F4(0x50E, 0, 3);
    switch (D_8015EDB0.unk_00) {
    case 0:
        switch (*(u32 *)&D_8015EBAC) {
        case 0:
            D_80121685 = 4;
            break;
        case 1:
            D_801215FD = 4;
            D_80121685 = 4;
            break;
        }
        if (D_8015EDC4 & 0x100000) {
            /* FAKE: D_8015EC50/54 reached through D_8015EC28 (one object for as1) keep the second load after the first store; the real aggregate is unknown. T-9320 */
            (&D_8015EC28)[10] = (&D_8015EC28)[0] - 0xEC;
            (&D_8015EC28)[11] = (&D_8015EC28)[1] - 0x17;
        } else {
            D_8015EC50 = -0xA4;
            D_8015EC54 = 0x74;
        }
        func_8013E97C(0x33, 0xEC, 0x2C);
        func_8013E810(0x33, 5, 1, 1);
        func_8014EBD0();
        break;
    case 1:
        func_8013E97C(0x33, D_8015EC50 * D_8015EDB0.unk_04 / 1000 + 0xEC, D_8015EC54 * D_8015EDB0.unk_04 / 1000 + 0x2C);
        D_8015EDB0.unk_04 = D_8015EDB0.unk_04 + 0x32;
        if (D_8015EDB0.unk_04 >= 0x3E9) {
            func_8014EBA8();
        }
        break;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80155CC0", func_80156064);
