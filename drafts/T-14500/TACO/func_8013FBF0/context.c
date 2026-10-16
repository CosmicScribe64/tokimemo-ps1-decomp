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

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E6294;
extern s32 D_800E6298;
extern s32 D_800E7378;
/* end bulk_m2c declarations */
void func_8013FBF0(void) {
    u8 sp42;
    u8 sp41;
    s32 temp_v0;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 temp_v1;
    s32 var_s0;
    u8 temp_v0_2;
    u8 var_t0;
    u8 var_t1;

    temp_v0 = GetWorkBase(0x14, D_8011ECA0);
    func_8009F0A4(temp_v0);
    *(s16 *)((unsigned char *)temp_v0 + (18)) = 0x40;
    *(s16 *)((unsigned char *)temp_v0 + (16)) = 0x20;
    SetSemiTrans(temp_v0, 1);
    *(s8 *)((unsigned char *)temp_v0 + (4)) = 0xFF;
    *(s8 *)((unsigned char *)temp_v0 + (5)) = 0xFF;
    *(s8 *)((unsigned char *)temp_v0 + (6)) = 0xFF;
    *(s16 *)((unsigned char *)temp_v0 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] - 0xC8);
    *(s16 *)((unsigned char *)temp_v0 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + 0x28);
    *(s8 *)((unsigned char *)temp_v0 + (12)) = 0x80;
    *(s8 *)((unsigned char *)temp_v0 + (13)) = 0x40;
    *(s16 *)((unsigned char *)temp_v0 + (14)) = func_8009ED74(0x10, 0x1EF);
    AddPrim((D_8011ECA0 << 0xA) + 4 + D_800E8CA0, temp_v0);
    dtd_on_tpage(0x340, 0x100, 1, 0, 0);
    temp_v0_2 = D_8015EDB4->unk84[1];
    var_s0 = 0;
    if (temp_v0_2 >= 0xC1U) {
        var_t0 = 0;
        var_t1 = 1;
    } else {
        var_s0 = 0;
        if (temp_v0_2 >= 0x81U) {
            var_t0 = 1;
        } else {
            var_s0 = 1;
            if (temp_v0_2 >= 0x41U) {
                var_s0 = 1;
                var_t0 = 1;
            } else {
                var_t0 = 0;
            }
        }
        var_t1 = 0;
    }
    sp42 = var_t0;
    sp41 = var_t1;
    temp_v1 = ((s32) (0x1000 - func_800A0070(D_800E7378 * 0xF0)) / 128) + 0x40;
    func_80049A40(-0xBF, 0x68, 0xE, (s16) ((u32) -(s32) D_8015EDB4->unk84[1] >> 2), 2, (((var_s0 * temp_v1) & 0xFF) << 0x10) + (((var_t0 * temp_v1) & 0xFF) << 8) + ((var_t1 * temp_v1) & 0xFF), 0x81);
    dtd_on_tpage(0, 0, 2, 1, 0);
    temp_v0_3 = GetWorkBase(0x14, D_8011ECA0);
    func_8009F0A4(temp_v0_3);
    *(s16 *)((unsigned char *)temp_v0_3 + (16)) = 0x20;
    *(s16 *)((unsigned char *)temp_v0_3 + (18)) = 0x20;
    SetSemiTrans(temp_v0_3, 1);
    *(s8 *)((unsigned char *)temp_v0_3 + (4)) = 0x80;
    *(s8 *)((unsigned char *)temp_v0_3 + (5)) = 0x80;
    *(s8 *)((unsigned char *)temp_v0_3 + (6)) = 0x80;
    *(s16 *)((unsigned char *)temp_v0_3 + (8)) = (s16) (D_800E6280.unk_014[D_8011ECA0] - 0xC8);
    *(s16 *)((unsigned char *)temp_v0_3 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + ((u32) (0xFF - D_8015EDB4->unk84[1]) >> 2) + 0x18);
    *(s8 *)((unsigned char *)temp_v0_3 + (12)) = 0xA0;
    *(s8 *)((unsigned char *)temp_v0_3 + (13)) = 0x60;
    *(s16 *)((unsigned char *)temp_v0_3 + (14)) = func_8009ED74(0x20, 0x1EF);
    AddPrim((D_8011ECA0 << 0xA) + 4 + D_800E8CA0, temp_v0_3);
    dtd_on_tpage(0x340, 0x100, 1, 0, 0);
    temp_v0_4 = GetWorkBase(0x14, D_8011ECA0);
    func_8009F0A4(temp_v0_4);
    *(s16 *)((unsigned char *)temp_v0_4 + (18)) = 0x18;
    *(s16 *)((unsigned char *)temp_v0_4 + (16)) = 0x28;
    SetSemiTrans(temp_v0_4, 1);
    *(s8 *)((unsigned char *)temp_v0_4 + (4)) = 0x80;
    *(s8 *)((unsigned char *)temp_v0_4 + (5)) = 0x80;
    *(s8 *)((unsigned char *)temp_v0_4 + (6)) = 0x80;
    *(s16 *)((unsigned char *)temp_v0_4 + (8)) = (s16) (*(&D_800E6294 + (D_8011ECA0 * 2)) - 0xCE);
    *(s16 *)((unsigned char *)temp_v0_4 + (10)) = (s16) (*(&D_800E6298 + (D_8011ECA0 * 2)) + 0x68);
    *(s8 *)((unsigned char *)temp_v0_4 + (12)) = 0xA0;
    *(s8 *)((unsigned char *)temp_v0_4 + (13)) = 0x40;
    *(s16 *)((unsigned char *)temp_v0_4 + (14)) = func_8009ED74(0x20, 0x1EF);
    AddPrim((D_8011ECA0 << 0xA) + 4 + D_800E8CA0, temp_v0_4);
    dtd_on_tpage(0x340, 0x100, 1, 0, 0);
}

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

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013F250", func_80140ADC);
