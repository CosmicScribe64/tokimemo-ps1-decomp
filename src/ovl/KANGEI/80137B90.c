#include "common.h"
#include "ovl/KANGEI.h"

void func_80137B90(void) {
    D_8013A170 = 0x801B00DC;
    D_8013A174 = 0x801B02AC;
    D_8013A178 = 0x801B03C8;
    D_8013A17C = 0x801B04C4;
    D_8013A180 = 0x801B0668;
    D_8013A184 = 0x801B0808;
    D_8013A188 = 0x801B09DC;
    D_8013A18C = 0x801B0BE8;
    D_8013A190 = 0x801B0D8C;
    D_8013A194 = 0x801B0F5C;
    D_8013A198 = 0x801B1164;
    D_8013A19C = 0x801B12D8;
    D_8013A1A0 = 0x801B1454;
    D_8013A1A4 = 0x801B00F4;
    D_8013A1A8 = 0x801B02C8;
    D_8013A1AC = 0x801B03D0;
    D_8013A1B0 = 0x801B04DC;
    D_8013A1B4 = 0x801B0680;
    D_8013A1B8 = 0x801B0820;
    D_8013A1BC = 0x801B09F8;
    D_8013A1C0 = 0x801B0C00;
    D_8013A1C4 = 0x801B0DA4;
    D_8013A1C8 = 0x801B0F78;
    D_8013A1CC = 0x801B1180;
    D_8013A1D0 = 0x801B12E0;
    D_8013A1D4 = 0x801B1470;
    D_8013A1D8 = 0x801B01A8;
    D_8013A1DC = 0x801B0398;
    D_8013A1E0 = 0x801B03F0;
    D_8013A1E4 = 0x801B0590;
    D_8013A1E8 = 0x801B0734;
    D_8013A1EC = 0x801B08D4;
    D_8013A1F0 = 0x801B0AE4;
    D_8013A1F4 = 0x801B0CB4;
    D_8013A1F8 = 0x801B0E58;
    D_8013A1FC = 0x801B1064;
    D_8013A200 = 0x801B126C;
    D_8013A204 = 0x801B1338;
    D_8013A208 = 0x801B155C;
}

