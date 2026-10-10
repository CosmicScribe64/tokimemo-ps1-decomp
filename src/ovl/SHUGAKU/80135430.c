#include "common.h"
#include "ovl/SHUGAKU.h"

void func_80135430(void) {
    D_8013C2D0 = 0x801C25B8;
    D_8013C2D4 = 0x801C25C8;
    D_8013C2D8 = 0x801C2674;
    D_8013C2DC = *(s16 *)0x801C26A0;
    D_8013C2E0 = 0x801B0000;
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80135430", func_80135480);

void func_801355D4(void) {
    if (D_8013C2EC != 0) {
        func_80139018();
        return;
    }
    func_80135CC8();
}

void func_80135610(void) {
    func_80042940(0x1F);
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80135430", func_80135630);

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80135430", func_801357AC);

void func_80135938(void) {
    func_80044750(0x203);
    func_8004284C();
}

void func_80135960(void) {
    if (D_8013C2EC == 0) {
        func_80044750(0x202);
    }
    func_80042908(5);
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80135430", func_80135994);

void func_80135A04(void) {
    func_80046318(0x25, 0x801B0000, 0x8240);
    func_80135430();
    func_8004284C();
}

void func_80135A3C(void) {
    func_800853FC();
    D_80120657 = D_800B593C;
    D_8012069B = D_800B593C;
    D_801206DF = D_800B593C;
    D_80120723 = D_800B593C;
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80135430", func_80135A80);

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80135430", func_80135AF4);

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80135430", func_80135B68);

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80135430", func_80135BB8);

void func_80135C14(void) {
    D_80120666 = 2;
    D_801206AA = 0;
    D_801206DA = 0x40;
    D_801206F0 = 0x14;
    func_8004284C();
}

void func_80135C5C(void) {
    D_8013C2E4 = 0;
    D_800B5BD4 = 0xF;
    func_8004284C();
}

void func_80135C8C(void) {
    func_80044750(0x203);
    bg_read_sub2(0x4761);
    func_80085B3C(2, 0x13);
    func_8004284C();
}

void func_80135CC8(void) {
    func_80044750(0x203);
    bg_read_sub2(0x4761);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80135430", func_80135CF8);

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80135430", func_80135DB8);

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80135430", func_80135EA4);

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80135430", func_801360B0);
