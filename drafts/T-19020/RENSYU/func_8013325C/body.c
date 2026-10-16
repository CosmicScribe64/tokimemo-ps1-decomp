/* T-19020 draft, about 100 differing lines. Frame, table offset (0x40) and the n slot (0x64) match with the locals declared
 * in this order before the table; case 0 stores its constants through v1 in the original (D_801342C4 as a register
 * range), here they go through ugen temporaries; case 1 pointers and the hoisted constants 1 and &D_801342C8 sit in other
 * registers. Needs in include/ovl/RENSYU.h:
 *   typedef struct RensyuTbl11 { u16 v[11]; } RensyuTbl11;  extern RensyuTbl11 D_80134378;  extern u8 D_801342C8[]; */
void func_8013325C(void) {
    u8 *p1;
    u8 *p2;
    s32 n;
    u32 i;
    u8 *p;
    u8 t;
    s16 diff;
    RensyuTbl11 tbl;

    tbl = D_80134378;
    switch (D_800E6280.unk_110A) {
    case 0:
        diff = D_800E6280.unk_66C[(u16) D_800E6280.unk_0F4.h >> 0xC].unk_00 - tbl.v[((RensyuClubCount *)&D_800E6280.unk_66C[(u16) D_800E6280.unk_0F4.h >> 0xC])->count];
        if (diff < -0x1E) {
            D_801342C4 = 0;
        } else if (diff < -0x14) {
            D_801342C4 = 1;
        } else if (diff < -0xA) {
            D_801342C4 = 1;
        } else if (diff < 0) {
            D_801342C4 = 2;
        } else if (diff >= 0x1E) {
            D_801342C4 = 5;
        } else if (diff >= 0x14) {
            D_801342C4 = 4;
        } else if (diff >= 0xA) {
            D_801342C4 = 4;
        } else if (diff >= 0) {
            D_801342C4 = 3;
        }
        func_8004284C();
        break;
    case 1:
        p = D_801342C8;
        do {
            *p++ = 0;
        } while (p < &D_801342C8[5]);
        i = 0;
        if (D_801342C4 != 0) {
            p = D_801342C8;
            do {
                i += 1;
                *p++ = 1;
            } while (i < D_801342C4);
            i = 0;
        }
        n = func_800AE0D0() & 0xF;
        if (D_801342C4 >= 3U) {
            t = D_801342C8[0];
            D_801342C8[0] = D_801342C8[4];
            D_801342C8[4] = t;
        }
        if (n > 0) {
            do {
                p1 = &D_801342C8[func_800AE0D0() & 3];
                p2 = &D_801342C8[func_800AE0D0() & 3];
                t = *p1;
                i += 1;
                *p1 = *p2;
                *p2 = t;
            } while (i != n);
        }
        if (((u16) D_800E6280.unk_0F4.h >> 0xC) == 9) {
            if (D_801342C4 >= 3U) {
                D_801342C8[0] = 1;
            } else {
                D_801342C8[0] = 0;
            }
        }
        func_8004284C();
        break;
    case 2:
        func_80048F64(0x60);
        ((DisplayActor44 *)D_8011ECD0)[96].unk_01 = 3;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_38 = 0x01000000;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_07 = 0x80;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_05 = 0x80;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_1A = 0x1000;
        *(s16 *)&((DisplayActor44 *)D_8011ECD0)[96].pad_1C[0] = 0x1000;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_06 = 1;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_43 = ((RensyuClubCount *)&D_800E6280.unk_66C[(u16) D_800E6280.unk_0F4.h >> 0xC])->count % 5U + 0x10;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_26 = -0x60;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_2A = 0x20;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_0C = (u8 *) D_801343D4;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_10 = (u8 *) D_801343D8;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_34 = (u8 *) D_801343D0;
        ((DisplayActor44 *)D_8011ECD0)[96].unk_14 = D_801343DC;
        D_801342C0 = 0;
        func_80042808();
        break;
    default:
        func_80046500();
        break;
    }
    Default_Disp();
}
