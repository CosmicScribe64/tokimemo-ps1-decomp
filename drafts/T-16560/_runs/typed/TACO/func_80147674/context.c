#include "common.h"
#include "ovl/TACO.h"

void func_80146D60(s32 arg0) {
    TcObj50 *q;
    s32 i;

    q = D_80127480;
    func_80099E30(0, q);
    for (i = 1; i < 64; i++) {
        func_80099E30(q, &D_80127480[i]);
        D_80127480[i].unk1C = 0;
        D_80127480[i].unk18 = 0;
        D_80127480[i].unk20 = arg0;
        *(s32 *)&D_80127480[i] = 0;
    }
    for (i = 0; i < 0x24; i++) {
        D_80128880[i].x = D_80128880[i].y = D_80128880[i].z = 0;
    }
}

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
/* NON_MATCHING: T-11300, 4 lines: before the second loop the original copies arg0 to $s4 (`move s4,s3; lw s6,4(s4); addiu s4,s4,8`), IDO folds the copy and reads 4(s3); a pointer local for it is folded too (funcdiff also shows one lui row from its HI16 pairing, not a real difference) */
void func_801471D0(s32 arg0) {
    s32 i;
    u32 n;

    arg0 += 4;
    func_8009B3C0(arg0);
    for (i = 0; i < 64; i++) {
        func_8009B430(arg0 + 8, (u8 *)&D_80127080[i], 0);
        D_80127080[i].unk4[0] = (s32)&D_80127480[i];
        D_80127080[i].unk0 = 0x80000000;
    }
    func_8009B3C0(arg0);
    n = *(u32 *)(arg0 + 4);
    for (i = 0; i < n; i++) {
        func_8009B430(arg0 + 8, (u8 *)&D_80127080[i + 1], i);
        D_80127080[i + 1].unk4[0] = (s32)&D_80127480[i + 1];
        D_80127080[i + 1].unk0 = 0x80000000;
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

void func_80147674(s32 arg0, s16 *arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 sp5C;
    s32 sp58;
    s32 sp54;
    MATRIX sp40;
    TcObj50 *sp38;
    TcPos *sp34;
    TcObj50 *temp_v1;
    TcPos *temp_v0;
    s32 temp_t1;
    u8 *temp_s0;
    u8 *temp_s0_2;

    sp40 = *(MATRIX *)D_80125C60;
    temp_s0 = arg1 + (arg2 * 0xC);
    temp_v1 = &D_80127480[arg0];
    temp_t1 = (*(s32 *)temp_s0) + temp_v1->unk18;
    sp54 = temp_t1;
    sp58 = *(s16 *)((unsigned char *)temp_s0 + (2)) + temp_v1->unk1C;
    temp_s0_2 = temp_s0 + 6;
    sp5C = *(s16 *)((unsigned char *)temp_s0 + (4)) + temp_v1->unk20;
    if ((arg3 != 0) && (arg4 != 0)) {
        sp54 = temp_t1 + ((s32) (D_8015EDB4->unk74 - temp_t1) / arg3);
        sp58 += (s32) (D_8015EDB4->unk76 - sp58) / arg4;
    }
    temp_v0 = &D_80128880[arg0];
    temp_v0->x += *(s16 *)((unsigned char *)temp_s0 + (6));
    sp38 = temp_v1;
    sp34 = temp_v0;
    func_8015D0D0((s32) (temp_v0->x << 0xC) / 360, (s32) &sp40);
    sp34->z += *(s16 *)((unsigned char *)temp_s0_2 + (2));
    func_8015CF30((s32) (sp34->z << 0xC) / 360, (s32) &sp40);
    sp34->y += *(s16 *)((unsigned char *)(temp_s0_2 + 2) + (2));
    func_8015D270((s32) (sp34->y << 0xC) / 360, (s32) &sp40);
    *(MATRIX *)sp38->unk4 = sp40;
    sp38->unk0 = 0;
}

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

void func_80147E80(s32 arg0) {
    s16 var_s2;
    s32 var_s0;
    s32 var_s1;
    s32 var_s3;
    s32 var_s4;
    s32 var_s5;
    u8 temp_v0;

    var_s3 = arg0;
    var_s1 = 0;
    var_s4 = 3;
    var_s5 = 0x880;
    do {
        var_s0 = var_s3 * 0x10;
        temp_v0 = *(u8 *)((unsigned char *)(&D_8015EDB4->unk0[var_s1]) + (3539));
        var_s0 = var_s3 * 0x11;
        if ((temp_v0 != 0) && (temp_v0 < 9U)) {
            var_s2 = func_800AE0D0() & 0xFF;
            var_s0 = (s32) &D_8015EDB4->unk0[var_s1];
            var_s2 = var_s2;
            func_8013D838((s32) *(s16 *)((unsigned char *)var_s0 + (3652)), (s32) *(s16 *)((unsigned char *)var_s0 + (3654)), (s32) *(s16 *)((unsigned char *)var_s0 + (3656)), *(u8 *)((unsigned char *)var_s0 + (3539)) - 1);
            var_s0 = (s32) &D_8015EDB4->unk0[var_s1];
            func_8013D838((s32) (s16) (*(s16 *)((unsigned char *)var_s0 + (3652)) + var_s2), (s32) *(s16 *)((unsigned char *)var_s0 + (3654)), (s32) *(s16 *)((unsigned char *)var_s0 + (3656)), *(u8 *)((unsigned char *)var_s0 + (3539)) - 2);
            var_s0 = (s32) &D_8015EDB4->unk0[var_s1];
            func_8013D838((s32) (s16) (*(s16 *)((unsigned char *)var_s0 + (3652)) - var_s2), (s32) (s16) (*(s16 *)((unsigned char *)var_s0 + (3654)) + var_s2), (s32) *(s16 *)((unsigned char *)var_s0 + (3656)), *(u8 *)((unsigned char *)var_s0 + (3539)) - 3);
            var_s0 = (s32) &D_8015EDB4->unk0[var_s1];
            func_8013D838((s32) *(s16 *)((unsigned char *)var_s0 + (3652)), (s32) (s16) (*(s16 *)((unsigned char *)var_s0 + (3654)) - var_s2), (s32) *(s16 *)((unsigned char *)var_s0 + (3656)), *(u8 *)((unsigned char *)var_s0 + (3539)) - 4);
        } else {
            var_s0 = var_s3 * 0x88;
            *(s16 *)((unsigned char *)(&D_8015EDB4->unk0[var_s1]) + (3652)) = (s16) ((D_8015EDB4[var_s3].unk74 + (func_800AE0D0() & 0x3FF)) - 0x200);
            *(s16 *)((unsigned char *)(&D_8015EDB4->unk0[var_s1]) + (3654)) = (s16) ((D_8015EDB4[var_s3].unk76 + (func_800AE0D0() & 0x3FF)) - 0x200);
            *(s16 *)((unsigned char *)(&D_8015EDB4->unk0[var_s1]) + (3656)) = (s16) ((func_800AE0D0() & 0xFFF) - 0x1770);
            *(s8 *)((unsigned char *)(&D_8015EDB4->unk0[var_s1]) + (3539)) = 0;
        }
        if ((D_800E6280.unk_10F8 & 3) == 3) {
            var_s0 = (s32) &D_8015EDB4->unk0[var_s1];
            *(u8 *)((unsigned char *)var_s0 + (3539)) = (u8) (*(u8 *)((unsigned char *)var_s0 + (3539)) + 1);
        }
        var_s1 += 0x88;
    } while (var_s1 != 0x880);
}
