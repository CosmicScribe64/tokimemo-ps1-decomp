#include "common.h"
#include "ovl/SHOUGATU.h"

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80132000);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_801322D4);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_801323D4);

void func_80132450(void) {
    D_800CA134 = &D_80143B00;
    D_800CA138 = &D_80143B04;
    D_800CA13C = D_80143AF4;
    D_800CA140 = D_80143AF8;
    D_800CA144 = D_80143AFC;
    func_80082764(0xFF, 1, 0);
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_801324C8);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80132550);

void func_801325CC(void) {
    func_80044890(1, 0xC4E5, 0xC4C7, 0xD294, 0xD264, 0xD25E);
    if (func_80044E8C() == 1) {
        func_80044750(0x200);
        func_8004284C();
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80132634);

void func_801326BC(void) {
    func_80046318(0x34, 0x80197000, 0xAF48);
    func_80132000();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_801326F8);

void func_80132864(void) {
    func_8007ED84(0x4167);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013288C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80132A34);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80132D00);

void func_80132D88(void) {
    func_80044750(0x603);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80132DB0);

void func_80132DE4(void) {
    func_8007ED84(0x4144);
    func_8004284C();
}

void func_80132E0C(void) {
    if (D_80122CDC != 0) {
        if ((D_800E71DF == 2) || (D_800E71DF == 7) || (D_800E71DF == 8) || (D_800E71DF == 9) || (D_800E71DF == 0xA)) {
            func_80083418();
            return;
        }
        func_800833F0();
        return;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80132E8C);

void func_80132F90(void) {
    if (D_80122CDC == 0) {
        func_80042908(3);
        return;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80132FCC);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_801330C0);

void func_80133134(void) {
    func_80044750(0x603);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013315C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80133208);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_801333F0);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80133610);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80133698);

void func_80133710(void) {
    func_8007ED84(0x4167);
    func_8004284C();
}

void func_80133738(void) {
    func_8007ED84(0x4144);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80133760);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80133874);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_801338DC);

void func_80133948(void) {
    if (D_80122CDC == 0) {
        func_8004284C();
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013397C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_801339DC);

void func_80133A60(void) {
    D_80143B00 = 0x27;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80133A88);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80133B44);

void func_80133CDC(void) {
    D_80143B00 = 0xF;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80133D10);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80133D98);

void func_80133E30(void) {
    if (D_80143B20 != 0) {
        func_801323D4();
        return;
    }
    func_8004284C();
}

void func_80133E6C(void) {
    if (D_80122CDC != 0) {
        func_800833F0();
        return;
    }
    func_800833A0();
}

void func_80133EA8(void) {
    if ((D_80143B20 == 0) && (((u32) D_800E7378 % 3U) == 0)) {
        if (D_80143B24 != 0) {
            func_80085B3C(5, 3);
        } else {
            func_80085B3C(5, 0);
        }
    }
    func_8007ED84(0x41ED);
    func_8004284C();
}

void func_80133F24(void) {
    if ((D_800E71DF != 5) || (D_80143B20 != 0) || (D_80143B18 == 0)) {
        func_80042808();
        return;
    }
    func_8004284C();
}

void func_80133F88(void) {
    if (D_80143B20 != 0) {
        D_800E738A += 4;
    }
    func_8004284C();
}

void func_80133FC8(void) {
    if (D_80143B18 == 0) {
        func_8004284C();
        return;
    }
    func_801323D4();
}

void func_80134004(void) {
    if ((D_800E71DF == 5) && (D_80143B18 == 0)) {
        func_801323D4();
        return;
    }
    func_8004284C();
}

void func_80134058(void) {
    if (D_80143B18 != 0) {
        func_801323D4();
        return;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80134094);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80134120);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_801341E4);

