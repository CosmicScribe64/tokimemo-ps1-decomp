#include "common.h"
#include "ovl/TACO.h"

void func_801447D0(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8014488C();
        break;
    case 1:
        func_80145DC4();
        break;
    case 2:
        func_8014616C();
        break;
    case 3:
        func_80146754();
        break;
    case 4:
        func_80144998();
        break;
    }
    if (D_800E6280.unk_110A != 5) {
        if (D_800E6280.unk_F88 & 0x860) {
            func_80042940(4);
        }
    }
}

void func_8014488C(void) {
    (&D_800E6280.unk_036)[0] = (&D_800E6280.unk_036)[1] = 8;
    D_800E6280.unk_038 = 0x10;
    draw2d3d(1, 1);
    func_80058D20();
    func_80146E3C();
    func_80146D60(-0xFA0);
    func_801471D0(0x8019C800);
    func_80144CC0((u8 *)D_800E6280.unk_163C);
    func_80144CC0(D_800E6280.unk_183C);
    func_8009C674(0);
    func_80144D90(7);
    func_80147488(1, D_8015F82C, 0);
    D_80127090.unk0 = 0;
    D_80128880[1].x = -0x5A;
    D_80128880[1].z = 0;
    D_80128880[1].y = 0x5A;
    func_80147488(2, D_8015F830, 0);
    D_801270A0.unk0 = 0;
    D_80128880[2].x = -0x5A;
    D_80128880[2].z = 0;
    D_80128880[2].y = -0x5A;
    func_8004284C();
}

void func_80144998(void) {
    D_80127090.unk0 = 0x80000000;
    D_801270A0.unk0 = 0x80000000;
    draw2d3d(1, 0);
    func_80042908(2);
}

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E6298;
/* end bulk_m2c declarations */
void func_801449D8(void *arg0, s32 arg1, s32 arg2) {
    void *sp7C;
    s16 temp_s3;
    s16 temp_v0_2;
    s16 temp_v1;
    s32 temp_t7;
    s32 temp_t9;
    s32 var_s4;
    void **var_s0;
    void *temp_s0;
    void *temp_v0;
    u8 *var_s1;

    var_s0 = &sp7C;
    var_s1 = arg0;
    var_s4 = 0;
    do {
        temp_v0 = GetWorkBase(0x18, D_8011ECA0);
        *var_s0 = temp_v0;
        func_8009F02C((s32) temp_v0);
        SetSemiTrans(*var_s0, 1);
        SetShadeTex(*var_s0, 0);
        temp_t7 = (*(s32 *)var_s1) + arg1;
        temp_t9 = *(s32 *)((unsigned char *)var_s1 + (4)) + arg2;
        (*(s32 *)var_s1) = temp_t7;
        *(s32 *)((unsigned char *)var_s1 + (4)) = temp_t9;
        temp_s3 = *(&D_800E6298 + (D_8011ECA0 * 2)) + temp_t9;
        if ((temp_t7 < 0) || (temp_t7 >= 0x141)) {
            if (arg1 > 0) {
                (*(s32 *)var_s1) = 0;
            } else {
                (*(s32 *)var_s1) = 0x140;
            }
            *(s32 *)((unsigned char *)var_s1 + (4)) = (s32) ((func_800AE0D0() % 240) - 0x78);
        } else if ((temp_s3 < 0) || (temp_s3 >= 0x1E1)) {
            (*(s32 *)var_s1) = (s32) (func_800AE0D0() % 320);
            if (arg2 > 0) {
                *(s32 *)((unsigned char *)var_s1 + (4)) = -0x78;
            } else {
                *(s32 *)((unsigned char *)var_s1 + (4)) = 0x78;
            }
        }
        temp_v0_2 = temp_t7 + 1;
        *(s16 *)((unsigned char *)(*var_s0) + (8)) = (s16) temp_t7;
        temp_v1 = temp_s3 + 1;
        *(s16 *)((unsigned char *)(*var_s0) + (10)) = temp_s3;
        *(s16 *)((unsigned char *)(*var_s0) + (12)) = temp_v0_2;
        *(s16 *)((unsigned char *)(*var_s0) + (14)) = temp_s3;
        *(s16 *)((unsigned char *)(*var_s0) + (16)) = (s16) temp_t7;
        *(s16 *)((unsigned char *)(*var_s0) + (18)) = temp_v1;
        *(s16 *)((unsigned char *)(*var_s0) + (20)) = temp_v0_2;
        *(s16 *)((unsigned char *)(*var_s0) + (22)) = temp_v1;
        if ((func_800AE0D0() % 5) >= 3) {
            *(s8 *)((unsigned char *)(*var_s0) + (4)) = 0x80;
            *(s8 *)((unsigned char *)(*var_s0) + (5)) = 0x80;
            *(s8 *)((unsigned char *)(*var_s0) + (6)) = 0x80;
        } else {
            *(s8 *)((unsigned char *)(*var_s0) + (4)) = 0xFF;
            *(s8 *)((unsigned char *)(*var_s0) + (5)) = 0xFF;
            *(s8 *)((unsigned char *)(*var_s0) + (6)) = 0xFF;
        }
        if ((func_800AE0D0() % 3) > 0) {
            AddPrim((D_8011ECA0 << 0xA) + 0x190 + D_800E8CA0, *var_s0);
        }
        var_s4 += 8;
        var_s0 += 1;
        var_s1 += 8;
    } while (var_s4 != 0xC8);
    temp_s0 = GetWorkBase(0xC, D_8011ECA0);
    func_8009D294((s32) temp_s0, 0, 0, func_8009ECB0(1, 1, 0, 0), 0);
    AddPrim((D_8011ECA0 << 0xA) + D_800E8CA0, temp_s0);
    safe_env(0);
}

void func_80144CC0(u8 *arg0) {
    s32 i;

    for (i = 0; i != 0x19; i++) {
        *(s32 *)(arg0 + i * 8) = func_800AE0D0() % 320;
        *(s32 *)(arg0 + i * 8 + 4) = func_800AE0D0() % 240 - 0x78;
    }
}

void func_80144D90(s32 arg0) {
    RECT r;

    if (arg0 < 0) {
        arg0 = 0;
    } else if (arg0 >= 8) {
        arg0 = 7;
    }
    /* FAKE: the assignment pairs share a source line each; as1 orders the stores by line (T-3330), T-6050 */
    r.x = arg0 * 8; r.y = 0x1F1;
    r.w = 8; r.h = 1;
    func_8009C93C(&r, 0xF8, 0x1F0);
}

void func_80144DF8(s32 arg0) {
    TcObj50 *o;
    TcPos *p;

    o = &D_80127480[arg0];
    p = &D_80128880[arg0];
    o->unk18 = 0;
    o->unk1C = 0;
    o->unk20 = 0;
    p->x = 0;
    p->y = 0;
    p->z = 0;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/801447D0", func_80144E3C);

void func_8014559C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    if ((arg4 >= 0) && (arg4 < 0x1F)) {
        func_80144E3C(arg0, arg1, arg2, arg3, (arg4 & 0xFF) * 8, 0, arg5);
        return;
    }
    if ((arg4 >= 0x1F) && (arg4 < 0x23)) {
        func_80144E3C(arg0, arg1, arg2, arg3, 0, 2, arg5);
        return;
    }
    if ((arg4 >= 0x23) && (arg4 < 0x61)) {
        func_80144E3C(arg0, arg1, arg2, arg3, ((arg4 - 0x23) & 0xFF) * 4, 1, arg5);
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/801447D0", func_80145650);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/801447D0", func_80145DC4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/801447D0", func_8014616C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/801447D0", func_80146754);
