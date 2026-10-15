#include "common.h"
#include "ovl/TACO.h"
/* .data of this object (T-9010, tools/data_island.py): one line per variable in
 * address order; replace a line by the variable's C definition. */
TacoCam D_8015F5F0 = { 0, 0x400, 0, 0 };
INCLUDE_RODATA("asm/ovl/TACO/data/TACO/80143E80.data", D_8015F5F8);
INCLUDE_RODATA("asm/ovl/TACO/data/TACO/80143E80.data", D_8015F5FC);
INCLUDE_RODATA("asm/ovl/TACO/data/TACO/80143E80.data", D_8015F604);
INCLUDE_RODATA("asm/ovl/TACO/data/TACO/80143E80.data", D_8015F82C);

void func_80143E80(void) {
    D_8015F5F0.a = 0;
    D_8015F5F0.b = 0x400;
    D_8015F5F0.c = 0;
    D_8015F5F0.d = 0;
}

void func_80143E9C(void) {
    if (D_8015EDB0 != 0) {
        func_80143FF4();
        func_80143F40();
        func_80143B58();
        func_80133694();
    }
    if (D_8015EDB0 & 1) {
        func_80144094();
    }
    if (D_8015EDB0 & 2) {
        func_80144430();
    }
    if (D_8015EDB0 & 4) {
        func_801446D0();
    }
}

s32 func_80143F40(void) {
    D_8015F5F0.c += 0x80;
    if (D_8015F5F0.c >= 0x1001) {
        D_8015F5F0.c -= 0x1000;
        D_8015F5F0.d += 1;
    }
    if (D_8015F5F0.c < 0) {
        D_8015F5F0.c += 0x1000;
        D_8015F5F0.d -= 1;
    }
    if (D_8015F5F0.a >= 0x801) {
        D_8015F5F0.a -= 0x800;
    }
    if (D_8015F5F0.a < 0) {
        D_8015F5F0.a += 0x800;
    }
}

