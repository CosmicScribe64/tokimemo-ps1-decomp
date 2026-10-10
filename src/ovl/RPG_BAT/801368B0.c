#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801368B0", func_801368B0);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801368B0", func_80136A9C);

void func_80136BD8(void) {
    func_8013F0F4(0x504, 0, 4);
    if (D_8015EDB0.unk_04 < 0x14) {
        if (!(D_8015EDB0.unk_04 & 1)) {
            func_8013F3F0(0, 4);
        } else {
            func_8013F3F0(0, -4);
        }
    }
    if (D_8015EDB0.unk_04 == 0xF) {
        func_8013E97C(0x2A, D_8015EC28, D_8015EC6C + 0x78);
        func_8013E810(0x2A, 5, 5, 1);
    }
    D_8015EDB0.unk_04 += 1;
    if (!(D_8012117A & 1)) {
        func_8013E810(0x2A, 0xFF, 1, 0);
        func_8014EBA8();
    }
}
INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801368B0", func_80136CCC);
