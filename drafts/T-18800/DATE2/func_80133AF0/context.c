#include "common.h"
#include "ovl/DATE2.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/80132DE0.data", D_8013A4C0);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/80132DE0.data", D_8013A4C4);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/80132DE0.data", D_8013A4C8);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/80132DE0.data", D_8013A5B0);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/80132DE0.data", D_8013A5B4);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/80132DE0.data", D_8013A5B8);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/80132DE0.data", D_8013A5BC);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/80132DE0.data", D_8013A5C0);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/80132DE0.data", D_8013A5C4);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/80132DE0.data", D_8013A5C8);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/80132DE0.data", D_8013A5CC);
INCLUDE_RODATA("asm/ovl/DATE2/data/DATE2/80132DE0.data", D_8013A5D0);
s16 D_8013A5D4[3] = { 0, 0, 0 };
s16 D_8013A5DC[3] = { 0, 0, 0 };
s16 D_8013A5E4[3] = { 0, 0, 0 };
s16 D_8013A5EC[3] = { 0, 0, 0 };

typedef struct {
    void (*f[44])();
} FnTbl44; /* size 0xB0 */
extern FnTbl44 D_8013A600;

typedef struct {
    void (*f[58])();
} FnTbl58; /* size 0xE8 */
extern FnTbl58 D_8013A4C8;

void func_80132DE0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl58 tbl;

    tbl = D_8013A4C8;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_80132E5C(void) {
    func_800AE0A0((void *)(0x80180000 + D_800E6280.unk_1104.w * 0x1400), 0x801C0000 + D_800E6280.unk_1104.w * 0x1400, 0x1400);
    D_800E6280.unk_1104.w += 1;
    if (D_800E6280.unk_1104.w == 0x12) {
        func_8004284C();
    }
}

void func_80132EC8(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    temp_v0 = D_800E6280.unk_1104.w * 0x1400;
    func_800AE0A0(temp_v0 + 0x80180000, temp_v0 + 0x801A0000, 0x1400);
    temp_v0_2 = D_800E6280.unk_1104.w + 1;
    D_800E6280.unk_1104.w = temp_v0_2;
    if (temp_v0_2 == 0x12) {
        D_800E6280.unk_F5F = 0;
        func_80051508(D_800CA17C, 0);
        func_8004284C();
    }
}

void func_80132F48(void) {
    s32 pad; /* FAKE: unused local that moves rect to sp+0x2C like the original (T-3330 frame rule). T-4050 */
    RECT rect;

    rect.x = 0x140;
    rect.y = 0x80;
    rect.w = 0x180;
    rect.h = 0x80;
    func_8009C8E0(&rect, (void *)0x80180000);
    D_800E6280.unk_F5F = D_800E6280.unk_75D;
    func_800847B8(D_800E6280.unk_75D);
    func_80133258();
    func_8004284C();
}

void func_80132FB0(void) {
    func_800AE0A0((void *)(0x801C0000 + D_800E6280.unk_1104.w * 0x1400), 0x80180000 + D_800E6280.unk_1104.w * 0x1400, 0x1400);
    D_800E6280.unk_1104.w += 1;
    if (D_800E6280.unk_1104.w == 0x12) {
        func_80083474();
    }
}

void func_8013301C(void) {
    func_800AE0A0((void *)(0x801A0000 + D_800E6280.unk_1104.w * 0x1400), 0x80180000 + D_800E6280.unk_1104.w * 0x1400, 0x1400);
    D_800E6280.unk_1104.w += 1;
    if (D_800E6280.unk_1104.w == 0x12) {
        func_80083474();
    }
}

