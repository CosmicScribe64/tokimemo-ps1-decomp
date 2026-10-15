#include "common.h"
#include "ovl/TAIIKU.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A110);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A118);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A11C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A120);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A124);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A128);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A12C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A130);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A134);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A138);
u8 D_8014A13C[0x90] = { 0 };
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A1CC);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A1D0);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A1D4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A1E8);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A1EC);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A1FC);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A208);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A228);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A254);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A27C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A290);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A2A4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A2C4);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A2F0);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A304);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A318);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A320);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A328);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A334);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A340);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A34C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A358);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A360);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A370);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A374);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A378);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A37C);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A380);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A384);
INCLUDE_RODATA("asm/ovl/TAIIKU/data/TAIIKU/80142240.data", D_8014A388);
extern u8 D_8014A13C[]; /* func_801446A0: same shape as D_80149208 */
extern u8 D_8014A161;
extern u8 D_8014A185;
extern u8 D_8014A1A9;

typedef struct {
    void (*f[10])();
} FnTbl10; /* size 0x28 */
extern FnTbl10 D_8014A254;

void func_80142240(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl10 tbl;

    tbl = D_8014A254;
    idx = D_800E6280.unk_1109;
    tbl.f[idx]();
}

void func_801422BC(void) {
    if (D_800E6280.unk_110A == 0) {
        if ((func_8008667C(1) < 2) && (((u32) (D_800E6280.unk_1BC[1].unk_0C.w << 0x1E) >> 0x1F) == 1) && (D_800E6280.unk_56C[13] == 0)) {
            D_800E6280.unk_56C[13] = 1;
            D_8014A110 = 1;
            func_80044890(0, 0xBF98, 0xBF79, 0xD712, 0xD6C3, 0xD6B7);
            func_8004284C();
        } else {
            D_8014A110 = 0;
            func_80042908(3);
        }
        Default_Disp();
        return;
    }
    if (func_80044E8C() == 1) {
        func_80042808();
    }
    Default_Disp();
}

