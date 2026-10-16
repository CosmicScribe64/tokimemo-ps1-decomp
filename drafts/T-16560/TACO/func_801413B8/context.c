#include "common.h"
#include "ovl/TACO.h"

void func_80140DF0(void) {
    func_80140E18(0);
    func_80140E18(1);
}

void func_80140E18(s32 arg0) {
    Tc14 *base;
    Tc14 *p;
    TcActor *act;
    s32 i;

    if (arg0 == 0) {
        act = D_8015EDB4;
        base = D_8015E270;
    } else {
        base = D_8015E3B0;
        act = &D_8015EDB4[8];
    }
    for (i = 0, p = base; i < 16; i++, p++) {
        p->unk12 += 1;
        if (p->unk12 == 0xFF) {
            p->unk13 = 0;
        }
        if (p->unk4 < -0x7D00) {
            p->unk13 = 0;
        }
        switch (p->unk13 & 0x2F) {
        case 4:
            if (p->unk13 & 0x80) {
                p->unk0 += p->unk8;
                p->unk2 += p->unkA;
                p->unk4 += p->unkC;
                func_801413B8(base, i);
            }
            break;
        case 8:
            if (p->unk13 & 0x80) {
                p->unk0 += p->unk8;
                p->unk2 += p->unkA;
                p->unk4 += p->unkC;
                func_801416E8(base, i);
            }
            break;
        }
    }
    if ((base->unk13 & 0xA0) == 0xA0) {
        base->unk12 += 1;
        if (base->unk12 >= 3U) {
            base[0].unk0 = act->unk74 + 0x40;
            base[0].unk2 = act->unk76 + 0x20;
            base[0].unk4 = act->unk78;
            base[1].unk0 = act->unk74 - 0x40;
            base[1].unk2 = act->unk76 + 0x20;
            base[1].unk4 = act->unk78;
            func_80141B18(base, arg0);
        }
        if (base->unk12 >= 0x81U) {
            base->unk13 = 0;
            base[1].unk13 = 0;
        }
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80140DF0", func_80141050);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80140DF0", func_80141170);

s32 func_80141290(s32 arg0, s32 arg1) {
    Tc14 *p;
    s32 free_count;
    s32 offset;
    TcActor *act;
    Tc14 *base;

    if (arg0 == 0) {
        act = D_8015EDB4;
        base = D_8015E270;
    } else {
        base = D_8015E3B0;
        act = D_8015EDB4 + 8;
    }
    free_count = 0;
    if (arg1 & 0x20) {
        offset = 0;
        p = base;
        do {
            offset += 0x14;
            if (!(p->unk13 & 0x80)) {
                free_count += 1;
            }
            p += 1;
        } while (offset != 0x140);
        if (free_count != 0x10) {
            return -1;
        }
        if (arg0 == 0) {
            func_801335A0(1, 0x501);
        }
        base->unk13 = 0xB0;
        base->unk0 = act->unk74;
        base->unk2 = act->unk76 + 0x20;
        base->unk4 = act->unk78;
        base[1].unk0 = (s16) act->unk74;
        base[1].unk2 = (s16) (act->unk76 + 0x20);
        base[1].unk4 = (s16) act->unk78;
        base->unk8 = 0;
        base->unkA = 0;
        base->unkC = 0;
        base->unk12 = 0;
        return 1;
    }
    return 0;
}

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E6298;
extern u16 D_8015F580;
s32 func_800A1070();
/* end bulk_m2c declarations */
void func_801413B8(Tc14 *arg0, s32 arg1) {
    s32 sp96;
    s32 sp98;
    s32 sp92;
    s32 sp9E;
    s32 sp9C;
    s32 sp9A;
    s32 sp94;
    MATRIX spA8;
    s16 spA4;
    s16 spA2;
    s16 spA0;
    s16 sp90;
    s32 sp7C[1];
    s32 sp78[1];
    u16 sp6C;
    s32 sp60;
    Tc14 *temp_s7;
    s16 *var_s1;
    s16 temp_v0;
    s32 temp_s2;
    s32 temp_v0_2;
    s32 var_fp;
    s32 var_s0;
    s32 var_s1_2;
    u8 *temp_v1;
    u8 *var_s2;

    sp6C = D_8015F580;
    spA8 = D_80129C40;
    func_800A1100((s32) &spA8);
    func_800A1070(&spA8);
    temp_s7 = &arg0[arg1];
    sp60 = 0;
    var_fp = 0;
    spA4 = temp_s7->unk4;
    do {
        var_s1 = &sp90;
        spA2 = (temp_s7->unk2 + sp60) - 0x28;
        var_s2 = var_fp + var_s1;
        var_s0 = 0;
loop_2:
        spA0 = (temp_s7->unk0 + var_s0) - 0x28;
        func_800AB170((s32) &spA0, (s32) var_s2, (s32) &sp7C, (s32) &sp78);
        var_s1 += 4;
        var_s0 += 0x50;
        var_s2 += 8;
        if (var_s1 != &spA0) {
            goto loop_2;
        }
        var_fp += 4;
        sp60 += 0x50;
    } while (var_fp != 8);
    if ((sp98 >= -0x2C0) && (sp90 < 0x2C1)) {
        temp_v0 = *(&D_800E6298 + (D_8011ECA0 * 2));
        if (sp96 >= (temp_v0 - 0x200)) {
            if ((temp_v0 + 0x200) >= sp9E) {
                temp_s2 = ((u8) temp_s7->unk12 >> 2) & 1;
                var_s1_2 = (s32) (D_80122754 - temp_s7->unk4) / 127;
                if (var_s1_2 >= 0x100) {
                    var_s1_2 = 0xFF;
                } else if (var_s1_2 <= 0) {
                    var_s1_2 = 1;
                }
                temp_v0_2 = GetWorkBase(0x28, D_8011ECA0);
                func_8009F040(temp_v0_2);
                SetSemiTrans(temp_v0_2, 1);
                *(s8 *)((unsigned char *)temp_v0_2 + (4)) = 0x80;
                *(s8 *)((unsigned char *)temp_v0_2 + (5)) = 0x80;
                *(s8 *)((unsigned char *)temp_v0_2 + (6)) = 0x80;
                *(s16 *)((unsigned char *)temp_v0_2 + (22)) = func_8009ECB0(0, 1, 0x380, 0x100);
                *(s16 *)((unsigned char *)temp_v0_2 + (14)) = func_8009ED74(0x80, 0x1E4);
                temp_v1 = temp_s2 + &sp6C;
                *(s8 *)((unsigned char *)temp_v0_2 + (13)) = 1;
                *(u8 *)((unsigned char *)temp_v0_2 + (12)) = (u8) *temp_v1;
                *(s8 *)((unsigned char *)temp_v0_2 + (21)) = 1;
                *(s8 *)((unsigned char *)temp_v0_2 + (20)) = (s8) (*temp_v1 + 0xE);
                *(s8 *)((unsigned char *)temp_v0_2 + (29)) = 0xF;
                *(u8 *)((unsigned char *)temp_v0_2 + (28)) = (u8) *temp_v1;
                *(s8 *)((unsigned char *)temp_v0_2 + (37)) = 0xF;
                *(s8 *)((unsigned char *)temp_v0_2 + (36)) = (s8) (*temp_v1 + 0xE);
                *(s16 *)((unsigned char *)temp_v0_2 + (8)) = sp90;
                *(s16 *)((unsigned char *)temp_v0_2 + (10)) = sp92;
                *(s16 *)((unsigned char *)temp_v0_2 + (16)) = sp94;
                *(s16 *)((unsigned char *)temp_v0_2 + (18)) = sp96;
                *(s16 *)((unsigned char *)temp_v0_2 + (24)) = sp98;
                *(s16 *)((unsigned char *)temp_v0_2 + (26)) = sp9A;
                *(s16 *)((unsigned char *)temp_v0_2 + (32)) = sp9C;
                *(s16 *)((unsigned char *)temp_v0_2 + (34)) = sp9E;
                AddPrim(&D_800E8CA0[(D_8011ECA0 << 0xA) + (var_s1_2 * 4)], temp_v0_2);
            }
        }
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80140DF0", func_801416E8);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80140DF0", func_80141B18);
