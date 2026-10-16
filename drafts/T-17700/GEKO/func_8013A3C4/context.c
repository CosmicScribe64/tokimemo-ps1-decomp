#define MAIN_API_OVERRIDE_D_80122CD0 /* switched as u32 (main_api.h: s32), T-6030 */
#include "common.h"
#include "ovl/GEKO.h"

extern u32 D_80122CD0;

typedef struct {
    s32 w[0x11];
} Ev44; /* size 0x44: record of D_800EAFA0 */

typedef struct {
    void (*f[55])();
} FnTbl55; /* size 0xDC */
extern FnTbl55 D_80146518;

typedef struct {
    void (*f[39])();
} FnTbl39; /* size 0x9C */
extern FnTbl39 D_80146420;

void func_80139910(void) {
    D_80146380 = 0x801D6394;
    D_80146384 = 0x801D639C;
    D_80146388 = 0x801D63BC;
    D_8014638C = *(s16 *)0x801D63D0;
    D_80146390 = 0x801B0000;
    D_80146394 = 0x801D6000;
    D_80146398 = 0x801B2000;
    D_8014639C = 0x801B6000;
    D_801463A0 = 0x801BA000;
    D_801463A4 = 0x801BE000;
    D_801463A8 = 0x801C2000;
    D_801463AC = 0x801C6000;
    D_801463B0 = 0x801D2000;
}

void func_801399DC(void) {
    D_801463B4 = 0x801D22D0;
    D_801463B8 = 0x801D22D8;
    D_801463BC = 0x801D233C;
    D_801463C0 = *(s16 *)0x801D2358;
    D_801463C4 = 0x801B0000;
    D_801463C8 = 0x801B2000;
    D_801463CC = 0x801B6000;
    D_801463D0 = 0x801BA000;
    D_801463D4 = 0x801BE000;
    D_801463D8 = 0x801C2000;
    D_801463DC = 0x801C6000;
}

void func_80139A8C(void) {
    switch (D_80122CD0) {
    case 1:
        func_80139B08();
        return;
    case 2:
        func_8013A144();
        return;
    case 8:
        func_8013A230();
        return;
    default:
        func_80046500();
        return;
    }
}

void func_80139B08(void) {
    if (D_800E6280.unk_1109 == 0) {
        func_80139B44();
        return;
    }
    func_80046500();
}

void func_80139B44(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl39 tbl;

    tbl = D_80146420;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
    func_80139E1C();
}

void func_80139BC0(void) {
    func_80044750(0x505);
    func_80044750(0x506);
    func_8004284C();
}

void func_80139BF0(void) {
    LoadSquare(0x140, 0, 0x40, 0x80, D_801463B0);
    D_800E6280.unk_1228[5] = 1;
    func_8004284C();
}

void func_80139C38(void) {
    cd_read_sectors(0x4D, 0x801B0000, 0x7F14);
    func_80139910();
    func_8004284C();
}

void func_80139C70(void) {
    s16 i;
    u8 state;
    u8 colour;

    state = D_800E6280.unk_110A;
    func_800853FC();
    colour = (u8)D_800B593C;

    for (i = 4; i < 19; i++) {
        if (colour < 0x60U) {
            D_801217D0[i].unk_14 = D_801217D0[i].unk_15 = D_801217D0[i].unk_16 = colour;
        }
    }
    if (state != D_800E6280.unk_110A) {
        for (i = 4; i < 19; i++) {
            D_801217D0[i].unk_00 |= 0x80000000;
            func_80044750(0x1506);
        }
    }
}

