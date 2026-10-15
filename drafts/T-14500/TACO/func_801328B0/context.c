#include "common.h"
#include "ovl/TACO.h"

void func_80132220(void) {
    D_8015EDB4->unk74 = 0;
    D_8015EDB4->unk76 = 0;
    D_8015EDB4->unk78 = 0x800;
    D_8015EDB4[8].unk74 = 0;
    D_8015EDB4[8].unk76 = 0;
    D_8015EDB4[8].unk78 = 0x800;
    D_8015EDB4->unk2 = 0;
    D_8015EDB4[8].unk2 = 0;
    D_8015EDB4->unk3 = 0;
    D_8015EDB4->unk68 = 0;
    D_8015EDB4[8].unk68 = 0;
}

void func_801322AC(void) {
    func_801438F0(0x8019C800, 0, D_8015EDB4);
    D_8015EDB4->pad0[1] = 0x80;
    D_8015EDB4->unk6C = 0x400;
    D_8015EDB4->unk6E = 0x400;
    D_8015EDB4->unk70 = 0x400;
    func_801438F0(0x8019C800, 1, &D_8015EDB4[8]);
    D_8015EDB4[8].pad0[1] = 0x80;
    D_8015EDB4[8].unk6C = 0x400;
    D_8015EDB4[8].unk6E = 0x400;
    D_8015EDB4[8].unk70 = 0x400;
    D_8015EDB4->unk7A = 0;
    D_8015EDB4[8].unk7A = 0;
    D_8015EDB4->unk72 = -1;
    D_8015EDB4[8].unk72 = -1;
    D_8015EDB4->unk82 = -1;
    D_8015EDB4[8].unk82 = -1;
    D_8015EDB4->unk7C = 4;
    D_8015EDB4->unk7E = 3;
    D_8015EDB4->unk80 = 0;
    D_8015EDB4->unk84[1] = 0xFF;
    D_8015EDB4[8].unk84[1] = 0x80;
    func_80132220();
    D_8015EDD0 = 0;
    D_8015EDD4 = 0;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80132220", func_8013240C);

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E7378;
/* end bulk_m2c declarations */
void func_801328B0(void) {
    s32 sp50;
    s32 sp4C;
    s32 sp70;
    s32 sp60;
    s32 sp5C;
    s32 sp58;
    s32 sp48;
    s32 sp40;
    s32 sp38;
    TcActor *temp_s0;
    TcActor *temp_v0_5;
    s16 temp_v0_2;
    s16 temp_v0_3;
    s16 temp_v1;
    s32 temp_v0;
    s32 temp_v0_4;
    s32 var_a1;
    s32 var_at;
    u8 var_v1;

    temp_v0 = func_80140638((s32) &sp70);
    sp38 = temp_v0;
    var_a1 = 0;
    if (temp_v0 != 0) {
        if (!(D_800E7378 & 7)) {
            goto block_4;
        }
        if (!(D_800E7378 & 0xF)) {
block_4:
            var_a1 = 0x20;
        }
    }
    temp_s0 = D_8015EDB4 + 0x440;
    *(s8 *)((unsigned char *)D_8015EDB4 + (1089)) = 0x80;
    temp_v0_2 = temp_s0->unk72;
    temp_s0->unk84[3] = 1;
    temp_s0->unk84[0] = 2;
    if (temp_v0_2 == 0) {
        sp40 = var_a1;
        func_80141170(1, var_a1);
    } else if (temp_v0_2 == 1) {
        sp40 = var_a1;
        func_80141290(1, var_a1);
    } else {
        sp40 = var_a1;
        func_80141050(1, var_a1);
    }
    temp_v0_3 = temp_s0->unk82;
    if (temp_v0_3 == 0) {
        func_801404FC(1, var_a1);
    } else if (temp_v0_3 == 1) {
        func_80140364(1, var_a1);
    }
    if ((u32) sp38 < 8U) {
        sp58 = D_8015EDB4->unk74 - temp_s0->unk74;
        sp60 = 0;
        sp5C = D_8015EDB4->unk76 - temp_s0->unk76;
        temp_v0_4 = func_800AA460((s32) &sp58, (s32) &sp48);
        var_v1 = temp_s0->unk2;
        switch (var_v1) {
        default:
            temp_s0->unk2 = 0;
            var_v1 = 0 & 0xFF;
            /* fallthrough */
        case 0:
            temp_s0->pad0[1] = 0x80;
            temp_s0->unk2 = var_v1 + 1;
            /* fallthrough */
        case 1:
            if ((u32) temp_v0_4 >= 0x10001U) {
                temp_s0->unk74 += sp48 / 103;
                temp_s0->unk76 += sp4C / 103;
            }
            break;
        case 2:
            temp_s0->unk2 = var_v1 + 1;
            /* fallthrough */
        case 3:
            if ((u32) temp_v0_4 >= 0x10001U) {
                temp_s0->unk74 += sp4C / 103;
                temp_s0->unk76 += sp48 / 103;
            }
            break;
        case 4:
            temp_s0->pad0[1] = 0x80;
            temp_s0->unk2 = var_v1 + 1;
            /* fallthrough */
        case 5:
            sp58 = D_8015EDB4->unk74 - temp_s0->unk74;
            sp60 = 0;
            sp5C = (D_8015EDB4->unk76 - temp_s0->unk76) + 0x80;
            if (func_800AA460((s32) &sp58, (s32) &sp48) >= 0x641U) {
                temp_s0->unk74 += sp4C / 103;
                temp_s0->unk76 += sp48 / 103;
            }
            break;
        case 6:
            temp_s0->pad0[1] = 0x90;
            temp_s0->unk7C = 3 - (func_800AE0D0() & 7);
            temp_s0->unk7E = 3 - (func_800AE0D0() & 7);
            temp_s0->unk2 += 1;
            /* fallthrough */
        case 7:
            if ((D_8015EDB4->unk78 + 0x21) < temp_s0->unk78) {
                temp_s0->unk80 = -0x20;
            }
            if (temp_s0->unk78 < 0x29) {
                temp_s0->unk80 = 0x20;
            }
            break;
        case 8:
            temp_s0->pad0[1] = 0x90;
            sp58 = 0xF - (func_800AE0D0() & 0x1F);
            sp5C = 0xF - (func_800AE0D0() & 0x1F);
            sp60 = 0xF - (func_800AE0D0() & 0x1F);
            func_800AA460((s32) &sp58, (s32) &sp48);
            temp_s0->unk7C = (s16) (sp48 / 103);
            temp_s0->unk7E = (s16) (sp4C / 103);
            temp_s0->unk2 += 1;
            temp_s0->unk80 = (s16) (sp50 / 103);
            /* fallthrough */
        case 9:
            if (!(func_800AE0D0() & 0x7F)) {
                temp_s0->unk2 = 8;
            }
            break;
        }
    } else {
        temp_v0_5 = &D_8015EDB4[sp70];
        sp58 = temp_v0_5->unk74 - temp_s0->unk74;
        sp60 = 0;
        sp5C = temp_v0_5->unk76 - temp_s0->unk76;
        func_800AA460((s32) &sp58, (s32) &sp48);
        temp_s0->unk74 += sp48 / 103;
        temp_s0->unk76 += sp4C / 103;
    }
    temp_s0->unk78 += temp_s0->unk80;
    if (temp_s0->unk74 >= 0x1001) {
        temp_s0->unk74 = 0x1000;
    }
    if (temp_s0->unk74 < -0x1000) {
        temp_s0->unk74 = -0x1000;
    }
    if (temp_s0->unk76 >= 0x181) {
        temp_s0->unk76 = 0x180;
    }
    if (temp_s0->unk76 < -0x800) {
        temp_s0->unk76 = -0x800;
    }
    if (!(func_800AE0D0() & 0x3F)) {
        temp_s0->unk80 = 0xF - (func_800AE0D0() & 0x1F);
    }
    temp_v1 = temp_s0->unk78;
    var_at = temp_v1 < -0x1000;
    if ((D_8015EDB4->unk78 + 0x200) < temp_v1) {
        temp_s0->unk80 = -(func_800AE0D0() & 0xF);
        var_at = temp_s0->unk78 < -0x1000;
    }
    if (var_at != 0) {
        temp_s0->unk80 = func_800AE0D0() & 0xF;
    }
    temp_s0->unk6C = 0x400;
    temp_s0->unk6E = 0x400;
    temp_s0->unk70 = 0x400;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80132220", func_80132FE8);

void func_80133374(void) {
    if (D_8015EDCC < 3U) {
        D_8015EDEC -= 1;
    }
    if (D_8015EDEC == 0) {
        func_80042908(0x2D);
    }
    if (D_800E6280.unk_F88 & 0x800) {
        func_80042908(0x2D);
    }
    if (D_8015EDB4->unk84[1] == 0) {
        func_80042908(0x2D);
    }
}

void func_80133410(s32 arg0) {
    s32 i;

    if (arg0 == 0) {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 16; i++) D_8015E270[i].unk13 &= ~0x20;
    } else {
        /* FAKE: loop body on the for line; IDO then schedules it as the original (line-based scheduling). T-7020 */
        for (i = 0; i < 16; i++) D_8015E3B0[i].unk13 &= ~0x20;
    }
}

void func_801334BC(void) {
    s32 i;

    D_80160460 = 0;
    D_80160464 = 0; for (i = 0; i < 16; i++) { /* FAKE: store and loop head on one source line, as1 then schedules the store after the pointer setup */
        D_8015E270[i].unk13 = 0;
        D_8015E3B0[i].unk13 = 0;
        D_8015E4F0[i].unk13 = 0;
        D_8015E630[i].unk13 = 0;
        D_8015E770[i].unk13 = 0;
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80132220", func_8013356C);
