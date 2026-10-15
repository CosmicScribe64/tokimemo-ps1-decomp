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

/* bulk_m2c declarations (T-11190) */
extern u32 D_800E7378;
/* end bulk_m2c declarations */
s32 func_801407D4(s32 *arg0, s32 arg1, s16 arg2) {
    s32 sp24;
    TcActor *temp_s0;
    TcActor *temp_s0_2;
    TcActor *temp_t0;
    s32 temp_hi_2;
    s32 temp_hi_3;
    s32 temp_hi_4;
    s32 temp_t2;
    s32 temp_t2_2;
    s32 var_a1;
    s32 var_t0;
    s32 var_t1;
    s32 var_t5;
    s32 var_v1;
    u32 temp_hi;

    *arg0 = -1;
    temp_hi = (u32) D_800E7378 % 48U;
    var_v1 = 0;
    var_a1 = 0x09C40000;
    temp_hi_2 = (s32) temp_hi % 48;
    if (*(u8 *)((unsigned char *)(&D_8015EDB4[temp_hi_2]) + (2177)) & 0x80) {
        temp_t0 = &D_8015EDB4[temp_hi_2 + 0x10];
        if ((temp_t0->unk84[3] & 1) && (D_8015EDB4->unk84[2] != 0xE)) {
            var_v1 = 1;
            if ((temp_t0->unk78 < arg2) && (sp24 < 0x09C40000)) {
                var_a1 = sp24;
                *arg0 = ((s32) temp_hi % 48) + 0x10;
            }
        }
    }
    var_t1 = temp_hi + 1;
    var_t5 = var_t1 + 1;
    var_t0 = 1;
    do {
        temp_hi_3 = var_t1 % 48;
        var_t1 += 2;
        if (*(u8 *)((unsigned char *)(&D_8015EDB4[temp_hi_3]) + (2177)) & 0x80) {
            temp_t2 = temp_hi_3 + 0x10;
            temp_s0 = &D_8015EDB4[temp_t2];
            if ((temp_s0->unk84[3] & 1) && (D_8015EDB4[var_t0].unk84[2] != 0xE)) {
                var_v1 += 1;
                if ((temp_s0->unk78 < arg2) && (sp24 < var_a1)) {
                    var_a1 = sp24;
                    *arg0 = temp_t2;
                }
            }
        }
        temp_hi_4 = var_t5 % 48;
        if (*(u8 *)((unsigned char *)(&D_8015EDB4[temp_hi_4]) + (2177)) & 0x80) {
            temp_t2_2 = temp_hi_4 + 0x10;
            temp_s0_2 = &D_8015EDB4[temp_t2_2];
            if ((temp_s0_2->unk84[3] & 1) && (*(u8 *)((unsigned char *)(&D_8015EDB4[var_t0]) + (270)) != 0xE)) {
                var_v1 += 1;
                if ((temp_s0_2->unk78 < arg2) && (sp24 < var_a1)) {
                    var_a1 = sp24;
                    *arg0 = temp_t2_2;
                }
            }
        }
        var_t0 += 2;
        var_t5 += 2;
    } while (var_t0 != 0x31);
    return var_v1;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013F250", func_80140ADC);
