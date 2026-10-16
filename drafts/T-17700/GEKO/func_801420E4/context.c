#include "common.h"
#include "ovl/GEKO.h"

void func_80141AF0(void) {
    D_80147530 = 0x801CE130;
    D_80147534 = 0x801CE134;
    D_80147538 = 0x801CE15C;
    D_8014753C = (*(s16 *)0x801CE170);
    D_80147540 = 0x801B0000;
    D_80147544 = 0x801B2000;
    D_80147548 = 0x801B6000;
    D_8014754C = 0x801BA000;
    D_80147550 = 0x801BE000;
    D_80147554 = 0x801C2000;
    D_80147558 = 0x801C6000;
}

void func_80141BA0(void) {
    func_80141BC0();
}

void func_80141BC0(void) {
    if (D_800E6280.unk_1109 == 0) {
        func_80141BFC();
        return;
    }
    func_80046500();
}

typedef struct {
    void (*f[43])();
} FnTbl43; /* size 0xAC */
extern FnTbl43 D_80147560;

void func_80141BFC(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl43 tbl;

    tbl = D_80147560;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
    if (D_8014755C != 0) {
        func_80063668();
    }
}

void func_80141C94(void) {
    cd_read_sectors(0x3D, 0x801B0000, 0x8FB5);
    func_80141AF0();
    func_8004284C();
}

void func_80141CCC(void) {
    D_800B5A60 = 0;
    func_80138AF8();
    D_80122CF4 = 0;
    /* records of 0x44 / 0x24 bytes: indexing from the first symbol keeps IDO from hoisting the loads above the stores (T-4070) */
    (&D_8011F4CB)[0x44] |= 0x80;
    D_8011F4CB |= 0x80;
    D_801217D0[0].unk_00 &= 0x7FFFFFFF;
    D_801217D0[1].unk_00 &= 0x7FFFFFFF;
    if ((u32)D_800E6280.unk_10F8 % 240U == 0) {
        func_800638C4();
    }
}

void func_80141D74(void) {
    D_800E6280.unk_56C[82] = D_800E6280.unk_75D;
    D_800E6280.unk_F5F = D_800E6280.unk_75D;
    func_800847B8(D_800E6280.unk_75D);
    D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.b[0] = D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0C.b[0] & 0xFFFD;
    func_8004284C();
}

void func_80141DDC(void) {
    (&D_8011F4EE)[0x22] += 0x40;
    D_8011F4EE += 0x40;
    D_801217D4 += 0x40;
    (&D_801217D4)[0x12] += 0x40;
    func_8004284C();
}

void func_80141E48(void) {
    func_80138AF8();
    if ((u16)D_800CA154 == 2) {
        D_800B5A60 = 0;
        D_80122CF4 = 0;
        (&D_8011F4CB)[0x44] |= 0x80;
        D_8011F4CB |= 0x80;
        D_8011F524 = 0;
        D_801217D0[0].unk_00 &= 0x7FFFFFFF;
        D_801217D0[1].unk_00 &= 0x7FFFFFFF;
        if ((u32)D_800E6280.unk_10F8 % 240U == 0) {
            func_800638C4();
        }
    } else {
        func_800634FC(0);
        D_80122CF4 = 1;
        (&D_8011F4CB)[0x44] |= 0x80;
        D_8011F4CB |= 0x80;
        D_801217D0[0].unk_00 &= 0x7FFFFFFF;
        D_801217D0[1].unk_00 &= 0x7FFFFFFF;
        D_80120652 = 0;
        D_80120668 = 0;
        if ((u32)D_800E6280.unk_10F8 % 240U == 0x78) {
            func_80067DD4();
        }
    }
}

void func_80141FB8(void) {
    D_800CA150 = (u16) D_800CA150 + D_800E6280.unk_F5F;
    func_8004284C();
}

void func_80141FF0(void) {
    D_800CA150 = 0xC;
    func_8004284C();
}

s32 func_80142018(void) {
    s32 mask;
    u8 value;

    func_800853FC();
    D_80122CF4 = 1;
    value = D_80120657;
    D_8011F513 = value;
    D_8011F4CF = value;
    func_80084C98(0, value);
    func_80084C98(1, D_80120657);
    if (D_80120657 < 8U) {
        mask = ~0x80;
        D_80122CF4 = 0;
        (&D_8011F4CB)[0x44] &= mask;
        D_8011F4CB &= mask;
        D_801217D0[0].unk_00 |= 0x80000000;
        D_801217D0[1].unk_00 |= 0x80000000;
    }
}