void func_801423A4(void) {
    s32 temp_v0;

    temp_v0 = func_80140A90();
    switch (temp_v0) {
    case 1:
        func_80042808();
        return;
    case 2:
        D_8014A110 = 0;
        func_80042808();
        /* fallthrough */
    case 0:
        return;
    }
}

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E628C;
extern s32 D_800E62AC;
extern s32 D_8014A1E8;
extern s32 D_8014A1EC;
extern s32 D_8014A208;
extern s32 D_8014A228;
extern s32 D_8014A27C;
extern s32 D_8014A290;
extern s32 D_8014A2A4;
extern s32 D_8014A2C4;
extern u8 D_800E738A;
void func_80145190(void);
void func_80143398(void);
void func_80143470(void);
/* end bulk_m2c declarations */
void func_80142400(void) {
    s32 sp92;
    typedef struct { s32 w[10]; } M2cBlock28; /* m2c memcpy of 40 bytes */
    s32 spB8[1];
    s16 spA4;
    s32 sp94[1];
    s16 sp90;
    s32 sp70;
    s32 sp44;
    s32 *var_v0_4;
    s32 *var_v1_3;
    s32 *var_v1_4;
    s32 *var_v1_6;
    GameState *var_v1;
    GameState *var_v1_2;
    s16 *var_v0_3;
    s16 temp_t2;
    s16 temp_t3;
    s16 temp_t4;
    s16 temp_t6;
    s16 temp_t7;
    s32 *var_v0;
    s32 *var_v0_2;
    s32 temp_t0;
    s32 temp_t8;
    s32 var_s1;
    u8 *var_s0;
    u8 *var_v1_5;

    spA4 = (s32) D_8014A27C;
    *(s32 *)((unsigned char *)(&spA4) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8014A27C) + (8));
    *(s32 *)((unsigned char *)(&spA4) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8014A27C) + (4));
    *(s32 *)((unsigned char *)(&spA4) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_8014A27C) + (12));
    *(s32 *)((unsigned char *)(&spA4) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_8014A27C) + (16));
    sp90 = (s32) D_8014A290;
    *(s32 *)((unsigned char *)(&sp90) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8014A290) + (4));
    *(s32 *)((unsigned char *)(&sp90) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8014A290) + (8));
    *(s32 *)((unsigned char *)(&sp90) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_8014A290) + (12));
    *(s32 *)((unsigned char *)(&sp90) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_8014A290) + (16));
    sp70 = D_8014A2A4;
    *(s32 *)((unsigned char *)(&sp70) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8014A2A4) + (4));
    *(s32 *)((unsigned char *)(&sp70) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_8014A2A4) + (12));
    *(s32 *)((unsigned char *)(&sp70) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8014A2A4) + (8));
    *(s32 *)((unsigned char *)(&sp70) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_8014A2A4) + (16));
    *(s32 *)((unsigned char *)(&sp70) + (20)) = (s32) *(s32 *)((unsigned char *)(&D_8014A2A4) + (20));
    *(s32 *)((unsigned char *)(&sp70) + (28)) = (s32) *(s32 *)((unsigned char *)(&D_8014A2A4) + (28));
    *(s32 *)((unsigned char *)(&sp70) + (24)) = (s32) *(s32 *)((unsigned char *)(&D_8014A2A4) + (24));
    *(M2cBlock28 *)(&sp44) = *(M2cBlock28 *)(&D_8014A2C4);
    *(s32 *)((unsigned char *)(&sp44 + 0x24) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8014A2C4 + 0x24) + (4));
    switch (D_800E738A) {
    case 0:
        if ((u8) D_8014A110 == 1) {
            func_80044750(0xBF);
        }
        func_80044890(0, 0xBF98, 0xBF79, 0xC20F, 0xC1EF, 0xD6B7);
        func_80085E30(1, 1);
        var_v1 = &D_800E6280;
        do {
            var_v1 += 4;
            var_v1->unk_1228[4] = 8;
        } while ((u32) var_v1 < (u32) &D_800E62AC);
        var_v1_2 = &D_800E6280;
        do {
            var_v1_2 += 4;
            var_v1_2->unk_1228[0x24] = 8;
        } while ((u32) var_v1_2 < (u32) &D_800E628C);
        var_v1_3 = &D_8014A208;
        var_v0 = &sp70;
        do {
            temp_t8 = *var_v0;
            var_v0 += 4;
            var_v1_3 += 4;
            *(s32 *)((unsigned char *)var_v1_3 + (-4)) = temp_t8;
        } while ((u32) var_v0 < (u32) &sp90);
        var_v1_4 = &D_8014A228;
        var_v0_2 = &sp44;
        do {
            temp_t0 = *var_v0_2;
            var_v0_2 += 4;
            var_v1_4 += 4;
            *(s32 *)((unsigned char *)var_v1_4 + (-4)) = temp_t0;
        } while ((u32) var_v0_2 < (u32) &sp70);
        var_v1_5 = D_8014A1D4;
        var_v0_3 = &spA4;
        do {
            temp_t2 = *var_v0_3;
            var_v0_3 += 2;
            var_v1_5 += 2;
            *(s16 *)((unsigned char *)var_v1_5 + (-2)) = temp_t2;
        } while ((u32) var_v0_3 < (u32) &spB8);
        var_v1_6 = &D_8014A1EC;
        var_v0_4 = &sp94;
        D_8014A1E8 = sp90;
        *(s16 *)((unsigned char *)(&D_8014A1E8) + (2)) = sp92;
        do {
            temp_t4 = *(s16 *)((unsigned char *)var_v0_4 + (2));
            temp_t7 = *(s16 *)((unsigned char *)var_v0_4 + (4));
            temp_t6 = *(s16 *)((unsigned char *)var_v0_4 + (6));
            temp_t3 = (*(s32 *)var_v0_4);
            var_v0_4 += 8;
            var_v1_6 += 8;
            *(s16 *)((unsigned char *)var_v1_6 + (-6)) = temp_t4;
            *(s16 *)((unsigned char *)var_v1_6 + (-4)) = temp_t7;
            *(s16 *)((unsigned char *)var_v1_6 + (-2)) = temp_t6;
            *(s16 *)((unsigned char *)var_v1_6 + (-8)) = temp_t3;
        } while (var_v0_4 != &spA4);
        func_80048EB8(0);
        func_8006BD6C(0);
        func_8004284C();
        break;
    case 1:
        if (func_80044E8C() == 1) {
            cd_read_sectors(0x1EU, 0x80180000, 0xBAD0);
            func_8004284C();
        }
        break;
    case 2:
        if (cd_read_sync_status() & 1) {
            cd_read_sectors(0x4FU, 0x8018F000, 0xB861);
            Fade_Out_Init();
            func_8004284C();
        }
        break;
    case 3:
        if ((Fade_Out_Main() != 0) && (cd_read_sync_status() & 1)) {
            func_80140810();
            func_80145190();
            var_s0 = &D_8014A1FC;
            do {
                search_load_tpage_buf_lock(*var_s0, 1);
                var_s0 += 4;
            } while ((u32) var_s0 < (u32) &D_8014A208);
            var_s1 = 0;
            do {
                load_palette(D_8014A378, (var_s1 + 0x10) & 0xFF, 1, 1, var_s1);
                var_s1 += 1;
            } while (var_s1 != 0x10);
            func_8014357C();
            func_80143398();
            func_80143470();
            cd_read_sectors(0xA1U, 0x8018F000, 0xB7C0);
            func_80042808();
        }
        break;
    default:
        func_80046500();
        break;
    }
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80142240", func_8014280C);

