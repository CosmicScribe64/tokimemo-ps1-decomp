#include "common.h"
#include "ovl/ETC.h"

typedef struct {
    s16 vx;
    s16 vy;
    s16 vz;
    s16 pad;
} EtcSVec; /* SVECTOR layout */

void func_8013F980(void) {
    s32 model;
    TaiikuBig *record;
    u32 *object;

    model = 0x801FA804;
    func_8009B3C0(model);
    model += 4;
    func_800AE0B0("NOBJ %x\n", *(u32 *)model);
    model += 4;
    /* FAKE: initializer/do line grouping reproduces setup order; original line layout unknown. T-24064. */
    object = D_80122640; record = D_801227A0; do {
        func_8009B430(model, (u8 *)object, 0);
        object[1] = (u32)record;
        record++;
        object += 4;
        object[-4] = 0x80000000;
    } while (record != D_80122CA0);
    func_8009B430(model, (u8 *)D_80122640, 0);
    D_80122640[1] = (u32)D_801227A0;
    D_80122640[0] = 0;
    D_801500C8 = 0x9F;
    D_801500CC = -0x78;
    D_801500D0 = 0;
    D_801500D4 = 0xC00;
}



void func_8013FA7C(void) {
    s32 pad1[2]; /* FAKE: two unused 8-byte locals (above rot and below scale) give the original frame 0x90; real source unknown. T-4010 */
    EtcSVec rot;
    MATRIX m1;
    MATRIX m2;
    VECTOR scale;
    s32 pad2[2];

    rot.vz = 0;
    rot.vx = 0;
    rot.vy = D_801500D4;
    if (D_801500D4 >= 0xC00) {
        rot.vy = 0xC00;
    }
    m1 = *(MATRIX *)D_80125C60;
    m2 = *(MATRIX *)D_80125C60;
    func_800A09D0(&rot, &m1);
    scale.vx = 0x1000;
    scale.vy = 0x1000;
    scale.vz = 0x1000;
    func_800A0F4C(&m2, &scale);
    func_800A0C64(&m1, &m2);
    m1.t[0] = D_801500C8;
    m1.t[1] = D_801500CC;
    m1.t[2] = D_801500D0;
    *(MATRIX *)D_801227A4 = m1;
    *(s32 *)(D_801227A4 - 4) = 0; /* FAKE: D_801227A0 reached through D_801227A4; TAIIKU declares it as TaiikuBig[], so it has no common declaration (CODING_STANDARDS 8a). T-4010 */
}

void func_8013FC08(void) {
    s32 pad; /* FAKE: unused 4-byte local puts rect at the original offset; real source unknown. T-2040 */
    RECT rect;

    rect.x = 0;
    rect.y = D_8011ECA0 * 0xF0;
    rect.w = 0x140;
    rect.h = 0xF0;
    func_8009C93C(&rect, 0x280, 0);
    func_8009C674(0);
}

void func_8013FC64(s32 a, s32 b, s32 c, s32 d) {
    SPRT *p;
    s32 v;
    s32 xoff;
    s32 n;
    s32 negative;
    s32 tpx;

    if (a < 0) {
        negative = 1;
        v = -a;
    } else {
        negative = 0;
        v = a;
    }
    n = 0;
    if (v >= 0) {
        tpx = d * 16;
        do {
            p = (SPRT *)GetWorkBase(0x14, D_8011ECA0);
            func_8009F0A4(p);
            p->w = 8;
            p->h = 8;
            SetSemiTrans(p, 0);
            p->r0 = 0x80;
            p->g0 = 0x80;
            p->b0 = 0x80;
            p->x0 = D_800E6280.unk_014[D_8011ECA0] + b - n * 8;
            p->y0 = D_800E6280.unk_018[D_8011ECA0] + c;
            p->u0 = D_801500A0[(v % 10) * 2];
            p->v0 = D_801500A0[(v % 10) * 2 + 1];
            p->clut = func_8009ED74(tpx, 0x1FD);
            AddPrim((D_8011ECA0 << 10) + 4 + D_800E8CA0, p);
            if (v < 10) {
                break;
            }
            v = v / 10;
            n++;
        } while (v >= 0);
    }
    if (negative != 0) {
        tpx = d * 16;
        n++;
        xoff = n * 8;
        p = (SPRT *)GetWorkBase(0x14, D_8011ECA0);
        func_8009F0A4(p);
        p->w = 8;
        p->h = 8;
        SetSemiTrans(p, 0);
        p->r0 = 0x80;
        p->g0 = 0x80;
        p->b0 = 0x80;
        p->x0 = D_800E6280.unk_014[D_8011ECA0] + b - xoff;
        p->y0 = D_800E6280.unk_018[D_8011ECA0] + c;
        p->u0 = D_801500B4;
        p->v0 = D_801500B5;
        p->clut = func_8009ED74(tpx, 0x1FD);
        AddPrim((D_8011ECA0 << 10) + 4 + D_800E8CA0, p);
    }
    dtd_on_tpage(0x3C0, 0, 1, 0, 1);
}
