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

#ifdef NON_MATCHING
void func_80144094(void) {
    MATRIX m;
    SVECTOR v;
    s32 sxy[13][9];
    TcTex4 ut;
    TcTex4 vt;
    s32 depth;
    s32 flag;
    s32 ang;
    s32 x;
    s32 i;
    s32 j;
    s16 *q;
    s32 idx;
    POLY_FT4 *poly;

    ut = D_8015F5F8;
    vt = D_8015F5FC;
    m = D_80129C40;
    func_800A1100(&m);
    func_800A1070(&m);
    v.vy = D_8015F5F0.b;
    ang = 0;
    for (i = 0; i < 9; i++) {
        x = 0x3000;
        if (i == 8) {
            v.vz = ang;
        } else {
            v.vz = ang + D_8015F5F0.c;
        }
        for (j = 0; j < 13; j++) {
            v.vx = x + D_8015F5F0.a;
            func_800AB170(&v, &sxy[j][i], &depth, &flag);
            x -= 0x800;
        }
        ang -= 0x1000;
    }
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 12; j++) {
            q = (s16 *)&sxy[j][i];
            if (q[20] >= 0 && q[2] < 0x201) {
                if (q[1] >= D_800E6280.unk_018[D_8011ECA0] - 0x200 && D_800E6280.unk_018[D_8011ECA0] + 0x400 >= q[3]) {
                    poly = func_800490F0(0x28, D_8011ECA0);
                    func_8009F040(poly);
                    poly->r0 = 0x80;
                    poly->g0 = 0x80;
                    poly->b0 = 0x80;
                    poly->tpage = func_8009ECB0(1, 0, 0x200, 0x100);
                    poly->clut = func_8009ED74(0, 0x1ED);
                    idx = (j + i + D_8015F5F0.d) & 7;
                    if (idx >= 4) {
                        idx = 0;
                    }
                    poly->u0 = ut.b[idx];
                    poly->v0 = vt.b[idx];
                    poly->u1 = ut.b[idx] + 0x3F;
                    poly->v1 = vt.b[idx];
                    poly->u2 = ut.b[idx];
                    poly->v2 = vt.b[idx] + 0x3F;
                    poly->u3 = ut.b[idx] + 0x3F;
                    poly->v3 = vt.b[idx] + 0x3F;
                    poly->x0 = q[0];
                    poly->y0 = q[1];
                    poly->x1 = q[2];
                    poly->y1 = q[3];
                    poly->x2 = q[18];
                    poly->y2 = q[19];
                    poly->x3 = q[20];
                    poly->y3 = q[21];
                    func_8009EED0(D_800E8CA0 + (D_8011ECA0 << 10) + 0x3F8, poly);
                }
            }
        }
    }
}
#else
INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80143E80", func_80144094);
#endif

void func_80144430(void) {
    s32 pad[2]; /* FAKE: eight unused bytes above m reproduce the original frame (216); T-19009 */
    MATRIX m;
    SVECTOR v;
    s32 pt[10];
    /* The scalars are declared in this order because IDO gives each a stack slot (register
     * resident or not) and the original's slots for rot, flag and depth are 0x60, 0x68, 0x6C. T-19009 */
    s32 o;
    s16 *q;
    POLY_FT4 *poly;
    s32 t;
    s32 depth;
    s32 flag;
    s32 j;
    s32 rot;

    m = D_80129C40;
    func_800A1100(&m);
    func_800A1070(&m);
    v.vz = -0x7FFF;
    rot = 0;
    for (o = 0; o < 2; o++) {
        v.vy = D_8015F5F0.b + rot / 4 + 0x80;
        for (j = 0; j < 5; j++) {
            v.vx = 0x4000 - j * 0x2000 + D_8012274C;
            func_800AB170(&v, &pt[j * 2 + o], &depth, &flag);
        }
        rot -= 0x1000;
    }
    for (o = 0; o < 1; o++) {
        for (j = 0; j < 4; j++) {
            q = (s16 *)&pt[j * 2 + o];
            poly = func_800490F0(0x28, D_8011ECA0);
            func_8009F040(poly);
            poly->r0 = 0x80;
            poly->g0 = 0x80;
            poly->b0 = 0x80;
            poly->tpage = func_8009ECB0(1, 0, 0x200, 0x100);
            t = (j & 1) << 6;
            poly->clut = func_8009ED74(0, 0x1EE);
            poly->v0 = t + 0x3F;
            poly->v2 = t + 0x3F;
            poly->v1 = t;
            poly->v3 = t;
            poly->u0 = 0;
            poly->u1 = 0;
            poly->u2 = 0xFF;
            poly->u3 = 0xFF;
            poly->x0 = q[0];
            poly->y0 = q[1];
            poly->x1 = q[2];
            poly->y1 = q[3];
            poly->x2 = q[4];
            poly->y2 = q[5];
            poly->x3 = q[6];
            poly->y3 = q[7];
            func_8009EED0(D_800E8CA0 + (D_8011ECA0 << 10) + 0x3F4, poly);
        }
    }
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
