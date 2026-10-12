#include "common.h"
#include "ovl/TACO.h"

#ifdef NON_MATCHING
/* NON_MATCHING: T-10520, only the saved-register order differs ($s1/$s2/$s3 for arg0, the first table base and the loop end: the original ranks the end constant last), 9 lines */
void func_80146D60(s32 arg0) {
    TcObj50 *q;
    TcObj50 *p;
    TcObj50 *e;
    s32 i;

    q = D_80127480;
    func_80099E30(0, q);
    p = &D_80127480[1];
    e = (TcObj50 *)D_80128880;
    do {
        func_80099E30(q, p);
        p->unk20 = arg0;
        p->unk1C = 0;
        p->unk18 = 0;
        *(s32 *)p = 0;
        p++;
    } while (p < e);
    for (i = 0; i < 0x24; i++) {
        D_80128880[i].x = D_80128880[i].y = D_80128880[i].z = 0;
    }
}
#else
INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80146D60", func_80146D60);
#endif

void func_80146E3C(void) {
    D_80122760 = 0x14;
    D_80122764 = 0x64;
    D_80122768 = -0x64;
    D_8012276C = 0xB0;
    D_8012276D = 0xB0;
    D_8012276E = 0xB0;
    func_8009AD70(0, &D_80122760);
    D_80122770 = 0x14;
    D_80122774 = -0x64;
    D_80122778 = 0x64;
    D_8012277C = 0x80;
    D_8012277D = 0x80;
    D_8012277E = 0x80;
    func_8009AD70(1, &D_80122770);
    D_80122780 = -0x14;
    D_80122784 = 0x14;
    D_80122788 = -0x64;
    D_8012278C = 0x60;
    D_8012278D = 0x60;
    D_8012278E = 0x60;
    func_8009AD70(2, &D_80122780);
    func_8009B310(0, 0, 0);
    func_8009B340(0);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80146D60", func_80146F74);

#ifdef NON_MATCHING
/* NON_MATCHING: T-10520, saved-register order differs: the original gives the end pointer (D_80128880) the first free $s register and the table pointer the next; about 30 lines (a K&R definition fixes the prologue but the header check rejects it) */
void func_801471D0(s32 arg0) {
    TcObj50 *p;
    Tc10 *q;
    Tc10 *r;
    TcObj50 *o;
    s32 a;
    s32 i;
    u32 n;

    arg0 += 4;
    func_8009B3C0(arg0);
    p = D_80127480;
    q = D_80127080;
    do {
        func_8009B430(arg0 + 8, (u8 *)q, 0);
        q->unk4[0] = (s32)p;
        p++;
        q++;
        q[-1].unk0 = 0x80000000;
    } while (p < (TcObj50 *)D_80128880);
    func_8009B3C0(arg0);
    n = *(u32 *)(arg0 + 4);
    i = 0;
    if (n != 0) {
        r = (Tc10 *)((u8 *)D_80127080 + 0x10);
        o = (TcObj50 *)((u8 *)D_80127480 + 0x50);
        q = D_80127080;
        do {
            func_8009B430(arg0 + 8, (u8 *)r, i);
            i++;
            q->unk4[3] = (s32)o;
            o++;
            q++;
            r++;
            q->unk0 = 0x80000000;
        } while ((u32)i < n);
    }
}
#else
INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80146D60", func_801471D0);
#endif

#ifdef NON_MATCHING
/* NON_MATCHING: T-10520, frame and body match (local MATRIX at sp+0x2C, `s32 pad` declared first for the 4 spare bytes); ugen temporaries of the block copy are $t7/$t8 against $t8/$t9 and one addiu of the first loop is scheduled a slot apart, about 12 lines */
s32 func_801472D4(arg0, arg1, arg2, arg3, arg4, arg5)
s32 arg0;
s32 arg1;
s16 *arg2;
s16 *arg3;
u8 arg4;
u8 arg5;
{
    Tc10 *rec;
    MATRIX m;
    s32 i;
    s32 j;
    s32 pad;

    if (((s32)arg2 & 3) || ((s32)arg3 & 3)) {
        return -1;
    }
    if (arg4 == 0) {
        arg2 += arg1 * 3;
    }
    if (arg5 == 0) {
        arg3 += arg1 * 9;
    }
    for (i = 0; i != 3; i++) {
        for (j = 0; j != 3; j++) {
            m.m[i][j] = *arg3++;
        }
        m.t[i] = *arg2++;
    }
    rec = &D_80127080[arg0];
    *(MATRIX *)((u8 *)rec->unk4[0] + 4) = m;
    *(s32 *)rec->unk4[0] = 0;
    return (s32)rec;
}
#else
INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80146D60", func_801472D4);
#endif

/* s32 counter against a u32 bound: IDO cannot count the trips and keeps the loop rolled, like the
 * original (T-5020). */
void func_80147400(u8 *arg0) {
    s32 i;
    u32 n;
    u8 *p;

    n = *(u32 *)(arg0 + 8);
    p = arg0 + 0x1C;
    for (i = 1; i < n + 1; i++) {
        D_8015F290[i] = *(s32 *)p;
        p += 0x1C;
    }
}

void func_80147444(void *arg0, s32 arg1, s32 arg2, s32 arg3)
{
    RECT rect;
    /* FAKE: preserve the original frame extent (T-9510). */
    s32 pad[2];

    rect.x = arg1;
    rect.y = arg2;
    rect.w = 0x100;
    rect.h = arg3;
    func_8009C884(&rect, arg0);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80146D60", func_80147488);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80146D60", func_80147674);

void func_801478F8(s32 arg0, s16 *arg1) {
    TcPos *p;

    p = &D_80128880[arg0];
    p->x = arg1[3];
    p->z = arg1[4];
    arg1 += 5;
    p->y = arg1[0];
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80146D60", func_80147928);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80146D60", func_80147B24);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80146D60", func_80147C98);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80146D60", func_80147E80);
