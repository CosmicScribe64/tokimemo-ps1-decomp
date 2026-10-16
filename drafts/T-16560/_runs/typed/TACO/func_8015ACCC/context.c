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

/* bulk_m2c declarations (T-11190) */
extern s32 D_80161DDC;
extern s32 D_80161EA4;
extern s32 D_80161EC8;
void func_8015B1B8();
void func_8015B2AC(s32 arg0, s16 *arg1);
/* end bulk_m2c declarations */
s32 func_8015ACCC(s32 a, s32 b, s16 c_unk0, s16 c_unk4, s16 d_unk0, s16 d_unk4, s16 e_unk0, s16 e_unk4, s32 f, s32 g) {
    s32 sp64;
    s32 sp50;
    Tc10 *temp_s4;
    TcActor *temp_v1;
    TcActor *var_v0;
    s16 *temp_v0_3;
    s32 *temp_s3;
    s32 temp_fp;
    s32 temp_fp_2;
    s32 temp_v0;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_v1;
    u32 var_s2;
    u8 *temp_s7;
    u8 *var_s1;
    u8 *var_s1_2;
    u8 temp_t8;
    u8 var_s2_2;
    u8 *temp_v0_2;

    var_v1 = 0;
    var_s2 = 0;
    var_s0 = 0x10;
    var_v0 = &D_8015EDB4[16];
loop_1:
    if (!(var_v0->unk0[1] & 0x80)) {
        var_v1 = var_s0;
    } else {
        var_s0 += 1;
        var_v0 += 1;
        if (var_s0 < 0x3F) {
            goto loop_1;
        }
    }
    if (var_v1 == 0) {
        return -1;
    }
    var_s1 = D_80161D9C;
    do {
        temp_t8 = *var_s1;
        var_s1 += 1;
        if (temp_t8 == 0) {
            var_s2 += 1;
        }
    } while ((u32) var_s1 < (u32) &D_80161DDC);
    temp_s7 = &D_80161E20[a];
    temp_v0 = a * 4;
    if (var_s2 < (u8) *temp_s7) {
        return -1;
    }
    sp50 = temp_v0;
    temp_fp = *(&D_80161DDC + temp_v0) + 4;
    sp64 = var_v1;
    func_8009B3C0(temp_fp);
    temp_fp_2 = temp_fp + 8;
    func_80143B34(sp64);
    func_80099E30(0, &D_8015EDB4[sp64].unk14);
    if (a == 0xA) {
        func_8009B430(temp_fp_2, (u8 *) &D_8015EDB4[sp64].unk4, 1);
    } else {
        func_8009B430(temp_fp_2, (u8 *) &D_8015EDB4[sp64].unk4, 0);
    }
    var_s2_2 = *temp_s7;
    var_s0_2 = 0;
    if (var_s2_2 != 0) {
        var_s1_2 = D_80161D9C;
        do {
            if (*var_s1_2 == 0) {
                temp_s4 = &D_80127080[var_s0_2];
                temp_s3 = sp50 + D_80161E60;
                func_80099E30((s32) &D_8015EDB4[sp64].unk14, &D_80127480[var_s0_2]);
                *var_s1_2 = (u8) sp64;
                if ((a == 0xF) || (a == 0x10)) {
                    temp_v0_2 = *temp_s3 + ((*temp_s7 - var_s2_2) * 8);
                    func_8015B1B8(var_s0_2, (*(s32 *)temp_v0_2), *(s32 *)((unsigned char *)temp_v0_2 + (4)), 0);
                } else {
                    func_8015B2AC(var_s0_2, *(s32 *)(*temp_s3 + (*temp_s7 * 4) + -(var_s2_2 * 4)));
                }
                func_8009B430(temp_fp_2, (u8 *) temp_s4, *temp_s7 - var_s2_2);
                var_s2_2 -= 1;
                temp_s4->unk4[0] = (s32) &D_80127480[var_s0_2];
                temp_s4->unk0 = 0x80000000;
            }
            var_s0_2 += 1;
            var_s1_2 += 1;
        } while (var_s2_2 != 0);
    }
    D_8015EDB4[sp64].unk4.unk0 = 0x80000000;
    D_8015EDB4[sp64].unk2 = (u8) b;
    D_8015EDB4[sp64].unk7A = 0x7FFF;
    D_8015EDB4[sp64].unk84[2] = (u8) a;
    D_8015EDB4[sp64].unk3 = 1;
    D_8015EDB4[sp64].unk82 = 0;
    D_8015EDB4[sp64].unk72 = 0;
    D_8015EDB4[sp64].unk7C = c_unk0;
    D_8015EDB4[sp64].unk7E = c_unk0;
    D_8015EDB4[sp64].unk80 = c_unk4;
    D_8015EDB4[sp64].unk0[1] = 0x80;
    D_8015EDB4[sp64].unk74 = d_unk0;
    D_8015EDB4[sp64].unk76 = d_unk0;
    D_8015EDB4[sp64].unk78 = d_unk4;
    D_8015EDB4[sp64].unk64 = e_unk0;
    D_8015EDB4[sp64].unk66 = e_unk0;
    D_8015EDB4[sp64].unk68 = e_unk4;
    D_8015EDB4[sp64].unk6A = e_unk4;
    temp_v0_3 = (a * 2) + &D_80161EA4;
    D_8015EDB4[sp64].unk6C = *temp_v0_3;
    D_8015EDB4[sp64].unk6E = *temp_v0_3;
    D_8015EDB4[sp64].unk70 = *temp_v0_3;
    D_8015EDB4[sp64].unk84[1] = (u8) f;
    D_8015EDB4[sp64].unk84[0] = *(&D_80161EC8 + a);
    D_8015EDB4[sp64].unk84[3] = g | 1;
    temp_v1 = &D_8015EDB4[sp64];
    temp_v1->unk4.unk4[0] = (s32) &temp_v1->unk14;
    return sp64;
}

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

/* NON_MATCHING: T-10850, 9 differing listing lines; see the ticket scratch evidence. */
#ifdef NON_MATCHING
/* FAKE: repeated empty tests split the actor pointer live range. T-10850 */
void func_8015C0A0(arg0, arg1)
s32 arg0;
s32 arg1;
{
  TcActor *a;
  s32 i;
  a = &D_8015EDB4[arg1];
  if (((!a) && (!a)) && (!a))
  {
  }
  if (a->unk0[1] & 0x80)
  {
    if (arg0 == 1)
    {
      if (a->unk84[2] < 0xB)
      {
        *((s32 *) (((u8 *) a) + 4)) = 0;
        return;
      }
      *((s32 *) (((u8 *) a) + 4)) = 0x80000000;
      for (i = 0; i < 0x40; i++)
      {
        if (arg1 == D_80161D9C[i])
        {
          D_80127080[i].unk0 = (u16) 0;
        }
      }

      return;
    }
    if (a->unk84[2] < 9)
    {
      *((s32 *) (((u8 *) a) + 4)) = 0x80000000;
      return;
    }
    *((s32 *) (((u8 *) a) + 4)) = 0x80000000;
    for (i = 0; i < 0x40; i++)
    {
      if (arg1 == D_80161D9C[i])
      {
        D_80127080[i].unk0 = 0x80000000;
      }
    }

  }
}
#else
INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8015ABF0", func_8015C0A0);
#endif

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
