#include "common.h"
#include "ovl/TACO.h"

/* FAKE: integer fallthrough retains the original index/pointer allocation. T-9550 */
s32 func_8013D1E0(TcPos position, TcPos velocity) {
    TacoParticle14 *particle;
    s32 index;
    index = D_8015E9F0[0].unk_00.unk6 % 16;
    particle = &D_8015E9F0[index];
    particle->unk_00 = position;
    particle->unk_08 = velocity;
    particle->unk_12 = 1;
    particle->unk_11 = 1;
    particle->unk_13 = 0x80;
    if (D_8015E9F0[0].unk_00.unk6++ >= 17) D_8015E9F0[0].unk_00.unk6 = 0;

}

/* FAKE: integer fallthrough retains original index/pointer allocation. T-9550 */
s32 func_8013D2A0(s16 x, s16 y, s16 z) {
    TacoParticle14 *particle;
    s32 index;
    index = D_8015E8B0[0].unk_00.unk6 % 16;
    particle = &D_8015E8B0[index];
    particle->unk_00.x = x;
    particle->unk_00.y = y;
    particle->unk_00.z = z;
    particle->unk_12 = 1;
    particle->unk_11 = 1;
    particle->unk_13 = 0x80;
    if (D_8015E8B0[0].unk_00.unk6++ >= 17) D_8015E8B0[0].unk_00.unk6 = 0;

}

/* FAKE: integer fall-through preserves the original loop/result registers. T-10010. */
s32 func_8013D348(void) {
    TacoParticle14 *particle;
    u32 i;

    for (i = 0; i < 16; i++) {
        particle = &D_8015E8B0[i];
        if (particle->unk_13 != 0) {
            func_8013D528((s32)particle->unk_00.x, (s32)particle->unk_00.y, (s32)particle->unk_00.z, particle->unk_11 - 1);
            if (particle->unk_12++ >= 5U) {
                particle->unk_12 = 0;
                if (particle->unk_11++ >= 7U) {
                    particle->unk_13 = 0;
                    particle->unk_11 = 0;
                }
            }
        }
    }
}

void func_8013D3F4(void) {
    TacoParticle14 *particle;
    s16 offset_x;
    s16 offset_y;
    u32 i;

    offset_x = func_800AE0D0() & 0x7F;
    offset_y = func_800AE0D0() & 0x3F;

    for (i = 0; i < 16; i++) {
        particle = &D_8015E9F0[i];
        if (particle->unk_13 != 0) {
            func_8013D838((s32) particle->unk_00.x, (s32) particle->unk_00.y, (s32) particle->unk_00.z, particle->unk_11 - 1);
            func_8013D838((s32) (s16) (particle->unk_00.x + offset_x), (s32) (s16) (particle->unk_00.y + offset_y), (s32) particle->unk_00.z, particle->unk_11 + 2);
            particle->unk_00.x += particle->unk_08.x;
            particle->unk_00.y += particle->unk_08.y;
            particle->unk_00.z += particle->unk_08.z;
            if (particle->unk_12++ >= 5U) {
                particle->unk_12 = 0;
                if (particle->unk_11++ >= 7U) {
                    particle->unk_13 = 0;
                    particle->unk_11 = 0;
                }
            }
        }
    }
}

/* s32 without a return: the callers above see an implicit int declaration. T-19018 */
s32 func_8013D528(s16 arg0, s16 arg1, s16 arg2, s32 arg3) {
    s32 pad[2]; /* FAKE: eight unused bytes above m reproduce the original frame (192), as in func_80140ADC; T-19018 */
    MATRIX m;
    SVECTOR v;
    s32 pt[4];
    POLY_FT4 *poly;
    s32 dist;
    s32 depth;
    s32 flag;
    s32 o;
    s32 j;
    s32 rot;

    m = D_80129C40;
    func_800A1100(&m);
    func_800A1070(&m);
    v.vz = arg2;
    rot = 0;
    for (o = 0; o < 2; o++) {
        v.vy = arg1 + rot - 0x80;
        for (j = 0; j < 2; j++) {
            v.vx = arg0 + j * 0x100 - 0x80;
            func_800AB170(&v, &pt[j * 2 + o], &depth, &flag);
        }
        rot += 0x100;
    }
    if (((s16 *)pt)[4] >= -0x2C0 && ((s16 *)pt)[0] < 0x2C1) {
        if (((s16 *)pt)[3] >= D_800E6280.unk_018[D_8011ECA0] - 0x200
            && D_800E6280.unk_018[D_8011ECA0] + 0x200 >= ((s16 *)pt)[7]) {
            dist = (D_80122754 - arg2) / 127;
            if (dist >= 0x100) {
                dist = 0xFF;
            } else if (dist <= 0) {
                dist = 1;
            }
            if (dist > 0x20) {
                dist = dist / 2;
            } else if (dist >= 6) {
                dist -= 4;
            }
            poly = func_800490F0(0x28, D_8011ECA0);
            func_8009F040(poly);
            SetSemiTrans(poly, 1);
            poly->r0 = 0x80;
            poly->g0 = 0x80;
            poly->b0 = 0x80;
            poly->tpage = func_8009ECB0(0, 1, 0x380, 0x100);
            poly->clut = func_8009ED74(arg3 * 0x10, 0x1E4);
            /* FAKE: hand expansion of the PsyQ macro setUVWH(poly, arg3 << 5, 0, 0x1F, 0x1F) on one line, as in
             * func_80140ADC: as1 schedules the eight stores of one line together; chained stores add a frame temp. T-19018 */
            poly->u0 = arg3 << 5, poly->v0 = 0, poly->u1 = (arg3 << 5) + 0x1F, poly->v1 = 0, poly->u2 = arg3 << 5, poly->v2 = 0x1F, poly->u3 = (arg3 << 5) + 0x1F, poly->v3 = 0x1F;
            poly->x0 = ((s16 *)pt)[0];
            poly->y0 = ((s16 *)pt)[1];
            poly->x1 = ((s16 *)pt)[2];
            poly->y1 = ((s16 *)pt)[3];
            poly->x2 = ((s16 *)pt)[4];
            poly->y2 = ((s16 *)pt)[5];
            poly->x3 = ((s16 *)pt)[6];
            poly->y3 = ((s16 *)pt)[7];
            func_8009EED0((D_8011ECA0 << 10) + dist * 4 + D_800E8CA0, poly);
        }
    }
}

