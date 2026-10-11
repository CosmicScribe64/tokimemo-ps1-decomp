#include "common.h"
#include "ovl/TACO.h"

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/801447D0", func_801447D0);

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

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/801447D0", func_80145650);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/801447D0", func_80145DC4);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/801447D0", func_8014616C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/801447D0", func_80146754);
