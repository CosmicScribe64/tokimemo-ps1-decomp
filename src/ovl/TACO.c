#include "common.h"
#include "ovl/TACO.h"

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80132000);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801321A0);

void func_80132220(void) {
    D_8015EDB4->unk74 = 0;
    D_8015EDB4->unk76 = 0;
    D_8015EDB4->unk78 = 0x800;
    D_8015EDB4[8].unk74 = 0;
    D_8015EDB4[8].unk76 = 0;
    D_8015EDB4[8].unk78 = 0x800;
    D_8015EDB4->unk2 = 0;
    D_8015EDB4[8].unk2 = 0;
    D_8015EDB4->unk3 = 0;
    D_8015EDB4->unk68 = 0;
    D_8015EDB4[8].unk68 = 0;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801322AC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013240C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801328B0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80132FE8);

void func_80133374(void) {
    if (D_8015EDCC < 3U) {
        D_8015EDEC -= 1;
    }
    if (D_8015EDEC == 0) {
        func_80042908(0x2D);
    }
    if (D_800E7208 & 0x800) {
        func_80042908(0x2D);
    }
    if (D_8015EDB4->unk84[1] == 0) {
        func_80042908(0x2D);
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80133410);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801334BC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013356C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801335A0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80133694);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80133738);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80133870);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80133980);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80133AF4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80133C08);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80133E98);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801342CC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80134450);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80134500);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801345D4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013474C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801347A8);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801347F4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80134884);

void func_80134930(void) {
    D_800E62B7 = 0;
    D_800E62B6 = 0;
    D_800E62B8 = 0;
    func_800438DC(1, 1);
    func_80058D20();
    func_80146D60(-0xFA0);
    func_80058F0C(0x8019C800);
    func_801471D0(0x8019C800);
    func_80147488(1, D_8015F3E0, 0);
    D_80127090.unk0 = 0;
    D_80128880[1].x = 0x32;
    D_80128880[1].z = 0x32;
    D_80128880[1].y = 0xD2;
    func_80147488(2, D_8015F3E4, 0);
    D_801270A0.unk0 = 0;
    D_80128880[2].x = 0x4B;
    D_80128880[2].z = -0x32;
    D_80128880[2].y = 0xC3;
    D_8015F3E8 = 0;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80134A18);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80134D8C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80134EBC);

void func_80134FB8(void) {
    func_80048F64(1);
    D_8011ED15 = 8;
    D_8011ED4C = 0;
    D_8011ED16 = 0x40;
    D_8011ED17 = 0x84;
    D_8011ED20 = D_800E7CCC;
    D_8011ED24 = D_800E7CDC;
    D_8011ED48 = D_800E7CBC;
    D_8011ED18 = 8;
    D_8011ED19 = 0x81;
    D_8011ED57 = 0xF;
    D_8011ED3A = -0xA0;
    D_8011ED3E = -0x64;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013506C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013515C);

void func_80135220(void) {
    if ((u32)D_800E7384 >= 0xD1U) {
        func_8004284C();
    } else if (D_800E7208 & 0x860) {
        func_80042940(5);
    }
}

void func_80135278(void) {
    if (func_8013515C() == 0) {
        func_8004284C();
    } else if (D_800E7208 & 0x860) {
        func_80042940(5);
    }
}

void func_801352CC(void) {
    if ((u32)D_800E7384 < 0xFFU) {
        if (D_800E7208 != 0) {
            func_80042940(5);
            return;
        }
        func_8004AC18(0xFF - D_800E7384);
        return;
    }
    func_8004284C();
}

void func_80135330(void) {
    func_8004284C();
}

void func_80135350(void) {
    func_80059048();
    func_80043914(D_800E7CEC, 0x1F, 1, 1, 0);
    func_80042908(3);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80135394);

void func_80135418(void) {
    func_8009C210(0);
    func_80059048();
    func_800591D8(0);
    func_80134FB8();
    D_8011ED1B = 0x40;
    D_8011ED3E = -0x78;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013546C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013555C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80135638);

void func_80135720(void) {
    D_8015F3F0 = 0;
    D_8015F3F4 = 0;
    D_8015F3F8 = 0;
    D_8015F3FC = 0;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80135744);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801357B0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013587C);

