#include "common.h"
#include "ovl/NAME_ENT.h"

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80142A80", func_80142A80);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80142A80", func_80142E24);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80142A80", func_80143498);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80142A80", func_80143580);

void func_80144654(void) {
    if (D_800E7200 & 0x10) {
        k_disp_switch(0, 1);
        k_disp_switch(1, 1);
        k_disp_switch(2, 1);
        k_disp_switch(3, 1);
        func_80049A40(-0x9C, 8, 0x120, 0x58, 2, 0x808080, 0x83);
        dtd_on_tpage(0, 0, 2, 0, 0);
        return;
    }
    k_disp_switch(0, 0);
    k_disp_switch(1, 0);
    k_disp_switch(2, 0);
    k_disp_switch(3, 0);
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80142A80", func_8014472C);

void func_80144908(void) {
    if (!(D_80120696 & 1) && ((rand() & 0x7F) == 0x7F)) {
        D_80120695 = 4;
        D_80120696 = 3;
        D_801206CC = 0x41000000;
        D_80120697 = 0xA4;
        D_801206A0 = 0x801B27B0;
        D_801206A4 = 0x801B27D8;
        D_801206C8 = 0x801B27AC;
        D_801206AA = 0;
        D_80120698 = 8;
        D_80120699 = 0x80;
        D_801206BA = -0xA0;
        D_801206BE = -0x78;
    }
}

void func_801449DC(void) {
    func_8004AC18(0xFF);
    func_800452C4();
    func_80041168(0);
    func_80041584();
    func_80048DD0(0);
    func_80044750(0x71);
    func_80044750(0x7F);
    func_80042808();
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80142A80", func_80144A34);