/* bulk_m2c declarations (T-11190) */
extern s32 D_80144B50;
/* end bulk_m2c declarations */
void func_801420E4(void) {
    GekoShorts6 sp4C;
    GekoShorts6 sp40;
    s16 temp_v1;
    s16 var_a0;
    s16 var_a1;
    s16 var_a2;
    s16 var_a3;
    s16 var_ra;
    s16 var_s0;
    s16 var_s1;
    s16 var_t0;
    s16 var_t1;
    s16 var_t2;
    s16 var_t3;
    s16 var_t4;
    s16 var_t5;
    s32 temp_a1;
    s32 temp_a2;
    s32 temp_a3;
    s32 temp_t0;
    s32 temp_t1;
    s32 temp_t1_2;
    s32 temp_t4;
    s32 temp_t4_2;

    sp4C = D_8014760C;
    sp40 = D_80147618;
    var_a1 = sp4C.v[4];
    var_a2 = sp40.v[4];
    var_a3 = sp4C.v[3];
    var_t0 = sp40.v[3];
    var_t1 = sp4C.v[5];
    var_t2 = sp4C.v[2];
    var_t3 = sp40.v[2];
    var_t4 = sp40.v[5];
    var_t5 = sp4C.v[1];
    var_ra = sp40.v[1];
    var_s0 = sp4C.v[0];
    var_s1 = sp40.v[0];
    D_800E6280.unk_75D = 1;
    var_a0 = 9;
    do {
        if (D_800E6280.unk_1BC[var_a0].unk_0C.w & 2) {
            temp_v1 = D_800E6280.unk_1BC[var_a0].unk_02 + D_800E6280.unk_1BC[var_a0].unk_06;
            if (temp_v1 >= var_s0) {
                temp_t1 = var_a1 << 0x10;
                temp_t4 = var_a2 << 0x10;
                temp_a3 = var_t2 << 0x10;
                temp_t0 = var_t3 << 0x10;
                var_a1 = var_a3;
                var_a2 = var_t0;
                var_t1 = (s16) (temp_t1 >> 0x10);
                var_t2 = var_t5;
                var_t3 = var_ra;
                var_t4 = (s16) (temp_t4 >> 0x10);
                var_a3 = (s16) (temp_a3 >> 0x10);
                var_t0 = (s16) (temp_t0 >> 0x10);
                var_t5 = var_s0;
                var_ra = var_s1;
                var_s0 = D_800E6280.unk_1BC[var_a0].unk_06 + D_800E6280.unk_1BC[var_a0].unk_02;
                var_s1 = var_a0;
            } else if (temp_v1 >= var_t5) {
                temp_a1 = var_a3 << 0x10;
                temp_a2 = var_t0 << 0x10;
                var_t1 = var_a1;
                var_t4 = var_a2;
                var_a3 = var_t2;
                var_t0 = var_t3;
                var_a1 = (s16) (temp_a1 >> 0x10);
                var_a2 = (s16) (temp_a2 >> 0x10);
                var_t2 = var_t5;
                var_t3 = var_ra;
                var_t5 = D_800E6280.unk_1BC[var_a0].unk_06 + D_800E6280.unk_1BC[var_a0].unk_02;
                var_ra = var_a0;
            } else if (temp_v1 >= var_t2) {
                temp_t1_2 = var_a1 << 0x10;
                temp_t4_2 = var_a2 << 0x10;
                var_a1 = var_a3;
                var_a2 = var_t0;
                var_t1 = (s16) (temp_t1_2 >> 0x10);
                var_t4 = (s16) (temp_t4_2 >> 0x10);
                var_a3 = var_t2;
                var_t0 = var_t3;
                var_t2 = D_800E6280.unk_1BC[var_a0].unk_06 + D_800E6280.unk_1BC[var_a0].unk_02;
                var_t3 = var_a0;
            } else if (temp_v1 >= var_a3) {
                var_t1 = var_a1;
                var_t4 = var_a2;
                var_a1 = var_a3;
                var_a2 = var_t0;
                var_a3 = D_800E6280.unk_1BC[var_a0].unk_06 + D_800E6280.unk_1BC[var_a0].unk_02;
                var_t0 = var_a0;
            } else if (temp_v1 >= var_a1) {
                var_t1 = var_a1;
                var_t4 = var_a2;
                var_a1 = D_800E6280.unk_1BC[var_a0].unk_06 + D_800E6280.unk_1BC[var_a0].unk_02;
                var_a2 = var_a0;
            } else if (temp_v1 >= var_t1) {
                var_t1 = temp_v1;
                var_t4 = var_a0;
            }
        }
        var_a0 -= 1;
    } while (var_a0 > 0);
    D_800E6280.unk_75D = (u8) var_t4;
    sp40.v[1] = var_ra;
    sp4C.v[4] = var_a1;
    sp40.v[0] = var_s1;
    sp4C.v[0] = var_s0;
    sp4C.v[1] = var_t5;
    sp40.v[2] = var_t3;
    sp4C.v[2] = var_t2;
    sp4C.v[5] = var_t1;
    sp40.v[3] = var_t0;
    sp4C.v[3] = var_a3;
    sp40.v[4] = var_a2;
    D_800E6280.unk_F5F = 0xD;
    sp40.v[5] = var_t4;
    draw2d3d(1, 0, var_a2, var_a3);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    back_clear_switch(1);
    tpage_buf_clear();
    func_8004E58C();
    func_8006612C((s32) &D_80144B50);
    *(s8 *)((unsigned char *)D_8011ECD0 + (207)) = 0;
    *(s8 *)((unsigned char *)D_8011ECD0 + (275)) = 0;
    D_800CA150 = 0;
    D_800CA154 = 0;
    addr_init_bustup();
    func_80137990();
    hizuke_init();
    message_window_init();
    func_80084E4C();
    D_800B593C = 0;
    func_800847B8(D_800E6280.unk_F5F);
    func_8008585C();
    func_80137990();
    D_80146274 = D_80145FF8;
    D_80146278 = D_80146134;
    D_8014627C = D_80146270;
    func_800AE0F0(D_800CA19C, "教室");
    D_8014755C = 0;
    D_800CA368 = 0xFF;
    func_8004284C();
    D_800E6280.unk_56C[0x52] = D_800E6280.unk_56C[0x52] + 1;
}

void func_801425CC(void) {
    func_80065F34(0);
    func_80141AF0();
    load_palette(D_80147540, 0x11, 1, 2, 0);
    func_80084E90(D_80147544, D_80147548, D_8014754C, D_80147550, D_80147554, D_80147558);
    func_800850D4(D_80147534, D_80147538, D_80147530, D_8014753C);
    func_8004284C();
}

void func_8014267C(void) {
    bg_read_sub2(0x4055);
    func_8004284C();
}

void func_801426A4(void) {
    func_8007ED84(0x4690);
    func_8004284C();
}
