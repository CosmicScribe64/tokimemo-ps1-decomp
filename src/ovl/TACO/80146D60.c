#define TACO_OVERRIDE_func_801471D0 /* pointer definition, s32 view for the callers: with u32 * in the header the caller in 80148FE0.c loses its shared constant register (T-19015) */
#include "common.h"
#include "ovl/TACO.h"

void func_801471D0(u32 *arg0);

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

/* Same shape as func_80147488 (T-16561): a local for the coordinate object would add a stack home and move the spill slots. */
void func_80146F74(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8) {
    MATRIX matrix;
    TcPos *pos;

    matrix = *(MATRIX *)D_80125C60;
    matrix.t[0] = ((TcTransformObj50 *)&D_80127480[arg0])->transform.t[0] + arg1;
    matrix.t[1] = ((TcTransformObj50 *)&D_80127480[arg0])->transform.t[1] + arg2;
    matrix.t[2] = ((TcTransformObj50 *)&D_80127480[arg0])->transform.t[2] + arg3;
    if (arg7 != 0 && arg8 != 0) {
        matrix.t[0] += (D_8015EDB4->unk74 - matrix.t[0]) / arg7;
        matrix.t[1] += (D_8015EDB4->unk76 - matrix.t[1]) / arg8;
    }
    pos = &D_80128880[arg0];
    pos->x += arg4;
    func_8015D0D0((pos->x << 12) / 360, &matrix);
    pos->z += arg6;
    func_8015CF30((pos->z << 12) / 360, &matrix);
    pos->y += arg5;
    func_8015D270((pos->y << 12) / 360, &matrix);
    ((TcTransformObj50 *)&D_80127480[arg0])->transform = matrix;
    ((TcTransformObj50 *)&D_80127480[arg0])->unk_00 = 0;
}

/* libgs model setup (GsMapModelingData, GsLinkObject4) walking the TMD header with dop. The second pass
 * restarts dop from arg0, which keeps the copy and the two in-place steps of the original
 * (`move s4,s3; lw s6,4(s4); addiu s4,s4,8`); a walker that is not restarted is folded into arg0 + 8.
 * arg0 must be a pointer: with an s32 parameter uopt keeps the first-pass copy as well (T-19015). */
void func_801471D0(u32 *arg0) {
    u32 *dop;
    s32 i;
    u32 n;

    dop = arg0;
    dop++;
    func_8009B3C0((s32)dop);
    dop += 2;
    for (i = 0; i < 64; i++) {
        func_8009B430((s32)dop, (u8 *)&D_80127080[i], 0);
        D_80127080[i].unk4[0] = (s32)&D_80127480[i];
        D_80127080[i].unk0 = 0x80000000;
    }
    dop = arg0;
    dop++;
    func_8009B3C0((s32)dop);
    dop++;
    n = *dop;
    dop++;
    for (i = 0; i < n; i++) {
        func_8009B430((s32)dop, (u8 *)&D_80127080[i + 1], i);
        D_80127080[i + 1].unk4[0] = (s32)&D_80127480[i + 1];
        D_80127080[i + 1].unk0 = 0x80000000;
    }
}

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
    s32 pad; /* FAKE: unused 4-byte local places m at sp+0x2C; real source unknown. T-16561 */

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
        for (j = 0; j != 3; j++) m.m[i][j] = *arg3++;
        m.t[i] = *arg2++;
    }
    rec = &D_80127080[arg0];
    ((TcTransformObj50 *)rec->unk4[0])->transform = m;
    ((TcTransformObj50 *)rec->unk4[0])->unk_00 = 0;
    return (s32)rec;
}

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

/* Same code as func_8015B3B0 (T-16561). No pointer local for the coordinate object: a second pointer local adds a stack home and
   moves the spill slots 4 bytes down. */
void func_80147488(s32 arg0, s16 *arg1, s32 arg2) {
    MATRIX matrix;
    TcPos *pos;

    arg1 += arg2 * 6;
    matrix = *(MATRIX *)D_80125C60;
    matrix.t[0] = ((TcTransformObj50 *)&D_80127480[arg0])->transform.t[0] + arg1[0];
    matrix.t[1] = ((TcTransformObj50 *)&D_80127480[arg0])->transform.t[1] + arg1[1];
    matrix.t[2] = ((TcTransformObj50 *)&D_80127480[arg0])->transform.t[2] + arg1[2];
    pos = &D_80128880[arg0];
    pos->x += arg1[3];
    arg1 += 4;
    func_8015D0D0((pos->x << 12) / 360, &matrix);
    pos->z += *arg1;
    arg1++;
    func_8015CF30((pos->z << 12) / 360, &matrix);
    pos->y += *arg1;
    func_8015D270((pos->y << 12) / 360, &matrix);
    ((TcTransformObj50 *)&D_80127480[arg0])->transform = matrix;
    ((TcTransformObj50 *)&D_80127480[arg0])->unk_00 = 0;
}