void func_8013428C(void) {
    func_801323D4();
    if ((D_80143B04 == 2) && (D_800E738D == 0) && (D_8011ED5B & 0x80)) {
        func_80044750(0x202);
        D_800E738D += 1;
    }
    if ((D_800E738D == 0) && (D_80143B04 == 3)) {
        func_80044750(0x202);
        D_800E738D += 1;
    }
}

void func_80134340(void) {
    D_800E69DD = D_800E71DF;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013436C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80134400);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80134554);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_801345C8);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80134648);

void func_801346E8(void) {
    if (D_80122CDC != 2) {
        func_8004284C();
        func_8004284C();
        func_8004284C();
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80134730);

void func_801347B4(void) {
    D_80122CEC = D_80122CDC;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_801347E0);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80134890);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80134930);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80134A54);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80134C34);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80134CB4);

void func_80134F10(void) {
    func_80044750(0x201);
    func_8004284C();
}

void func_80134F38(void) {
    func_80044750(0x200);
    func_8004284C();
}

void func_80134F60(void) {
    if ((D_800E71DF == 2) && (D_80143B20 == 0)) {
        func_8004284C();
    }
    func_8004284C();
}

void func_80134FAC(void) {
    if ((D_80143DD8 == 3) || (D_80143DD8 == 4)) {
        D_80143B00 += 2;
    } else if (D_80143DD8 != 0) {
        D_80143B00 += 1;
    }
    func_8004284C();
}

void func_80135014(void) {
    if (D_80143DD8 == 0) {
        D_80143B00 += 2;
    } else if ((D_80143DD8 != 3) && (D_80143DD8 != 4)) {
        D_80143B00 += 1;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013507C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_801350F8);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_801359FC);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80135B10);

void func_80135B54(void) {
    func_80046318(0x37, 0x801C0000, 0xBD5E);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80135B90);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80135E04);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80136078);

void func_80136148(void) {
    D_800CA148 = 2;
    D_800CA14C = 0;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80136178);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80136204);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80136340);

void func_80136424(void) {
    func_8007ED84(0x41ED);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013644C);

void func_80136590(void) {
    func_80046318(0x16, 0x801B0000, 0xAF0D);
    func_80135B90();
    func_8004284C();
}

void func_801365C8(void) {
    D_80122CDC = D_80143FF4;
    func_8004284C();
}

void func_801365F4(void) {
    D_80143FF4 = (u8) D_80122CDC;
    D_800CA148 = 0;
    D_800CA14C = 0;
    func_80135B90();
    D_800CA160 = D_80143EB4;
    D_800CA164 = D_80143EE8;
    D_800CA168 = D_80143F1C;
    if ((D_800E71DF == 0) && (D_80143B24 == 1)) {
        func_80085B3C(0xA, 0x32);
    } else {
        func_80085B3C(0xA, 0x2E);
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_801366A4);

void func_8013673C(void) {
    D_800CA148 = (D_80122CDC * 2) + 0xD;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80136770);

void func_80136824(void) {
    D_800CA148 = 0x1D;
    D_800CA160 = D_80143FE8;
    D_800CA164 = D_80143FEC;
    D_800CA168 = D_80143FF0;
    func_80136884();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80136884);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80136948);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_801369EC);

void func_80136B9C(void) {
    func_80046318(3, 0x801B0000, 0xAF43);
    func_80135E04();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80136BD4);

void func_80136CE0(void) {
    D_800CA148 = 1;
    D_800CA14C = 0;
    func_80136D18();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80136D18);

