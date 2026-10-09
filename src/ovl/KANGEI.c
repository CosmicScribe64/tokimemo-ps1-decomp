#include "common.h"
#include "ovl/KANGEI.h"

void func_80132000(void) {
    func_80083808();
    if (D_800E7389 == 0) {
        func_80132214();
    } else {
        func_80046500();
    }
    check_k_scroll();
    k_disp_inc2();
    func_80066C08(2);
    message_window_show();
    func_80066334();
    hizuke_show();
    func_80083A10();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80132090);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80132214);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80132290);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80132354);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_801323A8);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80132430);

void func_801324B0(void) {
    func_80044750(0x200);
    func_8004284C();
}

void func_801324D8(void) {
    func_80046318(3, 0x80197000, 0xAF3C);
    func_80132090();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80132514);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_801325D0);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_8013260C);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_801327DC);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80132C24);

void func_80132E40(void) {
    D_80139A50 = 0x801C1400;
    D_80139A54 = 0x801C1D6C;
    D_80139A58 = 0x801CA47C;
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80132E74);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80132FF8);

void func_801335A0(void) {
    D_800E699C += 1;
    D_80139AC4 = 0;
    D_80139AC8 = 0;
    func_80072338();
    hizuke_init();
    func_80042808();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_801335F0);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80133734);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_801337B4);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80133834);

void func_801338B4(void) {
    if (D_800E7384 == 0) {
        func_8004500C(0, 0);
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_801338EC);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80133A14);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80133A54);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80133C10);

void func_80133C84(void) {
    D_800E71DF = D_800E69DD;
    func_80042808();
}

void func_80133CB0(void) {
    func_80046318(0x3B, 0x80197000, 0xAE60);
    func_80132E74();
    func_8004284C();
}

void func_80133CEC(void) {
    func_80046318(0x2D, 0x801B4400, 0xAE9B);
    func_80132E40();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80133D28);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80133EF8);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80133F74);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80133FF4);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80134084);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80134120);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_801341E8);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_8013425C);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_801342EC);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80134374);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_8013454C);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80134724);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_801349D4);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80134BC4);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80134DB0);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80134EA8);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80134F00);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80135004);

void func_801350E0(void) {
    RECT rect;

    func_80083474();
    rect.x = 0x140;
    rect.y = 0x80;
    rect.w = 0x180;
    rect.h = 0x80;
    func_8009C884(&rect, (void *)0x80180000);
}

void func_8013512C(void) {
    *(s16 *)((u8 *)&D_800E6442 + D_800E71DF * 0x38) = 0x32;
    func_80044750(0x24);
    func_80135438();
}

void func_80135178(void) {
    if (D_800E71DF == 6) {
        don_wait();
        return;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_801351B8);

void func_80135220(void) {
    func_80044750(0x24);
    func_80044750(0x501);
    func_8004284C();
}

void func_80135250(void) {
    *(s16 *)((u8 *)&D_800E6442 + D_800E71DF * 0x38) = 0x46;
    D_80139ADC = 3;
    func_801349D4();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_801352A4);

void func_80135314(void) {
    bg_read_sub2(0x42C6);
    func_80085B3C(0xE, 0xB);
    func_800AE0F0(D_800CA1DC, &D_8013980C);
    func_8004284C();
}

void func_8013535C(void) {
    bg_read_sub2(0x429F);
    func_80085B3C(0xE, 0xC);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80135390);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80135438);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_801354AC);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80135570);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_801355F0);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80135634);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80135678);

void func_801357A4(void) {
    D_800E71DF = D_800E69DD;
    func_8004284C();
}

void func_801357D0(void) {
    func_80044750(0x23);
    func_80044750(0x24);
    func_80044750(0x2E);
    func_8004284C();
}

void func_80135808(void) {
    func_80044750(0x24);
    func_80044750(0x502);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80135838);

void func_80135AC0(void) {
    func_8004284C();
}

void func_80135AE0(void) {
    func_80042808();
    func_80042808();
    func_80042808();
    func_80042808();
}

void func_80135B18(void) {
    bg_read_sub2(0x42A9);
    func_800AE0F0(D_800CA19C, &D_801398B0);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80135B54);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80135C18);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80135C78);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80135CE0);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80135E90);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80135F10);

void func_80135F84(void) {
    D_80139DCC = 0x801EAFC4;
    D_80139DD0 = 0x801EB144;
    D_80139DD4 = 0x801EAFF0;
    D_80139DD8 = 0x801EB14C;
    D_80139DDC = 0x801EB030;
    D_80139DE0 = 0x801EB174;
    D_80139DE4 = *(s16 *)0x801EB17C;
    D_80139DE8 = *(s16 *)0x801EB180;
    D_80139DEC = 0x801B4400;
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80136018);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80136094);

