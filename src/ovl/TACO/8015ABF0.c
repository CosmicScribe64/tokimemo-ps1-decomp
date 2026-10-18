#include "common.h"
#include "ovl/TACO.h"


void func_8015ABF0(void) {
    s32 i;

    for (i = 0; i < 64; i++) {
        D_80161D9C[i] = 0;
        D_80127080[i].unk0 = 0x80000000;
    }
    for (i = 16; i != 63; i++) {
        func_80143B34(i);
        D_8015EDB4[i].unk4.unk0 = 0x80000000;
        D_8015EDB4[i].unk84[2] = 1;
    }
    D_801604C0 = 0;
    D_801604C4 = 0;
    D_801604C8 = 0;
    D_801604CC = 0;
    D_801604D0 = 0;
}

/* The two table walks are index loops over D_80161D9C (uopt shares one strength-reduced pointer between them, which
 * is why n, i and that pointer get s2, s0 and s1); the count loop compares the element address against the table
 * end. HYPOTHESIS: base is a model data address (+4, then +8 more for the link call). T-19026 */
s32 func_8015ACCC(s32 a, s32 b, TcPos c, TcPos d, TcPos e, s32 f, s32 g) {
    s32 i;
    s32 base;
    s32 id;
    u32 n;
    s32 *pair;

    id = 0;
    n = 0;
    for (i = 16; i < 63; i++) {
        if (!((i + D_8015EDB4)->unk0[1] & 0x80)) {
            id = i;
            break;
        }
    }
    if (id == 0) {
        return -1;
    }
    for (i = 0; &D_80161D9C[i] < &D_80161D9C[0x40]; i++) {
        if (D_80161D9C[i] == 0) {
            n++;
        }
    }
    if (n < D_80161E20[a]) {
        return -1;
    }
    base = D_80161DDC[a];
    base += 4;
    func_8009B3C0(base);
    base += 8;
    func_80143B34(id);
    /* FAKE: the empty test adds one basic block to the live range of a, which lowers its priority below base's, so base
     * gets $fp and a stays in its stack home as in the original; no code. T-19026 */
    if (b) {}
    func_80099E30(0, &(id + D_8015EDB4)->unk14);
    if (a == 0xA) {
        func_8009B430(base, (u8 *)&(id + D_8015EDB4)->unk4, 1);
    } else {
        func_8009B430(base, (u8 *)&(id + D_8015EDB4)->unk4, 0);
    }
    n = D_80161E20[a];
    for (i = 0; n != 0; i++) {
        if (D_80161D9C[i] == 0) {
            func_80099E30((s32)&(id + D_8015EDB4)->unk14, &D_80127480[i]);
            D_80161D9C[i] = id;
            if (a == 0xF || a == 0x10) {
                pair = (D_80161E20[a] - n) * 2 + D_80161E60[a];
                func_8015B1B8(i, pair[0], pair[1], 0);
            } else {
                func_8015B2AC(i, D_80161E60[a][D_80161E20[a] - n]);
            }
            func_8009B430(base, (u8 *)&D_80127080[i], D_80161E20[a] - n);
            n--;
            D_80127080[i].unk4[0] = (s32)&D_80127480[i];
            D_80127080[i].unk0 = 0x80000000;
        }
    }
    (D_8015EDB4 + id)->unk4.unk0 = 0x80000000;
    (D_8015EDB4 + id)->unk2 = b;
    (D_8015EDB4 + id)->unk7A = 0x7FFF;
    (D_8015EDB4 + id)->unk84[2] = a;
    (D_8015EDB4 + id)->unk3 = 1;
    (D_8015EDB4 + id)->unk82 = 0;
    (D_8015EDB4 + id)->unk72 = 0;
    (D_8015EDB4 + id)->unk7C = c.x;
    (D_8015EDB4 + id)->unk7E = c.y;
    (D_8015EDB4 + id)->unk80 = c.z;
    (D_8015EDB4 + id)->unk0[1] = 0x80;
    (D_8015EDB4 + id)->unk74 = d.x;
    (D_8015EDB4 + id)->unk76 = d.y;
    (D_8015EDB4 + id)->unk78 = d.z;
    (D_8015EDB4 + id)->unk64 = e.x;
    (D_8015EDB4 + id)->unk66 = e.y;
    (D_8015EDB4 + id)->unk68 = e.z;
    (D_8015EDB4 + id)->unk6A = e.unk6;
    (D_8015EDB4 + id)->unk6C = D_80161EA4[a];
    (D_8015EDB4 + id)->unk6E = D_80161EA4[a];
    (D_8015EDB4 + id)->unk70 = D_80161EA4[a];
    (D_8015EDB4 + id)->unk84[1] = f;
    (D_8015EDB4 + id)->unk84[0] = D_80161EC8[a];
    (D_8015EDB4 + id)->unk84[3] = g | 1;
    (id + D_8015EDB4)->unk4.unk4[0] = (s32)&(id + D_8015EDB4)->unk14;
    return id;
}

