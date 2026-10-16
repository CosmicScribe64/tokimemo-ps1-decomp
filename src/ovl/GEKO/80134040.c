#include "common.h"
#include "ovl/GEKO.h"

void func_801341A0(void);

void func_80134040(void) {
    func_80083808();
    if (D_800E6280.unk_1109 == 0) {
        func_801341A0();
    } else {
        func_80046500();
    }
    check_k_scroll();
    k_disp_inc2();
    if (D_80122D38 == 0) {
        func_80066C08(2);
    }
    message_window_show();
    func_80066334();
    hizuke_show();
    if (D_80122D38 == 0) {
        func_80083A10();
    }
    if (D_80122D38 != 0) {
        func_80066C08(1);
        func_800578F4(0);
        parameter_show();
        if ((D_80122D2C == 0) && (D_80122D04 == 0) && ((D_800E6280.unk_F80 & 0xF000F000) || (D_80122D30 == 1))) {
            D_8011ECD3 |= 0x80;
        }
        if (D_80122D04 != 0) {
            D_8011ECD3 &= 0x7F;
        }
        if (D_8011ECD3 & 0x80) {
            func_8006BA40();
        }
    }
}

typedef struct {
    void (*f[29])();
} FnTbl29; /* size 0x74 */
extern FnTbl29 D_80144E94;

void func_801341A0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl29 tbl;

    tbl = D_80144E94;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80134040", func_80134228);

s32 func_8013431C(void) {
    if (D_800E6280.unk_044[0].unk_20[D_800E6280.unk_040] == 5) {
        if (D_800E6280.unk_1104.w++ == 0) {
            func_80044750(0xBF);
        }
        if ((u32)D_800E6280.unk_1104.w < 0x80U) {
            return 0;
        }
    }
    func_80044890(1, 0xBF98, 0xBF79, 0xCA95, 0xCA4F, 0xCA3E);
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    func_8004284C();
}

void func_801343DC(void) {
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    if ((u32) D_800E6280.unk_1104.w++ >= 0x400U) {
        func_800452C4();
        func_8004482C();
        func_80042940((D_800E6280.unk_110A - 1) & 0xFF);
    }
}

void func_8013445C(void) {
    k_reset(1);
    func_80044750(0x200);
    func_8004284C();
}

void func_8013448C(void) {
    cd_read_sectors(3, 0x801B0000, 0xAF43);
    func_80134F84();
    if (!(D_800E6280.unk_71E & 8)) {
        func_80044750(0x603);
        func_8004E788(-0x28, 0x40, 2, "ピンポーン", 0);
    }
    func_8004284C();
}

void func_80134500(void) {
    cd_read_sectors(3, 0x801B0000, 0xAF43);
    func_80134F84();
    func_8004284C();
}

s32 func_80134538(void) {
    s16 i;
    u8 saved;

    if (*(u8 *)&D_800E6280.unk_71E & 4) {
        func_80134228();
        return 0;
    }
    *(u8 *)&D_800E6280.unk_71E |= 4;
    func_800438DC(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    func_800438F0(1);
    func_80048390();
    func_8004E58C();
    func_800AE0F0(D_800CA19C, "玄関");
    func_8004E9F4(1);
    D_800E6280.unk_10A2 = 0;
    D_800E6280.unk_10E8 = 1;
    saved = D_80122D40;
    func_8008585C();
    D_80122D40 = saved;
    D_80122D38 = 1;
    D_800E6280.unk_03A = 0x80;
    D_800B593C = 0;
    D_800B5940 = 0;
    func_8007C740();
    func_800649D4();
    func_80064E84();
    func_8006509C();
    for (i = 0; i < 0xF; i++) {
        func_80065048(i, 0);
    }
    func_80065B0C(1);
    func_800639D8();
    func_800639D8();
    func_80084E4C();
    D_80144E8C = 0;
    D_80144E90 = 0;
    func_8004284C();
}

void func_801346A0(void) {
    s32 pad; /* FAKE: unused slot above sp2B, the original frame has it (real source unknown). T-8070 */
    u8 sp2B;

    func_800438DC(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    func_800438F0(1);
    func_80048390();
    func_8004E58C();
    sp2B = D_80122D40;
    func_8008585C();
    D_80122D40 = sp2B;
    func_800AE0F0(D_800CA19C, "玄関");
    func_8004E9F4(1);
    D_800E6280.unk_10A2 = 0;
    D_800E6280.unk_10E8 = 1;
    D_800E6280.unk_03A = 0x80;
    D_800B593C = 0;
    D_800B5940 = 0;
    func_800649D4();
    func_80064E84();
    func_80084E4C();
    func_8007ED84(0x4144);
    func_80085B3C(0xD, 0x25);
    func_8004284C();
}

void func_8013478C(void) {
    D_800CA134 = &D_80144E8C;
    D_800CA138 = &D_80144E90;
    D_800CA13C = D_80144E80;
    D_800CA140 = D_80144E84;
    D_800CA144 = D_80144E88;
    func_80082764(D_80122CDC, 1, 0);
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80134040", func_80134808);

void func_801349B8(void) {
    func_80134F84();
    func_800847B8(0);
    D_80144E80 = D_80144FD8;
    D_80144E84 = D_8014500C;
    D_80144E88 = D_80145040;
    func_8004284C();
}

void func_80134A14(void) {
    switch (D_800E6280.unk_F5F) {
    case 0:
        D_80144E80 = D_80144FB0;
        D_80144E84 = D_80144FE4;
        D_80144E88 = D_80145018;
        return;
    case 1:
        D_80144E80 = D_80144FC4;
        D_80144E84 = D_80144FF8;
        D_80144E88 = D_8014502C;
        return;
    case 2:
        D_80144E80 = D_80144FB8;
        D_80144E84 = D_80144FEC;
        D_80144E88 = D_80145020;
        return;
    case 3:
        D_80144E80 = D_80144FBC;
        D_80144E84 = D_80144FF0;
        D_80144E88 = D_80145024;
        return;
    case 4:
        D_80144E80 = D_80144FD4;
        D_80144E84 = D_80145008;
        D_80144E88 = D_8014503C;
        return;
    case 5:
        D_80144E80 = D_80144FC8;
        D_80144E84 = D_80144FFC;
        D_80144E88 = D_80145030;
        return;
    case 6:
        D_80144E80 = D_80144FCC;
        D_80144E84 = D_80145000;
        D_80144E88 = D_80145034;
        return;
    case 7:
        D_80144E80 = D_80144FC0;
        D_80144E84 = D_80144FF4;
        D_80144E88 = D_80145028;
        return;
    case 8:
        D_80144E80 = D_80144FAC;
        D_80144E84 = D_80144FE0;
        D_80144E88 = D_80145014;
        return;
    case 9:
        D_80144E80 = D_80144FD0;
        D_80144E84 = D_80145004;
        D_80144E88 = D_80145038;
        return;
    case 10:
        D_80144E80 = D_80144FDC;
        D_80144E84 = D_80145010;
        D_80144E88 = D_80145044;
        /* fallthrough */
    default:
        return;
    }
}
