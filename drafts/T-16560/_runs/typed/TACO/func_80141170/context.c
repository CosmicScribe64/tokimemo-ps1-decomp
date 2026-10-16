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

s32 func_80141170(s32 arg0, s32 arg1) {
    s32 sp38;
    TcActor *sp34;
    Tc14 *sp2C;
    Tc14 *var_a0;
    Tc14 *var_v0_2;
    TcActor *var_a2;
    s32 var_at;
    s32 var_v0;
    s32 var_v1;

    if (arg0 == 0) {
        var_a2 = D_8015EDB4;
        var_a0 = D_8015E270;
    } else {
        var_a0 = D_8015E3B0;
        var_a2 = &D_8015EDB4[8];
    }
    var_v0 = 0;
    if (arg1 & 0x20) {
        var_v0_2 = var_a0;
        var_v1 = 0;
loop_5:
        var_at = var_v1 < 0x10;
        if (var_v0_2->unk13 & 0x80) {
            var_v1 += 1;
            var_v0_2 += 1;
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
            sp2C = var_v0_2;
            sp38 = var_v1;
            sp34 = var_a2;
            func_801335A0(1, 0x502);
        }
        var_v0_2->unk13 = 0x98;
        var_v0_2->unk0 = (var_a2->unk74 + ((var_v1 & 1) << 7)) - 0x40;
        var_v0_2->unk2 = (var_a2->unk76 - (arg0 << 6)) + 0x40;
        var_v0_2->unk8 = 0;
        var_v0_2->unkA = 0;
        var_v0_2->unkC = -0x200;
        var_v0_2->unk12 = 0;
        var_v0_2->unk4 = var_a2->unk78;
        var_v0 = 1;
        /* Duplicate return node #13. Try simplifying control flow for better match */
        return var_v0;
    }
    return var_v0;
}

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

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80140DF0", func_80141B18);