void func_80142D90(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        if (func_80086370() != 0) {
            func_80085ED0();
            func_8004284C();
        }
        func_80133438();
        break;
    case 1:
        if (func_80085EEC() >= 0x5B) {
            func_80044750(0x502);
            if ((u8) D_8014A110 != 0) {
                D_80120778 = 0x39;
            } else {
                D_80120778 = 0x2D;
                func_80044750(0x800);
            }
            D_80120763 = 0xA4;
            D_80120762 = 0x40;
            func_8004284C();
        }
        func_80133438();
        break;
    case 2:
        func_80143AC8();
        func_80132E20((s32) (s16) ((s16) D_8014A1CE / 2), 4);
        func_80143C04();
        func_80144768();
        func_8014430C();
        if (func_80144464() != 0) {
            func_80044750(0x504);
            func_800862D0();
            func_8004284C();
        }
        break;
    case 3:
        func_80085F0C((s32) ((s16) D_8014A1CE / 4) % 640, 0xF, 0, 0x70, 0x280);
        func_80085F0C((s32) ((s16) D_8014A1CE / 2) % 128, 0xF, 0x70, 0x80, 0x80);
        if (func_800862F0() != 0) {
            func_80144E54();
            func_80133258();
            func_80042808();
        }
        break;
    default:
        func_80046500();
        break;
    }
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80142240", func_80142FB8);

void func_80143398(void) {
    s32 i;

    if ((u8) D_8014A110 == 0) {
        func_8004E44C(0, D_8014A384, D_8014A370);
        srn_init(0, D_8014A37C, 0);
    } else {
        func_8004E44C(0, D_8014A388, D_8014A374);
        srn_init(0, D_8014A380, 0);
    }
    srn_vram_set(0, 5, 0x580, 0xF0);
    for (i = 0; i < 3; i++) {
        D_800E6280.unk_1228[i + 5] = 8;
    }
    for (i = 0; i < 3; i++) {
        D_800E6280.unk_1228[i + 0x25] = 8;
    }
}

void func_80143470(void) {
    func_80048F64(0x60);
    D_80120651 = 3;
    D_80120688 = 0x01000000;
    D_80120652 = 1;
    D_80120653 = 0xA4;
    D_80120657 = 0x80;
    D_8012065C = (u8 *) D_80149FC0;
    D_80120660 = (u8 *) D_80149FF8;
    D_80120684 = &D_8014A1FC;
    D_80120664 = D_8014A030;
    D_80120654 = 0;
    D_80120655 = 0x80;
    if ((u8) D_8014A110 == 0) {
        D_80120666 = 0;
    } else {
        D_80120666 = 2;
    }
    D_80120676 = 0x50;
    D_8012067A = 0xB0;
    D_8012066A = 0x1000;
    D_8012066C = 0x1000;
    D_80120656 = 2;
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80142240", func_8014357C);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80142240", func_80143814);