/* NON_MATCHING: T-22025, 8 rows (T-19026, T-16563); frame and MATRIX offset match (declare i, j before matrix: later-declared
 * locals sit lower), only the struct-copy temporary differs (ugen t2 against t3, T-0018).
 * Closer pass: before the copy's data register ugen skips one free temporary when the last store it emitted went
 * through a pointer (here the loop's stores to matrix), two after a direct stack or global store or at function entry.
 * The original copy register is t3, as if it skipped two (t9, t2). A stack store such as `matrix.m[0][0] = 0;` before
 * the copy gives t3 but adds code; a uopt range in t2 gives t3 but moves the loop registers. No form without extra
 * code found; TACO func_801472D4 has the same miss. T-19026: empty tests on the parameters (removed by uopt or they
 * change the frame), swapped operands of the two multiplies and both orders of the copy and the flag store do not
 * move it either. T-19026 closer: still 8 lines. In `cc -S` ugen loads &matrix into its own temporary t2
 * (`addu t2,sp,16; move v1,t2; move t0,t2`, which as1 folds to `addiu t0,sp,32; or v1,t0,zero`), so t2 is used once
 * before the loop. The original's temporaries (t3/t4 in the loop, t5..t8 after it, copy t3) are exactly what IDO gives
 * when t2 is a uopt register and &matrix is held in it. No C form found that does this: counter and parameter
 * types, table-typed parameters, pointer walks, a MATRIX pointer in obj's slot, do/while and != loops (about 110 more
 * variants) give 16 to 80 listing lines. */
#ifdef NON_MATCHING
void func_8015B1B8(arg0, arg1, arg2, arg3)
    s32 arg0;
    s16 *arg1;
    s16 *arg2;
    s32 arg3;
{
    s32 i;
    s32 j;
    MATRIX matrix;
    TcTransformObj50 *obj;

    arg2 += arg3 * 9;
    arg1 += arg3 * 3;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            matrix.m[i][j] = *arg2++;
        }
        matrix.t[i] = *arg1++;
    }
    obj = (TcTransformObj50 *)&D_80127480[arg0];
    obj->transform = matrix;
    obj->unk_00 = 0;
}
#else
INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8015ABF0", func_8015B1B8);
#endif

void func_8015B2AC(s32 arg0, s16 *arg1) {
    s32 pad[7]; /* FAKE: unused local reproduces the original frame (0x60) and spill offsets; real source unknown. T-6070 */
    TcObj50 *o;
    TcPos *pos;
    u8 *q;

    o = &D_80127480[arg0];
    o->unk18 += arg1[0];
    o->unk1C += arg1[1];
    o->unk20 += arg1[2];
    pos = &D_80128880[arg0];
    pos->x = arg1[3];
    pos->z = arg1[4];
    q = (u8 *)o->unk4;
    pos->y = arg1[5];
    func_8015D0D0((pos->x << 12) / 360, q);
    func_8015CF30((pos->z << 12) / 360, q);
    func_8015D270((pos->y << 12) / 360, q);
    *(s32 *)o = 0;
}

