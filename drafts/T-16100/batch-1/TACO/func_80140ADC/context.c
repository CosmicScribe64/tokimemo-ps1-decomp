#include "common.h"
#include "ovl/TACO.h"

void func_8013F250(void) {
    /* FAKE: preserve the observed local homes and frame extent. T-10200. */
    s32 pad[2];
    s32 sp34;
    s32 sp30;
    s32 sp2C;

    sp34 = (s16) D_8015EDEC / 3600;
    sp30 = (s32) ((s16) D_8015EDEC % 3600) / 60;
    sp2C = (s32) (((s16) D_8015EDEC % 60) * 0x3C) / 100;
    func_801432F0((s32) "SCORE", -0xE0, -0x64, 0);
    func_801430D0(D_8015EDF0 + (D_8015EDCC * 0xF4240), -0x50, -0x64, 0);
    func_801432F0((s32) "TIME", 0x40, -0x64, 0);
    func_801430D0(sp34, 0x90, -0x64, 1);
    func_801430D0(sp30, 0xB0, -0x64, 2);
    func_801430D0(sp2C, 0xD0, -0x64, 3);
    func_8013F430();
    /* This counter uses an unsigned byte load; other TACO users have a signed view. */
    if (*(u8 *)&D_8015EDD0 != 0) {
        (*(u8 *)&D_8015EDD0)--;
        func_801430D0(D_8015EDD4, 0x14, 0x64, 3);
    }
    func_80133738();
}