void func_80139D54(void) {
    s16 i;
    u8 colour;

    if (D_800E6280.unk_1104.w == 0) {
        for (i = 4; i < 19; i++) {
            D_801217D0[i].unk_00 &= 0x7FFFFFFF;
        }
    }
    func_80085368();
    colour = (u8)D_800B593C;
    for (i = 4; i < 19; i++) {
        if (colour < 0x61U) {
            D_801217D0[i].unk_14 = D_801217D0[i].unk_15 = D_801217D0[i].unk_16 = colour;
        }
    }
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80139910", func_80139E1C);

void func_8013A144(void) {
    if (D_800E6280.unk_1109 == 0) {
        func_8013A180();
        return;
    }
    func_80046500();
}

typedef struct {
    void (*f[23])();
} FnTbl23; /* size 0x5C */
extern FnTbl23 D_801464BC;

void func_8013A180(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl23 tbl;

    tbl = D_801464BC;
    idx = D_800E6280.unk_110A;
    tbl.f[idx]();
    if (D_800E6280.unk_110A >= 0xD) {
        if (D_800E6280.unk_110A < 0x14) {
            func_8006B900();
        }
    }
}

void func_8013A230(void) {
    if (D_800E6280.unk_1109 == 0) {
        func_8013A2EC();
        return;
    }
    func_80046500();
}

void func_8013A26C(void) {
    s32 temp_v0;

    D_800B3D60 = 0;
    temp_v0 = dec_bg_cd_read(0x3FBE, 0);
    if (temp_v0 == *D_800B5938) {
        func_8004284C();
        func_8004284C();
        func_8004284C();
    } else if (temp_v0 == (1 - *D_800B5938)) {
        func_8004284C();
        func_8004284C();
    }
    func_8004284C();
}

void func_8013A2EC(void) {
    void (**p)();
    FnTbl55 tbl;

    tbl = D_80146518;
    tbl.f[D_800E6280.unk_110A](0x80);
    p = &tbl.f[D_800E6280.unk_110A];
    if (tbl.f[D_800E6280.unk_110A] == func_80085368) {
        D_801206DB |= 0x80;
    }
    if (func_800853FC == *p) {
        /* FAKE: empty double test (decomp-permuter, score 20), same register-choice shift as SHOUGATU func_80136078 (T-4090); real source unknown. T-10820 */
        if (!D_800E6280.unk_110A && !D_800E6280.unk_110A) {
        }
        D_801206DB &= 0xFF7F;
    }
    func_8013A668();
}

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E7208;
extern u32 D_800E7384;
extern u8 D_800E738D;
/* end bulk_m2c declarations */
s32 func_8013A3C4(void) {
    s32 var_v0;

    if ((D_800E7208 & 0x400040) && ((u16) D_800CA154 != 0) && ((u16) D_800CA154 < 4U) && ((u32) D_800E7384 >= 2U)) {
        func_80044750(0x1508);
        D_80122D00 = 0;
        k_reset(1);
        D_800CA2AC = 0;
        D_800CA154 = 3;
        goto block_10;
    }
    if ((check_end_k() != 0) && ((u16) D_800CA154 < 4U)) {
        if ((u8) D_800E738D < 0x40U) {
            D_800E738D = (D_800E738D + 1) & 0xFF;
            var_v0 = 0;
        } else {
            D_800E738D = 0;
            k_reset(1);
            goto block_10;
        }
    } else {
block_10:
        if ((u16) D_800CA154 < 3U) {
            func_80138B74();
        } else {
            func_80138AF8();
        }
        if ((D_800E7384++ == 0) && ((u16) D_800CA154 < 4U) && ((u16) D_800CA150 == 0)) {
            func_80044750(0x609);
        }
        var_v0 = D_800E7384 == 1;
        D_800E7384 += 1;
        if (var_v0 != 0) {
            if ((u16) D_800CA154 == 5) {
                var_v0 = func_80044750(0x60A);
            }
        }
    }
    return var_v0;
}

void func_8013A564(void) {
    cd_read_sectors(0x45, 0x801B0000, 0x80BA);
    func_801399DC();
    func_8004284C();
}

void func_8013A59C(void) {
    D_800B3D60 = 0;
    func_80044890(1, 0xC007, 0xBFF7, 0xCBA3, 0xCB53, 0xCB4E);
    func_8004284C();
}

s32 func_8013A5E8(void) {
    if (D_800E6280.unk_1104.w == 0) {
        func_80044750(0x500);
    }
    if (D_800E6280.unk_1104.w++ == 0x40) {
        func_8004284C();
    }
}

void func_8013A640(void) {
    func_80062CD0(0x57CE);
    func_8004284C();
}

void func_8013A668(void) {
    if ((u8) D_800E6280.unk_110A >= 0x23U) {
        D_801206DF = D_80120657;
    }
    if (((u8) D_800E6280.unk_110A >= 0x23U) && (((u32) D_800E6280.unk_10F8 % 300U) == 0x14)) {
        D_801206DA |= 3;
    }
}

void func_8013A6D0(void) {
    D_800E6280.unk_F5F = 0;
    func_800438DC(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    func_800438F0(1);
    func_80048390();
    func_8004E58C();
    func_8006612C("");
    D_8011ED9F = 0;
    D_8011EDE3 = 0;
    D_800CA150 = 0;
    D_800CA154 = 0;
    func_8008585C();
    func_8007C740();
    func_800649D4();
    func_80064E84();
    func_80084E4C();
    func_800847B8(D_800E6280.unk_F5F);
    func_80137990();
    D_80146274 = D_80145ED8;
    D_80146278 = D_80146014;
    D_8014627C = D_80146150;
    func_800AE0F0(D_800CA19C, "教室");
    if (D_80122CD0 == 1) {
        D_800E6280.unk_1BC[0].unk_02 += 1;
        D_800E6280.unk_1BC[0].unk_06 += 2;
        func_80084D3C();
    }
    D_800CA368 = 0;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80139910", func_8013A820);

void func_8013AA1C(void) {
    D_800E6280.unk_F5F = 0;
    func_800438DC(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    func_800438F0(1);
    func_80048390();
    func_8004E58C();
    func_8006612C("");
    D_8011ED9F = 0;
    D_8011EDE3 = 0;
    D_800CA150 = 0;
    D_800CA154 = 0;
    func_8007C740();
    func_800649D4();
    func_80064E84();
    func_80084E4C();
    func_800847B8(D_800E6280.unk_F5F);
    func_8008585C();
    func_80137990();
    D_80146274 = D_80145EDC;
    D_80146278 = D_80146018;
    D_8014627C = D_80146154;
    func_800AE0F0(D_800CA19C, "自宅前");
    D_800E6280.unk_11C.unk_02 -= 1;
    func_80084D3C();
    D_800CA368 = 1;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/GEKO/nonmatchings/GEKO/80139910", func_8013AB4C);

void func_8013ACE0(void) {
    draw2d3d(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    back_clear_switch(1);
    tpage_buf_clear();
    func_8004E58C();
    func_8008585C();
    func_80084E4C();
    hizuke_init();
    message_window_init();
    D_800B593C = 0;
    D_800CA368 = 7;
    func_8004284C();
}

void func_8013AD6C(void) {
    func_801399DC();
    func_80043914(D_801463C4, 0x11, 1, 2, 0);
    func_80084E90(D_801463C8, D_801463CC, D_801463D0, D_801463D4, D_801463D8, D_801463DC);
    func_800850D4(D_801463B8, D_801463BC, D_801463B4, D_801463C0);
    D_80120650[0x48] = 8;
    D_80120650[4] = 8;
    *(Ev44 *)&D_80120650[0x88] = *(Ev44 *)D_80120650;
    *(s16 *)&D_80120650[0x9E] = 4;
    func_8004284C();
}

void func_8013AE7C(void) {
    bg_read_sub2(0x4068);
    func_8004284C();
}

void func_8013AEA4(void) {
    bg_read_sub2(0x46AA);
    func_8004284C();
}

void func_8013AECC(void) {
    bg_read_sub2(0x414C);
    func_8004284C();
}

void func_8013AEF4(void) {
    bg_read_sub2(0x4171);
    func_8004284C();
}

void func_8013AF1C(void) {
    bg_read_sub2(0x473A);
    func_8004284C();
}

void func_8013AF44(void) {
    func_800634FC(get_g_zyotai_h(0));
    bg_read_sub2(0x4156);
    func_8004284C();
}