void func_80135944(void) {
    RECT r;

    r.x = 0x2C0;
    r.y = 0;
    r.w = 0x140;
    r.h = 0x14;
    func_8009C7F8(&r, 0, 0, 0);
    func_8009C674(0);
}

void func_80135994(void) {
    func_80042908(2);
}

void func_801359B4(void) {
    if (D_8015F3F4 == 1) {
        func_80135F6C();
    } else {
        func_80136988();
    }
    func_800578F4(2);
    func_80135A04();
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80135A04);

void func_80135F6C(void) {
    D_8015F3FC += 1;
    switch (D_8015F3F0) {
    case 0:
        func_80136024();
        break;
    case 1:
        func_801361E8();
        break;
    case 2:
        func_80136438();
        break;
    case 3:
        func_80136688();
        break;
    }
    if (D_8015F3FC >= 0x1F) {
        D_8015F3FC = 0;
        D_8015F3F4 = 0;
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80136024);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801361E8);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80136438);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80136688);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801368D8);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80136988);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80136B40);

void func_80136C00(void) {
    func_8013AFDC();
    func_800450F4(0, 0x202);
    D_800E62B6 = 0;
    D_800E62B7 = 0;
    D_800E62B8 = 0;
    func_800438F0(1);
    D_8015EDB0 = 0;
    func_80059048();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80136C60);

void func_80136CD8(void) {
    func_80044750(0xC1);
    func_80042908(2);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80136D00);

void func_80136D78(void) {
    if (D_800E7200 & 0x40) {
        D_8015F400 += 0x78;
    } else {
        D_8015F400 += 0x3C;
    }
    func_8013A790(1, 0);
    func_80136FBC(D_8015F400, 3);
    if (D_8015F400 > 0x1000) {
        func_8004284C();
    }
}

void func_80136E08(void) {
    D_8015F400 += 0x1F4;
    func_8013A790(1, 0);
    func_80136FBC(D_8015F400, 0);
    if (D_8015F400 >= 0x9001) {
        D_8015F400 = 0;
        func_8004284C();
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80136E70);

void func_80136F4C(void) {
    func_8013A790(1, 0);
    func_80136FBC(0x1000, 3);
    if (D_800E7200 & 0x40) {
        func_8004284C();
    }
    if (D_800E7208 & 0x20) {
        func_8004284C();
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80136FBC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801371B4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80137264);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80137590);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013760C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013788C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80137A60);

void func_80137B14(s32 arg0) {
    func_80048F64(1);
    D_8011ED15 = 8;
    D_8011ED4C = 0;
    D_8011ED16 = 0x40;
    D_8011ED17 = 0x84;
    D_8011ED20 = D_800E7CCC;
    D_8011ED24 = D_800E7CDC;
    D_8011ED48 = D_800E7CBC;
    D_8011ED18 = 8;
    D_8011ED19 = 0x81;
    D_8011ED57 = 0xF;
    D_8011ED2C = arg0;
    D_8011ED3A = -0xAA;
    D_8011ED3E = 0x78;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80137BD8);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80137CBC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80137D00);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80137E48);

void func_80137F54(void) {
    func_80137CBC(8);
    func_80044750(0xB1);
    func_80042908(2);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80137F90);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013801C);