void func_80143AC8(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        *(s32 *)(D_8014A13C + i * 0x24 + 4) += 1;
        *(s32 *)(D_8014A13C + i * 0x24 + 0x10) += 1;
        *(u16 *)(D_8014A13C + i * 0x24 + 0x1A) += 1;
        *(u16 *)(D_8014A13C + i * 0x24 + 0x18) += 1;
    }
    D_8014A118 += 1;
}

void func_80143C04(void) {
    if ((D_80120778 != 0x2D) && (D_80120778 != 0x39)) {
        func_80143C84();
        func_80143CC0();
        func_80143D24();
        func_80143DD0();
        func_80143F50();
        return;
    }
    if ((u32)D_8014A154 >= 0x32) {
        D_80120778 = 0x2F;
    }
}

void func_80143C84(void) {
    if (D_8014A158 != 0) {
        D_8014A144 = D_8014A128;
        return;
    }
    D_8014A144 = -D_8014A128;
}

void func_80143CC0(void) {
    if ((u32)D_8014A14C >= 0xB) {
        D_8014A148 += D_8014A144;
        if (D_8014A148 < -0x4000) {
            D_8014A148 = -0x4000;
        }
        if (D_8014A12C < D_8014A148) {
            D_8014A148 = D_8014A12C;
        }
        D_8014A14C = 0;
    }
}

/* FAKE: overlapping word/halfword views use the existing byte data island;
 * its coordinate record layout remains incomplete. T-9550 */
