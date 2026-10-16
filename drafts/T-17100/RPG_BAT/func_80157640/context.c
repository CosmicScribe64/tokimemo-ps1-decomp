#include "common.h"
#include "ovl/RPG_BAT.h"

void func_80156CC0(void) {
    switch (D_8015EDD8) {
    case 0:
        func_800AE0F0(D_8015E828, "宇宙人「Ж○ξ♂×♀？〆£∞Й！");
        func_8014B738(D_8015E8F0, 1, 0x5A);
        func_8013F15C(0x512, 1, 0);
        func_8014EF3C();
        return;
    case 1:
        func_8014F178(0);
        return;
    case 2:
        func_8014F080(0x14);
        return;
    case 3:
        func_8014EF8C();
        return;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80156CC0", func_80156D78);

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80156CC0", func_80157158);

/* bulk_m2c declarations (T-11190) */
s32 func_800AE100();
/* end bulk_m2c declarations */
void func_80157640(void) {
    s32 temp_v0;

    switch (D_8015EDD8) {
    case 0:
        D_8015EC38 = 0x110 - D_8015EDE0;
        D_8015EDF0 = 0;
        D_8015EDE0 += 4;
        func_8013E97C(0x1D, 0, 0);
        func_8013E7C0(0x1D, 0x18, 1, 1);
        if (D_8015EDE0 >= 0xD1) {
            func_8014EF3C();
        default: ;
        }
        break;
    case 1:
        D_8015EDF0 = 0;
        func_8013E97C(0x1D, 0, 0);
        func_8013E7C0(0x1D, 0x18, 1, 1);
        func_8014EF3C();
        break;
    case 2:
        func_8014F080(0x1E);
        break;
    case 3:
        func_800AE0F0(D_8015E828, D_800E6280.unk_0D4);
        func_800AE100(D_8015E828, "達は逃げ出した。");
        func_8014B738((s32) D_8015E8F0, 1, 0x5A);
        func_8014EF3C();
        break;
    case 4:
        func_8014F178(0);
        break;
    case 5:
        ((DisplayActor44 *)D_8011ECD0)[125].unk_03 = 0x81;
        func_8013E7C0(0x30, 2, 1, 1);
        func_8013F15C(0x503, 1, 0);
        func_8014EF3C();
        break;
    case 6:
        ((DisplayActor44 *)D_8011ECD0)[125].unk_26 = (s16) (((DisplayActor44 *)D_8011ECD0)[125].unk_26 + 4);
        ((DisplayActor44 *)D_8011ECD0)[144].unk_26 = (s16) (((DisplayActor44 *)D_8011ECD0)[144].unk_26 + 4);
        temp_v0 = D_8015EDE0 + 1;
        D_8015EDE0 = temp_v0;
        if (temp_v0 >= 0x21) {
            func_8014EF3C();
        }
        break;
    case 7:
        func_8014F080(0x3C);
        break;
    case 8:
        D_8015EBFC.unk_18 = 1;
        func_80042808();
        break;
    }
}

void func_8015785C(void) {
    func_8013E97C(0x2F, D_8015EC38, D_8015EC3C);
    func_8013E97C(0x2E, D_8015EC38, D_8015EC3C);
    func_8013E97C(0x2A, D_8015EC38 - 0x67, D_8015EC3C - 6);
    func_8013E97C(0x2B, D_8015EC38 - 0x43, D_8015EC3C + 6);
    func_8013E97C(0x2C, D_8015EC38 + 0x43, D_8015EC3C + 6);
    func_8013E97C(0x2D, D_8015EC38 + 0x67, D_8015EC3C - 6);
    if (!(D_8012117A & 1)) {
        func_8013E810(0x2A, 0xFF, 1, 0);
        D_8015EC40 = 0;
    }
    if (!(D_801211BE & 1)) {
        func_8013E810(0x2B, 0xFF, 1, 0);
        D_8015EC44 = 0;
    }
    if (!(D_80121202 & 1)) {
        func_8013E810(0x2C, 0xFF, 1, 0);
        D_8015EC48 = 0;
    }
    if (!(D_80121246 & 1)) {
        func_8013E810(0x2D, 0xFF, 1, 0);
        D_8015EC4C = 0;
    }
    if ((D_8015EC40 == 0) && (func_8013E9A8(0x64) == 0)) {
        func_8013E810(0x2A, 7, 5, 1);
        D_8015EC40 = 1;
    }
    if ((D_8015EC44 == 0) && (func_8013E9A8(0x64) == 0)) {
        func_8013E810(0x2B, 7, 5, 1);
        D_8015EC44 = 1;
    }
    if ((D_8015EC48 == 0) && (func_8013E9A8(0x64) == 0)) {
        func_8013E810(0x2C, 7, 5, 1);
        D_8015EC48 = 1;
    }
    if (D_8015EC4C == 0) {
        if (func_8013E9A8(0x64) == 0) {
            func_8013E810(0x2D, 7, 5, 1);
            D_8015EC4C = 1;
        }
    }
}