void func_80138160(void) {
    func_8013A790(1, 1);
    if (D_8015EDBC == 0) {
        func_800450F4(0, 0x203);
        func_80043A00(0x80162000, 0xE, 1, 0, 0, 0x100, 0x100);
        func_80043A00(0x80172000, 0xC, 1, 0, 0, 0x100, 0x100);
        func_80043A00(0x80182000, 0xA, 1, 0, 0, 0x80, 0x80);
        func_8004284C();
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013822C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80138280);

void func_80138348(void) {
    D_8015EDBC = 0x14;
    D_8015EDC0 = 0;
    D_8015EDB0 = 0x80;
    func_8015ABF0();
    func_8004284C();
}

void func_8013838C(void) {
    func_80154960();
    if ((D_8015EDBC == 0) && (D_8015EDC0 == 0)) {
        D_8015EDC0 = 1;
    }
    if (D_8015EDC0 == 1) {
        D_8015EDC4 = func_80044C98();
        if (D_8015EDC4 == 1) {
            D_8015EDC0 = 2;
        }
    }
}

void func_8013840C(void) {
    if (D_8015EDBC == 0) {
        D_8015EDBC = 0x18;
        func_800450F4(1, 0x200);
        func_80042908(0xC);
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80138450);

void func_80138520(void) {
    func_800450F4(0, 0x203);
    func_80044750(0x24);
    func_8014394C();
    func_80132220();
    func_801334BC();
    D_800E62B6 = 0x78;
    D_800E62B7 = 0xA8;
    D_800E62B8 = 0xF0;
    D_8015EDB0 = 3;
    D_8015EDBC = 0x24;
    D_8015EDC0 = 0;
    func_8015ABF0();
    func_8004284C();
}

void func_801385B4(void) {
    func_80156A80();
    if ((D_8015EDBC == 0) && (D_8015EDC0 == 0)) {
        D_8015EDC0 = 1;
    }
    if (D_8015EDC0 == 1) {
        D_8015EDC4 = func_80044C98();
        if (D_8015EDC4 == 1) {
            D_8015EDC0 = 2;
        }
    }
}

void func_80138634(void) {
    if (D_8015EDBC == 0) {
        func_8015ABF0();
        D_8015EDBC = 0x28;
        func_800450F4(1, 0x201);
        func_80042908(0x16);
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80138680);

void func_8013873C(void) {
    D_800E62B6 = 8;
    D_800E62B7 = 0x10;
    D_800E62B8 = 0x60;
    D_8015EDB0 = 4;
    func_8015ABF0();
    func_80132220();
    func_801334BC();
    D_8015EDBC = 0x34;
    D_8015EDC0 = 0;
    func_800450F4(0, 0x200);
    func_8004284C();
}

void func_801387C0(void) {
    func_80159090();
    if ((D_8015EDBC == 0) && (D_8015EDC0 == 0)) {
        D_8015EDC0 = 1;
    }
    if (D_8015EDC0 == 1) {
        D_8015EDC4 = func_80044C98();
        if (D_8015EDC4 == 1) {
            D_8015EDC0 = 2;
        }
    }
}

void func_80138840(void) {
    if (D_8015EDBC == 0) {
        D_8015EDBC = 0x38;
        func_8015ABF0();
        func_800450F4(1, 0x200);
        func_80042908(0x20);
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80138890);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80138964);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80138B2C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80138D08);

void func_80139118(void) {
    func_800591D8(0);
    if (D_8015EE00 < D_8015EDF0) {
        func_80042908(9);
        return;
    }
    func_80042908(5);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80139170);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013920C);

void func_80139394(void) {
    D_800E738D += 1;
    func_8013A790(0, 0);
    if ((u32)(D_800E738D * 3) >= 0x81U) {
        func_8004284C();
    }
}

void func_801393F4(void) {
    func_80042908(5);
    func_8004284C();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80139424);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80139AB0);

void func_80139B54(void) {
    func_8013A790(0, 0);
    func_8013A580(D_8015F4F8);
    func_80139FB8(D_8015F4E0, 0, 0x280 - D_8015F4F8, 0);
    D_8015F4F8 -= 8;
    if (D_8015F4F8 < 0) {
        func_8004284C();
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80139BCC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80139FB8);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013A580);

void func_8013A764(void) {
    D_8015F4DC = 0;
    D_8015F4E0 = 0;
    D_8015F4E4 = 0;
    D_8015F4E8 = 0;
    D_8015F4F8 = 0;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013A790);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013AD64);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013AFDC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013B170);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013B2A0);

void func_8013B398(void) {
    if (D_800E7208 != 0) {
        func_8004284C();
    }
}

void func_8013B3C4(void) {
    if (D_8015EDBC == 0) {
        if (func_80044C98() == 1) {
            func_80042908(8);
        }
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013B404);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013B460);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013B990);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013BAC4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013C1F4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013C440);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013C6D4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013CC40);

void func_8013CE70(void) {
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013CE78);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013CFD8);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013D1E0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013D2A0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013D348);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013D3F4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013D528);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013D838);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013DB80);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013DDE4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013DEE4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013E424);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013E4FC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013E778);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013EAA4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013F250);

void func_8013F430(void) {
    func_8013F468();
    func_8013F7D8();
    func_8013F9C0();
    func_8013FBF0();
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013F468);