void func_80143D24(void) {
    if (*(s32 *)&D_8014A13C[8] < 0 || D_8014A1CE == 0) {
        if (D_8014A1D0 != 0) {
            *(s32 *)&D_8014A13C[0x14] -= D_8014A1D0;
        }
    } else {
        *(s32 *)&D_8014A13C[0x14] += (s32)D_8014A12C >> 2;
    }
    if (*(s16 *)&D_8014A13C[0x16] >= -0x1F) {
        *(s16 *)&D_8014A13C[0x16] = -0x20;
    }
    if (*(s16 *)&D_8014A13C[0x16] < -0x9F) {
        *(s16 *)&D_8014A13C[0x16] = -0xA0;
    }
    D_80120676 = *(s16 *)&D_8014A13C[0x16];
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80142240", func_80143DD0);

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80142240", func_80143F50);

void func_8014430C(void) {

    if ((u8) D_8014A118 >= 0xBU) {
        /* FAKE: word view of the existing byte-record array; its full
         * structure is not recovered yet. T-10010. */
        D_8014A1D0 += (*(s32 *)&D_8014A13C[8]);
        D_8014A118 = 0;
    }
    if (D_8014A12C < D_8014A1D0) {
        D_8014A1D0 = D_8014A12C;
    }
    if (D_8014A1D0 < 0) {
        D_8014A1D0 = 0;
    }
    if ((D_80120778 == 0x2D) || (D_80120778 == 0x39)) {
        D_8014A1D0 = 0;
    }
    D_8014A1CC.w += D_8014A1D0;
    Scroll((s32) ((s16) D_8014A1CC.h[1] / 4) % 640, 0xF, 0, 0x70, 0x280);
    Scroll((s32) ((s16) D_8014A1CC.h[1] / 2) % 128, 0xF, 0x70, 0x80, 0x80);
    func_8004B19C(D_8014A1D0, 0, -0x3C);
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80142240", func_80144464);

/* FAKE: v[6] as in func_8013815C. T-0016 */
s32 func_801446A0(void) {
    s16 v[6];
    s32 i;

    for (i = 0; i < 3; i++) {
        if (D_8014A13C[i * 0x24 + 0x25] == 0) {
            v[i] = *(s16 *)(D_8011ECD0 + i * 0x44 + 0x19EA);
        } else {
            v[i] = -0xF00;
        }
    }
    if (v[0] >= v[1] && v[0] >= v[2]) {
        D_8014A161 = 1;
        return 2;
    }
    if (v[1] >= v[2]) {
        D_8014A185 = 1;
        return 3;
    }
    D_8014A1A9 = 1;
    return 4;
}

typedef struct {
    s16 v[3];
} Tri3S; /* size 0x6 */
extern Tri3S D_8014A320;

void func_80144768(void) {
    s32 i; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    Tri3S tbl;

    tbl = D_8014A320;
    for (i = 0; i < 3; i++) {
        if (*(u16 *)&D_8014A13C[i * 0x24 + 0x3C] >= 0x33U) {
            if (i + 0x36 == *(s16 *)&D_8011ECD0[i * 0x44 + 0x19DC]) {
                *(s16 *)&D_8011ECD0[i * 0x44 + 0x19DC] = tbl.v[i];
            }
        }
    }
    func_80144834();
    func_801448D4();
    func_801449A0();
    func_80144B40();
    func_80144C70();
}

typedef struct {
    s32 v[3];
} Tri3; /* size 0xC */
extern Tri3 D_8014A328;

void func_80144834(void) {
    s32 i; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    Tri3 tbl;

    tbl = D_8014A328;
    for (i = 0; i < 3; i++) {
        if (i + 0x36 != *(s16 *) (D_8011ECD0 + i * 0x44 + 0x19DC)) {
            if (*(s32 *) (D_8014A13C + i * 0x24 + 0x40) != 0) {
                *(s32 *) (D_8014A13C + i * 0x24 + 0x2C) = tbl.v[i];
            } else {
                *(s32 *) (D_8014A13C + i * 0x24 + 0x2C) = -0x500;
            }
        }
    }
}

extern Tri3 D_8014A334;

void func_801448D4(void) {
    s32 i; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    Tri3 tbl;
    u8 *q;
    u8 *r;
    s32 v;

    tbl = D_8014A334;
    for (i = 0, q = D_8011ECD0; i < 3; i++, q += 0x44) {
        if (i + 0x36 != *(s16 *)(q + 0x19DC)) {
            r = D_8014A13C + i * 0x24;
            if (*(u32 *)(r + 0x34) >= 0xBU) {
                v = *(s32 *)(r + 0x30) += *(s32 *)(r + 0x2C);
                if (v < 0) {
                    *(s32 *)(r + 0x30) = 0;
                    v = 0;
                }
                if (tbl.v[i] < v) {
                    *(s32 *)(r + 0x30) = tbl.v[i];
                }
                *(s32 *)(r + 0x34) = 0;
            }
        }
    }
}

extern Tri3 D_8014A340;

/* FAKE: retain the established byte-buffer views until the record layouts
 * are recovered. The factored flag-store stride preserves the height reload.
 * T-10010. */
void func_801449A0(void) {
    s32 i;
    s32 d;
    u8 *q; /* FAKE: unused word keeps tbl at stack +0x48. T-10010. */
    Tri3 tbl;
    s32 y;
    s32 n;

    tbl = D_8014A340;
    d = D_8014A1CE;
    for (i = 0; i < 3; i++) {
        *(s32 *)&D_8014A13C[i * 0x24 + 0x38] += *(s32 *)&D_8014A13C[i * 0x24 + 0x30];
        y = *(s16 *)&D_8014A13C[i * 0x24 + 0x3A] - d;
        *(s16 *)&D_8011ECD0[i * 0x44 + 0x19EA] = y - 0xA0;
        if (i + 0x36 != *(s16 *)&D_8011ECD0[i * 0x44 + 0x19DC] && D_8014A130[i] > 0) {
            *(s32 *)&D_8014A13C[i * 0x24 + 0x38] += tbl.v[i];
            D_8014A130[i] -= tbl.v[i];
            y = *(s16 *)&D_8014A13C[i * 0x24 + 0x3A] - d;
        }
        if (*(s16 *)&D_8011ECD0[0x1AFA] + i * 8 + 8 < y - 0x60 && D_8014A13C[i * 0x24 + 0x25] == 0) {
            *(s16 *)&D_8011ECD0[0x1B30] = 0x32;
            *(s16 *)&D_8011ECD0[0x1B3E] = *(s16 *)&D_8011ECD0[0x1AFA] + 0x20;
            n = 0;
            while (D_8014A120[n] != 0) {
                n++;
            }
            D_8014A120[n] = i + 2;
            D_8014A13C[(i * 3) * 12 + 0x25] = 1;
            y = *(s16 *)&D_8014A13C[i * 0x24 + 0x3A] - d;
        }
        if (y >= 0x1E1 || y < -0xA0) {
            D_8011ECD0[i * 0x44 + 0x19C7] = 0;
        } else {
            D_8011ECD0[i * 0x44 + 0x19C7] = 0xA4;
        }
    }
}

extern Tri3 D_8014A34C;

void func_80144B40(void) {
    s32 i;
    Tri3 tbl;

    tbl = D_8014A34C;
    for (i = 0; i < 3; i++) {
        if (i + 0x36 != *(s16 *)&D_8011ECD0[i * 0x44 + 0x19DC]) {
            if ((u32)tbl.v[i] < *(u32 *)&D_8014A13C[i * 0x24 + 0x28] && *(s32 *)&D_8014A13C[i * 0x24 + 0x40] == 1) {
                if ((s32)D_8014A13C[i * 0x24 + 0x24] >= 9) {
                    D_8014A13C[i * 0x24 + 0x24] = 2;
                    *(s32 *)&D_8014A13C[i * 0x24 + 0x28] = 0;
                } else {
                    D_8014A13C[i * 0x24 + 0x24] += 1;
                    *(s32 *)&D_8014A13C[i * 0x24 + 0x28] = 0;
                }
                *(s16 *)&D_8011ECD0[i * 0x44 + 0x19DC] = *(s16 *)&D_8014A1D4[D_8014A13C[i * 0x24 + 0x24] * 2] + i * 9 + 0x12;
            }
            if (D_8014A13C[i * 0x24 + 0x24] == 6) {
                D_8014A13C[i * 0x24 + 0x26] = 1;
            }
        }
    }
}

extern Tri3S D_8014A358;

void func_80144C70(void) {
    s32 i; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    Tri3S tbl;
    u8 *q;
    u8 *r;

    tbl = D_8014A358;
    for (i = 0, q = D_8011ECD0; i < 3; i++, q += 0x44) {
        if (i + 0x36 != *(s16 *)(q + 0x19DC)) {
            r = D_8014A13C + i * 0x24;
            if (*(s32 *)(r + 0x28) == 0 && (r[0x24] == 2 || r[0x24] == 6) && *(s32 *)(r + 0x40) == 1 &&
                tbl.v[i] < (func_800AE0D0() & 0xFF) && r[0x26] == 1) {
                *(s32 *)(r + 0x40) = 0;
                *(s16 *)(r + 0x3E) = 0;
                *(s32 *)(r + 0x30) = 0;
                *(s16 *)(q + 0x19DC) = func_800AE0D0() % 3 + i * 9 + 0x18;
            }
        }
    }
    for (i = 0, q = D_8011ECD0; i != 3; i++, q += 0x44) {
        if (i + 0x36 != *(s16 *)(q + 0x19DC)) {
            r = D_8014A13C + i * 0x24;
            if (*(s32 *)(r + 0x40) == 0 && *(u16 *)(r + 0x3E) >= 0x1EU) {
                *(s32 *)(r + 0x40) = 1;
                *(s16 *)(r + 0x3C) = 0;
                r[0x24] = 0;
                r[0x26] = 0;
                *(s16 *)(q + 0x19DC) = i + 0x36;
            }
        }
    }
}

INCLUDE_ASM("asm/ovl/TAIIKU/nonmatchings/TAIIKU/80142240", func_80144E54);

void func_80145044(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        *(s16 *)&D_8011ECD0[0x1998 + i * 0x44] += 1;
    }
    for (i = 0; i < 4; i++) {
        *(s16 *)&D_8011ECD0[0x1AA8 + i * 0x44] -= 4 - i;
    }
}

void func_801450C0(void) {
    s32 i;
    TkS16x4 t;

    t = D_8014A360;
    for (i = 0; i < 4; i++) {
        *(s16 *)&D_8011ECD0[0x1998 + i * 0x44] += t.v[i];
    }
}

void func_80145148(void) {
    if ((u32)D_8014A11C >= 0xF) {
        *(s16 *)(D_8011ECD0 + 0x19DC) ^= 1;
        *(s16 *)(D_8011ECD0 + 0x1A20) ^= 1;
        D_8014A11C = 0;
    }
}

void func_80145190(void) {
    D_8014A370 = 0x801AD000;
    D_8014A374 = 0x801AE018;
    D_8014A378 = 0x8018F000;
    D_8014A37C = 0x801AF1B0;
    D_8014A380 = 0x801B29EC;
    D_8014A384 = 0x801AD008;
    D_8014A388 = 0x801AE020;
}