void func_8015B3B0(s32 arg0, s16 *arg1, s32 arg2) {
    /* The pointer precedes the payload to preserve its local layout. T-9520. */
    TcTransformObj50 *obj;
    MATRIX matrix;
    TcPos *pos;

    arg1 += arg2 * 6;
    matrix = *(MATRIX *)D_80125C60;
    obj = (TcTransformObj50 *)&D_80127480[arg0];
    matrix.t[0] = obj->transform.t[0] + arg1[0];
    matrix.t[1] = obj->transform.t[1] + arg1[1];
    matrix.t[2] = obj->transform.t[2] + arg1[2];
    pos = &D_80128880[arg0];
    pos->x += arg1[3];
    arg1 += 4;
    func_8015D0D0((pos->x << 12) / 360, &matrix);
    pos->z += *arg1;
    arg1++;
    func_8015CF30((pos->z << 12) / 360, &matrix);
    pos->y += *arg1;
    func_8015D270((pos->y << 12) / 360, &matrix);
    obj->transform = matrix;
    obj->unk_00 = 0;
}

/* FAKE: byte-offset actor views preserve the original alias/reload behavior;
 * direct TcActor fields differ by 30 listing rows. T-12070. */
void func_8015B59C(void) {
    s32 i;
    TcPos position;
    for (i = 0x10; i != 0x3F; i++) {
        if (*(u8 *)((u8 *)&D_8015EDB4[i] + 1) & 0x80) {
            if (*(s16 *)((u8 *)&D_8015EDB4[i] + 122) == 0x7FFF) {
                func_8015C0A0(1, i);
                *(s16 *)((u8 *)&D_8015EDB4[i] + 122) = 0;
            }
            if ((*(u8 *)((u8 *)&D_8015EDB4[i] + 133) == 0) && (*(u8 *)((u8 *)&D_8015EDB4[i] + 2) != 0xFF)) {
                *(s16 *)((u8 *)&D_8015EDB4[i] + 122) = 0;
                *(u8 *)((u8 *)&D_8015EDB4[i] + 3) = 2;
                *(u8 *)((u8 *)&D_8015EDB4[i] + 2) = 0xFF;
                position.x = *(s16 *)((u8 *)&D_8015EDB4[i] + 116);
                position.y = *(s16 *)((u8 *)&D_8015EDB4[i] + 118);
                position.z = *(s16 *)((u8 *)&D_8015EDB4[i] + 120);
                switch (*(u8 *)((u8 *)&D_8015EDB4[i] + 135) & 0xF0) {
                case 0x10:
                    func_8013E424(position, 6);
                    break;
                case 0x20:
                    func_8013E424(position, 7);
                    break;
                case 0x30:
                    func_8013E424(position, 0);
                    break;
                case 0x40:
                    func_8013E424(position, 1);
                    break;
                case 0x50:
                    func_8013E424(position, 2);
                    break;
                case 0x60:
                    func_8013E424(position, 3);
                    break;
                case 0x70:
                    func_8013E424(position, 4);
                    break;
                case 0x80:
                    func_8013E424(position, 5);
                    break;
                case 0xD0:
                    func_8013E424(position, 6);
                    func_8013E424(position, 6);
                    func_8013E424(position, 6);
                    func_8013E424(position, 6);
                    func_8013E424(position, 6);
                    break;
                case 0xE0:
                    func_8013E424(position, 0);
                    func_8013E424(position, 1);
                    func_8013E424(position, 7);
                    break;
                case 0xF0:
                    func_8013E424(position, 6);
                    func_8013E424(position, 6);
                    func_8013E424(position, 7);
                    func_8013E424(position, 2);
                    func_8013E424(position, 0);
                    func_8013E424(position, 1);
                    func_8013E424(position, 5);
                    func_8013E424(position, 3);
                    func_8013E424(position, 4);
                    break;
                }
            }
            func_8015A948((s32) *(u8 *)((u8 *)&D_8015EDB4[i] + 2), i);
            func_8015C2CC(i);
            *(s16 *)((u8 *)&D_8015EDB4[i] + 122) = (s16) (*(s16 *)((u8 *)&D_8015EDB4[i] + 122) + 1);
        }
    }
}