void func_8013F7D8(void) {
    s16 v;

    v = D_8015EDB4->unk7C;
    if ((v == 3) || (v == 4)) {
        func_8013F874(0xB4, 0x60, v);
    }
    v = D_8015EDB4->unk7E;
    if ((v == 3) || (v == 4)) {
        func_8013F874(0xC8, 0x60, v);
    }
    v = D_8015EDB4->unk80;
    if ((v == 3) || (v == 4)) {
        func_8013F874(0xDC, 0x60, v);
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013F874);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013F9C0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8013FBF0);

void func_80140000(void) {
    func_80140028(0);
    func_80140028(1);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80140028);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80140364);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801404FC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80140638);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801407D4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80140ADC);

void func_80140DF0(void) {
    func_80140E18(0);
    func_80140E18(1);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80140E18);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80141050);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80141170);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80141290);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801413B8);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801416E8);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80141B18);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80141DE0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801420DC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801421AC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80142220);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801423B0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801424A8);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80142820);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801428DC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80142AF0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80142DE0);

void func_80142EC4(void) {
    func_80059E00();
}

void func_80142EE4(void) {
    func_80059BE8();
    func_8004284C();
}

void func_80142F0C(void) {
    func_8004ADE4();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80142F34);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80142FE8);

void func_801430B0(void) {
    func_8004ACC8(0x800000);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801430D0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801432F0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80143574);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80143644);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801436D4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80143730);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80143814);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801438F0);

void func_8014394C(void) {
    D_80122760 = 0;
    D_80122764 = 0;
    D_80122768 = -0x64;
    D_8012276C = 0xFF;
    D_8012276D = 0xFF;
    D_8012276E = 0xFF;
    func_8009AD70(0, &D_80122760);
    D_80122770 = 0;
    D_80122774 = 0x64;
    D_80122778 = -0x64;
    D_8012277C = 0xFF;
    D_8012277D = 0xFF;
    D_8012277E = 0xFF;
    func_8009AD70(1, &D_80122770);
    D_80122780 = -0x64;
    D_80122784 = 0;
    D_80122788 = -0x64;
    D_8012278C = 0xFF;
    D_8012278D = 0xFF;
    D_8012278E = 0xFF;
    func_8009AD70(2, &D_80122780);
    func_8009B310(0x800, 0x800, 0x800);
    func_8009B340(0);
}

void func_80143A74(void) {
    func_8009AD30(0x800);
    D_80122740 = 0;
    D_80122744 = 0;
    D_80122748 = 0x800;
    D_8012274C = 0;
    D_80122750 = 0;
    D_80122754 = 0;
    D_80122758 = 0;
    D_8012275C = 0;
    func_80099540(&D_80122740);
    func_8009AD50(-0x64);
    func_8009AD60(0x7FFF);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80143AF4);

void func_80143B34(s32 arg0) {
    func_80059688((arg0 * 2) + 2);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80143B58);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80143E80);

void func_80143E9C(void) {
    if (D_8015EDB0 != 0) {
        func_80143FF4();
        func_80143F40();
        func_80143B58();
        func_80133694();
    }
    if (D_8015EDB0 & 1) {
        func_80144094();
    }
    if (D_8015EDB0 & 2) {
        func_80144430();
    }
    if (D_8015EDB0 & 4) {
        func_801446D0();
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80143F40);

void func_80143FF4(void) {
    D_80122740 = D_8015EDB4->unk74;
    D_80122744 = D_8015EDB4->unk76;
    D_80122748 = D_8015EDB4->unk78;
    D_8012274C = D_8015EDB4->unk74;
    D_80122750 = D_8015EDB4->unk76;
    D_80122754 = 0;
    D_80122758 = D_8015EDB4->unk68 * 0x168;
    D_8012275C = 0;
    func_80099540(&D_80122740);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80144094);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80144430);

void func_801446D0(void) {
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801446D8);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801447D0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014488C);

void func_80144998(void) {
    D_80127090.unk0 = 0x80000000;
    D_801270A0.unk0 = 0x80000000;
    func_800438DC(1, 0);
    func_80042908(2);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801449D8);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80144CC0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80144D90);

void func_80144DF8(s32 arg0) {
    TcObj50 *o;
    TcPos *p;

    o = &D_80127480[arg0];
    p = &D_80128880[arg0];
    o->unk18 = 0;
    o->unk1C = 0;
    o->unk20 = 0;
    p->x = 0;
    p->y = 0;
    p->z = 0;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80144E3C);

