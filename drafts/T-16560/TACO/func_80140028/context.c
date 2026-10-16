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

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E7378;
extern s32 subroutine_arg1;
extern s32 subroutine_arg2;
/* end bulk_m2c declarations */
void func_80140028(s32 arg0) {
    s32 sp6C;
    s32 sp8C;
    s32 sp80;
    s32 sp7C;
    s32 sp78;
    s32 sp68;
    Tc14 *var_s0;
    Tc14 *var_s3;
    TcActor *temp_v0_2;
    TcActor *var_s4;
    s32 var_s1;
    u8 temp_t5;
    u8 temp_t7;
    u8 temp_v0;
    u8 temp_v1;

    if (arg0 == 0) {
        var_s4 = D_8015EDB4;
        var_s3 = D_8015E4F0;
    } else {
        var_s3 = D_8015E630;
        var_s4 = &D_8015EDB4[8];
    }
    var_s1 = 0;
    var_s0 = var_s3;
    do {
        temp_t7 = var_s0->unk12 + 1;
        var_s0->unk12 = temp_t7;
        if ((temp_t7 & 0xFF) == 0xFF) {
            var_s0->unk13 = 0;
        }
        if (var_s0->unk4 < -0x7D00) {
            var_s0->unk13 = 0;
        }
        if ((var_s4->unk78 + 0x400) < var_s0->unk4) {
            var_s0->unk13 = 0;
        }
        temp_v1 = var_s0->unk13;
        if (temp_v1 & 0x80) {
            temp_v0 = var_s0->unk12;
            if ((temp_v1 & 1) && (temp_v0 >= 0x11U)) {
                if (!(D_800E7378 & 3)) {
                    temp_t5 = var_s0->unk11;
                    sp8C = (s32) temp_t5;
                    if (temp_t5 != -1U) {
                        temp_v0_2 = &D_8015EDB4[temp_t5];
                        if ((temp_v0_2->unk78 < var_s0->unk4) && (temp_v0_2->unk0[1] & 0x80) && (temp_v0_2->unk84[3] & 1)) {
                            sp78 = temp_v0_2->unk74 - var_s0->unk0;
                            sp80 = 0;
                            sp7C = temp_v0_2->unk76 - var_s0->unk2;
                            func_800AA460((s32) &sp78, (s32) &sp68);
                            var_s0->unk8 = (s16) ((s32) (var_s0->unk8 + (sp68 / 64)) / 2);
                            if (var_s0->unk8 >= 0x41) {
                                var_s0->unk8 = 0x40;
                            }
                            var_s0->unkA = (s16) ((s32) (var_s0->unkA + (sp6C / 64)) / 2);
                            if (var_s0->unkA >= 0x41) {
                                var_s0->unk8 = 0x40;
                            }
                        }
                    } else {
                        func_801407D4((s32) &sp8C, subroutine_arg1, subroutine_arg2);
                        var_s0->unk11 = (u8) sp8C;
                    }
                }
            } else if ((temp_v0 >= 0x11U) && (var_s0->unk11 == 0)) {
                var_s0->unk11 = 1;
                var_s0->unk12 = 0;
                var_s0->unkC = -0x200;
                var_s0->unk8 = (s16) ((s16) var_s0->unk8 / 16);
                var_s0->unkA = (s16) ((s16) var_s0->unkA / 16);
            }
            var_s0->unk0 += var_s0->unk8;
            var_s0->unk2 += var_s0->unkA;
            var_s0->unk4 += var_s0->unkC;
            func_80140ADC((s32) var_s3, var_s1);
        }
        var_s1 += 1;
        var_s0 += 1;
    } while (var_s1 != 0x10);
}

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
        if ((var_a2[0].unk0[1] & 0x80) && (var_a2[0].unk84[3] & 1) && (var_a2[0].unk84[2] != 0xE)) {
            if (*arg0 == -1) {
                *arg0 = var_v0;
                var_a2 = &D_8015EDB4[var_v0];
            }
            var_v1 += 1;
        }
        if ((var_a2[1].unk0[1] & 0x80) && (var_a2[1].unk84[3] & 1) && (var_a2[1].unk84[2] != 0xE)) {
            var_v1 += 1;
            if (*arg0 == -1) {
                *arg0 = var_v0 + 1;
                var_a2 = &D_8015EDB4[var_v0];
            }
        }
        if ((var_a2[2].unk0[1] & 0x80) && (var_a2[2].unk84[3] & 1) && (var_a2[2].unk84[2] != 0xE)) {
            var_v1 += 1;
            if (*arg0 == -1) {
                *arg0 = var_v0 + 2;
                var_a2 = &D_8015EDB4[var_v0];
            }
        }
        if ((var_a2[3].unk0[1] & 0x80) && (var_a2[3].unk84[3] & 1) && (var_a2[3].unk84[2] != 0xE)) {
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