/* FAKE: the actor is addressed through a byte-offset view in the loop: one pointer then serves the field loads and
 * &unk14, the array form builds a second one. Real source unknown. T-16563 */
void func_8015BD30(s32 arg0, s16 *arg1) {
    s32 i;
    s32 val;
    s16 frame;
    s32 phase;
    s32 j;
    s32 n;
    s32 last;
    u8 *id;

    frame = D_8015EDB4[arg0].unk82;
    if (frame >= arg1[3]) {
        D_8015EDB4[arg0].unk82 = 0;
        frame = D_8015EDB4[arg0].unk82;
    }
    phase = frame % arg1[2];
    n = 0;
    for (i = 0;; i++) {
        if (phase < arg1[4 + i * 2]) {
            if (i) {} /* FAKE: the empty test splits the live range of i at the found key, so i and j * 4 get s1 and s0 as in the original. T-16563 */
            val = arg1[5 + i * 2];
            break;
        }
    }
    id = D_80161D9C;
    j = arg1[0];
    last = arg1[1];
    for (i = 0; i != 0x40; i++, id++) {
        if (arg0 == *id) {
            if (n >= j) {
                if (((TcActor *)((u8 *)D_8015EDB4 + arg0 * 0x88))->unk82 % arg1[2] == 0) {
                    func_80099E30((s32)&((TcActor *)((u8 *)D_8015EDB4 + arg0 * 0x88))->unk14, (i + D_80127480));
                    func_8015B2AC(i, D_80161E60[D_8015EDB4[arg0].unk84[2]][j]);
                } else {
                    func_8015B3B0(i, D_80161E60[((TcActor *)((u8 *)D_8015EDB4 + arg0 * 0x88))->unk84[2]][j], val);
                }
                j++;
            }
            n++;
        }
        if (last < j) {
            break;
        }
    }
}

/* No local actor pointer (see func_8015C0A0); `n = 0` after the first test lands in its branch delay slot as in the original. T-16563 */
void func_8015BF70(s32 arg0) {
    u32 n;
    s32 i;
    s32 *rec;

    if ((arg0 + D_8015EDB4)->unk82 >= 0xF) {
        (arg0 + D_8015EDB4)->unk82 = 0;
    }
    n = 0;
    for (i = 0; i < 0x40; i++) {
        if (arg0 == D_80161D9C[i]) {
            rec = D_80161E60[(arg0 + D_8015EDB4)->unk84[2]] + n * 2;
            func_8015B1B8(i, rec[0], rec[1], (arg0 + D_8015EDB4)->unk82);
            n++;
        }
        if (n >= D_80161E20[(arg0 + D_8015EDB4)->unk84[2]]) {
            break;
        }
    }
}

/* The actor is addressed as (arg1 + D_8015EDB4) in every use, no local: uopt then keeps it in $v0 and the index multiply comes first (T-16563). */
void func_8015C0A0(s32 arg0, s32 arg1) {
    s32 i;

    if ((arg1 + D_8015EDB4)->unk0[1] & 0x80) {
        if (arg0 == 1) {
            if ((arg1 + D_8015EDB4)->unk84[2] < 0xB) {
                (arg1 + D_8015EDB4)->unk4.unk0 = 0;
                return;
            }
            (arg1 + D_8015EDB4)->unk4.unk0 = 0x80000000;
            for (i = 0; i < 0x40; i++) {
                if (arg1 == D_80161D9C[i]) {
                    D_80127080[i].unk0 = 0;
                }
            }
            return;
        }
        if ((arg1 + D_8015EDB4)->unk84[2] < 9) {
            (arg1 + D_8015EDB4)->unk4.unk0 = 0x80000000;
            return;
        }
        (arg1 + D_8015EDB4)->unk4.unk0 = 0x80000000;
        for (i = 0; i < 0x40; i++) {
            if (arg1 == D_80161D9C[i]) {
                D_80127080[i].unk0 = 0x80000000;
            }
        }
    }
}