void func_80136F90(void) {
    D_80144D90 = 0x801976F8;
    D_80144D94 = 0x80198BF0;
    D_80144D98 = 0x8019A37C;
    D_80144D9C = 0x8019ADCC;
    D_80144DA0 = 0x8019BC80;
    D_80144DA4 = 0x8019C918;
    D_80144DA8 = 0x8019D460;
    D_80144DAC = 0x8019E168;
    D_80144DB0 = 0x8019EB4C;
    D_80144DB4 = 0x80197774;
    D_80144DB8 = 0x80198CF8;
    D_80144DBC = 0x8019A414;
    D_80144DC0 = 0x8019ADF8;
    D_80144DC4 = 0x8019BD34;
    D_80144DC8 = 0x8019C960;
    D_80144DCC = 0x8019D4C4;
    D_80144DD0 = 0x8019E210;
    D_80144DD4 = 0x8019EBA8;
    D_80144DD8 = 0x80197C94;
    D_80144DDC = 0x80199738;
    D_80144DE0 = 0x8019AB30;
    D_80144DE4 = 0x8019B000;
    D_80144DE8 = 0x8019C4B4;
    D_80144DEC = 0x8019CC70;
    D_80144DF0 = 0x8019D910;
    D_80144DF4 = 0x8019E7B4;
    D_80144DF8 = 0x8019EE04;
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80137144);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013737C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80137664);

void func_801377CC(void) {
    func_80044750(0xCF);
    func_800674B0();
    if (func_80044E8C() == 1) {
        func_80042808();
    }
    func_80042808();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80137818);

void func_80137898(void) {
    func_80044750(0xCF);
    func_8004284C();
}

void func_801378C0(void) {
    D_800B3D60 = 0;
    func_80044890(0, 0xBF98, 0xBF79, 0xD989, 0xD949, 0xD941);
    func_80044750(0x23);
    func_80044750(0x24);
    func_80044750(0x2E);
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80137940);

void func_801379D8(void) {
    func_8004500C(1, 0x200);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80137A04);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80137A5C);

void func_80137AB4(void) {
    D_800CA134 = &D_80144E08;
    D_800CA138 = &D_80144E0C;
    D_800CA13C = D_80144DFC;
    D_800CA140 = D_80144E00;
    D_800CA144 = D_80144E04;
    func_80082764(D_80122CDC, 1, 0);
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80137B30);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80137BB0);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80137C28);

void func_80137C9C(void) {
    func_80044750(0x23);
    func_80044750(0x24);
    func_80044750(0x500);
    func_8004284C();
}

void func_80137CD4(void) {
    func_80044750(0x23);
    func_80044750(0x24);
    func_80044750(0x507);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80137D0C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80137D68);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80137DE4);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80137E54);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80137F70);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80137FC4);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80137FF0);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013808C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80138158);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_801381B8);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80138240);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80138354);

void func_801383AC(void) {
    func_80044750(0x500);
    func_8004284C();
}

void func_801383D4(void) {
    func_80044750(0x501);
    func_8004284C();
}

void func_801383FC(void) {
    if (D_80144F10 != 0) {
        D_80144E08 += 1;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013843C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_801384B0);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80138524);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_801385AC);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_801386CC);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80138710);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80138758);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80138868);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_801388F0);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80138964);

void func_801389D8(void) {
    func_8007ED84(0x405F);
    func_8004284C();
}

void func_80138A00(void) {
    func_8007ED84(0x403B);
    func_8004284C();
}

void func_80138A28(void) {
    if (D_80144F10 != 0) {
        func_8007ED84(0x40D0);
    } else {
        func_8007ED84(0x40C6);
    }
    func_8004284C();
}

void func_80138A6C(void) {
    func_8007ED84(0x40C6);
    func_8004284C();
}

void func_80138A94(void) {
    func_8007ED84(0x40D0);
    func_8004284C();
}

void func_80138ABC(void) {
    if (D_80144F10 != 0) {
        D_80144E08 += 1;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80138AFC);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80138B40);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80138BA0);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80138C1C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80138CA4);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80138D2C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80138E1C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80138E60);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80138ED4);

void func_80138F48(void) {
    func_8007ED84(0x4071);
    func_8004284C();
}

void func_80138F70(void) {
    func_8007ED84(0x40E2);
    func_8004284C();
}

void func_80138F98(void) {
    func_8007ED84(0x40D9);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80138FC0);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_801390C0);

void func_801391D4(void) {
    func_801391F4();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_801391F4);