void func_8014559C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    if ((arg4 >= 0) && (arg4 < 0x1F)) {
        func_80144E3C(arg0, arg1, arg2, arg3, (arg4 & 0xFF) * 8, 0, arg5);
        return;
    }
    if ((arg4 >= 0x1F) && (arg4 < 0x23)) {
        func_80144E3C(arg0, arg1, arg2, arg3, 0, 2, arg5);
        return;
    }
    if ((arg4 >= 0x23) && (arg4 < 0x61)) {
        func_80144E3C(arg0, arg1, arg2, arg3, ((arg4 - 0x23) & 0xFF) * 4, 1, arg5);
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80145650);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80145DC4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014616C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80146754);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80146D60);

void func_80146E3C(void) {
    D_80122760 = 0x14;
    D_80122764 = 0x64;
    D_80122768 = -0x64;
    D_8012276C = 0xB0;
    D_8012276D = 0xB0;
    D_8012276E = 0xB0;
    func_8009AD70(0, &D_80122760);
    D_80122770 = 0x14;
    D_80122774 = -0x64;
    D_80122778 = 0x64;
    D_8012277C = 0x80;
    D_8012277D = 0x80;
    D_8012277E = 0x80;
    func_8009AD70(1, &D_80122770);
    D_80122780 = -0x14;
    D_80122784 = 0x14;
    D_80122788 = -0x64;
    D_8012278C = 0x60;
    D_8012278D = 0x60;
    D_8012278E = 0x60;
    func_8009AD70(2, &D_80122780);
    func_8009B310(0, 0, 0);
    func_8009B340(0);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80146F74);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801471D0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801472D4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80147400);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80147444);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80147488);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80147674);

void func_801478F8(s32 arg0, s16 *arg1) {
    TcPos *p;

    p = &D_80128880[arg0];
    p->x = arg1[3];
    p->z = arg1[4];
    arg1 += 5;
    p->y = arg1[0];
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80147928);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80147B24);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80147C98);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80147E80);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801480B0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80148184);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80148308);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801483B0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801484E4);

void func_8014866C(void) {
    D_80122760 = -0x64;
    D_80122764 = 0x1E;
    D_80122768 = -0x1E;
    D_8012276C = 0xE0;
    D_8012276D = 0xE0;
    D_8012276E = 0xE0;
    func_8009AD70(0, &D_80122760);
    D_80122770 = 0x28;
    D_80122774 = 0x32;
    D_80122778 = -0x64;
    D_8012277C = 0x40;
    D_8012277D = 0x40;
    D_8012277E = 0x40;
    func_8009AD70(1, &D_80122770);
    D_80122780 = 0xA;
    D_80122784 = -0x14;
    D_80122788 = -0x64;
    D_8012278C = 0x30;
    D_8012278D = 0x30;
    D_8012278E = 0x30;
    func_8009AD70(2, &D_80122780);
    func_8009B310(0, 0, 0);
    func_8009B340(0);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801487A4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80148B58);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80148D6C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80148FE0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801490B4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014927C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801493D4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80149538);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801497EC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80149B7C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80149D1C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80149F90);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014A070);

void func_8014A1D4(void) {
    func_8014F210();
    func_8015185C();
    if (((u32)D_800E7384 % 180U) == 0) {
        func_801335A0(0, 0x500);
    }
    D_800E7384 += 1;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014A234);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014A390);

void func_8014A42C(void) {
    func_8014A480(D_8015FE95, 1, D_8015FEC4, D_8015FEA0, D_8015FEAC, 1);
    func_8014F820();
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014A480);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014A79C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014A8E0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014AB50);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014B2A4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014B374);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014B4B4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014B5F0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014B790);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014B8B4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014BC80);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014BCEC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014BE14);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014BED8);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014BF38);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014C0D4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014C1DC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014C2E4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014C3B4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014C578);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014C8DC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014C978);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014CB24);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014D200);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014D2E0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014D3B4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014D530);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014D918);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014DA6C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014DC94);

