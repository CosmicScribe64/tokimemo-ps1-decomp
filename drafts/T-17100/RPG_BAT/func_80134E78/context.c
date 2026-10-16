#include "common.h"
#include "ovl/RPG_BAT.h"

void func_80134D30(void) {
    switch (D_8015EDAC) {
    case 0:
        func_800AE0F0(D_8015E74C, "　　　　ハーメルンのホラ吹き　　　　");
        func_8014B738((s32) &D_8015E814, 1, 0x5A);
        func_80144480();
        func_8013EEAC(D_8015EBAC);
        func_80148F70();
        func_8014EBA8();
        func_8014EBA8();
        return;
    case 2:
        func_8013D418();
        return;
    case 3:
        func_801359FC();
        return;
    case 4:
        func_8013D1D0();
        return;
    case 5:
        func_80134E78();
        return;
    case 6:
        func_80135358();
        return;
    case 7:
        func_8013D350();
        func_8013E7C0(0x34, 2, 1, 0);
        func_8014EBA8();
        return;
    case 8:
        func_8013D290();
        return;
    case 9:
        func_8013D59C();
        return;
    case 10:
        func_8014EBEC();
        /* fallthrough */
    default:
        return;
    }
}

/* bulk_m2c declarations (T-11190) */
extern s32 D_8015EC9C;
extern s32 D_8015ECA0;
extern s32 D_8015ECA4;
extern s32 D_8015ECA8;
extern s32 D_8015ECAC;
extern s32 D_8015ECB0;
extern s32 D_8015ECB4;
extern s32 D_8015ECB8;
extern s32 D_8015ECC4;
extern s32 D_8015ECC8;
extern s32 D_8015ECCC;
extern s32 D_8015ECD0;
extern s32 D_8015ECD4;
extern s32 D_8015ECD8;
extern s32 D_8015ECDC;
extern s32 D_8015ECE0;
extern s32 D_8015ECFC;
extern s32 D_8015ED00;
extern s32 D_8015ED04;
extern s32 D_8015ED08;
/* end bulk_m2c declarations */
s32 func_80134E78(void) {
    DisplayActor44 *var_s0;
    s32 *var_s2;
    s32 temp_s0;
    s32 temp_s0_2;
    s32 temp_s1;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 temp_v1_3;
    s32 temp_v1_4;
    s32 var_s3;
    s32 var_t9;

    D_8015EC78 = -0x1C0;
    D_8015EC7C = 0x280;
    D_8015EC80 = -0x140;
    ((DisplayActor44 *)D_8011ECD0)[146].unk_01 = 2;
    ((DisplayActor44 *)D_8011ECD0)[147].unk_01 = 2;
    ((DisplayActor44 *)D_8011ECD0)[150].unk_01 = 8;
    ((DisplayActor44 *)D_8011ECD0)[151].unk_01 = 8;
    D_8015EC74 = 0x300;
    func_8013E97C(0x32, 0x300, 0x88);
    func_8013E97C(0x33, D_8015EC74, 0x88);
    func_8013E97C(0x36, D_8015EC7C, 0x50);
    func_8013E97C(0x37, D_8015EC7C, 0x50);
    func_8013E810(0x32, 6, 1, 1);
    func_8013E810(0x33, 5, 1, 1);
    func_8013E810(0x36, 6, 1, 1);
    func_8013E810(0x37, 5, 1, 1);
    temp_v0 = func_8013E9A8(0x20);
    D_8015EC9C = 0x2F0;
    D_8015ECC4 = -0x190;
    D_8015ECEC = 0x7D;
    temp_v1 = temp_v0 + 0x10;
    D_8015ECA0 = temp_v1 + 0x310;
    D_8015ECC8 = temp_v1 - 0x170;
    D_8015ECF0 = 0x7A;
    temp_s0 = func_8013E9A8(0x10) + 0x20;
    temp_v0_2 = func_8013E9A8(0x10);
    D_8015ECA4 = 0x2B0;
    D_8015ECCC = -0x190;
    D_8015ECF4 = 0x6C;
    D_8015ECA8 = temp_s0 + 0x2C0;
    D_8015ECD0 = temp_s0 - 0x180;
    D_8015ECF8 = 0x69;
    temp_v1_2 = temp_v0_2 + temp_s0;
    D_8015ECAC = temp_v1_2 + 0x2F0;
    D_8015ECD4 = temp_v1_2 - 0x150;
    D_8015ECFC = 0x6F;
    temp_s0_2 = func_8013E9A8(0x10) + 0x10;
    temp_v0_3 = func_8013E9A8(0x10);
    D_8015ECB0 = temp_s0_2 + 0x2A0;
    D_8015ECD8 = temp_s0_2 - 0x160;
    D_8015ED00 = 0x61;
    temp_v1_3 = temp_v0_3 + temp_s0_2;
    D_8015ECB4 = temp_v1_3 + 0x2D0;
    D_8015ECDC = temp_v1_3 - 0x130;
    D_8015ED04 = 0x5E;
    switch (D_8015EBAC) {
    case 0:
        D_8015ECB8 = 0x2F8;
        D_8015ECE0 = -0x148;
        D_8015ED08 = 0x64;
        D_8015EC58.unk_10 = 0x77;
        var_t9 = 0x3E;
block_7:
        D_8015EC58.unk_14 = var_t9;
        break;
    case 1:
        D_8015ECB8 = 0x2D0;
        D_8015ECE0 = -0x130;
        D_8015ED08 = 0x5B;
        D_8015EC58.unk_10 = 0x3F;
        D_8015EC58.unk_14 = 0x2E;
        break;
    case 2:
        D_8015ECB8 = 0x310;
        D_8015ECE0 = -0x170;
        D_8015ED08 = 0x85;
        D_8015EC58.unk_10 = 0x37;
        var_t9 = 0x5E;
        goto block_7;
    case 3:
        D_8015ECB8 = 0x310;
        D_8015ECE0 = -0x170;
        D_8015ED08 = 0x85;
        D_8015EC58.unk_10 = 0x4C;
        D_8015EC58.unk_14 = 0xE;
        break;
    case 4:
        D_8015ECB8 = 0x310;
        D_8015ECE0 = -0x170;
        D_8015ED08 = 0x85;
        D_8015EC58.unk_10 = 0x54;
        var_t9 = 0x2A;
        goto block_7;
    }
    var_s0 = ((DisplayActor44 *)D_8011ECD0);
    var_s2 = &D_8015EBAC;
    var_s3 = 0;
    do {
        temp_s1 = var_s3 + 0x28;
        func_8013E810(temp_s1, 5, 1, 1);
        func_8013E97C(temp_s1, *(s32 *)((unsigned char *)var_s2 + (240)), *(s32 *)((unsigned char *)var_s2 + (320)));
        var_s0[136].unk_18 = func_8013E9A8(4);
        temp_v1_4 = *(s32 *)((unsigned char *)var_s2 + (320));
        var_s3 += 1;
        var_s0[136].unk_08 = func_8013E9A8(7);
        if (temp_v1_4 >= 0x85) {
            var_s0[136].unk_01 = 3;
        } else if (temp_v1_4 >= 0x66) {
            var_s0[136].unk_01 = 4;
        } else if (temp_v1_4 >= 0x64) {
            var_s0[136].unk_01 = 5;
        } else if (temp_v1_4 >= 0x5D) {
            var_s0[136].unk_01 = 6;
        } else {
            var_s0[136].unk_01 = 7;
        }
        var_s2 += 1;
        var_s0 += 1;
    } while (var_s3 != 8);
    if (D_8015EDC4 & 0x100000) {
        ((DisplayActor44 *)D_8011ECD0)[155].unk_01 = 5;
        ((DisplayActor44 *)D_8011ECD0)[156].unk_01 = 7;
        ((DisplayActor44 *)D_8011ECD0)[157].unk_01 = 3;
    } else {
        ((DisplayActor44 *)D_8011ECD0)->unk_00 = 3;
    }
    func_8014EBA8();
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80134D30", func_80135358);

void func_801359FC(void) {
    switch (D_8015EDB0.unk_00) {
    case 0:
        func_8013E7C0(0x34, 3, 5, 1);
        func_8014EBD0();
        break;
    case 1:
        func_8014ED80(0xF);
        break;
    case 2:
        func_8014EBD0();
        break;
    case 3:
        func_8014EDFC(0x34);
        break;
    case 4:
        func_8013E7C0(0x34, 7, 1, 1);
        func_8013F0F4(0x50D, 1, 0);
        func_8014EBD0();
        break;
    case 5:
        func_8014ED80(0x4B);
        break;
    case 6:
        func_8014EBA8();
        break;
    }
}
