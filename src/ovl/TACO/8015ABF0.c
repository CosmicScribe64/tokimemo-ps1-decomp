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

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8015ABF0", func_8015ACCC);

/* NON_MATCHING: T-10810, frame and MATRIX offset match now (declare i, j before matrix: later-declared locals sit lower); only the struct-copy temporary differs (ugen t2 against t3, 8 lines) */
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

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8015ABF0", func_8015BD30);

/* NON_MATCHING: T-10810, loop, callee arguments and saved-register set match (78 vs 76 instructions); the local actor pointer and the type byte take s4/s2 where the original has s2/s4 (the CSE'd &D_8015EDB4[arg0] range is coloured before the local), plus a spill-order difference in the prologue */
#ifdef NON_MATCHING
void func_8015BF70(s32 arg0) {
    TcActor *a;
    u32 n;
    s32 i;
    u8 *id;
    u8 t;
    s32 *rec;

    a = &D_8015EDB4[arg0];
    n = 0;
    if (a->unk82 >= 0xF) {
        a->unk82 = 0;
        a = &D_8015EDB4[arg0];
    }
    t = a->unk84[2];
    id = D_80161D9C;
    i = 0;
    do {
        if (arg0 == *id) {
            rec = D_80161E60[t] + n * 2;
            func_8015B1B8(i, rec[0], rec[1], a->unk82);
            n++;
            a = &D_8015EDB4[arg0];
            t = a->unk84[2];
        }
        i++;
        if (n >= D_80161E20[t]) {
            break;
        }
        id++;
    } while (i != 0x40);
}
#else
INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8015ABF0", func_8015BF70);
#endif

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

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8015ABF0", func_8015CE64);
