#include "common.h"
#include "ovl/ENDING.h"

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80132000", func_80132000);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80132000", func_80132334);

void func_80132400(void) {
    func_80042878(0xC2);
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80132000", func_80132420);

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80132000", func_80132B04);

void func_80132D98(void) {
    D_800CA134 = (u8 *)&D_8013C358;
    D_800CA138 = (u8 *)&D_8013C35C;
    D_800CA13C = D_8013C34C;
    D_800CA140 = D_8013C350;
    D_800CA144 = D_8013C354;
    func_80132B04(0xFF, 1, 1);
}

void func_80132E10(void) {
    func_80044890(1, 0xBF98, 0xBF79, 0xCE6D, 0xCE33, 0xCE1E);
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80132000", func_80132E70);

void func_80132EF0(void) {
    func_80044750(0x200);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/ENDING/nonmatchings/ENDING/80132000", func_80132F18);
