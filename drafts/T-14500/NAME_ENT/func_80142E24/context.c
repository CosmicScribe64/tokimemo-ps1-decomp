#include "common.h"
#include "ovl/NAME_ENT.h"

INCLUDE_ASM("asm/ovl/NAME_ENT/nonmatchings/NAME_ENT/80142A80", func_80142A80);

/* bulk_m2c declarations (T-11190) */
extern s8 D_800E62B6;
extern s8 D_800E62B7;
extern s8 D_800E62B8;
extern s8 D_800E62BA;
extern u8 D_800E738D;
/* end bulk_m2c declarations */
void func_80142E24(void) {
    s32 sp5C[1];
    s32 *var_v0;
    s32 var_a0;
    s32 var_a1;
    s32 var_a2;
    s32 var_v1;

    var_v1 = 0x8018C000;
    var_a2 = 0x80188000;
    var_a1 = 0x80184000;
    var_v0 = &sp5C;
    var_a0 = 0x80180000;
    do {
        *(s32 *)((unsigned char *)var_v0 + (12)) = var_v1;
        var_v1 += 0x10000;
        (*(s32 *)var_v0) = var_a0;
        *(s32 *)((unsigned char *)var_v0 + (4)) = var_a1;
        *(s32 *)((unsigned char *)var_v0 + (8)) = var_a2;
        var_a2 += 0x10000;
        var_a1 += 0x10000;
        var_a0 += 0x10000;
        var_v0 += 0x10;
    } while (var_v1 != 0x801AC000);
    switch (D_800E738D) {
    case 0:
        D_800E62BA = 0x80;
        draw2d3d(1, 0, var_a2, 0x801AC000);
        func_80048DAC(1);
        func_8006BD6C(0);
        func_80048DD0(0);
        func_80048E78();
        func_80041584();
        func_8004E58C();
        back_clear_switch(1);
        D_800E62B7 = 0;
        D_800E62B6 = 0;
        D_800E62B8 = 0;
        func_80044750(0x7F);
        func_80044890(1, 0xC592, 0xC580, 0xCFA7, 0xCF8B, 0xCF83);
        D_800E738D += 1;
        return;
    case 1:
        if (func_80044E8C() == 1) {
            func_8004500C(1, 0);
            cd_read_sectors(0x21U, 0x80180000, 0x9FD5);
            D_800E738D += 1;
            return;
        }
    default:
        return;
    case 2:
        if (cd_read_sync_status() & 1) {
            func_8004A1A0((s32) &sp5C, 0, 4, 2, 2, 0x1FF, -0xA0, -0x78, 8, 9);
            load_palette(0x80190000, 0x1F, 1, 3, 0);
            D_801220D0 = 0x80000000;
            D_800E738D += 1;
            return;
        }
        break;
    case 3:
        cd_read_sectors(0x21U, 0x80180000, 0xA048);
        D_800E738D += 1;
        return;
    case 4:
        if (cd_read_sync_status() & 1) {
            func_8004A1A0((s32) &sp5C, 0, 4, 2, 2, 0x1FE, -0xA0, -0x78, 8, 0xB);
            load_palette(0x80190000, 0x1E, 1, 3, 0);
            D_801220D0 = 0x80000000;
            D_800E738D += 1;
            return;
        }
        break;
    case 5:
        cd_read_sectors(0x21U, 0x80180000, 0x9FF6);
        D_800E738D += 1;
        return;
    case 6:
        if (cd_read_sync_status() & 1) {
            func_8004A1A0((s32) &sp5C, 0, 4, 2, 2, 0x1FD, -0xA0, -0x78, 8, 0xD);
            load_palette(0x80190000, 0x1D, 1, 3, 0);
            D_801220D0 = 0x80000000;
            D_800E738D += 1;
            return;
        }
        break;
    case 7:
        cd_read_sectors(0x41U, 0x80180000, 0x9F22);
        D_800E738D += 1;
        return;
    case 8:
        if (cd_read_sync_status() & 1) {
            func_8004A1A0((s32) &sp5C, 1, 8, 4, 2, 0x1EE, -0x140, -0x78, 8, 0x15);
            load_palette(0x801A0000, 0xE, 1, 3, 0);
            D_801220F4 = 0x51000000;
            D_80122105 = 0;
            D_80122104 = 0;
            D_800E738D += 1;
            D_80122106 = 0;
            return;
        }
        break;
    case 9:
        cd_read_sectors(0x31U, 0x801B0000, 0x9FA4);
        D_800E738D += 1;
        return;
    case 10:
        if (cd_read_sync_status() & 1) {
            cd_read_sectors(0x41U, 0x80180000, 0x9EE1);
            D_800E738D += 1;
            return;
        }
        break;
    case 11:
        if (cd_read_sync_status() & 1) {
            func_8004A1A0((s32) &sp5C, 0, 8, 4, 2, 0x1EF, -0xA0, -0x78, 8, 5);
            load_palette(0x801A0000, 0xF, 1, 3, 0);
            D_801220E1 = 0;
            D_801220E0 = 0;
            D_801220D0 = 0x51000000;
            D_800E738D += 1;
            D_801220E2 = 0;
            return;
        }
        break;
    case 12:
        cd_read_sectors(0x41U, 0x80180000, 0x9F63);
        D_800E738D += 1;
        return;
    case 13:
        if (cd_read_sync_status() & 1) {
            func_80046290(0x06000BC8, 0x11000CBA, 0xF);
            func_80044750(0x300);
            func_8004E58C();
            set_kanji_string(-0x90, 0x14, 0, (s32) "始まりがあれば終わりがあるように、", 0);
            set_kanji_string(-0x90, 0x24, 0, (s32) "出会いがあればまた、別れもあるのです。", 0);
            set_kanji_string(-0x90, 0x34, 0, (s32) "永遠に続く二人の関係、", 0);
            set_kanji_string(-0x90, 0x44, 0, (s32) "それはどんなに幸せなことでしょう。", 0);
            k_disp_start(4);
            k_disp_switch(0, 0);
            k_disp_switch(1, 0);
            k_disp_switch(2, 0);
            k_disp_switch(3, 0);
            func_8004284C();
        }
        break;
    }
}

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