void func_8015C208(s32 arg0) {
    s32 i;

    func_8015C0A0(0, arg0);
    D_8015EDB4[arg0].unk0[1] = 0;
    D_8015EDB4[arg0].unk84[2] = 0;
    for (i = 0; i < 0x40; i++) {
        if (arg0 == D_80161D9C[i]) {
            D_80161D9C[i] = 0;
        }
    }
}

/* FAKE: byte-offset views preserve actor reloads; explicit case 0 preserves
 * the original table range. Keep the case-13 call/load on one source line
 * for the original load scheduling. T-12070. */
void func_8015C2CC(s32 arg0) {
    u8 var_a0;
    if ((*(u8 *)((u8 *)&D_8015EDB4[arg0] + 3)) != 0) {
        switch (*(u8 *)((u8 *)&D_8015EDB4[arg0] + 134)) {
        case 11:
            (*(s16 *)((u8 *)&D_8015EDB4[arg0] + 114)) += 1;
            if ((*(s16 *)((u8 *)&D_8015EDB4[arg0] + 114)) >= 6) {
                (*(s16 *)((u8 *)&D_8015EDB4[arg0] + 114)) = 0;
                func_8015BD30(arg0, D_801606AC);
                func_8015BD30(arg0, D_801606C4);
                (*(s16 *)((u8 *)&D_8015EDB4[arg0] + 130)) += 1;
            }
        case 0:
        default:
block_28:
            break;
        case 12:
            (*(s16 *)((u8 *)&D_8015EDB4[arg0] + 114)) += 1;
            if ((*(s16 *)((u8 *)&D_8015EDB4[arg0] + 114)) >= 6) {
                (*(s16 *)((u8 *)&D_8015EDB4[arg0] + 114)) = 0;
                if ((*(u8 *)((u8 *)&D_8015EDB4[arg0] + 3)) == 1) {
                    func_8015BD30(arg0, D_80160858);
                } else {
                    func_8015BD30(arg0, D_80160858);
                }
                (*(s16 *)((u8 *)&D_8015EDB4[arg0] + 130)) = (s16) ((*(s16 *)((u8 *)&D_8015EDB4[arg0] + 130)) + 1);
                goto block_28;
            }
            break;
        case 13:
            (*(s16 *)((u8 *)&D_8015EDB4[arg0] + 114)) += 1;
            if ((*(s16 *)((u8 *)&D_8015EDB4[arg0] + 114)) >= 6) {
                (*(s16 *)((u8 *)&D_8015EDB4[arg0] + 114)) = 0;
                var_a0 = (*(u8 *)((u8 *)&D_8015EDB4[arg0] + 3));
                if (var_a0 == 1) {
                    func_8015BD30(arg0, D_8016090C);                     var_a0 = (*(u8 *)((u8 *)&D_8015EDB4[arg0] + 3));
                }
                if (var_a0 == 2) {
                    func_8015BD30(arg0, D_8016090C);                 }
                (*(s16 *)((u8 *)&D_8015EDB4[arg0] + 130)) += 1;
                goto block_28;
            }
            break;
        case 14:
            (*(s16 *)((u8 *)&D_8015EDB4[arg0] + 114)) += 1;
            if ((*(s16 *)((u8 *)&D_8015EDB4[arg0] + 114)) >= 6) {
                (*(s16 *)((u8 *)&D_8015EDB4[arg0] + 114)) = 0;
                if ((*(u8 *)((u8 *)&D_8015EDB4[arg0] + 3)) == 1) {
                    func_8015BD30(arg0, D_80160A60);
                }
                (*(s16 *)((u8 *)&D_8015EDB4[arg0] + 130)) += 1;
                goto block_28;
            }
            break;
        case 15:
            (*(s16 *)((u8 *)&D_8015EDB4[arg0] + 114)) += 1;
            if ((*(s16 *)((u8 *)&D_8015EDB4[arg0] + 114)) >= 6) {
                (*(s16 *)((u8 *)&D_8015EDB4[arg0] + 114)) = 0;
                if ((*(u8 *)((u8 *)&D_8015EDB4[arg0] + 3)) == 1) {
                    func_8015BF70(arg0);
                }
                (*(s16 *)((u8 *)&D_8015EDB4[arg0] + 130)) += 1;
                goto block_28;
            }
            break;
        case 16:
            (*(s16 *)((u8 *)&D_8015EDB4[arg0] + 114)) += 1;
            if ((*(s16 *)((u8 *)&D_8015EDB4[arg0] + 114)) >= 6) {
                (*(s16 *)((u8 *)&D_8015EDB4[arg0] + 114)) = 0;
                if ((*(u8 *)((u8 *)&D_8015EDB4[arg0] + 3)) == 1) {
                    func_8015BF70(arg0);
                }
                (*(s16 *)((u8 *)&D_8015EDB4[arg0] + 130)) += 1;
                goto block_28;
            }
            break;
        }
    }
}

