#include "common.h"
#include "ovl/EN_NICHI.h"

void func_80132000(void) {
    if (D_800E62BE == 0x5F) {
        func_801320C0();
    } else {
        func_80136B10();
    }
}

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80132040);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_801320C0);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_8013216C);

void func_801321EC(void) {
    func_80085E30(1, 0);
    func_80044750(0x7F);
    func_8006BC28(0);
    func_8006BD6C(0);
    func_800AE120(D_800E7374);
    hizuke_disp_switch(0);
    func_80065F34(0);
    message_disp_switch(0);
    func_8006764C(0);
    func_8004284C();
    D_80139B14 = D_800E71EF;
    D_800E71EF = 1;
}

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80132278);

void func_80132308(void) {
    if (D_800E738D == 0) {
        func_80044750(0x201);
        func_80046318(0x3F, 0x801B0000, 0x7C8C);
        D_800E738D += 1;
    } else if (func_800460CC() & 1) {
        func_8004284C();
    }
}

void func_80132378(void) {
    load_palette(D_80139210, 0x10, 1, 1, 0);
    load_palette(D_80139210, 0x11, 1, 1, 1);
    load_palette(D_80139210, 0x12, 1, 1, 2);
    load_palette(D_80139210, 0x13, 1, 1, 3);
    load_palette(D_80139210, 0x1F, 1, 3, 0);
    load_palette(D_80139210, 0x1E, 1, 3, 1);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80132450);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_8013275C);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80132834);

void func_80132A0C(void) {
    s32 v;

    func_80136A2C();
    v = D_80139B1C - 1;
    D_80139B1C = v;
    if (v < 0) {
        D_80139B1C = 0;
    }
    func_80132B40();
    func_8013556C();
    func_80132ADC();
    func_80132D44();
    func_801338EC();
}

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80132A74);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80132ADC);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80132B40);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80132C4C);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80132D44);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80132DC4);

void func_80132E3C(void) {
    func_801330D0();
    if (D_800E7200 & 0x20) {
        if (D_80121531 != 7) {
            func_80135A48(D_8011ECF6, D_8011ECFA);
            func_801369F4(0x501);
        }
        D_80121531 = 7;
        return;
    }
    if (D_80121531 != 3) {
        func_80135A48(D_8011ECF6, D_8011ECFA);
        func_801369F4(0x502);
    }
    D_80121531 = 3;
}

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80132EE8);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_801330D0);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_801333B0);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_8013358C);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_801337E0);

void func_801338EC(void) {
    func_801340CC();
    func_80134258(1);
    func_80133924();
    func_80133CA8();
}

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80133924);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_801339D4);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80133CA8);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80133F00);

s32 func_80134008(s32 arg0) {
    s32 v;

    v = D_80139868[arg0].val;
    if (v < 0x100) {
        return 0;
    }
    if (v < 0x300) {
        return 1;
    }
    if (v < 0x500) {
        return 2;
    }
    if (v < 0x700) {
        return 3;
    }
    if (v < 0x900) {
        return 4;
    }
    if (v < 0xB00) {
        return 5;
    }
    if (v < 0xD00) {
        return 6;
    }
    if (v < 0xF00) {
        return 7;
    }
    return 0;
}

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_801340CC);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80134258);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_801344F0);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80134784);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80134800);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80134C1C);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80134D74);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80134F2C);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80135044);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_8013515C);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80135274);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_801354E0);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_8013556C);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80135600);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_801359D0);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80135A48);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80135ACC);

void func_80135F54(void) {
    s32 i;
    s32 j;

    for (j = 0; j < 11; j++) {
        for (i = 0; i < 14; i++) {
            func_80135FBC(i, j);
        }
    }
}

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80135FBC);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_801362A4);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80136700);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80136904);

void func_801369F4(s32 arg0) {
    if (D_80139B1C <= 0) {
        D_80139B1C = 1;
        func_80044750(arg0 & 0xFFFF);
    }
}

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80136A2C);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80136B10);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80136B74);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80136DEC);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_8013735C);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80137C58);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80137E2C);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80137FEC);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_801381C8);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_801382B4);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_801383A0);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80138434);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80138524);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80138614);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80138708);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_801387FC);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80138890);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80138980);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80138A18);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80138AB0);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80138B9C);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80138C34);

INCLUDE_ASM("asm/ovl/EN_NICHI/nonmatchings/EN_NICHI", func_80138CCC);
