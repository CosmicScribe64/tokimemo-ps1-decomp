#include "common.h"
#include "ovl/TACO.h"

void func_8015ABF0(void) {
    s32 i;

    for (i = 0; i < 64; i++) {
        D_80161D9C[i] = 0;
        D_80127080[i].unk0 = 0x80000000;
    }
    for (i = 16; i != 63; i++) {
        func_80143B34(i);
        *(s32 *) D_8015EDB4[i].pad4 = 0x80000000;
        D_8015EDB4[i].unk84[2] = 1;
    }
    D_801604C0 = 0;
    D_801604C4 = 0;
    D_801604C8 = 0;
    D_801604CC = 0;
    D_801604D0 = 0;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8015ABF0", func_8015ACCC);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8015ABF0", func_8015B1B8);

void func_8015B2AC(s32 arg0, s16 *arg1) {
    s32 pad[7]; /* FAKE: unused local reproduces the original frame (0x60) and spill offsets; real source unknown. T-6070 */
    TcObj50 *o;
    TcPos *pos;
    u8 *q;

    o = &D_80127480[arg0];
    o->unk18 += arg1[0];
    o->unk1C += arg1[1];
    o->unk20 += arg1[2];
    pos = &D_80128880[arg0];
    pos->x = arg1[3];
    pos->z = arg1[4];
    q = &o->pad0[4];
    pos->y = arg1[5];
    func_8015D0D0((pos->x << 12) / 360, q);
    func_8015CF30((pos->z << 12) / 360, q);
    func_8015D270((pos->y << 12) / 360, q);
    *(s32 *)o = 0;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8015ABF0", func_8015B3B0);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8015ABF0", func_8015B59C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8015ABF0", func_8015BD30);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8015ABF0", func_8015BF70);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8015ABF0", func_8015C0A0);

void func_8015C208(s32 arg0) {
    s32 i;

    func_8015C0A0(0, arg0);
    D_8015EDB4[arg0].pad0[1] = 0;
    D_8015EDB4[arg0].unk84[2] = 0;
    for (i = 0; i < 0x40; i++) {
        if (arg0 == D_80161D9C[i]) {
            D_80161D9C[i] = 0;
        }
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8015ABF0", func_8015C2CC);

void func_8015C698(void) {
    s32 i;

    for (i = 16; i < 63; i++) {
        D_8015EDB4[i].unk84[1] = 0;
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8015ABF0", func_8015C718);

s32 func_8015CC54(void) {
    s32 n;
    s32 i;

    n = 0;
    for (i = 0; i < 0x40; i++) {
        if (D_80161D9C[i] == 0) {
            n += 1;
        }
    }
    return n;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8015ABF0", func_8015CCC8);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8015ABF0", func_8015CE64);
