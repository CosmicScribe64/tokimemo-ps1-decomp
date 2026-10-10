#include "common.h"
#include "ovl/EVENT.h"

void func_8010A210(void) {
    D_80123E90 = 0x801B0000;
    D_80123E94 = 0x801B2000;
    D_80123E98 = 0x801B6000;
    D_80123E9C = 0x801BA000;
    D_80123EA0 = 0x801BE000;
    D_80123EA4 = 0x801C2000;
    D_80123EA8 = 0x801C6000;
}

void func_8010A280(void) {
    D_80123EAC = 0x801CE150;
    D_80123EB0 = 0x801CE154;
    D_80123EB4 = 0x801CE1A4;
    D_80123EB8 = *(s16 *)0x801CE1BC;
    D_80123EBC = 0x801B0000;
    D_80123EC0 = 0x801B2000;
    D_80123EC4 = 0x801B6000;
    D_80123EC8 = 0x801BA000;
    D_80123ECC = 0x801BE000;
    D_80123ED0 = 0x801C2000;
    D_80123ED4 = 0x801C6000;
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010A210", func_8010A330);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010A210", func_8010A3D0);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010A210", func_8010A430);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010A210", func_8010A490);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010A210", func_8010A4F0);

void func_8010A584(void) {
    func_800433D0(0x500);
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010A210", func_8010A5AC);

void func_8010A628(void) {
    func_80015D28(0, 0x801B0000, 0);
    func_80011DFC();
}

void func_8010A658(void) {
    func_80012D64(D_80123E90, 0x11, 1, 2, 0);
    func_8004C46C(D_80123E94, D_80123E98, D_80123E9C, D_80123EA0, D_80123EA4, D_80123EA8);
    func_8004C6B0(0, 0, 0, 0);
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010A210", func_8010A6E4);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010A210", func_8010A790);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010A210", func_8010A7E0);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010A210", func_8010A91C);

void func_8010AAAC(void) {
    D_80120678 = D_8012062C;
    D_8012067C = D_8012064C;
    D_80120680 = D_8012066C;
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010A210", func_8010AAF8);

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010A210", func_8010AC40);

void func_8010ACF8(void) {
    func_800469F4(0x3FE8);
    func_80011DFC();
}

void func_8010AD20(void) {
    func_800469F4(0x3FDE);
    func_80011DFC();
}

void func_8010AD48(void) {
    func_800469F4(0x4678);
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010A210", func_8010AD70);

void func_8010AE04(void) {
    func_800469F4(0x4284);
    func_80011DFC();
}

void func_8010AE2C(void) {
    func_800469F4(0x4296);
    func_80011DFC();
}

void func_8010AE54(void) {
    func_800469F4(0x473D);
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010A210", func_8010AE7C);

void func_8010AF40(void) {
    func_80015D28(0x3D, 0x801B0000, 0x8BD8);
    func_8010A280();
    func_80011DFC();
}

void func_8010AF78(void) {
    func_800143DC(1, 0xB19B, 0xB178, 0xBE66, 0xBE24, 0xBE21);
    func_80011DFC();
}

void func_8010AFBC(void) {
    func_800335E0(0x692F);
    func_80011DFC();
}

void func_8010AFE4(void) {
    func_8001A1EC(0x120, -0xF8, 0xA0, 0x3C, -0xA0, 0x3C, 9, 0x808080, 0);
    func_8001A1EC(0x120, -0xF8, 0x40, 0x3C, 0, 0xB4, 9, 0xA0A0A0, 0);
    func_8001A1EC(0x120, -0xF8, 0x10, 0x3C, 0, 0xB4, 9, 0xA0A0A0, 0x40);
    func_8001A1EC(0x120, -0xF8, -0x1C, 0x3C, 0, 0xB4, 9, 0xA0A0A0, 0x63);
    func_8001A1EC(0x120, -0xF8, -0x50, 0x3C, 0, 0xB4, 9, 0xA0A0A0, 0x24);
    func_8001A1EC(0x120, -0xF8, -0x8C, 0x3C, 0, 0xB4, 9, 0xA0A0A0, 0x73);
    func_8001A1EC(0x120, -0xF8, -0xF0, 0x3C, 0, 0xB4, 9, 0xC0C0C0, 0x11);
    func_8001A1EC(0x120, -0xF8, -0x140, 0x3C, 0, 0xB4, 9, 0xC0C0C0, 0x50);
    func_8001A1EC(0x120, -0xF8, -0x30, 0x3C, 0, 0xB4, 9, 0x808080, 0);
    func_8001A0CC(0, 0, 9, 1, 0);
}

void func_8010B228(void) {
    func_800F7074();
    D_80094714 = (u16) D_80094714 + 1;
    D_80094718 = 0;
    func_80011DFC();
}

void func_8010B268(void) {
    func_800201FC(1);
    D_800EECC0 = 0;
    D_800EECCC = 0;
    func_80011DFC();
}

INCLUDE_ASM("asm/ovl/EVENT/nonmatchings/EVENT/8010A210", func_8010B29C);

void func_8010B330(void) {
    func_800335E0(0x692F);
    func_80011DFC();
}
