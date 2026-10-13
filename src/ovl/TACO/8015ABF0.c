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

/* NON_MATCHING: T-10810, frame and MATRIX offset match now (declare i, j before matrix: later-declared locals sit lower); only the struct-copy temporary differs (ugen t2 against t3, 8 lines) */
#ifdef NON_MATCHING
void func_8015B1B8(arg0, arg1, arg2, arg3)
    s32 arg0;
    s16 *arg1;
    s16 *arg2;
    s32 arg3;
{
    s32 i;
    s32 j;
    MATRIX matrix;
    TcTransformObj50 *obj;

    arg2 += arg3 * 9;
    arg1 += arg3 * 3;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            matrix.m[i][j] = *arg2++;
        }
        matrix.t[i] = *arg1++;
    }
    obj = (TcTransformObj50 *)&D_80127480[arg0];
    obj->transform = matrix;
    obj->unk_00 = 0;
}
#else
INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8015ABF0", func_8015B1B8);
#endif

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

void func_8015B3B0(s32 arg0, s16 *arg1, s32 arg2) {
    /* The pointer precedes the payload to preserve its local layout. T-9520. */
    TcTransformObj50 *obj;
    MATRIX matrix;
    TcPos *pos;

    arg1 += arg2 * 6;
    matrix = *(MATRIX *)D_80125C60;
    obj = (TcTransformObj50 *)&D_80127480[arg0];
    matrix.t[0] = obj->transform.t[0] + arg1[0];
    matrix.t[1] = obj->transform.t[1] + arg1[1];
    matrix.t[2] = obj->transform.t[2] + arg1[2];
    pos = &D_80128880[arg0];
    pos->x += arg1[3];
    arg1 += 4;
    func_8015D0D0((pos->x << 12) / 360, &matrix);
    pos->z += *arg1;
    arg1++;
    func_8015CF30((pos->z << 12) / 360, &matrix);
    pos->y += *arg1;
    func_8015D270((pos->y << 12) / 360, &matrix);
    obj->transform = matrix;
    obj->unk_00 = 0;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8015ABF0", func_8015B59C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8015ABF0", func_8015BD30);

/* NON_MATCHING: T-10810, loop, callee arguments and saved-register set match (78 vs 76 instructions); the local actor pointer and the type byte take s4/s2 where the original has s2/s4 (the CSE'd &D_8015EDB4[arg0] range is coloured before the local), plus a spill-order difference in the prologue */
#ifdef NON_MATCHING
void func_8015BF70(s32 arg0) {
    TcActor *a;
    u32 n;
    s32 i;
    u8 *id;
    u8 t;
    s32 *rec;

    a = &D_8015EDB4[arg0];
    n = 0;
    if (a->unk82 >= 0xF) {
        a->unk82 = 0;
        a = &D_8015EDB4[arg0];
    }
    t = a->unk84[2];
    id = D_80161D9C;
    i = 0;
    do {
        if (arg0 == *id) {
            rec = D_80161E60[t] + n * 2;
            func_8015B1B8(i, rec[0], rec[1], a->unk82);
            n++;
            a = &D_8015EDB4[arg0];
            t = a->unk84[2];
        }
        i++;
        if (n >= D_80161E20[t]) {
            break;
        }
        id++;
    } while (i != 0x40);
}
#else
INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8015ABF0", func_8015BF70);
#endif

/* NON_MATCHING: T-10620, only the base register of the actor pointer: the original keeps &D_8015EDB4[arg1] in v0 (no copy), IDO allocates the local and the expression as two ranges (v1, v0), about 6 lines */
#ifdef NON_MATCHING
void func_8015C0A0(arg0, arg1)
s32 arg0;
s32 arg1;
{
    TcActor *a;
    s32 i;

    a = &D_8015EDB4[arg1];
    if (a->pad0[1] & 0x80) {
        if (arg0 == 1) {
            if (a->unk84[2] < 0xB) {
                *(s32 *)((u8 *)a + 4) = 0;
                return;
            }
            *(s32 *)((u8 *)a + 4) = 0x80000000;
            for (i = 0; i < 0x40; i++) {
                if (arg1 == D_80161D9C[i]) {
                    D_80127080[i].unk0 = 0;
                }
            }
            return;
        }
        if (a->unk84[2] < 9) {
            *(s32 *)((u8 *)a + 4) = 0x80000000;
            return;
        }
        *(s32 *)((u8 *)a + 4) = 0x80000000;
        for (i = 0; i < 0x40; i++) {
            if (arg1 == D_80161D9C[i]) {
                D_80127080[i].unk0 = 0x80000000;
            }
        }
    }
}
#else
INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8015ABF0", func_8015C0A0);
#endif

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

s32 func_8015CCC8(arg0)
s32 arg0;
{
    s32 n;
    s32 i;

    n = 0;
    for (i = 16; i != 63; i++) {
        if (((TcActor *)(i * 0x88 + (u8 *)D_8015EDB4))->unk84[1] != 0) {
            if (!(((TcActor *)(i * 0x88 + (u8 *)D_8015EDB4))->pad0[1] & 0x80)) {
                ((TcActor *)(i * 0x88 + (u8 *)D_8015EDB4))->unk84[1] = 0;
            }
        }
        if (arg0 != 0xFF) {
            if (arg0 == ((TcActor *)(i * 0x88 + (u8 *)D_8015EDB4))->unk84[2] && ((TcActor *)(i * 0x88 + (u8 *)D_8015EDB4))->unk84[1] != 0) {
                n++;
            }
        } else if (((TcActor *)(i * 0x88 + (u8 *)D_8015EDB4))->unk84[1] != 0) {
            n++;
        }
    }
    return n;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8015ABF0", func_8015CE64);
