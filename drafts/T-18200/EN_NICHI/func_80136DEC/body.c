void func_80136DEC(void) {
    EnHalf22 spC4;
    EnHalf22 sp98;
    EnHalf22 sp6C;
    EnBytes22 sp54;
    DisplayActor44 *sp4C;
    s16 *sp44;
    s16 *sp40;
    s16 *sp3C;
    DisplayActor44 *var_v0;
    DisplayActor44 *var_v0_2;
    DisplayActor44 *var_v0_3;
    s16 *var_a1;
    s16 *var_a2;
    s16 *var_v1;
    s16 temp_t2;
    s16 temp_t5;
    s16 temp_t9;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_s1;
    u8 *var_s0_3;
    u8 temp_t0;
    u8 temp_t1;
    u8 temp_t2_2;
    u8 temp_t4;
    u8 temp_t8;
    u8 temp_v1;

    temp_v1 = D_800E6280.unk_110A;
    spC4 = D_80139C68;
    sp98 = D_80139C94;
    sp6C = D_80139CC0;
    sp54 = D_80139CEC;
    switch (temp_v1) {
    case 0:
        load_tpage_buf_lock(5, *(s32 *)((unsigned char *)D_801391E4 + (28)), 1);
        var_s0 = 0;
        do {
            search_load_tpage_buf_lock(*(s32 *)(D_801391E4 + var_s0), 1);
            var_s0 += 4;
        } while (var_s0 < 0x1C);
        var_s0_2 = 0x20;
        do {
            search_load_tpage_buf_lock(*(s32 *)(D_801391E4 + var_s0_2), 1);
            var_s0_2 += 4;
        } while (var_s0_2 != 0x28);
        func_8004284C();
        return;
    case 1:
        var_v0 = ((DisplayActor44 *)D_8011ECD0);
        var_s1 = 0;
        var_s0_3 = sp54.value;
        var_v1 = spC4.value;
        var_a1 = sp98.value;
        var_a2 = sp6C.value;
        do {
            sp4C = var_v0;
            sp44 = var_v1;
            sp40 = var_a1;
            sp3C = var_a2;
            func_80048F64(var_s1 + 0x60);
            var_v0[96].unk_38 = 0x01000000;
            var_v0[96].unk_07 = 0;
            var_v0[96].unk_02 = 0x40;
            var_v0[96].unk_03 = 4;
            var_v0[96].unk_01 = (u8) *var_s0_3;
            *(s32 *)&var_v0[96].unk_0C = (s32) D_801391E8;
            *(s32 *)&var_v0[96].unk_10 = (s32) D_801391EC;
            *(s32 *)&var_v0[96].unk_34 = (s32) D_801391E4;
            temp_t9 = *var_v1;
            temp_t2 = *var_a1;
            temp_t5 = *var_a2;
            var_s1 += 1;
            var_s0_3 += 1;
            var_v0 += 1;
            var_v0[95].unk_04 = 0;
            var_v0[95].unk_05 = 0x80;
            var_v0[95].unk_1A = 0x1000;
            *(s16 *)((u8 *)&var_v0[95] + 28) = 0x1000;
            var_v0[95].unk_06 = 0;
            var_v0[95].unk_14 = (s16) D_801391F0;
            var_v1 += 1;
            var_a1 += 1;
            var_a2 += 1;
            var_v0[95].unk_18 = temp_t9;
            var_v0[95].unk_26 = temp_t2;
            var_v0[95].unk_2A = temp_t5;
        } while (var_s1 < 0x16);
        ((DisplayActor44 *)D_8011ECD0)[96].unk_03 = (u8) (((DisplayActor44 *)D_8011ECD0)[96].unk_03 | 0x80);
        var_v0_2 = &((DisplayActor44 *)D_8011ECD0)[1];
        do {
            temp_t0 = var_v0_2[97].unk_03;
            temp_t8 = var_v0_2[98].unk_03;
            temp_t2_2 = var_v0_2[99].unk_03;
            temp_t1 = var_v0_2[96].unk_03;
            var_v0_2 += 4;
            var_v0_2[95].unk_03 = (s8) (temp_t2_2 | 0x80);
            var_v0_2[94].unk_03 = (s8) (temp_t8 | 0x80);
            var_v0_2[93].unk_03 = (s8) (temp_t0 | 0x80);
            var_v0_2[92].unk_03 = (s8) (temp_t1 | 0x80);
        } while (var_v0_2 != (&((DisplayActor44 *)D_8011ECD0)[21]));
        func_8004284C();
        return;
    case 2:
        if ((u8) ((DisplayActor44 *)D_8011ECD0)[96].unk_07 >= 0x80U) {
            D_800E6280.unk_03A = 0x80;
            ((DisplayActor44 *)D_8011ECD0)[96].unk_07 = 0x80U;
            func_8004284C();
        } else {
            ((DisplayActor44 *)D_8011ECD0)[96].unk_07 = (u8) (((DisplayActor44 *)D_8011ECD0)[96].unk_07 + 4);
        }
        var_v0_3 = &((DisplayActor44 *)D_8011ECD0)[2];
        ((DisplayActor44 *)D_8011ECD0)[97].unk_07 = (u8) ((DisplayActor44 *)D_8011ECD0)[96].unk_07;
        do {
            var_v0_3[96].unk_07 = (u8) ((DisplayActor44 *)D_8011ECD0)[96].unk_07;
            var_v0_3[97].unk_07 = (u8) ((DisplayActor44 *)D_8011ECD0)[96].unk_07;
            var_v0_3[98].unk_07 = (u8) ((DisplayActor44 *)D_8011ECD0)[96].unk_07;
            var_v0_3 += 4;
            var_v0_3[95].unk_07 = (u8) ((DisplayActor44 *)D_8011ECD0)[96].unk_07;
        } while (var_v0_3 != (&((DisplayActor44 *)D_8011ECD0)[22]));
        return;
    case 3:
        temp_t4 = D_800E6280.unk_110D;
        switch (temp_t4) {
        case 0:
            func_80046290(0x07000002, 0x03000008, 0xE);
            func_80044750(0x300);
            D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
            return;
        case 1:
        case 3:
            if (func_800460EC() == 4) {
                D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
                return;
            }
        default:
            return;
        case 2:
            if (func_800460EC() == 2) {
                func_80046290(0x08000001, 0x02000004, 0xE);
                func_80044750(0x300);
                D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
                return;
            }
            break;
        case 4:
            if (func_800460EC() == 2) {
                D_80139C58 = 0;
                D_80139C30 = 5;
                *D_80139C34 = 0;
                *D_80139C3C = 0;
                *D_80139C44 = 0;
                *(s8 *)((unsigned char *)(&D_80139C44[1]) + (1)) = 0;
                *(s8 *)((unsigned char *)(&D_80139C44[1]) + (2)) = 0;
                *(s8 *)((unsigned char *)(&D_80139C44[1]) + (3)) = 0;
                *(s8 *)((unsigned char *)(&D_80139C3C[1]) + (1)) = 0;
                *(s8 *)((unsigned char *)(&D_80139C3C[1]) + (2)) = 0;
                *(s8 *)((unsigned char *)(&D_80139C3C[1]) + (3)) = 0;
                *(s8 *)((unsigned char *)(&D_80139C34[1]) + (1)) = 0;
                *(s8 *)((unsigned char *)(&D_80139C34[1]) + (2)) = 0;
                *(s8 *)((unsigned char *)(&D_80139C34[1]) + (3)) = 0;
                D_80139C34[1] = 0;
                D_80139C3C[1] = 0;
                D_80139C44[1] = 0;
                ((DisplayActor44 *)D_8011ECD0)->unk_26 = 0;
                ((DisplayActor44 *)D_8011ECD0)->unk_2A = 0;
                func_80042808();
                return;
            }
            break;
        }
        break;
    default:
        func_80046500();
        break;
    }
}
