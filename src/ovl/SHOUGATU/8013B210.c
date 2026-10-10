#include "common.h"
#include "ovl/SHOUGATU.h"

void func_8013B210(void) {
    D_80145960 = 0x80197270;
    D_80145964 = 0x801977D8;
    D_80145968 = 0x80197D18;
    D_8014596C = 0x8019834C;
    D_80145970 = 0x801988F4;
    D_80145974 = 0x80198E54;
    D_80145978 = 0x8019948C;
    D_8014597C = 0x80199AC8;
    D_80145980 = 0x8019A0C4;
    D_80145984 = 0x8019A694;
    D_80145988 = 0x8019AC3C;
    D_8014598C = 0x8019AFD4;
    D_80145990 = 0x8019B354;
    D_80145994 = 0x801972C0;
    D_80145998 = 0x80197824;
    D_8014599C = 0x80197D64;
    D_801459A0 = 0x80198394;
    D_801459A4 = 0x80198940;
    D_801459A8 = 0x80198EA4;
    D_801459AC = 0x801994D8;
    D_801459B0 = 0x80199B14;
    D_801459B4 = 0x8019A114;
    D_801459B8 = 0x8019A6E0;
    D_801459BC = 0x8019AC88;
    D_801459C0 = 0x8019AFE4;
    D_801459C4 = 0x8019B3A0;
    D_801459C8 = 0x80197514;
    D_801459CC = 0x80197A5C;
    D_801459D0 = 0x80197F9C;
    D_801459D4 = 0x80198654;
    D_801459D8 = 0x80198B78;
    D_801459DC = 0x80199114;
    D_801459E0 = 0x80199748;
    D_801459E4 = 0x80199D84;
    D_801459E8 = 0x8019A384;
    D_801459EC = 0x8019A934;
    D_801459F0 = 0x8019AEF8;
    D_801459F4 = 0x8019B05C;
    D_801459F8 = 0x8019B610;
}

void func_8013B484(void) {
    func_80083808();
    switch (D_800E7389) {
    case 0:
        func_8013B5B0();
        break;
    case 1:
        func_8013BA00();
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

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013B210", func_8013B528);