void func_8014DF50(s32 arg0) {
    s32 t;

    t = (-0x41A0 - D_801274A0) / 15;
    if (arg0 < 5) {
        func_80146F74(0, 0x64, 0, t, 0, 0, 0, 0x32, 0x32);
    }
    if ((arg0 >= 5) && (arg0 < 0xF)) {
        func_80146F74(0, -0x64, 0, t, 0, 0, 0, 0x32, 0x32);
    }
    if ((arg0 >= 0xF) && (arg0 < 0x14)) {
        func_80146F74(0, 0x64, 0, t, 0, 0, 0, 0x32, 0x32);
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014E048);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014E1A0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014E4A4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014E5F8);

void func_8014EA4C(void) {
    if (D_8015EDB4[16].unk3 >= 3U) {
        func_8014EB24(D_8015EDB4[16].unk2);
        D_8015EDB4[16].unk2 = D_8015EDB4[16].unk2 + 1;
        D_8015EDB4[16].unk3 = 0;
    }
    if ((D_8015EDB4[16].unk2 >= 0x50U) || (D_8015EDB4[17].unk84[2] == 1)) {
        D_8015EDB4[16].unk2 = 0;
        D_8015EDB4[16].unk3 = D_8015EDB4[16].unk2;
        func_8014F044();
        return;
    }
    D_8015EDB4[16].unk3 = D_8015EDB4[16].unk3 + 1;
}

void func_8014EB24(s32 arg0) {
    if (arg0 < 0xE) {
        func_80146F74(0, 0, -0x14, -0x1E, 0, 0, 0, 0x32, 0x1F4);
    } else if (arg0 == 0xE) {
        D_8015EDB4[24].unk84[2] = 1;
        func_8014C8DC();
    } else if ((arg0 >= 0xF) && (arg0 < 0x41)) {
        if (D_8015EDB4[24].unk84[2] == 1) {
            func_8014C3B4(0);
        }
        func_80146F74(0, 0, 0, 0, 0, 0, 0, 0x32, 0x1F4);
    }
    if ((arg0 >= 0x41) && (arg0 < 0x50)) {
        func_80146F74(0, 0, 0xF, 0x1E, 0, 0, 0, 0x32, 0x1F4);
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014EC4C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014EDCC);

void func_8014EE3C(void) {
    if (D_8015EDB4[16].unk3 >= 3U) {
        func_8014EF14(D_8015EDB4[16].unk2);
        D_8015EDB4[16].unk2 = D_8015EDB4[16].unk2 + 1;
        D_8015EDB4[16].unk3 = 0;
    }
    if ((D_8015EDB4[16].unk2 >= 0x73U) || (D_8015EDB4[17].unk84[2] == 1)) {
        D_8015EDB4[16].unk2 = 0;
        D_8015EDB4[16].unk3 = D_8015EDB4[16].unk2;
        func_8014F044();
        return;
    }
    D_8015EDB4[16].unk3 = D_8015EDB4[16].unk3 + 1;
}

void func_8014EF14(s32 arg0) {
    if (arg0 < 0xE) {
        func_80146F74(0, 0, -0x14, -0x1E, -1, 0, 0, 0x32, 0x1F4);
    } else if (arg0 == 0xE) {
        D_8015EDB4[25].unk84[2] = 1;
        func_8014D200();
    } else if ((arg0 >= 0xF) && (arg0 < 0x64)) {
        if (D_8015EDB4[25].unk84[2] == 1) {
            func_8014C978(0);
        }
        func_80146F74(0, 0, 0, 0, 0, 0, 0, 0x32, 0x1F4);
    }
    if ((arg0 >= 0x64) && (arg0 < 0x72)) {
        func_80146F74(0, 0, 0xF, 0x1E, 1, 0, 0, 0x32, 0x1F4);
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014F044);

void func_8014F210(void) {
    func_80151264(5);
    func_8014FBD4(7);
    func_80146F74(0, 0, 1, 0, 0, 0, 0, 0x32, 0x1F4);
    if (func_800AE0C0(D_8012749C - D_8015EDB4->unk76) < 0x64) {
        func_8014A79C();
        func_8004284C();
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014F2A0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014F5F0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014F820);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014F9B0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014FBD4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014FDC0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8014FFDC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801508D0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80150B3C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80150DE8);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80150F44);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801511C0);