void func_80143FF4(void) {
    D_80122740 = D_8015EDB4->unk74;
    D_80122744 = D_8015EDB4->unk76;
    D_80122748 = D_8015EDB4->unk78;
    D_8012274C = D_8015EDB4->unk74;
    D_80122750 = D_8015EDB4->unk76;
    D_80122754 = 0;
    D_80122758 = D_8015EDB4->unk68 * 0x168;
    D_8012275C = 0;
    func_80099540(&D_80122740);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80143E80", func_80144094);

/* bulk_m2c declarations (T-11190) */
extern s16 D_8015F5F2;
extern s32 D_80129C40;
s32 func_800A1070();
/* end bulk_m2c declarations */
void func_80144430(void) {
    s32 spB0[8];
    s16 spAC;
    s16 spAA;
    s16 spA8;
    s32 sp80[1];
    s32 sp6C[1];
    s32 sp68[1];
    s32 sp60;
    s32 *var_s2;
    s32 temp_v0;
    s32 var_fp;
    s32 var_fp_2;
    s32 var_s0;
    s32 var_s3;
    s8 temp_a0;
    s8 temp_t2;
    u8 *var_s1;
    u8 *var_s1_2;

    (*(s32 *)&spB0) = (s32) D_80129C40;
    *(s32 *)((unsigned char *)(&spB0) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (4));
    *(s32 *)((unsigned char *)(&spB0) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (8));
    *(s32 *)((unsigned char *)(&spB0) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (12));
    *(s32 *)((unsigned char *)(&spB0) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (16));
    *(s32 *)((unsigned char *)(&spB0) + (20)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (20));
    *(s32 *)((unsigned char *)(&spB0) + (24)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (24));
    *(s32 *)((unsigned char *)(&spB0) + (28)) = (s32) *(s32 *)((unsigned char *)(&D_80129C40) + (28));
    func_800A1100((s32) &spB0);
    func_800A1070(&spB0);
    spAC = -0x7FFF;
    var_fp = 0;
    sp60 = 0;
    do {
        var_s2 = &sp80;
        spAA = D_8015F5F2 + (sp60 / 4) + 0x80;
        var_s1 = var_fp + var_s2;
        var_s0 = 0x4000;
loop_2:
        spA8 = var_s0 + D_8012274C;
        func_800AB170((s32) &spA8, (s32) var_s1, (s32) &sp6C, (s32) &sp68);
        var_s2 += 8;
        var_s0 -= 0x2000;
        var_s1 += 8;
        if (var_s2 != &spA8) {
            goto loop_2;
        }
        var_fp += 4;
        sp60 -= 0x1000;
    } while (var_fp < 8);
    var_fp_2 = 0;
    do {
        var_s1_2 = var_fp_2 + &sp80;
        var_s3 = 0;
loop_6:
        temp_v0 = GetWorkBase(0x28, D_8011ECA0);
        func_8009F040(temp_v0);
        *(s8 *)((unsigned char *)temp_v0 + (4)) = 0x80;
        *(s8 *)((unsigned char *)temp_v0 + (5)) = 0x80;
        *(s8 *)((unsigned char *)temp_v0 + (6)) = 0x80;
        *(s16 *)((unsigned char *)temp_v0 + (22)) = func_8009ECB0(1, 0, 0x200, 0x100);
        temp_t2 = (var_s3 & 1) << 6;
        temp_a0 = temp_t2 + 0x3F;
        *(s16 *)((unsigned char *)temp_v0 + (14)) = func_8009ED74(0, 0x1EE);
        *(s8 *)((unsigned char *)temp_v0 + (13)) = temp_a0;
        *(s8 *)((unsigned char *)temp_v0 + (29)) = temp_a0;
        *(s8 *)((unsigned char *)temp_v0 + (21)) = temp_t2;
        *(s8 *)((unsigned char *)temp_v0 + (37)) = temp_t2;
        *(s8 *)((unsigned char *)temp_v0 + (12)) = 0;
        *(s8 *)((unsigned char *)temp_v0 + (20)) = 0;
        *(s8 *)((unsigned char *)temp_v0 + (28)) = 0xFF;
        *(s8 *)((unsigned char *)temp_v0 + (36)) = 0xFF;
        *(s16 *)((unsigned char *)temp_v0 + (8)) = (s16) (*(s32 *)var_s1_2);
        *(s16 *)((unsigned char *)temp_v0 + (10)) = (s16) *(s16 *)((unsigned char *)var_s1_2 + (2));
        *(s16 *)((unsigned char *)temp_v0 + (16)) = (s16) *(s16 *)((unsigned char *)var_s1_2 + (4));
        *(s16 *)((unsigned char *)temp_v0 + (18)) = (s16) *(s16 *)((unsigned char *)var_s1_2 + (6));
        *(s16 *)((unsigned char *)temp_v0 + (24)) = (s16) *(s16 *)((unsigned char *)var_s1_2 + (8));
        *(s16 *)((unsigned char *)temp_v0 + (26)) = (s16) *(s16 *)((unsigned char *)var_s1_2 + (10));
        *(s16 *)((unsigned char *)temp_v0 + (32)) = (s16) *(s16 *)((unsigned char *)var_s1_2 + (12));
        *(s16 *)((unsigned char *)temp_v0 + (34)) = (s16) *(s16 *)((unsigned char *)var_s1_2 + (14));
        AddPrim(D_800E8CA0 + (D_8011ECA0 << 0xA) + 0x3F4, temp_v0);
        var_s3 += 1;
        var_s1_2 += 8;
        if (var_s3 != 4) {
            goto loop_6;
        }
        var_fp_2 += 4;
    } while (var_fp_2 != 4);
}

void func_801446D0(void) {
}

void func_801446D8(void) {
    /* FAKE: two spare workspace words retain the observed frame size;
     * keeping the pointer init on the do line permits the original schedule.
     * T-9510: the transform helpers themselves use 32-byte workspaces. */
    u32 sp70[10];
    u32 sp50[8];
    Tc10 *var_s0;

    if (D_800E6280.unk_1113 != 0) {
        func_80099540(&D_80122740);
        var_s0 = D_80127080; do {
            func_80099B10(var_s0->unk4[0], &sp50, &sp70);
            func_80099F04(&sp50);
            func_8009A200(var_s0->unk4[0], &sp70);
            func_80099ED8(&sp70);
            func_8009A4D0((s32) var_s0, (s32) ((u8 *)&D_800E8C70 + (D_8011ECA0 * 0x14)), 6, 0x1F800000);
            var_s0++;
        } while (var_s0 != (Tc10 *)D_80127480);
    }
}
