#include "common.h"
#include "ovl/EVENT.h"

void func_8011C650(void) {
    D_801258C0 = 0x801C6000;
    D_801258C4 = 0x801AE000;
    D_801258C8 = 0x801B2000;
    D_801258CC = 0x801B6000;
    D_801258D0 = 0x801BA000;
    D_801258D4 = 0x801BE000;
    D_801258D8 = 0x801C2000;
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8011C650", func_8011C6C4);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8011C650", func_8011C750);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8011C650", func_8011D5C4);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8011C650", func_8011DDB8);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8011C650", func_8011ED50);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8011C650", func_8011EDFC);