void func_80133088(void) {
    RECT rect;

    func_80083474();
    rect.x = 0x140;
    rect.y = 0x80;
    rect.w = 0x180;
    rect.h = 0x80;
    func_8009C884(&rect, (void *)0x80180000);
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80132DE0", func_801330D4);

void func_80133258(void) {
    switch (D_800E6280.unk_F5F) {
    case 0:
        D_8013A410 = D_8013A384;
        D_8013A414 = D_8013A3B0;
        D_8013A418 = D_8013A3DC;
        return;
    case 1:
        D_8013A410 = D_8013A394;
        D_8013A414 = D_8013A3C0;
        D_8013A418 = D_8013A3EC;
        return;
    case 2:
        D_8013A410 = D_8013A388;
        D_8013A414 = D_8013A3B4;
        D_8013A418 = D_8013A3E0;
        return;
    case 3:
        D_8013A410 = D_8013A38C;
        D_8013A414 = D_8013A3B8;
        D_8013A418 = D_8013A3E4;
        return;
    case 4:
        D_8013A410 = D_8013A3A4;
        D_8013A414 = D_8013A3D0;
        D_8013A418 = D_8013A3FC;
        return;
    case 5:
        D_8013A410 = D_8013A398;
        D_8013A414 = D_8013A3C4;
        D_8013A418 = D_8013A3F0;
        return;
    case 6:
        D_8013A410 = D_8013A39C;
        D_8013A414 = D_8013A3C8;
        D_8013A418 = D_8013A3F4;
        return;
    case 7:
        D_8013A410 = D_8013A390;
        D_8013A414 = D_8013A3BC;
        D_8013A418 = D_8013A3E8;
        return;
    case 8:
        D_8013A410 = D_8013A380;
        D_8013A414 = D_8013A3AC;
        D_8013A418 = D_8013A3D8;
        return;
    case 9:
        D_8013A410 = D_8013A3A0;
        D_8013A414 = D_8013A3CC;
        D_8013A418 = D_8013A3F8;
        /* fallthrough */
    default:
        return;
    }
}

void func_80133494(void) {
    s32 temp;
    temp = D_8013A4C0;
    switch (temp) {
    case 0:
        func_8007ED84(0x4350);
        func_800AE0F0(D_800CA1DC, "観覧車");
        break;
    case 1:
        if ((u8) D_800E6280.unk_03E >= 0x61U) {
            func_8007ED84(0x4359);
            func_800AE0F0(D_800CA1DC, "絶叫マシーンビビール");
        } else {
            func_8007ED84(0x433E);
            func_800AE0F0(D_800CA1DC, "ジェットコースター");
        }
        break;
    case 2:
        func_8007ED84(0x4347);
        func_800AE0F0(D_800CA1DC, "ゴーストハウス");
        break;
    }
    func_8004284C();
}

void func_8013357C(void) {
    func_80062CD0(0x6C8C);
    func_8004284C();
}

void func_801335A4(void) {
    D_800E6280.unk_F5F = 0;
    get_p_name(&D_800CA17C, 0);
    func_80062CD0(0x55DF);
    func_8004284C();
}

void func_801335E4(void) {
    D_800E6280.unk_F5F = D_800E6280.unk_75D;
    func_800847B8(D_800E6280.unk_75D);
    func_80133258();
    func_80083B24();
}

void func_80133620(void) {
    D_800E6280.unk_F5F = D_800E6280.unk_75D;
    func_80133258();
    func_800847B8(D_800E6280.unk_F5F);
    switch (D_800E6280.unk_F5F) {
    case 1:
        func_80062CD0(0x5DC8);
        break;
    case 2:
        func_80062CD0(0x52B5);
        break;
    case 3:
        func_80062CD0(0x5936);
        break;
    case 4:
        func_80062CD0(0x6746);
        break;
    case 5:
        func_80062CD0(0x603E);
        break;
    case 6:
        func_80062CD0(0x62B4);
        break;
    case 7:
        func_80062CD0(0x5B7F);
        break;
    case 8:
        func_80062CD0(0x506C);
        break;
    case 9:
        func_80062CD0(0x652A);
        break;
    }
    func_8004284C();
}

void func_80133720(void) {
    if (D_80122CDC != 0) {
        func_80133620();
        D_8013A4C4 |= 1 << (s8)D_8013A4C0;
    } else {
        func_801335A4();
    }
    func_80133258();
}

void func_80133784(void) {
    s32 temp;
    temp = D_8013A4C0;
    switch (temp) {
    case 0:
        D_8013A430 = 1;
        D_8013A438 = 1;
        break;
    case 1:
        if ((u8) D_800E6280.unk_03E >= 0x61U) {
            D_8013A430 = 5;
            D_8013A438 = 5;
            D_8013A428 += 2;
        } else {
            D_8013A430 = 3;
            D_8013A438 = 3;
        }
        break;
    case 2:
        D_8013A430 = 7;
        D_8013A438 = 7;
        if ((u8) D_800E6280.unk_03E < 0x61U) {
            D_8013A428 += 2;
        }
        break;
    }
    func_801338F8();
    func_8004284C();
}

void func_80133884(void) {
    if (D_8013A4C0 == 2) {
        func_80042808();
        return;
    }
    func_8004284C();
}

void func_801338C4(void) {
    D_8013A4C0 += 1;
    func_80042940(0x1B);
}

/* FAKE: s32 fall-through keeps the original counter/result register. T-10010. */
s32 func_801338F8(void) {
    u8 previous_state;
    u8 first_state;
    first_state = func_80051A68(0U) & 0x7F;
    if (first_state >= 2U) {
        D_8013A430 += 0xA;
    }
    if (first_state >= 3U) {
        D_8013A430 += 0xA;
    }
    if (first_state == 4) {
        D_8013A430 += 0xA;
    }
    previous_state = func_80051A68(D_800E6280.unk_75D) & 0x7F;
    if (previous_state >= 2U) {
        D_8013A438 += 0xA;
    }
    if (previous_state >= 3U) {
        D_8013A438 += 0xA;
    }
    if (previous_state == 4) {
        D_8013A438 += 0xA;
    }
}

void func_801339F8(void) {
    D_8013A5B0 = 0x801C6000;
    D_8013A5B4 = 0x801AE000;
    D_8013A5B8 = 0x801B2000;
    D_8013A5BC = 0x801B6000;
    D_8013A5C0 = 0x801BA000;
    D_8013A5C4 = 0x801BE000;
    D_8013A5C8 = 0x801C2000;
}

void func_80133A6C(void) {
    u8 sel = D_8013A4C0; /* FAKE: copy of unit-private data, which the original does not promote (T-5010) */

    switch (sel) {
    case 0:
        func_801348F0();
        return;
    case 1:
        if ((u8) D_800E6280.unk_03E >= 0x61U) {
            func_80133AF0();
            return;
        }
        func_801350A4();
        return;
    default:
        func_80135E50();
        return;
    }
}

/* bulk_m2c declarations (T-11190) */
extern s8 D_800B593A;
/* end bulk_m2c declarations */
void func_80133AF0(void) {
    GameState *g = &D_800E6280;
    s16 sp60;
    s16 sp5A;
    s16 sp58;
    s16 sp56;
    s16 sp54;
    s32 sp50;
    s32 sp4C;
    Rec24 *temp_v0;
    Rec24 *temp_v0_2;
    s16 temp_t9;
    s16 temp_t9_2;
    s16 var_v1;
    s16 var_v1_2;
    s16 var_v1_3;
    s16 var_v1_4;
    s16 var_v1_5;
    s16 var_v1_6;
    s16 var_v1_7;
    s16 var_v1_8;
    s16 var_v1_9;
    s32 temp_lo;
    s32 temp_lo_2;
    s32 temp_lo_3;
    s32 temp_lo_4;
    s32 temp_lo_5;
    s32 temp_lo_6;
    s32 temp_t5;
    s32 temp_t5_2;
    s32 temp_t5_3;
    s32 temp_t6;
    s32 temp_t8;
    s32 temp_t9_3;
    s32 var_at;
    u16 temp_t9_4;
    u8 temp_t7_3;
    u8 temp_v0_7;
    u8 temp_v0_8;
    u8 var_a0;
    u8 *temp_t7;
    u8 *temp_t7_2;
    Rec24 *temp_v0_3;
    Rec24 *temp_v0_4;
    Rec24 *temp_v0_5;
    Rec24 *temp_v0_6;

    D_800B3D60 = 0;
    switch (g->unk_1104.w) {
    case 0:
        D_80122D2C = 1;
        func_800AE120((s32) g->unk_10F8);
        func_80044750(0x206);
        ((RECT *)D_800B5920)[0].h = 0;
        ((RECT *)D_800B5920)[0].y = 0;
        ((RECT *)D_800B5920)[0].w = 0;
        ((RECT *)D_800B5920)[0].x = 0;
        D_800B5939 = 0;
        D_800B5938 = 0;
        D_800B5948 = -1;
        ((RECT *)D_800B5920)[1].y = 0;
        D_800B593A = 0;
        hizuke_disp_switch(0);
        D_8013A5D0 = 2;
        g->unk_110D = 0;
        D_8013A5CC = 0;
        var_v1 = 0;
        do {
            temp_t9 = (var_v1 * 2) + 0x15;
            temp_v0 = &D_801217D0[var_v1];
            temp_v0[4].unk_08 = (s16) ((var_v1 << 6) + 0x80);
            temp_t5 = var_v1 << 7;
            temp_t7 = (u8 *)g + var_v1;
            var_v1 += 1;
            temp_v0[4].unk_00 = 0x82000000;
            temp_v0[4].unk_0A = 0;
            temp_v0[4].unk_0E = 0;
            temp_v0[4].unk_0F = 0xA0;
            temp_v0[4].unk_10 = 0;
            temp_v0[4].unk_0C = temp_t9;
            temp_v0[4].unk_12 = 0x1E0;
            temp_v0[4].unk_14 = 0;
            temp_v0[4].unk_15 = 0;
            temp_v0[4].unk_16 = 0;
            temp_v0[4].unk_18 = 0;
            temp_v0[4].unk_1A = 0;
            temp_v0[4].unk_1C = 0x1000;
            temp_v0[4].unk_1E = 0x1000;
            temp_v0[4].unk_20 = 0;
            temp_v0[4].unk_04 = (s16) (temp_t5 - 0xA0);
            temp_v0[4].unk_06 = -0x78;
            *(s8 *)((unsigned char *)temp_t7 + (4265)) = 0xB;
        } while (var_v1 < 2);
        var_v1_2 = 0;
        do {
            temp_t9_2 = (var_v1_2 * 2) + 0xA;
            temp_v0_2 = &D_801217D0[var_v1_2];
            temp_v0_2[6].unk_08 = (s16) ((var_v1_2 << 6) + 0x80);
            temp_t5_2 = var_v1_2 << 7;
            temp_t7_2 = (u8 *)g + var_v1_2;
            var_v1_2 += 1;
            temp_v0_2[6].unk_00 = 0x82000000;
            temp_v0_2[6].unk_0A = 0xA0;
            temp_v0_2[6].unk_0E = 0;
            temp_v0_2[6].unk_0F = 0;
            temp_v0_2[6].unk_10 = 0;
            temp_v0_2[6].unk_0C = temp_t9_2;
            temp_v0_2[6].unk_12 = 0x1E0;
            temp_v0_2[6].unk_14 = 0;
            temp_v0_2[6].unk_15 = 0;
            temp_v0_2[6].unk_16 = 0;
            temp_v0_2[6].unk_18 = 0;
            temp_v0_2[6].unk_1A = 0;
            temp_v0_2[6].unk_1C = 0x1000;
            temp_v0_2[6].unk_1E = 0x1000;
            temp_v0_2[6].unk_20 = 0;
            temp_v0_2[6].unk_04 = (s16) (temp_t5_2 - 0xA0);
            temp_v0_2[6].unk_06 = -0x78;
            *(s8 *)((unsigned char *)temp_t7_2 + (4267)) = 0xB;
        } while (var_v1_2 < 2);
        dec_bg_cd_read(0x43CB, 1);
        g->unk_1104.w += 1;
block_49:
        if (((u32) g->unk_1104.w < 9U) || ((u32) g->unk_1104.w >= 0xCU)) {
            return;
        }
        var_v1_3 = D_801217D0->unk0 + D_8013A5D0;
        var_at = var_v1_3 < 0x40;
        if (var_v1_3 < 0) {
            sp60 = 0;
            var_v1_3 = sp60;
            D_8013A5D0 = (func_800AE0D0() % 4) + 2;
            var_at = var_v1_3 < 0x40;
        }
        if (var_at == 0) {
            sp60 = 0x3F;
            var_v1_3 = 0x3F;
            D_8013A5D0 = -2 - (func_800AE0D0() % 4);
        }
        var_a0 = *(u8 *)((unsigned char *)D_801217D0 + (159));
        *(s8 *)((unsigned char *)D_801217D0 + (266)) = (s8) var_v1_3;
        *(s8 *)((unsigned char *)D_801217D0 + (230)) = (s8) var_v1_3;
        *(s8 *)((unsigned char *)D_801217D0 + (194)) = (s8) var_v1_3;
        *(s8 *)((unsigned char *)D_801217D0 + (158)) = (s8) var_v1_3;
        if (var_a0 != 0) {
            *(u16 *)((unsigned char *)D_801217D0 + (154)) = (u16) (*(u16 *)((unsigned char *)D_801217D0 + (154)) + 0x14);
            *(u16 *)((unsigned char *)D_801217D0 + (190)) = (u16) (*(u16 *)((unsigned char *)D_801217D0 + (190)) + 0x14);
            var_a0 = (var_a0 - 0x14) & 0xFF;
            *(u8 *)((unsigned char *)D_801217D0 + (195)) = (u8) (*(u8 *)((unsigned char *)D_801217D0 + (195)) - 0x14);
        } else {
            *(u16 *)((unsigned char *)D_801217D0 + (154)) = (u16) (*(u16 *)((unsigned char *)D_801217D0 + (154)) - 0x14);
            *(s16 *)((unsigned char *)D_801217D0 + (150)) = (s16) (*(s16 *)((unsigned char *)D_801217D0 + (150)) + 0x14);
            *(u16 *)((unsigned char *)D_801217D0 + (190)) = (u16) (*(u16 *)((unsigned char *)D_801217D0 + (190)) - 0x14);
            *(s16 *)((unsigned char *)D_801217D0 + (186)) = (s16) (*(s16 *)((unsigned char *)D_801217D0 + (186)) + 0x14);
        }
        if (*(u8 *)((unsigned char *)D_801217D0 + (231)) != 0) {
            *(u16 *)((unsigned char *)D_801217D0 + (226)) = (u16) (*(u16 *)((unsigned char *)D_801217D0 + (226)) + 0x14);
            *(u16 *)((unsigned char *)D_801217D0 + (262)) = (u16) (*(u16 *)((unsigned char *)D_801217D0 + (262)) + 0x14);
            *(u8 *)((unsigned char *)D_801217D0 + (231)) = (u8) (*(u8 *)((unsigned char *)D_801217D0 + (231)) - 0x14);
            *(u8 *)((unsigned char *)D_801217D0 + (267)) = (u8) (*(u8 *)((unsigned char *)D_801217D0 + (267)) - 0x14);
        } else {
            D_801217D0->unk0 = (s16) ((u16) D_801217D0->unk0 - 0x14);
            *(s16 *)((unsigned char *)D_801217D0 + (222)) = (s16) (*(s16 *)((unsigned char *)D_801217D0 + (222)) + 0x14);
            *(u16 *)((unsigned char *)D_801217D0 + (262)) = (u16) (*(u16 *)((unsigned char *)D_801217D0 + (262)) - 0x14);
            *(s16 *)((unsigned char *)D_801217D0 + (258)) = (s16) (*(s16 *)((unsigned char *)D_801217D0 + (258)) + 0x14);
        }
        *(u8 *)((unsigned char *)D_801217D0 + (159)) = var_a0;
        if (((var_a0 == 0) && ((g->unk_110D == 1) || (g->unk_110D == 3))) || ((*(u8 *)((unsigned char *)D_801217D0 + (231)) == 0) && ((g->unk_110D == 0) || (g->unk_110D == 2)))) {
            var_v1_4 = 0;
            if (g->unk_110D != 0) {
                var_v1_5 = 0;
                if (g->unk_110D != 1) {
                    var_v1_6 = 0;
                    if (g->unk_110D != 2) {
                        var_v1_7 = 0;
                        if (g->unk_110D != 3) {

                        } else {
                            do {
                                temp_lo = var_v1_7 * 0x24;
                                temp_t6 = var_v1_7 * 2;
                                var_v1_7 += 1;
                                temp_v0_3 = (Rec24 *)((u8 *)D_801217D0 + temp_lo);
                                temp_v0_3[6].unk_0A = 0;
                                temp_v0_3[6].unk_0F = 0xA0;
                                temp_v0_3[6].unk_06 = -0x78;
                                temp_v0_3[6].unk_0C = (s16) (temp_t6 + 0xA);
                            } while (var_v1_7 < 2);
                            LoadSquare(0x280U, 0U, 0x180U, 0xA0U, 0x801DD000);
                        }
                    } else {
                        do {
                            temp_lo_2 = var_v1_6 * 0x24;
                            temp_t8 = var_v1_6 * 2;
                            var_v1_6 += 1;
                            temp_v0_4 = (Rec24 *)((u8 *)D_801217D0 + temp_lo_2);
                            temp_v0_4[4].unk_0A = 0;
                            temp_v0_4[4].unk_0F = 0xA0;
                            temp_v0_4[4].unk_06 = -0x78;
                            temp_v0_4[4].unk_0C = (s16) (temp_t8 + 0x15);
                        } while (var_v1_6 < 2);
                        LoadSquare(0x140U, 0x100U, 0x180U, 0xA0U, 0x801BF000);
                    }
                } else {
                    do {
                        temp_lo_3 = var_v1_5 * 0x24;
                        temp_t9_3 = var_v1_5 * 2;
                        var_v1_5 += 1;
                        temp_v0_5 = (Rec24 *)((u8 *)D_801217D0 + temp_lo_3);
                        temp_v0_5[6].unk_0A = 0;
                        temp_v0_5[6].unk_0F = 0xA0;
                        temp_v0_5[6].unk_06 = -0x78;
                        temp_v0_5[6].unk_0C = (s16) (temp_t9_3 + 0xA);
                    } while (var_v1_5 < 2);
                    LoadSquare(0x280U, 0U, 0x180U, 0xA0U, 0x801A1000);
                }
            } else {
                do {
                    temp_lo_4 = var_v1_4 * 0x24;
                    temp_t5_3 = var_v1_4 * 2;
                    var_v1_4 += 1;
                    temp_v0_6 = (Rec24 *)((u8 *)D_801217D0 + temp_lo_4);
                    temp_v0_6[4].unk_0A = 0;
                    temp_v0_6[4].unk_0F = 0xA0;
                    temp_v0_6[4].unk_06 = -0x78;
                    temp_v0_6[4].unk_0C = (s16) (temp_t5_3 + 0x15);
                } while (var_v1_4 < 2);
                LoadSquare(0x140U, 0x100U, 0x180U, 0xA0U, 0x80183000);
            }
            temp_t7_3 = g->unk_110D + 1;
            g->unk_110D = temp_t7_3;
            if ((u32) (temp_t7_3 & 0xFF) >= 4U) {
                g->unk_110D = 0;
            }
        }
        temp_t9_4 = (D_8013A5CC + 1) & 0xFFFF;
        D_8013A5CC = temp_t9_4;
        if (temp_t9_4 == 0x96) {
            g->unk_1104.w += 1;
        }
        return;
    case 1:
        if (cd_read_sync_status() & 1) {
            func_800536AC(0x140, 0x100, 0x180, 0xA0);
            sp54 = 0x140;
            sp56 = 0x100;
            sp58 = 0x180;
            sp5A = 0xA0;
            func_8009C8E0((RECT *) &sp54, (void *)0x80183000);
            g->unk_1104.w += 1;
        }
block_48:
        goto block_49;
    case 2:
        dec_bg_cd_read(0x43D4, 1);
        g->unk_1104.w += 1;
        goto block_48;
    case 3:
        if (cd_read_sync_status() & 1) {
            func_800536AC(0x280, 0, 0x180, 0xA0);
            sp54 = 0x280;
            sp56 = 0;
            sp58 = 0x180;
            sp5A = 0xA0;
            func_8009C8E0((RECT *) &sp54, (void *)0x801A1000);
            g->unk_1104.w += 1;
        }
        goto block_48;
    case 4:
        dec_bg_cd_read(0x43DE, 1);
        g->unk_1104.w += 1;
        goto block_48;
    case 5:
        if (cd_read_sync_status() & 1) {
            func_800536AC(0x140, 0x100, 0x180, 0xA0);
            sp54 = 0x140;
            sp56 = 0x100;
            sp58 = 0x180;
            sp5A = 0xA0;
            func_8009C8E0((RECT *) &sp54, (void *)0x801BF000);
            g->unk_1104.w += 1;
        }
        goto block_48;
    case 6:
        dec_bg_cd_read(0x43E7, 1);
        g->unk_1104.w += 1;
        goto block_48;
    case 7:
        if (cd_read_sync_status() & 1) {
            func_800536AC(0x280, 0, 0x180, 0xA0);
            sp54 = 0x280;
            sp56 = 0;
            sp58 = 0x180;
            sp5A = 0xA0;
            func_8009C8E0((RECT *) &sp54, (void *)0x801DD000);
            g->unk_1104.w += 1;
        }
        goto block_48;
    case 8:
        var_v1_8 = 0;
        do {
            temp_lo_5 = var_v1_8 * 0x24;
            var_v1_8 += 1;
            *(s32 *)((unsigned char *)(D_801217D0 + temp_lo_5) + (144)) = 0x02000000;
        } while (var_v1_8 < 4);
        g->unk_1104.w += 1;
        func_80044750(0x500);
        goto block_49;
    case 9:
        temp_v0_7 = (*(u8 *)((unsigned char *)D_801217D0 + (164)) + 2) & 0xFF;
        *(u8 *)((unsigned char *)D_801217D0 + (274)) = temp_v0_7;
        *(u8 *)((unsigned char *)D_801217D0 + (273)) = temp_v0_7;
        *(u8 *)((unsigned char *)D_801217D0 + (272)) = temp_v0_7;
        *(u8 *)((unsigned char *)D_801217D0 + (238)) = temp_v0_7;
        *(u8 *)((unsigned char *)D_801217D0 + (237)) = temp_v0_7;
        *(u8 *)((unsigned char *)D_801217D0 + (236)) = temp_v0_7;
        *(u8 *)((unsigned char *)D_801217D0 + (202)) = temp_v0_7;
        *(u8 *)((unsigned char *)D_801217D0 + (201)) = temp_v0_7;
        *(u8 *)((unsigned char *)D_801217D0 + (200)) = temp_v0_7;
        *(u8 *)((unsigned char *)D_801217D0 + (166)) = temp_v0_7;
        *(u8 *)((unsigned char *)D_801217D0 + (165)) = temp_v0_7;
        *(u8 *)((unsigned char *)D_801217D0 + (164)) = temp_v0_7;
        if (temp_v0_7 == 0x80) {
            g->unk_1104.w += 1;
        }
        goto block_48;
    case 10:
        goto block_48;
    case 11:
        temp_v0_8 = (*(u8 *)((unsigned char *)D_801217D0 + (164)) - 2) & 0xFF;
        *(u8 *)((unsigned char *)D_801217D0 + (274)) = temp_v0_8;
        *(u8 *)((unsigned char *)D_801217D0 + (273)) = temp_v0_8;
        *(u8 *)((unsigned char *)D_801217D0 + (272)) = temp_v0_8;
        *(u8 *)((unsigned char *)D_801217D0 + (238)) = temp_v0_8;
        *(u8 *)((unsigned char *)D_801217D0 + (237)) = temp_v0_8;
        *(u8 *)((unsigned char *)D_801217D0 + (236)) = temp_v0_8;
        *(u8 *)((unsigned char *)D_801217D0 + (202)) = temp_v0_8;
        *(u8 *)((unsigned char *)D_801217D0 + (201)) = temp_v0_8;
        *(u8 *)((unsigned char *)D_801217D0 + (200)) = temp_v0_8;
        *(u8 *)((unsigned char *)D_801217D0 + (166)) = temp_v0_8;
        *(u8 *)((unsigned char *)D_801217D0 + (165)) = temp_v0_8;
        *(u8 *)((unsigned char *)D_801217D0 + (164)) = temp_v0_8;
        if (temp_v0_8 == 0) {
            var_v1_9 = 0;
            do {
                temp_lo_6 = var_v1_9 * 0x24;
                var_v1_9 += 1;
                *(s32 *)((unsigned char *)(D_801217D0 + temp_lo_6) + (144)) = 0x82000000;
            } while (var_v1_9 < 4);
            g->unk_1104.w += 1;
        }
        goto block_49;
    case 12:
        sp50 = (s32) g->unk_110A;
        sp4C = g->unk_1104.w;
        read_bustup();
        g->unk_1104.w = sp4C;
        if (sp50 != g->unk_110A) {
            g->unk_1104.w = sp4C + 1;
            g->unk_110A -= 1;
        }
        goto block_48;
    case 13:
    case 16:
    case 19:
    case 21:
    case 23:
        if (cd_read_sync_status() & 1) {
            g->unk_1104.w += 1;
        }
        goto block_48;
    case 14:
        sp4C = g->unk_1104.w;
        func_80083474();
        g->unk_110A -= 1;
        g->unk_1104.w = sp4C + 1;
        goto block_48;
    case 15:
        func_800462C8(0xAU, 0x80162000, 0x4359);
        g->unk_1104.w += 1;
        goto block_48;
    case 17:
        if ((((*(s32 *)D_80125C04) & 0xFFFF0000) != 0x38000000) || ((*(s32 *)((unsigned char *)D_80125C04 + (4)) & 0xFF010000) != 0x10000)) {
            func_800AE0B0("DEC_BS header Error \n");
            ((RECT *)D_800B5920)[0].w = 0;
            ((RECT *)D_800B5920)[0].h = 0;
            g->unk_1104.w -= 2;
            return;
        }
        func_8005751C(1);
        dec_bg_show_switch(1);
        g->unk_1104.w += 1;
        goto block_48;
    case 18:
        func_80044750(0x200);
        sp4C = g->unk_1104.w;
        func_80132BB0();
        g->unk_110A -= 1;
        g->unk_1104.w = sp4C + 1;
        goto block_48;
    default:
        hizuke_disp_switch(1);
        D_80122D2C = 0;
        func_8004284C();
        goto block_48;
    }
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80132DE0", func_801348F0);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80132DE0", func_801350A4);

s32 func_80135DA4(s16 arg0) {
    D_801217D0[4].unk_06 += arg0;
    D_801217D0[5].unk_06 += arg0;
    D_801217D0[4].unk_0A -= arg0;
    D_801217D0[5].unk_0A -= arg0;
    D_801217D0[6].unk_0A += arg0;
    D_801217D0[7].unk_0A += arg0;
    D_801217D0[6].unk_0F -= arg0;
    D_801217D0[7].unk_0F -= arg0;
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80132DE0", func_80135E50);

void func_801365DC(s32 arg0) {
    s32 v;

    D_800B3D60 = 0;
    v = func_8005742C(arg0, 1);
    if (v == D_800B5939) {
        func_800AE0B0("same\n");
        D_800E6280.unk_1104.w += 3;
        return;
    }
    if (v == 1 - D_800B5939) {
        func_800AE0B0("another\n");
        D_800E6280.unk_1104.w += 2;
    }
}

void func_80136660(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl44 tbl;

    tbl = D_8013A600;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

s32 func_801366E8(void) {
    s32 v;

    v = func_80051A68(0);
    if ((D_800E6280.unk_0F8 & 0xF) == D_800E6280.unk_03F && ((u32)(D_800E6280.unk_0F8 << 0x17) >> 0x1B) == D_800E6280.unk_040) {
        if (((u8)v & 0x7F) < 2U) {
            D_800E6280.unk_71E |= 4;
            D_800E6280.unk_722 = D_800E6280.unk_110A + 1;
            func_80042908(4);
            return 0;
        }
        D_800E6280.unk_71E |= 8;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80132DE0", func_80136794);

INCLUDE_ASM("asm/ovl/DATE2/nonmatchings/DATE2/80132DE0", func_80136860);

void func_80136A90(void) {
    D_8013A430 = 9;
    D_8013A438 = 9;
    func_801338F8();
    if (((u8) func_80051A68(0) & 0x7F) == 4 && (func_800AE0D0() % 2)) {
        D_8013A430 += 1;
    }
    if (((u8) func_80051A68(D_800E6280.unk_75D) & 0x7F) == 4 && (func_800AE0D0() % 2) && (D_800E6280.unk_75D != 6)) {
        D_8013A438 += 1;
    }
    func_8004284C();
}

void func_80136B7C(void) {
    bg_read_sub2(0x432B);
    func_8004284C();
}

void func_80136BA4(void) {
    if (D_800E6280.unk_1BC[0].unk_06 < 0xA) {
        D_8013A428 += 2;
        D_800E6280.unk_F5F = 0;
    } else if (get_h_yuukou(D_800E6280.unk_F5F) < 0xA) {
        D_8013A428 += 2;
    } else if (D_800E6280.unk_1BC[0].unk_0A >= 0x33) {
        D_800E6280.unk_F5F = 0;
    } else if (D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0A < 0x33) {
        if (D_800E6280.unk_1BC[0].unk_02 >= 0x32) {
            D_8013A428 += 1;
            D_800E6280.unk_F5F = 0;
        } else if (get_h_tokimeki(D_800E6280.unk_F5F) >= 0x32) {
            D_8013A428 += 1;
        } else {
            D_8013A428 += 4;
            func_8004284C();
            func_8004284C();
        }
    }
    func_8004284C();
}

void func_80136CEC(void) {
    D_8013A428 = 0x17;
    func_8004284C();
}

void func_80136D14(void) {
    func_800847B8((u8) *(s8 *)&D_800E6280.unk_F5F);
    D_8013A428 = 0x15;
    func_8004284C();
}