void func_8015C698(void) {
    s32 i;

    for (i = 16; i < 63; i++) {
        D_8015EDB4[i].unk84[1] = 0;
    }
}

/* FAKE: byte-offset actor views retain the original reload/alias behavior.
 * The switch intentionally reads the selector as u16. T-12070. */
void func_8015C718(s32 arg0) {
    TcPos position;
    s16 counter;

    if ((*(s16 *)((u8 *)&D_8015EDB4[arg0] + 122)) == 0) {
        if ((*(u8 *)((u8 *)&D_8015EDB4[arg0] + 134)) != 6) {
            func_800AE120(D_801604C0 + arg0);
            (*(u8 *)((u8 *)&D_8015EDB4[arg0] + 1)) = 0x90;
            (*(s16 *)((u8 *)&D_8015EDB4[arg0] + 124)) = (func_800AE0D0() % 40) - 0x14;
            (*(s16 *)((u8 *)&D_8015EDB4[arg0] + 126)) = (func_800AE0D0() % 10) + 0xA;
            (*(s16 *)((u8 *)&D_8015EDB4[arg0] + 128)) = (func_800AE0D0() % 10) + 0xA;
            (*(s16 *)((u8 *)&D_8015EDB4[arg0] + 106)) = (s16) (func_800AE0D0() % 6);
            func_8013D1E0(*(TcPos *)&(*(s16 *)((u8 *)&D_8015EDB4[arg0] + 116)), *(TcPos *)&(*(s16 *)((u8 *)&D_8015EDB4[arg0] + 124)), 0);

        } else {
            func_800AE120(D_801604C0 + arg0);
            (*(u8 *)((u8 *)&D_8015EDB4[arg0] + 1)) = 0x90;
            (*(s16 *)((u8 *)&D_8015EDB4[arg0] + 124)) = (func_800AE0D0() % 10) - 5;
            (*(s16 *)((u8 *)&D_8015EDB4[arg0] + 126)) = (func_800AE0D0() % 2) + 2;
            (*(s16 *)((u8 *)&D_8015EDB4[arg0] + 128)) = (func_800AE0D0() % 2) + 2;
            func_8013D1E0(*(TcPos *)&(*(s16 *)((u8 *)&D_8015EDB4[arg0] + 116)), *(TcPos *)&(*(s16 *)((u8 *)&D_8015EDB4[arg0] + 124)), 0);

        }
    }
    if ((*(u8 *)((u8 *)&D_8015EDB4[arg0] + 134)) != 6) {
        switch (*(u16 *)((u8 *)&D_8015EDB4[arg0] + 106)) {
        case 0:
            (*(s16 *)((u8 *)&D_8015EDB4[arg0] + 100)) += 0x10;

            break;
        case 1:
            (*(s16 *)((u8 *)&D_8015EDB4[arg0] + 102)) += 0x10;

            break;
        case 2:
            (*(s16 *)((u8 *)&D_8015EDB4[arg0] + 104)) += 0x10;

            break;
        case 3:
            (*(s16 *)((u8 *)&D_8015EDB4[arg0] + 100)) -= 0x10;

            break;
        case 4:
            (*(s16 *)((u8 *)&D_8015EDB4[arg0] + 102)) -= 0x10;

            break;
        case 5:
            (*(s16 *)((u8 *)&D_8015EDB4[arg0] + 104)) -= 0x10;

            break;
        }
        counter = (*(s16 *)((u8 *)&D_8015EDB4[arg0] + 122));
    } else {
        if (((s16) (*(s16 *)((u8 *)&D_8015EDB4[arg0] + 122)) % 3) == 0) {
            func_800AE120(D_801604C0 + arg0);
            position.x = ((*(s16 *)((u8 *)&D_8015EDB4[arg0] + 116)) + (func_800AE0D0() % 500)) - 0xFA;

            position.y = (*(s16 *)((u8 *)&D_8015EDB4[arg0] + 118)) + (func_800AE0D0() % 250);
            position.z = (*(s16 *)((u8 *)&D_8015EDB4[arg0] + 120));
            func_8013D1E0(position, *(TcPos *)&(*(s16 *)((u8 *)&D_8015EDB4[arg0] + 124)), 0);

        }
        (*(s16 *)((u8 *)&D_8015EDB4[arg0] + 100)) += 2;

        (*(s16 *)((u8 *)&D_8015EDB4[arg0] + 104)) += 2;
        counter = (*(s16 *)((u8 *)&D_8015EDB4[arg0] + 122));
    }
    if (!(counter & 1)) {
        func_8015C0A0(1, arg0);
    } else {
        func_8015C0A0(0, arg0);
    }
    if ((*(s16 *)((u8 *)&D_8015EDB4[arg0] + 122)) >= 0x3D) {
        func_8015C208(arg0);
    }
}

