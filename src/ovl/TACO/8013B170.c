#include "common.h"
#include "ovl/TACO.h"

void func_8013B170(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8013B2A0();
        break;
    case 1:
        func_8013B398();
        break;
    case 2:
        func_8013B3C4();
        break;
    }
    if (D_800E6280.unk_110A != 0) {
        if (D_800E6280.unk_10F8 & 0x10) {
            if ((u8) D_8015EDCC >= 3U) {
                func_801432F0((s32) "MISSION COMPLETE", -0x40, 0, 2);
            } else if (D_8015EDEC == 0) {
                func_801432F0((s32) "TIME OVER", -0x40, 0, 1);
            } else {
                func_801432F0((s32) "GAME OVER", -0x40, 0, 1);
            }
        }
        func_8013B460(0x64);
    } else {
        func_8013F250();
    }
    func_80143B58();
}

void func_8013B2A0(void) {
    s32 pad; /* FAKE: unused 4-byte local gives frame 0x40 and puts flag at sp+0x38; real source unknown. T-19030 */
    s32 flag;
    TcPos save;

    if (D_8015EDBC == 0) {
        func_8013B404();
        func_801347A8();
        func_80059048();
        save = *(TcPos *)&D_8015EDB4->unk7C;
        flag = D_8015EDB4->unk84[1];
        func_80143AF4();
        *(TcPos *)&D_8015EDB4->unk7C = save;
        D_8015EDB4->unk84[1] = flag;
        /* FAKE: element chain through the first symbol shares one li between the last two stores and
         * keeps a separate lui per store (struct-field chains and separate statements load three
         * constants); GameState fields unk_036..unk_038. T-19030 */
        (&D_800E6280.unk_036)[0] = (&D_800E6280.unk_036)[1] = (&D_800E6280.unk_036)[2] = 0x10;
        D_8015EDB0 = 0;
        D_8015EDBC = 0x44;
        func_800450F4(0, 0x202);
        func_8004284C();
    }
}

void func_8013B398(void) {
    if (D_800E6280.unk_F88 != 0) {
        func_8004284C();
    }
}

void func_8013B3C4(void) {
    if (D_8015EDBC == 0) {
        if (func_80044C98() == 1) {
            func_80042908(8);
        }
    }
}

void func_8013B404(void) {
    s32 pad; /* FAKE: unused 4-byte local puts rect at the original offset; real source unknown. T-2040 */
    RECT rect;

    rect.x = 0;
    rect.y = D_8011ECA0 * 0xF0;
    rect.w = 0x200;
    rect.h = 0xF0;
    func_8009C93C(&rect, 0x200, 0);
    func_8009C674(0);
}

/* NON_MATCHING: T-24200, 4 differing lines (2 rows, one per quad): the p->y3 halfword store is scheduled before the eight
 * UV byte stores; the original computes y3 first but stores it after them (as1 order; a y3 temporary gets
 * that order but recolours the 0xFF/0xF0 constants). */
#ifdef NON_MATCHING
s32 func_8013B460(s32 arg0) {
    GameState *g = &D_800E6280;
    s32 pad; /* FAKE: unused word gives the frame home slot of the original; real source unknown. T-19030 */
    POLY_FT4 *p;

    p = (POLY_FT4 *)GetWorkBase(0x28, D_8011ECA0);
    func_8009F040(p);
    SetSemiTrans(p, 0);
    SetShadeTex(p, 0);
    p->tpage = func_8009ECB0(2, 0, 0x200, 0);
    p->r0 = 0x80;
    p->g0 = 0x80;
    p->b0 = 0x80;
    p->x0 = g->unk_014[D_8011ECA0] + (((0x64 - arg0) << 9) / 100 - ((0x64 - arg0) * 0x10) / 100) - 0x100;
    p->y0 = g->unk_018[D_8011ECA0] + ((0x64 - arg0) << 6) / 100 - 0x78;
    p->x1 = g->unk_014[D_8011ECA0] + (((0x64 - arg0) << 9) / 100 - ((0x64 - arg0) * 0x10) / 100) + (arg0 << 8) / 100 - 0x100;
    p->y1 = g->unk_018[D_8011ECA0] + ((0x64 - arg0) << 6) / 100 - 0x78;
    p->x2 = g->unk_014[D_8011ECA0] + (((0x64 - arg0) << 9) / 100 - ((0x64 - arg0) * 0x10) / 100) - 0x100;
    p->y2 = g->unk_018[D_8011ECA0] + ((0x64 - arg0) << 6) / 100 + (arg0 * 0xF0) / 100 - 0x78;
    p->x3 = g->unk_014[D_8011ECA0] + (((0x64 - arg0) << 9) / 100 - ((0x64 - arg0) * 0x10) / 100) + (arg0 << 8) / 100 - 0x100;
    p->y3 = g->unk_018[D_8011ECA0] + ((0x64 - arg0) << 6) / 100 + (arg0 * 0xF0) / 100 - 0x78;
    /* one comma statement, the shape of the libgpu setUV4 macro: gives the original's store order and constant registers. T-24200 */
    p->u0 = 0, p->v0 = 0, p->u1 = 0xFF, p->v1 = 0, p->u2 = 0, p->v2 = 0xF0, p->u3 = 0xFF, p->v3 = 0xF0;
    AddPrim((D_8011ECA0 << 0xA) + 0x3FC + D_800E8CA0, p);
    p = (POLY_FT4 *)GetWorkBase(0x28, D_8011ECA0);
    func_8009F040(p);
    SetSemiTrans(p, 0);
    SetShadeTex(p, 0);
    p->tpage = func_8009ECB0(2, 0, 0x300, 0);
    p->r0 = 0x80;
    p->g0 = 0x80;
    p->b0 = 0x80;
    p->x0 = g->unk_014[D_8011ECA0] + (((0x64 - arg0) << 8) / 100 - ((0x64 - arg0) * 0x10) / 100);
    p->y0 = g->unk_018[D_8011ECA0] + ((0x64 - arg0) << 6) / 100 - 0x78;
    p->x1 = g->unk_014[D_8011ECA0] + (((0x64 - arg0) << 8) / 100 - ((0x64 - arg0) * 0x10) / 100) + (arg0 << 8) / 100;
    p->y1 = g->unk_018[D_8011ECA0] + ((0x64 - arg0) << 6) / 100 - 0x78;
    p->x2 = g->unk_014[D_8011ECA0] + (((0x64 - arg0) << 8) / 100 - ((0x64 - arg0) * 0x10) / 100);
    p->y2 = g->unk_018[D_8011ECA0] + ((0x64 - arg0) << 6) / 100 + (arg0 * 0xF0) / 100 - 0x78;
    p->x3 = g->unk_014[D_8011ECA0] + (((0x64 - arg0) << 8) / 100 - ((0x64 - arg0) * 0x10) / 100) + (arg0 << 8) / 100;
    p->y3 = g->unk_018[D_8011ECA0] + ((0x64 - arg0) << 6) / 100 + (arg0 * 0xF0) / 100 - 0x78;
    p->u0 = 0, p->v0 = 0, p->u1 = 0xFF, p->v1 = 0, p->u2 = 0, p->v2 = 0xF0, p->u3 = 0xFF, p->v3 = 0xF0;
    AddPrim((D_8011ECA0 << 0xA) + 0x3FC + D_800E8CA0, p);
}
#else
INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013B170", func_8013B460);
#endif