void func_8013F430(void) {
    func_8013F468();
    func_8013F7D8();
    func_8013F9C0();
    func_8013FBF0();
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013F250", func_8013F468);

void func_8013F7D8(void) {
    s16 v;

    v = D_8015EDB4->unk7C;
    if ((v == 3) || (v == 4)) {
        func_8013F874(0xB4, 0x60, v);
    }
    v = D_8015EDB4->unk7E;
    if ((v == 3) || (v == 4)) {
        func_8013F874(0xC8, 0x60, v);
    }
    v = D_8015EDB4->unk80;
    if ((v == 3) || (v == 4)) {
        func_8013F874(0xDC, 0x60, v);
    }
}

/* FAKE: the UV stores share a source line to preserve constant scheduling.
 * T-9560; original statement layout unknown. */
void func_8013F874(s32 arg0, s32 arg1, s32 arg2) {
    s32 palette;
    SPRT *sprite;

    sprite = (SPRT *)GetWorkBase(0x14, D_8011ECA0);
    func_8009F0A4(sprite);
    sprite->w = 16;
    sprite->h = 16;
    SetSemiTrans(sprite, 1);
    sprite->r0 = 0x80;
    sprite->g0 = 0x80;
    sprite->b0 = 0x80;
    sprite->x0 = D_800E6280.unk_014[D_8011ECA0] + arg0;
    sprite->y0 = D_800E6280.unk_018[D_8011ECA0] + arg1;
    sprite->u0 = 0xC0; sprite->v0 = arg2 * 16 - 0x30;
    palette = ((D_800E6280.unk_10F8 >> 1) % 14) * 16;
    if (arg2 == 3) {
        sprite->clut = func_8009ED74(palette, 0x1EA);
    } else {
        sprite->clut = func_8009ED74(palette, 0x1E9);
    }
    AddPrim((D_8011ECA0 << 10) + 4 + D_800E8CA0, (s32)sprite);
    dtd_on_tpage(0x380, 0x100, 1, 0, 0);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013F250", func_8013F9C0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013F250", func_8013FBF0);

void func_80140000(void) {
    func_80140028(0);
    func_80140028(1);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013F250", func_80140028);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013F250", func_80140364);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013F250", func_801404FC);

s32 func_80140638(s32 *arg0) {
    /* FAKE: explicit groups and first count placement preserve the unrolled
       schedule; the original source loop is unknown. T-9560. */
    s32 var_v0;
    s32 var_v1;
    TcActor *var_a2;

    var_v1 = 0;
    *arg0 = -1;
    var_v0 = 0x10;
    do {
        var_a2 = &D_8015EDB4[var_v0];
        if ((var_a2[0].pad0[1] & 0x80) && (var_a2[0].unk84[3] & 1) && (var_a2[0].unk84[2] != 0xE)) {
            if (*arg0 == -1) {
                *arg0 = var_v0;
                var_a2 = &D_8015EDB4[var_v0];
            }
            var_v1 += 1;
        }
        if ((var_a2[1].pad0[1] & 0x80) && (var_a2[1].unk84[3] & 1) && (var_a2[1].unk84[2] != 0xE)) {
            var_v1 += 1;
            if (*arg0 == -1) {
                *arg0 = var_v0 + 1;
                var_a2 = &D_8015EDB4[var_v0];
            }
        }
        if ((var_a2[2].pad0[1] & 0x80) && (var_a2[2].unk84[3] & 1) && (var_a2[2].unk84[2] != 0xE)) {
            var_v1 += 1;
            if (*arg0 == -1) {
                *arg0 = var_v0 + 2;
                var_a2 = &D_8015EDB4[var_v0];
            }
        }
        if ((var_a2[3].pad0[1] & 0x80) && (var_a2[3].unk84[3] & 1) && (var_a2[3].unk84[2] != 0xE)) {
            var_v1 += 1;
            if (*arg0 == -1) {
                *arg0 = var_v0 + 3;
            }
        }
        var_v0 += 4;
    } while (var_v0 != 0x40);
    return var_v1;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013F250", func_801407D4);

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E6298;
extern s32 D_80129C40;
s32 func_800A1070();
/* end bulk_m2c declarations */
void func_80140ADC(s32 arg0, s32 arg1) {
    s16 screen_xy[8];
    s32 spA0[8];
    s16 sp9C;
    s16 sp9A;
    s16 sp98;
    s32 sp74[1];
    s32 sp70[1];
    s32 sp64;
    s16 *var_s1;
    s16 temp_s3;
    s16 temp_v0;
    s32 temp_v0_2;
    s32 var_fp;
    s32 var_s0;
    s32 var_s2_2;
    s8 temp_a1;
    s8 temp_v1;
    u8 *temp_s7;
    u8 *var_s2;

    (*(s32 *)&spA0) = (s32) D_80129C40;
    *(s32 *)((unsigned char *)(&spA0) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (4));
    *(s32 *)((unsigned char *)(&spA0) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (8));
    *(s32 *)((unsigned char *)(&spA0) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (12));
    *(s32 *)((unsigned char *)(&spA0) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (16));
    *(s32 *)((unsigned char *)(&spA0) + (20)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (20));
    *(s32 *)((unsigned char *)(&spA0) + (24)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (24));
    *(s32 *)((unsigned char *)(&spA0) + (28)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (28));
    func_800A1100((s32) &spA0);
    func_800A1070(&spA0);
    temp_s7 = (arg1 * 0x14) + arg0;
    sp64 = 0;
    var_fp = 0;
    sp9C = *(s16 *)((unsigned char *)temp_s7 + (4));
    do {
        var_s1 = &screen_xy[0];
        sp9A = (*(s16 *)((unsigned char *)temp_s7 + (2)) + sp64) - 0x10;
        var_s2 = var_fp + var_s1;
        var_s0 = 0;
loop_2:
        sp98 = ((*(s32 *)temp_s7) + var_s0) - 0x10;
        func_800AB170((s32) &sp98, (s32) var_s2, (s32) &sp74, (s32) &sp70);
        var_s1 += 4;
        var_s0 += 0x20;
        var_s2 += 8;
        if (var_s1 != &sp98) {
            goto loop_2;
        }
        var_fp += 4;
        sp64 += 0x20;
    } while (var_fp != 8);
    if ((screen_xy[4] >= -0x2C0) && (screen_xy[0] < 0x2C1)) {
        temp_v0 = *(&D_800E6298 + (D_8011ECA0 * 2));
        if (screen_xy[3] >= (temp_v0 - 0x200)) {
            if ((temp_v0 + 0x200) >= screen_xy[7]) {
                temp_s3 = ((u8) *(u8 *)((unsigned char *)temp_s7 + (18)) >> 4) & 1;
                var_s2_2 = (s32) (D_80122754 - *(s16 *)((unsigned char *)temp_s7 + (4))) / 127;
                if (var_s2_2 >= 0x100) {
                    var_s2_2 = 0xFF;
                    goto block_11;
                }
                if (var_s2_2 > 0) {
block_11:
                    temp_v0_2 = GetWorkBase(0x28, D_8011ECA0);
                    func_8009F040(temp_v0_2);
                    SetSemiTrans(temp_v0_2, 0);
                    *(s8 *)((unsigned char *)temp_v0_2 + (4)) = 0x80;
                    *(s8 *)((unsigned char *)temp_v0_2 + (5)) = 0x80;
                    *(s8 *)((unsigned char *)temp_v0_2 + (6)) = 0x80;
                    *(s16 *)((unsigned char *)temp_v0_2 + (22)) = func_8009ECB0(1, 1, 0x3C0, 0x100);
                    temp_v1 = temp_s3 << 5;
                    temp_a1 = temp_v1 + 0x20;
                    *(s16 *)((unsigned char *)temp_v0_2 + (14)) = func_8009ED74(0x100, 0x1F8);
                    *(s8 *)((unsigned char *)temp_v0_2 + (20)) = temp_a1;
                    *(s8 *)((unsigned char *)temp_v0_2 + (36)) = temp_a1;
                    *(s8 *)((unsigned char *)temp_v0_2 + (13)) = 0x60;
                    *(s8 *)((unsigned char *)temp_v0_2 + (21)) = 0x60;
                    *(s8 *)((unsigned char *)temp_v0_2 + (12)) = temp_v1;
                    *(s8 *)((unsigned char *)temp_v0_2 + (28)) = temp_v1;
                    *(s8 *)((unsigned char *)temp_v0_2 + (29)) = 0x80;
                    *(s8 *)((unsigned char *)temp_v0_2 + (37)) = 0x80;
                    *(s16 *)((unsigned char *)temp_v0_2 + (8)) = screen_xy[0];
                    *(s16 *)((unsigned char *)temp_v0_2 + (10)) = screen_xy[1];
                    *(s16 *)((unsigned char *)temp_v0_2 + (16)) = screen_xy[2];
                    *(s16 *)((unsigned char *)temp_v0_2 + (18)) = screen_xy[3];
                    *(s16 *)((unsigned char *)temp_v0_2 + (24)) = screen_xy[4];
                    *(s16 *)((unsigned char *)temp_v0_2 + (26)) = screen_xy[5];
                    *(s16 *)((unsigned char *)temp_v0_2 + (32)) = screen_xy[6];
                    *(s16 *)((unsigned char *)temp_v0_2 + (34)) = screen_xy[7];
                    AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + (var_s2_2 * 4)], temp_v0_2, 0x60);
                }
            }
        }
    }
}