void func_80136108(void) {
    if (D_80139AE0 == 0) {
        D_80139AE4 = 2;
    } else {
        D_80139AE4 = 0;
    }
    func_80133EF8();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_8013614C);

void func_801361F8(void) {
    func_80044750(0x23);
    func_80044750(0x24);
    func_80044750(0x2E);
    func_80044750(0x203);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80136238);

void func_801362BC(void) {
    func_80044750(0x500);
    D_80139DF0 = D_80122CDC;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_801362F0);

void func_80136494(void) {
    load_palette(D_80139DB0, 0x11, 1, 2, 0);
    func_80084E90(D_80139DB4, D_80139DB8, D_80139DBC, D_80139DC0, D_80139DC4, D_80139DC8);
    func_800850D4(0, 0, 0, 0);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80136520);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_801365C4);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80136630);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80136810);

void func_80136900(void) {
    D_80139DFC += 1;
    normal_date_two_select();
}

void func_80136930(void) {
    if (D_80122CDC != 0) {
        D_800E738A += 0x13;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80136970);

void func_80136A24(void) {
    D_80139ADC = 0xC;
    D_80139AE0 = (D_80139DFC >= 0x97U) + (D_80139DFC >= 0x12DU);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80136A70);

void func_80136BB8(void) {
    bg_read_sub2(0x42BC);
    func_8004284C();
    func_800AE0F0(D_800CA19C, &D_801398E0);
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80136BF4);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80136CD8);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80136D58);

void func_80136DF4(void) {
    func_80081190(-0x64, 0x30, 2, D_80139AD0->unk_34->unk_08, 0, &D_801398E8, &D_801398EC, 0);
    func_8004284C();
}

void func_80136E54(void) {
    func_80081190(-0x56, 0x40, 1, D_80139AD0->unk_34->unk_0C, 0, &D_801398F0, &D_801398F4, 0);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80136EB4);

void func_80136F1C(void) {
    D_80139AE4 = 2;
    func_8004284C();
}

void func_80136F44(void) {
    D_80139AE4 = 1;
    func_8004284C();
}

void func_80136F6C(void) {
    D_80139AE4 = 0;
    func_8004284C();
}

void func_80136F90(void) {
    func_80044750(0xBF);
    func_80044890(1, 0xBF98, 0xBF79, 0xC982, 0xC935, 0xC91B);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80136FDC);

void func_80137064(void) {
    func_80046318(0x6E, 0x801B4400, 0xBBA5);
    func_80135F84();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_801370A0);

void func_80137440(void) {
    D_80120666 = 2;
    D_80120668 = 0;
    D_80120658 = 0;
    D_80120652 = 5;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80137484);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80137544);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80137638);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_8013769C);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80137B90);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80137E04);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80138078);

void func_8013812C(void) {
    D_80139AC4 = 1;
    func_80042940(0);
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80138158);

void func_80138264(void) {
    D_8013A2A8 = 1;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_8013828C);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80138320);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_8013836C);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_801383D0);

void func_8013844C(void) {
    D_80139AC0 = 0;
    func_80133C84();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80138470);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_8013852C);

void func_801385E0(void) {
    D_80139AD0 = (KObj *)D_8013A210;
    D_80139AD4 = D_8013A244;
    D_80139AD8 = D_8013A278;
    D_80139ADC = 3;
    func_80139540();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80138640);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_801386C4);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80138728);

void func_801387B8(void) {
    func_80083440(D_80122CDC + 1);
    D_80139ADC = (D_80122CDC * 2) + 0xD;
    func_8004284C();
}

void func_801387FC(void) {
    if (D_80122CDC != 0) {
        D_80139AC0 = 1;
        D_800E738A += 0x39;
        return;
    }
    D_80139ADC += 1;
    func_8004284C();
}

void func_8013885C(void) {
    D_80139ADC = 0x22;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80138884);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_801388E4);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80138944);

void func_801389F4(void) {
    D_800E71DF = D_8013A2B0;
    func_801392D4();
    D_80139ADC = 0;
    D_80139AE0 = 0;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80138A34);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80138B28);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80138B90);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80138C18);

void func_80138CD0(void) {
    func_80046318(0x16, 0x801B0000, 0xAF0D);
    func_80137E04();
    func_8004284C();
}

void func_80138D08(void) {
    func_80046318(3, 0x801B0000, 0xAF43);
    func_80137B90();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80138D40);

void func_80138E3C(void) {
    bg_read_sub2(0x42A9);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80138E64);

void func_80138F54(void) {
    D_80139AD0 = (KObj *)D_8013A210;
    D_80139AD4 = D_8013A244;
    D_80139AD8 = D_8013A278;
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80138F88);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_801392D4);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI", func_80139540);
