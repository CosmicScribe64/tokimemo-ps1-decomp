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

s32 func_80140364(s32 arg0, s32 arg1) {
    s32 sp38;
    TcActor *sp34;
    Tc14 *sp2C;
    s32 sp28;
    Tc14 *var_a2;
    Tc14 *var_v0;
    TcActor *var_a3;
    s32 temp_a1;
    s32 var_at;
    s32 var_v1;

    if (arg0 == 0) {
        var_a3 = D_8015EDB4;
        var_v0 = D_8015E4F0;
    } else {
        var_v0 = D_8015E630;
        var_a3 = &D_8015EDB4[8];
    }
    var_v1 = 0;
    if (arg1 & 0x20) {
        var_a2 = var_v0;
loop_5:
        var_at = var_v1 < 0x10;
        if (var_a2->unk13 & 0x80) {
            var_v1 += 1;
            var_a2 += 1;
            if (var_v1 == 0x10) {
                var_at = var_v1 < 0x10;
            } else {
                goto loop_5;
            }
        }
        if (var_at == 0) {
            return -1;
        }
        if (arg0 == 0) {
            sp38 = var_v1;
            sp2C = var_a2;
            sp34 = var_a3;
            func_801335A0(1, 0x503);
        }
        var_a2->unk13 = 0x92;
        temp_a1 = var_v1 & 1;
        var_a2->unk0 = var_a3->unk74;
        var_a2->unk2 = (var_a3->unk76 - (temp_a1 * 0x60)) + 0x40;
        var_a2->unk4 = var_a3->unk78;
        sp34 = var_a3;
        sp2C = var_a2;
        sp28 = temp_a1;
        var_a2->unk8 = (s16) (func_800A0070((s32) var_a3->unk68) / 256);
        if (temp_a1 != 0) {
            sp2C = var_a2;
            var_a2->unkA = (s16) ((s32) -func_800A0140((s32) var_a3->unk68) / 256);
        } else {
            sp2C = var_a2;
            var_a2->unkA = (s16) (func_800A0140((s32) var_a3->unk68) / 256);
        }
        var_a2->unkC = -0x40;
        var_a2->unk12 = 0;
        var_a2->unk11 = 0;
        return 1;
    }
    return 0;
}

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
