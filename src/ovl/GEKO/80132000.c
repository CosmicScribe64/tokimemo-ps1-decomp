#include "common.h"
#include "ovl/GEKO.h"

void func_80132000(void) {
    D_80144BA0 = 0x80197BFC;
    D_80144BA4 = 0x801996B8;
    D_80144BA8 = 0x8019B050;
    D_80144BAC = 0x8019C9A8;
    D_80144BB0 = 0x8019E3F0;
    D_80144BB4 = 0x8019FF58;
    D_80144BB8 = 0x801A18FC;
    D_80144BBC = 0x801A3030;
    D_80144BC0 = 0x801A48F4;
    D_80144BC4 = 0x801A63EC;
    D_80144BC8 = 0x801A7324;
    D_80144BCC = 0x801A7F90;
    D_80144BD0 = 0x80197D80;
    D_80144BD4 = 0x80199838;
    D_80144BD8 = 0x8019B1C8;
    D_80144BDC = 0x8019CB2C;
    D_80144BE0 = 0x8019E584;
    D_80144BE4 = 0x801A00D8;
    D_80144BE8 = 0x801A1A60;
    D_80144BEC = 0x801A31AC;
    D_80144BF0 = 0x801A4A74;
    D_80144BF4 = 0x801A657C;
    D_80144BF8 = 0x801A7354;
    D_80144BFC = 0x801A8108;
    D_80144C00 = 0x80198980;
    D_80144C04 = 0x8019A3C8;
    D_80144C08 = 0x8019BD20;
    D_80144C0C = 0x8019D6BC;
    D_80144C10 = 0x8019F1C4;
    D_80144C14 = 0x801A0C84;
    D_80144C18 = 0x801A24F4;
    D_80144C1C = 0x801A3D04;
    D_80144C20 = 0x801A5674;
    D_80144C24 = 0x801A7134;
    D_80144C28 = 0x801A7414;
    D_80144C2C = 0x801A8C28;
}

void func_80132244(void) {
    func_80083808();
    switch (D_800E6280.unk_1109) {                           /* irregular */
    case 0:
        func_801323E0();
        break;
    case 1:
        func_801338F0();
        break;
    default:
        func_80046500();
        break;
    }
    check_k_scroll();
    k_disp_inc2();
    func_80066C08(2);
    message_window_show();
    func_80066334();
    hizuke_show();
    func_80083A10();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80132000", func_801322E8);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80132000", func_80132364);