s32 func_8015CC54(void) {
    s32 n;
    s32 i;

    n = 0;
    for (i = 0; i < 0x40; i++) {
        if (D_80161D9C[i] == 0) {
            n += 1;
        }
    }
    return n;
}

s32 func_8015CCC8(arg0)
s32 arg0;
{
    s32 n;
    s32 i;

    n = 0;
    for (i = 16; i != 63; i++) {
        if (((TcActor *)(i * 0x88 + (u8 *)D_8015EDB4))->unk84[1] != 0) {
            if (!(((TcActor *)(i * 0x88 + (u8 *)D_8015EDB4))->unk0[1] & 0x80)) {
                ((TcActor *)(i * 0x88 + (u8 *)D_8015EDB4))->unk84[1] = 0;
            }
        }
        if (arg0 != 0xFF) {
            if (arg0 == ((TcActor *)(i * 0x88 + (u8 *)D_8015EDB4))->unk84[2] && ((TcActor *)(i * 0x88 + (u8 *)D_8015EDB4))->unk84[1] != 0) {
                n++;
            }
        } else if (((TcActor *)(i * 0x88 + (u8 *)D_8015EDB4))->unk84[1] != 0) {
            n++;
        }
    }
    return n;
}

/* Index first, as in func_8015C0A0: the global then gets the later colour. T-16563 */
s32 func_8015CE64(void) {
    s32 n;
    s32 i;

    n = 0;
    for (i = 0x10; i != 0x3F; i++) {
        if ((i + D_8015EDB4)->unk0[1] & 0x80) {
            n++;
        }
    }
    return n;
}