#ifdef NON_MATCHING
/* NON_MATCHING: T-19018, 10 differing lines; the first load of arg3 in the UV stores takes $v1 and the shifted value
 * $t4 where the original has $t4 and $t5 (the other rows are that rename). poly and dist are declared before m: the
 * original has two scalar slots above the MATRIX, and size, o, j, t fill the six below pt (spill slots 0x58/0x64/0x78
 * then match). Closer pass, still 10 lines: that load carries the colour of the uopt range shl(arg3, 6), which gets
 * $v1 (the call result holds $v0); the original needs $t4 while the four v/u1 ranges keep $a1..$t0, and no tie order
 * gives that, because u1's range is numbered after the shift. Layout notes: matching-notes, "T-19018".
 * T-22005: regdiff (traced uopt) agrees: the shl range (priority 3) takes colour 2 ($v1) and the original has an interfering range in $v1 with no instruction in this block. No change from autotweak (1,308 tries in two runs), 150 permutations of the eight UV stores, one statement per store, two lines, dummy tests, `u8`/`s32` for `t`, `arg3 * 64`, or early-return shapes of the two ifs (those give 22 lines).
 * T-22005 closer: viewsweep finds no gain (10 views); writing `(arg3 & 4) << 4` without the `t` local gives 66 lines, 132
 * without its frame word. */
s32 func_8013D838(s16 arg0, s16 arg1, s16 arg2, s32 arg3) {
    POLY_FT4 *poly;
    s32 dist;
    MATRIX m;
    SVECTOR v;
    s32 pt[4];
    s32 size;
    s32 o;
    s32 depth;
    s32 flag;
    s32 j;
    u16 t;

    size = 0x100;
    m = D_80129C40;
    func_800A1100(&m);
    func_800A1070(&m);
    if (arg3 < 0 || arg3 == 6) {
        size = 0x200;
    }
    v.vz = arg2;
    for (o = 0; o != 2; o++) {
        v.vy = arg1 + o * size - size / 2;
        for (j = 0; j != 2; j++) {
            v.vx = arg0 + j * size - size / 2;
            func_800AB170(&v, &pt[j * 2 + o], &depth, &flag);
        }
    }
    if (((s16 *)pt)[4] >= -0x2C0 && ((s16 *)pt)[0] < 0x2C1) {
        if (((s16 *)pt)[3] >= D_800E6280.unk_018[D_8011ECA0] - 0x200
            && D_800E6280.unk_018[D_8011ECA0] + 0x200 >= ((s16 *)pt)[7]) {
            dist = (D_80122754 - arg2) / 127;
            if (dist >= 0x100) {
                dist = 0xFF;
            } else if (dist <= 0) {
                dist = 1;
            }
            poly = func_800490F0(0x28, D_8011ECA0);
            func_8009F040(poly);
            SetSemiTrans(poly, 1);
            poly->r0 = 0x80;
            poly->g0 = 0x80;
            poly->b0 = 0x80;
            poly->tpage = func_8009ECB0(0, 1, 0x380, 0x100);
            poly->clut = func_8009ED74(0, 0x1E5);
            t = arg3 & 4;
            poly->u0 = arg3 << 6, poly->v0 = (t << 4) + 0x80, poly->u1 = (arg3 << 6) + 0x3F, poly->v1 = (t << 4) + 0x80, poly->u2 = arg3 << 6, poly->v2 = (t << 4) + 0x80 + 0x3F, poly->u3 = (arg3 << 6) + 0x3F, poly->v3 = (t << 4) + 0x80 + 0x3F;
            poly->x0 = ((s16 *)pt)[0];
            poly->y0 = ((s16 *)pt)[1];
            poly->x1 = ((s16 *)pt)[2];
            poly->y1 = ((s16 *)pt)[3];
            poly->x2 = ((s16 *)pt)[4];
            poly->y2 = ((s16 *)pt)[5];
            poly->x3 = ((s16 *)pt)[6];
            poly->y3 = ((s16 *)pt)[7];
            func_8009EED0((D_8011ECA0 << 10) + dist * 4 + D_800E8CA0, poly);
        }
    }
}
#else
INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013D1E0", func_8013D838);
#endif