void func_80151264(u32 arg0) {
    if (arg0 < D_8015EDB4[16].unk3) {
        func_8015131C(D_8015EDB4[16].unk2);
        D_8015EDB4[16].unk2 = D_8015EDB4[16].unk2 + 1;
        D_8015EDB4[16].unk3 = 0;
    }
    if (D_8015EDB4[16].unk2 >= 0x7CU) {
        D_8015EDB4[16].unk2 = 0;
        D_8015EDB4[16].unk3 = D_8015EDB4[16].unk2;
        return;
    }
    D_8015EDB4[16].unk3 = D_8015EDB4[16].unk3 + 1;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8015131C);

void func_8015163C(s32 arg0, s32 arg1) {
    if (!(arg1 & 1)) {
        D_80127080[arg0].unk0 = 0x40000000;
        return;
    }
    D_80127080[arg0].unk0 = 0x80000000;
}

void func_80151678(void) {
    D_80122760 = 0x64;
    D_80122764 = 0x32;
    D_80122768 = -0x64;
    D_8012276C = 0x70;
    D_8012276D = 0x70;
    D_8012276E = 0x80;
    func_8009AD70(0, &D_80122760);
    D_80122770 = 0;
    D_80122774 = -0x64;
    D_80122778 = -0x64;
    D_8012277C = 0x40;
    D_8012277D = 0x40;
    D_8012277E = 0x40;
    func_8009AD70(1, &D_80122770);
    D_80122780 = -0x64;
    D_80122784 = 0x32;
    D_80122788 = -0x64;
    D_8012278C = 0x80;
    D_8012278D = 0x80;
    D_8012278E = 0x90;
    func_8009AD70(2, &D_80122780);
    func_8009B310(0, 0, 0);
    func_8009B340(0);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801517AC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8015185C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80151C30);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80151D1C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80151F94);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80152110);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801521EC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801522B0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801523D0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8015266C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80152764);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80152808);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80152944);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80152C60);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80152DB4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80153068);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801531E8);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801532C4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801534E0);

void func_801536BC(u32 arg0) {
    if (arg0 < D_8015EDB4[23].unk3) {
        func_8015131C(D_8015EDB4[23].unk2);
        D_8015EDB4[23].unk2 = D_8015EDB4[23].unk2 + 1;
        D_8015EDB4[23].unk3 = 0;
    }
    if (D_8015EDB4[23].unk2 >= 0x7CU) {
        D_8015EDB4[23].unk2 = 0;
        D_8015EDB4[23].unk3 = D_8015EDB4[23].unk2;
        return;
    }
    D_8015EDB4[23].unk3 = D_8015EDB4[23].unk3 + 1;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80153774);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80153AA0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80153B38);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80153E78);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80154014);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80154230);

s32 func_8015431C(u32 arg0) {
    if (arg0 < 0x8018A000U) {
        return 0;
    }
    if (arg0 >= 0x801B8001U) {
        return 0;
    }
    return 1;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8015435C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8015443C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801546FC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80154820);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80154960);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80154F74);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80155230);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801553AC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801557E8);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8015595C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80155C64);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80155E10);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80155F98);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80156178);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80156244);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801563E4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80156924);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80156A80);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80156F50);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801570A4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801571E8);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801572D8);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801573C8);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8015776C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80157AE0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80157CD4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80157EA8);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8015800C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801585D0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_801589B0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80158AB0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80158B84);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80158CDC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80158DBC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80158F48);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80159090);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80159260);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80159678);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80159878);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_80159F60);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8015A164);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8015A378);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8015A680);

void func_8015A900(void) {
    D_801604C0 = 0;
    D_801604C4 += 1;
    D_801604D0 = 0;
}

void func_8015A928(void) {
    D_801604C8 = 0;
    D_801604CC += 1;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8015A948);

void func_8015AB18(s32 arg0) {
    (arg0 + D_8015EDB4)->unk78 += 0x100;
    if ((arg0 + D_8015EDB4)->unk78 > 0x1000) {
        func_8015C208();
    }
}

void func_8015AB80(s32 arg0) {
    (arg0 + D_8015EDB4)->unk78 -= 0xFA;
    if ((arg0 + D_8015EDB4)->unk78 < -0x7530) {
        func_8015C208();
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8015ABF0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8015ACCC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8015B1B8);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8015B2AC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8015B3B0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8015B59C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8015BD30);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8015BF70);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8015C0A0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8015C208);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8015C2CC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8015C698);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8015C718);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8015CC54);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8015CCC8);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8015CE64);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8015CF30);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8015D0D0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO", func_8015D270);