void func_801392A4(void) {
    k_reset(1);
    func_8004284C();
}

void func_801392CC(void) {
    func_80046318(0x71, 0x801B0000, 0x85B6);
    func_801390C0();
    func_8004284C();
}

void func_80139304(void) {
    func_80044750(0x24);
    func_80044750(0x501);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80139334);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80139514);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013960C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80139734);

void func_80139778(void) {
    func_8007ED84(0x4045);
    func_8004284C();
}

void func_801397A0(void) {
    func_8007ED84(0x46F0);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_801397D0);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_801398F0);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80139980);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80139A08);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80139A90);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80139B0C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80139B94);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80139C10);

void func_80139C68(void) {
    D_800E71DF = 1;
    func_80137AB4();
}

void func_80139C90(void) {
    func_80046318(0x71, 0x801AE000, 0x8768);
    func_801397D0();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80139CCC);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80139D80);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80139DC4);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80139E0C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80139E54);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80139EC8);

void func_8013A098(void) {
    func_8007ED84(0x40BC);
    func_8004284C();
}

void func_8013A0C0(void) {
    func_8007ED84(0x403B);
    func_8004284C();
}

void func_8013A0E8(void) {
    func_8007ED84(0x40BC);
    func_8004284C();
}

void func_8013A110(void) {
    func_8007ED84(0x40B2);
    func_8004284C();
}

void func_8013A138(void) {
    func_8007ED84(0x46C4);
    func_8004284C();
}

void func_8013A160(void) {
    func_80062CD0(0x5D6E);
    func_8004284C();
}

void func_8013A190(void) {
    if (D_80144E14 == 0) {
        func_8013A1CC();
        return;
    }
    func_8013A27C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013A1CC);

void func_8013A254(void) {
    func_80044750(0x502);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013A27C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013A2F8);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013A3FC);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013A440);

void func_8013A488(void) {
    func_8007ED84(0x4097);
    func_8004284C();
}

void func_8013A4B0(void) {
    if (D_800E71DF == 5) {
        func_80062CD0(0x5FE4);
    } else {
        func_80062CD0(0x552B);
    }
    func_8004284C();
}

void func_8013A500(void) {
    if (D_80144E14 == 0) {
        func_8013A53C();
        return;
    }
    func_8013A600();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013A53C);

void func_8013A5B0(void) {
    func_80044750(0x502);
    func_8004284C();
}

void func_8013A5D8(void) {
    func_80044750(0x503);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013A600);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013A688);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013A7B0);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013A7F4);

void func_8013A83C(void) {
    func_8007ED84(0x42D8);
    func_8004284C();
}

void func_8013A864(void) {
    func_8007ED84(0x40A0);
    func_8004284C();
}

void func_8013A88C(void) {
    func_80062CD0(0x6449);
    func_8004284C();
}

void func_8013A8B4(void) {
    func_80062CD0(0x625A);
    func_8004284C();
}

void func_8013A8DC(void) {
    if (((u32) D_800E6374 >> 0xC) == 9) {
        D_80144E08 = 5;
    }
    func_8004284C();
}

void func_8013A91C(void) {
    D_80144E08 = 3;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013A950);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013A9B0);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013AA24);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013AAAC);

void func_8013AB34(void) {
    func_80062CD0(0x66EC);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013AB5C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013AC7C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013ACC0);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013AD08);

void func_8013AD50(void) {
    if (((u8) D_800E62BF >= 6U) && ((u8) D_800E62BF < 0xAU)) {
        func_8007ED84(0x404D);
    } else {
        func_8007ED84(0x4055);
    }
    func_8004284C();
}

void func_8013ADA4(void) {
    func_8007ED84(0x4097);
    func_8004284C();
}

