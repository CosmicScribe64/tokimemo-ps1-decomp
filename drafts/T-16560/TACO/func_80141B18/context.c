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

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80140DF0", func_801413B8);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80140DF0", func_801416E8);

/* bulk_m2c declarations (T-11190) */
s32 func_800A1070();
/* end bulk_m2c declarations */
void func_80141B18(Tc14 *arg0, s32 arg1) {
    s32 spA2;
    s32 spA6;
    s32 spAA;
    s32 spAE;
    s32 spAC;
    MATRIX spB8;
    s16 spB4;
    s16 spB2;
    s16 spB0;
    s16 spA8;
    s16 spA4;
    s16 spA0;
    s32 sp80[1];
    s32 sp7C[1];
    u8 sp75;
    Tc14 *var_s0;
    s16 *var_s1;
    s16 *var_s2;
    s16 *var_v0;
    s16 temp_v1_2;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s8 temp_a0;
    s8 temp_v1;
    u8 temp_v0;

    temp_v0 = arg0->unk12;
    if (temp_v0 >= 0x81U) {
        sp75 = 0xFF - temp_v0;
    } else {
        sp75 = temp_v0;
    }
    spB8 = D_80129C40;
    func_800A1100((s32) &spB8);
    func_800A1070(&spB8);
    temp_v0_2 = 1 - arg1;
    var_s0 = arg0;
    var_s2 = &spA0;
    var_s1 = &spA4;
    do {
        spB4 = var_s0->unk4 + (temp_v0_2 << 0xB);
        spB2 = var_s0->unk2 + (temp_v0_2 << 8);
        spB0 = var_s0->unk0;
        func_800AB170((s32) &spB0, (s32) var_s2, (s32) &sp80, (s32) &sp7C);
        spB4 = -0x7FFF;
        spB2 = var_s0->unk2 + 0x100;
        spB0 = var_s0->unk0;
        func_800AB170((s32) &spB0, (s32) var_s1, (s32) &sp80, (s32) &sp7C);
        var_s1 += 4;
        var_s0 += 1;
        var_s2 += 4;
    } while ((u32) var_s1 < (u32) &spB4);
    temp_v0_3 = GetWorkBase(0x24, D_8011ECA0);
    func_8009F054(temp_v0_3);
    SetSemiTrans(temp_v0_3, 1);
    temp_a0 = sp75 + 0x10;
    temp_v1 = sp75 + 0x20;
    *(s8 *)((unsigned char *)temp_v0_3 + (13)) = temp_a0;
    *(s8 *)((unsigned char *)temp_v0_3 + (21)) = temp_a0;
    *(s8 *)((unsigned char *)temp_v0_3 + (29)) = temp_a0;
    *(s8 *)((unsigned char *)temp_v0_3 + (5)) = 0x10;
    *(s8 *)((unsigned char *)temp_v0_3 + (4)) = temp_v1;
    *(s8 *)((unsigned char *)temp_v0_3 + (6)) = 0xC0;
    *(s8 *)((unsigned char *)temp_v0_3 + (12)) = temp_v1;
    *(s8 *)((unsigned char *)temp_v0_3 + (14)) = 0xC0;
    *(s8 *)((unsigned char *)temp_v0_3 + (20)) = 0x20;
    *(s8 *)((unsigned char *)temp_v0_3 + (22)) = 0xC0;
    *(s8 *)((unsigned char *)temp_v0_3 + (28)) = temp_v1;
    *(s8 *)((unsigned char *)temp_v0_3 + (30)) = 0xC0;
    var_v0 = &spA0;
    do {
        temp_v1_2 = *(s16 *)((unsigned char *)var_v0 + (8));
        if ((*(s32 *)var_v0) == temp_v1_2) {
            *(s16 *)((unsigned char *)var_v0 + (8)) = (s16) (temp_v1_2 + 1);
        }
        var_v0 += 2;
    } while (var_v0 != &spA8);
    *(s16 *)((unsigned char *)temp_v0_3 + (8)) = spA4;
    *(s16 *)((unsigned char *)temp_v0_3 + (10)) = spA6;
    *(s16 *)((unsigned char *)temp_v0_3 + (16)) = spA0;
    *(s16 *)((unsigned char *)temp_v0_3 + (18)) = spA2;
    *(s16 *)((unsigned char *)temp_v0_3 + (24)) = spAC;
    *(s16 *)((unsigned char *)temp_v0_3 + (26)) = spAE;
    *(s16 *)((unsigned char *)temp_v0_3 + (32)) = spA8;
    *(s16 *)((unsigned char *)temp_v0_3 + (34)) = spAA;
    AddPrim((D_8011ECA0 << 0xA) + 0x10 + D_800E8CA0, temp_v0_3);
    dtd_on_tpage(0x340, 0x100, 4, 1, 0);
}