/* Same shape as func_80147488 with an interpolation step. The record pointer is the parameter itself (a separate local is not kept in $s0). T-16561 */
void func_80147674(s32 arg0, s16 *arg1, s32 arg2, s32 arg3, s32 arg4) {
    MATRIX matrix;
    TcPos *pos;

    arg1 += arg2 * 6;
    matrix = *(MATRIX *)D_80125C60;
    matrix.t[0] = ((TcTransformObj50 *)&D_80127480[arg0])->transform.t[0] + arg1[0];
    matrix.t[1] = ((TcTransformObj50 *)&D_80127480[arg0])->transform.t[1] + arg1[1];
    matrix.t[2] = ((TcTransformObj50 *)&D_80127480[arg0])->transform.t[2] + arg1[2];
    arg1 += 3;
    if (arg3 != 0 && arg4 != 0) {
        matrix.t[0] += (D_8015EDB4->unk74 - matrix.t[0]) / arg3;
        matrix.t[1] += (D_8015EDB4->unk76 - matrix.t[1]) / arg4;
    }
    pos = &D_80128880[arg0];
    pos->x += *arg1;
    arg1++;
    func_8015D0D0((pos->x << 12) / 360, &matrix);
    pos->z += *arg1;
    arg1++;
    func_8015CF30((pos->z << 12) / 360, &matrix);
    pos->y += *arg1;
    func_8015D270((pos->y << 12) / 360, &matrix);
    ((TcTransformObj50 *)&D_80127480[arg0])->transform = matrix;
    ((TcTransformObj50 *)&D_80127480[arg0])->unk_00 = 0;
}

void func_801478F8(s32 arg0, s16 *arg1) {
    TcPos *p;

    p = &D_80128880[arg0];
    p->x = arg1[3];
    p->z = arg1[4];
    arg1 += 5;
    p->y = arg1[0];
}

/* FAKE: the empty do-while adds a basic block after `base = arg2 + 4`, so arg2 gets an allocated range ($a3: `lw a3,72(sp); addiu a0,a3,4`) as in the original;
   found by the permuter, real source unknown (probably an empty macro). T-16561 */
void func_80147928(s32 arg0, s32 arg1, s32 arg2, void *arg3, void *arg4, s32 arg5) {
    u8 *p;
    TcPos *pos;
    s32 slot;
    s32 base;

    slot = arg1 + 0x10;
    func_80143B34(slot);
    base = arg2 + 4;
    do { } while (0);
    func_8009B3C0(base);
    func_8009B430(base + 8, (u8 *)&D_8015EDB4[slot].unk4, arg0 - 1, base);
    D_8015EDB4[slot].unk4.unk0 = 0x80000000;
    D_8015EDB4[slot].unk0[1] = 0x80;
    D_8015EDB4[slot].unk74 = D_80127480[arg0].unk18;
    D_8015EDB4[slot].unk76 = D_80127480[arg0].unk1C;
    D_8015EDB4[slot].unk78 = D_80127480[arg0].unk20;
    pos = &D_80128880[arg0];
    D_8015EDB4[slot].unk64 = pos->x;
    D_8015EDB4[slot].unk66 = pos->y;
    D_8015EDB4[slot].unk68 = pos->z;
    p = (u8 *)(arg1 * 2 + (s32)arg4);
    D_8015EDB4[slot].unk84[1] = p[0];
    D_8015EDB4[slot].unk7A = p[1];
    D_8015EDB4[slot].unk82 = 1;
    D_8015EDB4[slot].unk84[0] = *(u8 *)(arg1 * arg5 + (s32)arg3);
    D_8015EDB4[slot].unk84[3] = 1;
    D_8015EDB4[slot].unk4.unk4[0] = (s32)&D_80127480[arg0];
}

s32 func_80147B24(s32 arg0, void *arg1) {
    s32 i;
    s32 num;
    s32 den;

    den = 0;
    num = 0;
    for (i = 0; i < arg0; i++) {
        /* FAKE: byte-pointer Uadd instead of Uixa keeps uopt from replacing the loop exit test i != arg0 with a pointer bound (T-24120). */
        num += ((TcActor *)((u8 *)D_8015EDB4 + (i + 16) * 0x88))->unk84[1];
        den += ((u8 *)arg1)[i * 2] * (i + 16 + D_8015EDB4)->unk7A;
    }
    return 100 - (num * 100) / den;
}

/* No pointer locals for D_80128880[arg0] / D_80127480[...]: each would add a stack home and move the locals 4 bytes. T-16561 */
void func_80147C98(s32 arg0, s32 arg1, s16 arg2, s16 arg3, s32 arg4, s32 arg5, s32 arg6) {
    MATRIX m;
    SVECTOR vec;
    SVECTOR rot;
    TcPos out;

    m = *(MATRIX *)D_80125C60;
    func_8015D0D0((D_80128880[arg0].x << 12) / 360, &m);
    func_8015CF30((D_80128880[arg0].z << 12) / 360, &m);
    func_8015D270((D_80128880[arg0].y << 12) / 360, &m);
    vec.vx = D_80127480[arg1].unk18;
    vec.vy = D_80127480[arg1].unk1C;
    vec.vz = D_80127480[arg1].unk20;
    func_800A0ECC(&m, &vec, &rot);
    out.x = D_80127480[arg0].unk18 + rot.vx + arg2;
    out.y = D_80127480[arg0].unk1C + rot.vy + arg3;
    out.z = D_80127480[arg0].unk20 + rot.vz + (s16)arg4;
    func_8013DB80(out, arg5, arg6);
}

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