void func_80137E04(void) {
    D_8013A20C = 0x801B0478;
    D_8013A210 = 0x801B13A4;
    D_8013A214 = 0x801B2998;
    D_8013A218 = 0x801B38C8;
    D_8013A21C = 0x801B4258;
    D_8013A220 = 0x801B4B98;
    D_8013A224 = 0x801B5574;
    D_8013A228 = 0x801B6560;
    D_8013A22C = 0x801B7B74;
    D_8013A230 = 0x801B8B18;
    D_8013A234 = 0x801B93D0;
    D_8013A238 = 0x801B9DA0;
    D_8013A23C = 0x801BA780;
    D_8013A240 = 0x801B0510;
    D_8013A244 = 0x801B14E0;
    D_8013A248 = 0x801B2AD0;
    D_8013A24C = 0x801B3960;
    D_8013A250 = 0x801B42F0;
    D_8013A254 = 0x801B4C38;
    D_8013A258 = 0x801B560C;
    D_8013A25C = 0x801B66A0;
    D_8013A260 = 0x801B7CB4;
    D_8013A264 = 0x801B8BB0;
    D_8013A268 = 0x801B9468;
    D_8013A26C = 0x801B9EA8;
    D_8013A270 = 0x801BA818;
    D_8013A274 = 0x801B0888;
    D_8013A278 = 0x801B1E14;
    D_8013A27C = 0x801B33B0;
    D_8013A280 = 0x801B3CF4;
    D_8013A284 = 0x801B4684;
    D_8013A288 = 0x801B4FEC;
    D_8013A28C = 0x801B59F4;
    D_8013A290 = 0x801B6FB8;
    D_8013A294 = 0x801B85CC;
    D_8013A298 = 0x801B8F28;
    D_8013A29C = 0x801B97E0;
    D_8013A2A0 = 0x801BA2C8;
    D_8013A2A4 = 0x801BAB90;
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80137B90", func_80138078);

void func_8013812C(void) {
    D_80139AC4 = 1;
    func_80042940(0);
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80137B90", func_80138158);

void func_80138264(void) {
    D_8013A2A8 = 1;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80137B90", func_8013828C);

void func_80138320(void) {
    u8 s;

    s = get_g_zyotai_s(0) & 0x7F;
    if (s >= 2U) {
        D_800E738A -= 0xB;
        return;
    }
    func_8004284C();
}

void func_8013836C(void) {
    if (((u32)(D_800E6378 << 0x17) >> 0x1B) == D_800E62C0 && (D_800E6378 & 0xF) == D_800E62BF) {
        D_800E738A += 0xC;
        return;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80137B90", func_801383D0);

void func_8013844C(void) {
    D_80139AC0 = 0;
    func_80133C84();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80137B90", func_80138470);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80137B90", func_8013852C);

void func_801385E0(void) {
    D_80139AD0 = (KObj *)D_8013A210;
    D_80139AD4 = D_8013A244;
    D_80139AD8 = D_8013A278;
    D_80139ADC = 3;
    func_80139540();
    func_8004284C();
}

void func_80138640(void) {
    s32 unused; /* FAKE: the original frame has one more 4-byte local above sp2A (same extra scalar as the T-3330 table functions); its real use is unknown. T-4040 */
    s16 sp2A;

    sp2A = (s16) D_80122CDC;
    func_80134374();
    D_80122CDC = (s32) sp2A;
    D_80139AC0 = 0;
    D_800E71DF = 0;
    func_800847B8(0U);
    D_80139AD0 = (KObj *) D_8013A210;
    D_80139AD4 = D_8013A244;
    D_80139AD8 = D_8013A278;
    D_80139ADC = 4;
}

void func_801386C4(void) {
    u8 s;

    s = get_g_zyotai_s(0) & 0x7F;
    if ((s < 2U) && (D_80122CDC < 2)) {
        func_80062CD0(0x5774);
    } else {
        func_80062CD0(0x55DF);
    }
    func_8004284C();
}

void func_80138728(void) {
    u8 s;

    s = get_g_zyotai_s(0) & 0x7F;
    if (s == 2) {
        D_80139ADC += 2;
    } else if (s == 3) {
        D_80139ADC += 4;
    } else if (s == 4) {
        D_80139ADC += 6;
    }
    func_8004284C();
}

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

void func_80138884(void) {
    u8 s;

    s = get_g_zyotai_s(0) & 0x7F;
    D_80139ADC = (s >= 2U) * 2 + (s >= 3U) * 2 + (s >= 4U) * 2 + 0x16;
    func_8004284C();
}

void func_801388E4(void) {
    u8 s;

    s = get_g_zyotai_s(0) & 0x7F;
    D_80139ADC = (s >= 2U) * 2 + (s >= 3U) * 2 + (s >= 4U) * 2 + 6;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80137B90", func_80138944);

void func_801389F4(void) {
    D_800E71DF = D_8013A2B0;
    func_801392D4();
    D_80139ADC = 0;
    D_80139AE0 = 0;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80137B90", func_80138A34);

void func_80138B28(void) {
    D_800B3D60 = 0;
    func_80044890(1, 0xBF98, 0xBF79, 0xCA95, 0xCA4F, 0xCA3E);
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    func_8004284C();
}

void func_80138B90(void) {
    if (func_80044E8C() == 1) {
        func_80044750(0x200);
        func_8004284C();
    }
    if ((u32) D_800E7384++ >= 0x400U) {
        func_800452C4();
        func_8004482C();
        func_80042940((D_800E738A - 1) & 0xFF);
    }
}

void func_80138C18(void) {
    draw2d3d(1, 0);
    func_80048DAC(1);
    func_80041584();
    back_clear_switch(1);
    func_80048EB8(0);
    message_window_init();
    hizuke_init();
    tpage_buf_clear();
    func_8004E58C();
    D_800E7322 = 0;
    D_800E7368 = 1;
    D_800B593C = 0;
    func_8008585C();
    parameter_disp_switch(0);
    parameter_disp_switch(0);
    icon_disp_switch(0);
    addr_init_bustup();
    func_80084E4C();
    D_80139AC0 = 0;
    func_8004284C();
}

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

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80137B90", func_80138D40);

void func_80138E3C(void) {
    bg_read_sub2(0x42A9);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80137B90", func_80138E64);

void func_80138F54(void) {
    D_80139AD0 = (KObj *)D_8013A210;
    D_80139AD4 = D_8013A244;
    D_80139AD8 = D_8013A278;
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80137B90", func_80138F88);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80137B90", func_801392D4);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80137B90", func_80139540);
