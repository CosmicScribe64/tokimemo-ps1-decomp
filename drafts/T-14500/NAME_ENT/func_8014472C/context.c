#include "common.h"
#include "ovl/NAME_ENT.h"

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80142A80", func_80142A80);

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80142A80", func_80142E24);

void func_80143498(void) {
    if ((func_800460EC() & 2) && D_8014D170 == 0) {
        func_80046290(0x06000CBB, 0x12000E0C, 0xF);
        func_80044750(0x300);
        func_8004E58C();
        func_8004E788(-0x90, 0x14, 0, "ここ、きらめき高校には、", 0);
        func_8004E788(-0x90, 0x24, 0, "一つの伝説があります。", 0);
        func_8004E788(-0x90, 0x34, 0, "校庭のはずれにある一本の古木。", 0);
        func_8004E884(3);
        func_8004E93C(0, 0);
        func_8004E93C(1, 0);
        func_8004E93C(2, 0);
        D_8014D170 = 1;
    }
}

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80142A80", func_80143580);

void func_80144654(void) {
    if (D_800E6280.unk_F80 & 0x10) {
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

/* bulk_m2c declarations (T-11190) */
extern s16 D_80125C80;
extern s32 D_800E7378;
extern s32 D_800E7380;
extern u32 D_800E7200;
/* end bulk_m2c declarations */
void func_8014472C(void) {
    s16 var_v1;

    if (func_800460EC() & 4) {
        var_v1 = getCDlevel();
    } else {
        var_v1 = 0;
        if (((u32) D_800E7200 >> 0x10) != 0) {
            var_v1 = D_800E7380 & 0x3F;
        }
    }
    (&D_80125C80)[D_800E7378 & 3] = var_v1;
    if (func_800460EC() & 4) {
        if (!((u8) D_801206DA & 1) || !(D_801206DB & 0x80)) {
            func_80048F64(0x62);
            D_801206D9 = 3;
            D_801206DA = 0x11;
            D_80120710 = 0x41000000;
            D_801206DB = 0xAC;
            D_801206E4 = 0x801B27B0;
            D_801206E8 = 0x801B27D8;
            D_8012070C = 0x801B27AC;
            D_801206EE = 1;
            D_801206DC = 8;
            D_801206DD = 0x80;
            D_801206FE = -0xA0;
            D_80120702 = -0x78;
            load_tpage_buf_lock(5, *(s32 *)0x801B27AC, 1);
        }
    } else {
        func_80048F64(0x62);
        D_801206DB = 0;
        D_801206DA = 0;
        D_801206F0 = 0;
        D_801206E0 = 0;
    }
}

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

void func_80144A34(void) {
    D_800E6280.unk_1104.w += 1;
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80142E24();
        break;
    case 1:
        func_80143580();
        break;
    case 2:
        func_801449DC();
        break;
    }
    if (D_800E6280.unk_F88 & 0x860) {
        func_80042940(2);
    }
}