void func_8013ADCC(void) {
    if (((u32) D_800E652A >> 4) == 7) {
        D_80144E08 = 6;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013AE0C);

void func_8013AE6C(void) {
    if (((u32) D_800E652A >> 4) != ((u32) D_800E6374 >> 0xC)) {
        D_80144E08 = 4;
    }
    func_8004284C();
}

void func_8013AEB4(void) {
    if (((u32) D_800E652A >> 4) == ((u32) D_800E6374 >> 0xC)) {
        D_800E738A += 0xD;
        return;
    }
    func_8004284C();
}

void func_8013AF10(void) {
    func_8013AF30();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013AF30);

void func_8013AFA4(void) {
    func_80137AB4();
    if (D_80144E0C == 2) {
        D_80122D20 = 1;
        return;
    }
    D_80122D20 = 0;
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013AFEC);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013B114);

void func_8013B158(void) {
    func_8007ED84(0x4016);
    func_8004284C();
}

void func_8013B180(void) {
    if ((u8) D_800E6723 < 2U) {
        D_80144E08 += 1;
    }
    func_8004284C();
}

void func_8013B1C4(void) {
    if ((u8) D_800E6723 >= 2U) {
        D_80144E08 += 1;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013B210);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013B484);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013B528);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013B5B0);

void func_8013B62C(void) {
    func_80044890(1, 0xBF98, 0xBF79, 0xCA95, 0xCA4F, 0xCA3E);
    if (func_80044E8C() == 1) {
        func_80044750(0x202);
        func_8004284C();
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013B694);

void func_8013B71C(void) {
    func_80046318(9, 0x80197000, 0xAFB3);
    func_8013B210();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013B758);

void func_8013B8CC(void) {
    func_80085B3C(7, 0);
    func_8007ED84(0x4045);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013B900);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013B944);

void func_8013B984(void) {
    if (D_80145A10 != 0) {
        func_8004284C();
        return;
    }
    func_8013B528();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013B9C0);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013BA00);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013BA7C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013BC2C);

void func_8013BCE0(void) {
    D_800E71DF = (u8) D_80122CDC;
    func_80085B3C(7, 0);
    func_800847B8(D_800E71DF);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013BD2C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013BDE0);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013C070);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013C2E4);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013D180);

void func_8013D2A8(void) {
    D_800CA134 = &D_80145F2C;
    D_800CA138 = &D_80145F30;
    D_800CA13C = D_80145F20;
    D_800CA140 = D_80145F24;
    D_800CA144 = D_80145F28;
    func_80082764(D_80122CDC, 1, 0);
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013D324);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013D3B4);

void func_8013D560(void) {
    D_800CA134 = &D_800CA150;
    D_800CA138 = &D_800CA154;
    D_800CA13C = D_80145F34;
    D_800CA140 = D_80145F38;
    D_800CA144 = D_80145F3C;
    func_80082764(0xFF, 1, 0);
}

void func_8013D5D8(void) {
    D_800CA134 = (u8 *) &D_800CA150;
    D_800CA138 = (u8 *) &D_800CA154;
    D_800CA13C = D_80145F34;
    D_800CA140 = D_80145F38;
    D_800CA144 = D_80145F3C;
    func_80082764(D_80122CDC, 1, 0);
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013D660);

void func_8013D6D4(void) {
    func_80044890(1, 0xC621, 0xC613, 0xD93B, 0xD8F0, 0xD8D0);
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013D734);

void func_8013D7BC(void) {
    func_80046318(0xE, 0x80197000, 0xAFBC);
    func_8013C070();
    func_8004284C();
}

void func_8013D7F8(void) {
    D_80122CFC = 0x1E;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013D820);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013D92C);

void func_8013DA5C(void) {
    if (((u32) (D_800E6758 << 0x14) >> 0x1D) == 1) {
        func_8013D2A8();
        return;
    }
    func_8004284C();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013DAAC);

void func_8013DCFC(void) {
    func_8007ED84(0x41CA);
    func_8004284C();
}

void func_8013DD24(void) {
    func_80062CD0(0x6B24);
    func_8004284C();
}

void func_8013DD4C(void) {
    func_80042808();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013DD6C);

