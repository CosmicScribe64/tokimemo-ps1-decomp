#include "common.h"
#include "ovl/GEKO.h"

typedef struct {
    void (*f[33])();
} FnTbl33; /* size 0x84 */
extern FnTbl33 D_80144C5C;

void func_801323E0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl33 tbl;

    tbl = D_80144C5C;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_80132454(void) {
    func_80046318(0x24, 0x80197000, 0xAEC8);
    func_80132000();
    func_8004284C();
}

void func_80132490(void) {
    func_80044890(1, 0xC043, 0xC01D, D_800B3688[D_800E6280.unk_F5F], D_800B36C8[D_800E6280.unk_F5F], D_800B3708[D_800E6280.unk_F5F]);
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    func_8004284C();
}

void func_80132518(void) {
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    if ((u32) D_800E6280.unk_1104.w++ >= 0x400U) {
        func_800452C4();
        func_8004482C();
        func_80042940((D_800E6280.unk_110A - 1) & 0xFF);
    }
}

void func_80132598(void) {
    func_80044750(0x200);
    func_8004284C();
}

s32 func_801325C0(void) {
    D_800E6280.unk_F5F = D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01;
    draw2d3d(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    back_clear_switch(1);
    tpage_buf_clear();
    func_8004E58C();
    func_800AE0F0(D_800CA19C, "校門前");
    func_800AE0F0(D_800CA1DC, "近所の公園");
    k_reset(1);
    D_800E6280.unk_10A2 = 0;
    D_800E6280.unk_10E8 = 1;
    func_8008585C();
    D_800E6280.unk_03A = 0x80;
    D_800B593C = 0;
    D_800B5940 = 0;
    addr_init_bustup();
    hizuke_init();
    message_window_init();
    func_80084E4C();
    D_80144C54 = 0;
    D_80144C3C = 0;
    /* T-9510: this initializer and func_801327AC store a full halfword. */
    *(s16 *)&D_80144C40 = 0;
    func_800847B8(D_800E6280.unk_F5F);
    func_80132000();
    D_80144C30 = D_80144BC8;
    D_80144C34 = D_80144BF8;
    D_80144C38 = D_80144C28;
    func_80133024();
    if ((D_80144C50 == 0) && (D_800E6280.unk_F5F == 7) && !(func_800AE0D0() & 3)) {
        D_80144C50 = 0x64;
    }
    if (D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0A >= 0x4C) {
        D_80144C58 = 1;
    } else {
        D_80144C58 = 0;
    }
    func_8004284C();
    func_80045318(9, 0, 0);
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/801323E0", func_801327AC);

/* FAKE: s32 fall-through preserves narrowing through v0 (T-9780). */
s32 func_80132A4C(s16 arg0) {
    /* FAKE: place the 22-byte table at sp+0x14 (T-9780). */
    s32 pad;
    GekoThresholds table;

    table = D_80144CE0;
    if (D_80144C50 == 0) {
        D_80144C50 = arg0;
    } else if (D_80144C50 == 10) {
        if ((D_800E6280.unk_10F8 & 7) >= (u32)table.threshold[D_800E6280.unk_F5F]) {
            D_80144C50 = arg0;
        }
    } else if (D_800E6280.unk_10F8 & 1) {
        D_80144C50 = arg0;
    }
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/801323E0", func_80132B24);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/801323E0", func_80132D04);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/801323E0", func_80133024);

void func_8013313C(void) {
    bg_read_sub2(0x4032);
    func_8004284C();
}

void func_80133164(void) {
    if (D_80144C58 != 0) {
        func_801322E8();
        return;
    }
    func_8004284C();
}

void func_801331A0(void) {
    u8 temp_v0;

    temp_v0 = func_80051A68(D_800E6280.unk_F5F);
    if (((temp_v0 & 0x7F) == 4) || (D_80144C58 != 0)) {
        D_80144C3C += 0x12;
        func_80083440(4U);
        D_80144C54 = 0;
    } else {
        if ((temp_v0 & 0x7F) == 2) {
            D_80144C3C += 6;
        }
        if ((temp_v0 & 0x7F) == 3) {
            D_80144C3C += 0xC;
        }
    }
    func_8004284C();
}

void func_80133250(void) {
    u8 temp_v0;

    temp_v0 = func_80051A68(D_800E6280.unk_F5F);
    if (((temp_v0 & 0x7F) == 4) || (D_80144C58 != 0)) {
        func_8004284C();
        func_8004284C();
        func_8004284C();
        func_8004284C();
    } else if ((D_800E6280.unk_F5F == 6) && ((temp_v0 & 0x7F) < 2U)) {
        func_8004284C();
        func_8004284C();
        func_8004284C();
        func_8004284C();
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/801323E0", func_80133308);

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/801323E0", func_80133598);

void func_8013375C(void) {
    u8 temp_v0;

    temp_v0 = func_80051A68(D_800E6280.unk_F5F);
    if (((u32)(temp_v0 & 0x7F) < 2U) && (D_80122CDC != 0)) {
        if ((D_800E6280.unk_F5F == 2) || (D_800E6280.unk_F5F == 7) || (D_800E6280.unk_F5F == 9)) {
            func_80083418();
            return;
        }
        func_800833F0();
        return;
    }
    func_8004284C();
}

s32 func_801337EC(void) {
    if (D_800E6280.unk_110D == 0) {
        func_800674B0();
        D_800E6280.unk_110D += 1;
    }
    if (D_800E6280.unk_110D == 1) {
        if ((u32)D_800E6280.unk_1104.w++ >= 0x400U) {
            func_800452C4();
            func_8004482C();
            D_800E6280.unk_110D = 0;
        }
        if (func_80044E8C() == 1) {
            D_800E6280.unk_110D += 1;
        } else {
            return 0;
        }
    }
    D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_06 += 2;
    func_80084D3C();
    return func_80072B5C(1);
}
