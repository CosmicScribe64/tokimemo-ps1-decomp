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

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/801447D0", func_801449D8);

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

/* bulk_m2c declarations (T-11190) */
extern s32 D_80125C60;
s32 func_800A0070();
s32 func_800A0140();
s32 func_800A0ECC();
s32 func_8015CF30();
/* end bulk_m2c declarations */
void func_80145650(void *arg0, s32 arg1, s32 arg2) {
    s16 sp80;
    s16 sp7E;
    s16 sp7C;
    s16 sp74;
    s32 sp70;
    s32 sp6C;
    s32 sp68;
    s32 sp54[8];
    s16 temp_s2;
    s16 temp_s3;
    s32 temp_lo;
    u8 *temp_v0;

    temp_v0 = *(void **)((unsigned char *)arg0 + (8));
    temp_s2 = *(s16 *)((unsigned char *)temp_v0 + (8));
    temp_s3 = *(s16 *)((unsigned char *)temp_v0 + (10));
    sp7C = 0;
    sp7E = 0;
    sp80 = 0;
    (*(s32 *)&sp54) = (s32) (*(s32 *)&D_80125C60);
    *(s32 *)((unsigned char *)(&sp54) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80125C60) + (4));
    *(s32 *)((unsigned char *)(&sp54) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80125C60) + (8));
    *(s32 *)((unsigned char *)(&sp54) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80125C60) + (12));
    *(s32 *)((unsigned char *)(&sp54) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80125C60) + (16));
    *(s32 *)((unsigned char *)(&sp54) + (20)) = (s32) *(s32 *)((unsigned char *)(&D_80125C60) + (20));
    *(s32 *)((unsigned char *)(&sp54) + (24)) = (s32) *(s32 *)((unsigned char *)(&D_80125C60) + (24));
    temp_lo = (s32) (arg1 << 0xC) / 360;
    *(s32 *)((unsigned char *)(&sp54) + (28)) = (s32) *(s32 *)((unsigned char *)(&D_80125C60) + (28));
    sp68 = 0;
    sp6C = 0;
    sp70 = 0;
    func_8015CF30(temp_lo, &sp54);
    func_800A0ECC(&sp54, &sp7C, &sp74);
    *(s16 *)((unsigned char *)(*(void **)((unsigned char *)arg0 + (8))) + (8)) = (s16) (sp74 + temp_s2);
    *(s16 *)((unsigned char *)(*(void **)((unsigned char *)arg0 + (8))) + (10)) = (s16) (sp76 + temp_s3);
    sp7C = *(s16 *)((unsigned char *)(*(void **)((unsigned char *)arg0 + (8))) + (16)) - temp_s2;
    sp80 = 0;
    sp7E = *(s16 *)((unsigned char *)(*(void **)((unsigned char *)arg0 + (8))) + (18)) - temp_s3;
    (*(s32 *)&sp54) = (s32) (*(s32 *)&D_80125C60);
    *(s32 *)((unsigned char *)(&sp54) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80125C60) + (4));
    *(s32 *)((unsigned char *)(&sp54) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80125C60) + (8));
    *(s32 *)((unsigned char *)(&sp54) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80125C60) + (12));
    *(s32 *)((unsigned char *)(&sp54) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80125C60) + (16));
    *(s32 *)((unsigned char *)(&sp54) + (20)) = (s32) *(s32 *)((unsigned char *)(&D_80125C60) + (20));
    *(s32 *)((unsigned char *)(&sp54) + (24)) = (s32) *(s32 *)((unsigned char *)(&D_80125C60) + (24));
    *(s32 *)((unsigned char *)(&sp54) + (28)) = (s32) *(s32 *)((unsigned char *)(&D_80125C60) + (28));
    sp6C = 0;
    sp70 = 0;
    sp68 = (s32) sp7C;
    func_8015CF30(temp_lo, &sp54);
    func_800A0ECC(&sp54, &sp7C, &sp74);
    *(s16 *)((unsigned char *)(*(void **)((unsigned char *)arg0 + (8))) + (16)) = (s16) (sp74 + temp_s2);
    *(s16 *)((unsigned char *)(*(void **)((unsigned char *)arg0 + (8))) + (18)) = (s16) (sp76 + temp_s3);
    sp7C = *(s16 *)((unsigned char *)(*(void **)((unsigned char *)arg0 + (8))) + (24)) - temp_s2;
    sp80 = 0;
    sp7E = *(s16 *)((unsigned char *)(*(void **)((unsigned char *)arg0 + (8))) + (26)) - temp_s3;
    (*(s32 *)&sp54) = (s32) (*(s32 *)&D_80125C60);
    *(s32 *)((unsigned char *)(&sp54) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80125C60) + (4));
    *(s32 *)((unsigned char *)(&sp54) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80125C60) + (8));
    *(s32 *)((unsigned char *)(&sp54) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80125C60) + (12));
    *(s32 *)((unsigned char *)(&sp54) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80125C60) + (16));
    *(s32 *)((unsigned char *)(&sp54) + (20)) = (s32) *(s32 *)((unsigned char *)(&D_80125C60) + (20));
    *(s32 *)((unsigned char *)(&sp54) + (24)) = (s32) *(s32 *)((unsigned char *)(&D_80125C60) + (24));
    *(s32 *)((unsigned char *)(&sp54) + (28)) = (s32) *(s32 *)((unsigned char *)(&D_80125C60) + (28));
    sp6C = 0;
    sp70 = 0;
    sp68 = (s32) sp7C;
    func_8015CF30(temp_lo, &sp54);
    func_800A0ECC(&sp54, &sp7C, &sp74);
    *(s16 *)((unsigned char *)(*(void **)((unsigned char *)arg0 + (8))) + (24)) = (s16) (sp74 + temp_s2);
    *(s16 *)((unsigned char *)(*(void **)((unsigned char *)arg0 + (8))) + (26)) = (s16) (sp76 + temp_s3);
    sp7C = *(s16 *)((unsigned char *)(*(void **)((unsigned char *)arg0 + (8))) + (32)) - temp_s2;
    sp80 = 0;
    sp7E = *(s16 *)((unsigned char *)(*(void **)((unsigned char *)arg0 + (8))) + (34)) - temp_s3;
    (*(s32 *)&sp54) = (s32) (*(s32 *)&D_80125C60);
    *(s32 *)((unsigned char *)(&sp54) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80125C60) + (4));
    *(s32 *)((unsigned char *)(&sp54) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80125C60) + (8));
    *(s32 *)((unsigned char *)(&sp54) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80125C60) + (12));
    *(s32 *)((unsigned char *)(&sp54) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80125C60) + (16));
    *(s32 *)((unsigned char *)(&sp54) + (20)) = (s32) *(s32 *)((unsigned char *)(&D_80125C60) + (20));
    *(s32 *)((unsigned char *)(&sp54) + (24)) = (s32) *(s32 *)((unsigned char *)(&D_80125C60) + (24));
    *(s32 *)((unsigned char *)(&sp54) + (28)) = (s32) *(s32 *)((unsigned char *)(&D_80125C60) + (28));
    sp6C = 0;
    sp70 = 0;
    sp68 = (s32) sp7C;
    func_8015CF30(temp_lo, &sp54);
    func_800A0ECC(&sp54, &sp7C, &sp74);
    *(s16 *)((unsigned char *)(*(void **)((unsigned char *)arg0 + (8))) + (32)) = (s16) (sp74 + temp_s2);
    *(s16 *)((unsigned char *)(*(void **)((unsigned char *)arg0 + (8))) + (34)) = (s16) (sp76 + temp_s3);
    *(s16 *)((unsigned char *)(*(s32 *)arg0) + (8)) = (s16) (((s32) ((s32) (func_800A0070(temp_lo) * arg2) / 3) / 4096) + *(s16 *)((unsigned char *)(*(void **)((unsigned char *)arg0 + (8))) + (8)));
    *(s16 *)((unsigned char *)(*(s32 *)arg0) + (10)) = (s16) (*(s16 *)((unsigned char *)(*(void **)((unsigned char *)arg0 + (8))) + (10)) - ((s32) ((s32) (func_800A0140(temp_lo) * arg2) / 3) / 4096));
    *(s16 *)((unsigned char *)(*(s32 *)arg0) + (16)) = (s16) (((s32) ((s32) (func_800A0070(temp_lo) * arg2) / 3) / 4096) + *(s16 *)((unsigned char *)(*(void **)((unsigned char *)arg0 + (8))) + (16)));
    *(s16 *)((unsigned char *)(*(s32 *)arg0) + (18)) = (s16) (*(s16 *)((unsigned char *)(*(void **)((unsigned char *)arg0 + (8))) + (18)) - ((s32) ((s32) (func_800A0140(temp_lo) * arg2) / 3) / 4096));
    *(s16 *)((unsigned char *)(*(s32 *)arg0) + (24)) = (s16) *(s16 *)((unsigned char *)(*(void **)((unsigned char *)arg0 + (8))) + (8));
    *(s16 *)((unsigned char *)(*(s32 *)arg0) + (26)) = (s16) *(s16 *)((unsigned char *)(*(void **)((unsigned char *)arg0 + (8))) + (10));
    *(s16 *)((unsigned char *)(*(s32 *)arg0) + (32)) = (s16) *(s16 *)((unsigned char *)(*(void **)((unsigned char *)arg0 + (8))) + (16));
    *(s16 *)((unsigned char *)(*(s32 *)arg0) + (34)) = (s16) *(s16 *)((unsigned char *)(*(void **)((unsigned char *)arg0 + (8))) + (18));
    *(s16 *)((unsigned char *)(*(void **)((unsigned char *)arg0 + (4))) + (8)) = (s16) *(s16 *)((unsigned char *)(*(void **)((unsigned char *)arg0 + (8))) + (24));
    *(s16 *)((unsigned char *)(*(void **)((unsigned char *)arg0 + (4))) + (10)) = (s16) *(s16 *)((unsigned char *)(*(void **)((unsigned char *)arg0 + (8))) + (26));
    *(s16 *)((unsigned char *)(*(void **)((unsigned char *)arg0 + (4))) + (16)) = (s16) *(s16 *)((unsigned char *)(*(void **)((unsigned char *)arg0 + (8))) + (32));
    *(s16 *)((unsigned char *)(*(void **)((unsigned char *)arg0 + (4))) + (18)) = (s16) *(s16 *)((unsigned char *)(*(void **)((unsigned char *)arg0 + (8))) + (34));
    *(s16 *)((unsigned char *)(*(void **)((unsigned char *)arg0 + (4))) + (24)) = (s16) (*(s16 *)((unsigned char *)(*(void **)((unsigned char *)arg0 + (8))) + (24)) - ((s32) ((s32) (func_800A0070(temp_lo) * arg2) / 3) / 4096));
    *(s16 *)((unsigned char *)(*(void **)((unsigned char *)arg0 + (4))) + (26)) = (s16) (((s32) ((s32) (func_800A0140(temp_lo) * arg2) / 3) / 4096) + *(s16 *)((unsigned char *)(*(void **)((unsigned char *)arg0 + (8))) + (26)));
    *(s16 *)((unsigned char *)(*(void **)((unsigned char *)arg0 + (4))) + (32)) = (s16) (*(s16 *)((unsigned char *)(*(void **)((unsigned char *)arg0 + (8))) + (32)) - ((s32) ((s32) (func_800A0070(temp_lo) * arg2) / 3) / 4096));
    *(s16 *)((unsigned char *)(*(void **)((unsigned char *)arg0 + (4))) + (34)) = (s16) (((s32) ((s32) (func_800A0140(temp_lo) * arg2) / 3) / 4096) + *(s16 *)((unsigned char *)(*(void **)((unsigned char *)arg0 + (8))) + (34)));
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/801447D0", func_80145DC4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/801447D0", func_8014616C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/801447D0", func_80146754);