void func_8013DD90(void) {
    D_80145FD0 = 0x801A6558;
    D_80145FD4 = 0x801A655C;
    D_80145FD8 = 0x801A6580;
    D_80145FE0 = 0x801A0000;
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013DDD0);

void func_8013DEBC(void) {
    func_80044750(0x204);
    func_8004284C();
}

void func_8013DEE4(void) {
    D_80120653 |= 0x80;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013DF14);

void func_8013E0C8(void) {
    func_8007ED84(0x41D4);
    func_8004284C();
}

void func_8013E0F0(void) {
    D_80145F2C = (D_80145F2C + (((u32) (D_800E66E8 << 0x14) >> 0x1D) * 3)) - 3;
    func_8004284C();
}

void func_8013E13C(void) {
    func_80062CD0(0x5855);
    func_8004284C();
}

void func_8013E164(void) {
    func_80046318(0xD, 0x801A0000, 0xBD9F);
    func_8013DD90();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013E19C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013E330);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013E410);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013E464);

void func_8013E4B4(void) {
    if (D_80146050 != 0) {
        if (D_80145F2C == 1) {
            D_80145F2C += 1;
        } else {
            D_80145F2C += 2;
        }
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013E50C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013E6E4);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013EA98);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013EE30);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013F070);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013F190);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013F1E0);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013F224);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013F360);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013F3C0);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013F494);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013F9C4);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013FA80);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8013FB28);

void func_8013FFA8(void) {
    func_8007ED84(0x41E4);
    func_8004284C();
}

void func_8013FFD0(void) {
    s16 i;

    for (i = 0; i < 6; i++) {
        D_8011ECD0[i * 0x44 + 0x1983] = 0;
    }
    D_80146160 = 0;
    D_80146164 = 0;
    func_8004284C();
}

s32 func_80140030(void) {
    D_80122D2C = 0;
    if (D_80145FE4 == 0) {
        D_800E71DF = 0;
        D_80122CD4 = 0;
        D_80122CD0 = 7;
        func_80042908(6);
        D_800E67F2 += 1;
        return 0;
    }
    if (D_80145FE4 == 7) {
        D_800E71DF = 7;
        D_80122CD4 = 7;
        D_80122CD0 = 7;
        func_80042908(7);
        D_800E6820 += 1;
        return 0;
    }
    if (D_80145FE4 == 9) {
        D_800E71DF = 9;
        D_80122CD4 = 9;
        D_80122CD0 = 7;
        func_80042908(8);
        D_800E682E += 1;
        return 0;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80140144);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_801401DC);

void func_80140220(void) {
    func_80046318(0x4D, 0x801B0000, 0xBDAC);
    func_8004284C();
}

void func_80140250(void) {
    func_80046318(0xE, 0x801D7000, 0xBDF9);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80140284);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80140314);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80140510);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80140698);

void func_80140780(void) {
    D_80146230 = 0x801B21E0;
    D_80146234 = 0x801B21E4;
    D_80146238 = 0x801B2228;
    D_80146240 = 0x801B0000;
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_801407C0);

void func_8014088C(void) {
    func_80044750(0xBF);
    func_80044890(1, 0xBF98, 0xBF79, D_800B36AC, D_800B36EC, D_800B372C);
    if (func_80044E8C() == 1) {
        func_80044750(0x200);
        func_8004284C();
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80140908);

void func_80140990(void) {
    func_80044750(0xBF);
    func_80044890(1, 0xC621, 0xC613, 0xD93B, 0xD8F0, 0xD8D0);
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_801409F8);

void func_80140A78(void) {
    func_80046318(0xD, 0x801B0000, 0xBCBB);
    func_80140780();
    func_8004284C();
}

void func_80140AB0(void) {
    func_80044750(0x203);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80140AD8);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80140BD8);

void func_80140D30(void) {
    func_8007ED84(0x41DC);
    func_8004284C();
}

