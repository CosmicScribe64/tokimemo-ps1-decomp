#include "common.h"
#include "ovl/SHOUGATU.h"

void func_8013D660(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl33 tbl;

    tbl = D_80145F40;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_8013D6D4(void) {
    func_80044890(1, 0xC621, 0xC613, 0xD93B, 0xD8F0, 0xD8D0);
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    func_8004284C();
}

void func_8013D734(void) {
    if (func_80044E8C() == 1) {
        func_80044750(0x202);
        func_8004284C();
    }
    if ((u32) D_800E6280.unk_1104.w++ >= 0x400U) {
        func_800452C4();
        func_8004482C();
        func_80042940((D_800E6280.unk_110A - 1) & 0xFF);
    }
}

void func_8013D7BC(void) {
    cd_read_sectors(0xE, 0x80197000, 0xAFBC);
    func_8013C070();
    func_8004284C();
}

void func_8013D7F8(void) {
    D_80122CFC = 0x1E;
    func_8004284C();
}

void func_8013D820(void) {
    s16 thresh = D_800E6280.unk_1BC[12].unk_0C.f.f9 * 25 + 150;

    if (D_800E6280.unk_10C.unk_02 >= thresh &&
        D_800E6280.unk_1BC[14].unk_0C.f.f9 != 0) {
        func_8004284C();
        func_8004284C();
        func_8004284C();
        func_8004284C();
        return;
    }
    if (D_800E6280.unk_114.unk_02 >= (s16)(D_800E6280.unk_1BC[12].unk_0C.f.f9 * 30 - (D_800E6280.unk_1BC[12].unk_0C.f.f9 >= 2) * 10 + 70)) {
        func_8004284C();
        return;
    }
    func_8004284C();
    func_8004284C();
    func_8004284C();
    func_8004284C();
}

/* NON_MATCHING: uopt promotes D_800E6280.unk_1BC[14].unk_0C to $v0 in the if body instead of $t2. T-24116. */
#ifdef NON_MATCHING
void func_8013D92C(void) {
    s16 v;

    v = D_800E6280.unk_1BC[12].unk_0C.f.f9 * 25 + 150;
    if (D_800E6280.unk_10C.unk_02 >= v) {
        D_80145F2C = (D_800E6280.unk_1BC[14].unk_0C.f.f9 >= 1) * 3 + 4;
        D_800E6280.unk_1BC[14].unk_0C.f.f9 += 1;
        func_8004284C();
        return;
    }
    D_80145F2C += 1;
    D_800E6280.unk_118.unk_02 += 10;
    D_800E6280.unk_11C.unk_02 += 10;
    check_para_limit();
    func_8004284C();
    func_8004284C();
    func_8004284C();
    func_8004284C();
    func_8004284C();
    func_8004284C();
    func_8004284C();
    func_8004284C();
}
#else
INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013D660", func_8013D92C);
#endif

void func_8013DA5C(void) {
    if (((u32) (D_800E6280.unk_1BC[14].unk_0C.w << 0x14) >> 0x1D) == 1) {
        func_8013D2A8();
        return;
    }
    func_8004284C();
    func_8004284C();
}

/* NON_MATCHING: unk_71E bit-flag check delay-slot scheduling and D_80145F20 struct-copy coloring. T-24116. */
#ifdef NON_MATCHING
s32 func_8013DAAC(void) {
    if (D_800E6280.unk_1BC[0].unk_10.date.month == 12 &&
        D_800E6280.unk_1BC[0].unk_10.date.day == 24 &&
        !(D_800E6280.unk_71E & 0x20)) {
        D_800E6280.unk_71E |= 0x20;
        D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01 = 0;
        main_state_set(0x57);
        return 0;
    }
    if (((GsWord *)&D_800E6280.unk_0F8)->date.month == 12 &&
        ((GsWord *)&D_800E6280.unk_0F8)->date.day == 24 &&
        !(D_800E6280.unk_71E & 0x40)) {
        D_800E6280.unk_71E |= 0x40;
        main_state_set(0x56);
        return 0;
    }
    draw2d3d(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    back_clear_switch(1);
    tpage_buf_clear();
    func_8004E58C();
    func_8008585C();
    strcpy(D_800CA19C, "伊集院邸");
    D_800E6280.unk_10A2 = 0;
    D_800E6280.unk_10E8 = 1;
    D_800E6280.unk_03A = 0x80;
    D_800B593C = 0;
    D_800B5940 = 0;
    D_80145F2C = 0;
    D_80145F30 = 0;
    addr_init_bustup();
    hizuke_init();
    message_window_init();
    func_80084E4C();
    strcpy(D_800CA16C, D_800E6280.unk_0D4);
    if (D_800E6280.unk_1BC[12].unk_0C.f.f9 >= 3) {
        D_800E6280.unk_1BC[12].unk_0C.f.f9 = 2;
    }
    if (D_800E6280.unk_1BC[12].unk_0C.f.f9 == 0) {
        strcpy(D_800CA188, "黒服");
    } else {
        strcpy(D_800CA188, "外井");
    }
    D_800E6280.unk_F5F = 12;
    func_8013C070();
    D_80145F20 = D_80145AFC;
    D_80145F24 = D_80145B30;
    D_80145F28 = D_80145B64;
    func_8004284C();
}
#else
INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013D660", func_8013DAAC);
#endif

void func_8013DCFC(void) {
    bg_read_sub2(0x41CA);
    func_8004284C();
}

void func_8013DD24(void) {
    func_80062CD0(0x6B24);
    func_8004284C();
}

void func_8013DD4C(void) {
    func_80042808();
}

void func_8013DD6C(void) {
    func_80042908(4);
}