void func_80140D58(void) {
    D_80120652 = 9;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80140D80);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80140DC0);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80140F54);

void func_80140FDC(void) {
    D_80145F2C = 3;
    func_8004284C();
}

void func_80141004(void) {
    func_80044750(0x506);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8014102C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_801410B4);

void func_801410F0(void) {
    if (((u32) D_800E64BA >> 4) == 3) {
        D_80145F2C += 1;
    }
    func_8004284C();
}

void func_80141138(void) {
    if (((u32) D_800E64BA >> 4) != 3) {
        D_80145F2C += 1;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80141180);

void func_801411FC(void) {
    D_80145F2C = 3;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80141224);

void func_801412AC(void) {
    func_80044750(0x501);
    func_8004284C();
}

void func_801412D4(void) {
    if (((u32) D_800E652A >> 4) == 7) {
        D_80145F2C += 3;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8014131C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80141358);

void func_801413CC(void) {
    D_80145F2C = 3;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_801413F4);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8014147C);

void func_801414B8(void) {
    func_80044750(0x505);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_801414E0);

void func_80141568(void) {
    if (func_8004ECB4() != 0) {
        func_8004E9F4(1);
        func_8004284C();
    }
}

void func_801415A0(void) {
    D_80145F2C = 3;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_801415C8);

void func_80141650(void) {
    D_80145F2C = 3;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80141678);

void func_801416EC(void) {
    D_80145F2C = 3;
    func_8004284C();
}

void func_80141714(void) {
    func_80062CD0(0x56C0);
    func_8004284C();
}

void func_8014173C(void) {
    func_80042908((s32) D_800E69A1);
    func_80042940((s32) D_800E69A2);
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80141780);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80141850);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80141920);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_801419D0);

void func_80141AC0(void) {
    D_800B5A60 = 1;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80141AE8);

void func_80141B98(void) {
    func_80044750(0x208);
    func_8004284C();
}

void func_80141BC0(void) {
    D_8014653C = func_8004EC14();
    D_80122CFC = 0x72;
    func_8004EBEC(0x1E);
    func_8004E884(2);
    func_8004284C();
}

void func_80141C0C(void) {
    func_8004EBEC(D_8014653C);
    func_8004E884(1);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80141C44);

void func_80141CA0(void) {
    func_80046318(0x55, 0x801B0000, 0x8065);
    func_80141780();
    func_8004284C();
}

void func_80141CD8(void) {
    D_801467FC = 1;
    if (D_800E71DF == 0) {
        func_80044750(0x206);
    }
    D_80121864 = -0x64;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80141D20);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_801421D4);

void func_801424CC(void) {
    func_8007ED84(0x475A);
    func_80085B3C(2, 6);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80142500);

void func_8014257C(void) {
    func_80062CD0(0x5C8D);
    func_8004284C();
}

void func_801425A4(void) {
    func_80044750(0x207);
    func_8004284C();
}

void func_801425CC(void) {
    func_80083440(5);
    func_8004284C();
}

void func_801425F4(void) {
    func_80046318(0x45, 0x801B0000, 0x8571);
    func_80141850();
    func_8004284C();
}

void func_8014262C(void) {
    D_801206DB &= 0x7F;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8014265C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80142784);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80142844);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_8014288C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80142DA0);

void func_8014306C(void) {
    func_8007ED84(0x4752);
    func_80085B3C(2, 0x34);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_801430A0);

void func_80143200(void) {
    func_80044750(0x205);
    func_8004284C();
}

void func_80143228(void) {
    func_80044750(0x504);
}

void func_80143248(void) {
    func_80046318(0x45, 0x801B0000, 0x8BC9);
    func_80141920();
    func_8004284C();
}

void func_80143280(void) {
    D_801206F0 = 0;
    D_801206E0 = 0;
    D_801206DA = 3;
    func_80044750(0x503);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_801432C4);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_801433A4);

void func_801434E0(void) {
    func_8007ED84(0x42E2);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU", func_80143508);
