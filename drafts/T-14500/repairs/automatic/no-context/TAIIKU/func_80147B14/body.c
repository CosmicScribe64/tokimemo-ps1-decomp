void func_80147B14(s32 arg0) {
    s32 sp78;
    s32 sp74;
    s32 sp60[5];
    s32 sp4C[5];
    s32 sp3C[4];
    void *sp14;
    s32 *var_t2;
    s32 *var_v1_3;
    s16 temp_a2;
    s16 temp_t6_3;
    s32 temp_a0;
    s32 temp_a3;
    s32 temp_t8;
    s32 var_v1;
    s32 var_v1_2;
    u16 *var_a1;
    u16 *var_a1_2;
    u16 temp_t6_2;
    u16 temp_t7;
    u8 *temp_a1;
    u8 *temp_t2;
    u8 *temp_t6;

    (*(s32 *)&sp60) = (s32) D_8014A620.unk0;
    *(s32 *)((unsigned char *)(&sp60) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8014A620) + (4));
    *(s32 *)((unsigned char *)(&sp60) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8014A620) + (8));
    *(s32 *)((unsigned char *)(&sp60) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_8014A620) + (12));
    *(s32 *)((unsigned char *)(&sp60) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_8014A620) + (16));
    (*(s32 *)&sp4C) = (s32) D_8014A634.unk0;
    *(s32 *)((unsigned char *)(&sp4C) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8014A634) + (4));
    *(s32 *)((unsigned char *)(&sp4C) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8014A634) + (8));
    *(s32 *)((unsigned char *)(&sp4C) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_8014A634) + (12));
    *(s32 *)((unsigned char *)(&sp4C) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_8014A634) + (16));
    (*(s32 *)&sp3C) = (s32) D_8014A648.unk0;
    *(s32 *)((unsigned char *)(&sp3C) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8014A648) + (4));
    *(s32 *)((unsigned char *)(&sp3C) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8014A648) + (8));
    *(s32 *)((unsigned char *)(&sp3C) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_8014A648) + (12));
    sp2C[0].unk0 = (s32) D_8014A658.unk0;
    *(s32 *)((unsigned char *)(&sp2C[0]) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_8014A658) + (4));
    temp_t2 = (arg0 * 0x44) + &D_8011ECD0;
    *(s32 *)((unsigned char *)(&sp2C[0]) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_8014A658) + (8));
    *(s32 *)((unsigned char *)(&sp2C[0]) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_8014A658) + (12));
    temp_t6 = (arg0 * 0x1C) + &D_8014A454;
    sp14 = temp_t6;
    temp_t8 = *(s32 *)((unsigned char *)temp_t2 + (6568));
    sp74 = temp_t8;
    temp_a3 = arg0 * 0x13;
    sp78 = *(s32 *)((unsigned char *)temp_t2 + (6564)) + (*(s32 *)temp_t6);
    temp_a2 = *(s16 *)((unsigned char *)temp_t2 + (6552));
    sp74 = temp_t8 + *(s32 *)((unsigned char *)temp_t6 + (4));
    var_a1 = &D_8014A4C4;
    var_v1 = 0;
    do {
        temp_t6_2 = *var_a1;
        var_a1 += 2;
        if (temp_a2 == (temp_t6_2 + temp_a3)) {
            *(s16 *)((unsigned char *)(&sp74) + (2)) = (s16) (*(s16 *)((unsigned char *)(&sp74) + (2)) + *(&sp60 + var_v1));
            *(s16 *)((unsigned char *)(&sp78) + (2)) = (s16) (*(s16 *)((unsigned char *)(&sp78) + (2)) + *(&sp4C + var_v1));
        }
        var_v1 += 2;
    } while ((u32) var_a1 < (u32) &D_8014A4D4);
    var_a1_2 = &D_8014A4D8;
    var_v1_2 = 4;
    do {
        temp_t7 = *var_a1_2;
        var_a1_2 += 2;
        if (temp_a2 == (temp_t7 + temp_a3)) {
            *(s16 *)((unsigned char *)(&sp78) + (2)) = (s16) (*(s16 *)((unsigned char *)(&sp78) + (2)) + 0x16);
            *(s16 *)((unsigned char *)(&sp74) + (2)) = (s16) (*(s16 *)((unsigned char *)(&sp74) + (2)) - 4);
            *(s16 *)((unsigned char *)(&sp74) + (2)) = (s16) (*(s16 *)((unsigned char *)(&sp74) + (2)) + *(&sp3C + var_v1_2));
        }
        var_v1_2 += 2;
    } while ((u32) var_a1_2 < (u32) &D_8014A4E4);
    if (temp_a2 == (D_8014A4E8 + temp_a3)) {
        *(s16 *)((unsigned char *)(&sp74) + (2)) = (s16) (*(s16 *)((unsigned char *)(&sp74) + (2)) - 4);
        *(s16 *)((unsigned char *)(&sp78) + (2)) = (s16) (*(s16 *)((unsigned char *)(&sp78) + (2)) - 0xC);
        *(s16 *)((unsigned char *)(&sp74) + (2)) = (s16) (*(s16 *)((unsigned char *)(&sp74) + (2)) + sp30);
    }
    if (temp_a2 == (D_8014A4EA + temp_a3)) {
        *(s16 *)((unsigned char *)(&sp74) + (2)) = (s16) (*(s16 *)((unsigned char *)(&sp74) + (2)) - 4);
        *(s16 *)((unsigned char *)(&sp78) + (2)) = (s16) (*(s16 *)((unsigned char *)(&sp78) + (2)) - 0xC);
        *(s16 *)((unsigned char *)(&sp74) + (2)) = (s16) (*(s16 *)((unsigned char *)(&sp74) + (2)) + sp32);
    }
    temp_a1 = (4 * 2) + &D_8014A4E4;
    if (temp_a2 == ((*(s32 *)temp_a1) + temp_a3)) {
        *(s16 *)((unsigned char *)(&sp78) + (2)) = (s16) (*(s16 *)((unsigned char *)(&sp78) + (2)) - 0xC);
        *(s16 *)((unsigned char *)(&sp74) + (2)) = (s16) (*(s16 *)((unsigned char *)(&sp74) + (2)) - 4);
        *(s16 *)((unsigned char *)(&sp74) + (2)) = (s16) (*(s16 *)((unsigned char *)(&sp74) + (2)) + (&sp2C[0])[4].unk0);
    }
    if (temp_a2 == (*(u16 *)((unsigned char *)temp_a1 + (2)) + temp_a3)) {
        *(s16 *)((unsigned char *)(&sp78) + (2)) = (s16) (*(s16 *)((unsigned char *)(&sp78) + (2)) - 0xC);
        *(s16 *)((unsigned char *)(&sp74) + (2)) = (s16) (*(s16 *)((unsigned char *)(&sp74) + (2)) - 4);
        *(s16 *)((unsigned char *)(&sp74) + (2)) = (s16) (*(s16 *)((unsigned char *)(&sp74) + (2)) + *(s16 *)((unsigned char *)(&(&sp2C[0])[4]) + (2)));
    }
    if (temp_a2 == (*(u16 *)((unsigned char *)temp_a1 + (4)) + temp_a3)) {
        *(s16 *)((unsigned char *)(&sp78) + (2)) = (s16) (*(s16 *)((unsigned char *)(&sp78) + (2)) - 0xC);
        *(s16 *)((unsigned char *)(&sp74) + (2)) = (s16) (*(s16 *)((unsigned char *)(&sp74) + (2)) - 4);
        *(s16 *)((unsigned char *)(&sp74) + (2)) = (s16) (*(s16 *)((unsigned char *)(&sp74) + (2)) + *(s16 *)((unsigned char *)(&(&sp2C[0])[4]) + (4)));
    }
    if (temp_a2 == (*(u16 *)((unsigned char *)temp_a1 + (6)) + temp_a3)) {
        *(s16 *)((unsigned char *)(&sp78) + (2)) = (s16) (*(s16 *)((unsigned char *)(&sp78) + (2)) - 0xC);
        *(s16 *)((unsigned char *)(&sp74) + (2)) = (s16) (*(s16 *)((unsigned char *)(&sp74) + (2)) - 4);
        *(s16 *)((unsigned char *)(&sp74) + (2)) = (s16) (*(s16 *)((unsigned char *)(&sp74) + (2)) + *(s16 *)((unsigned char *)(&(&sp2C[0])[4]) + (6)));
    }
    *(s16 *)((unsigned char *)temp_t2 + (6838)) = (s16) *(s16 *)((unsigned char *)(&sp78) + (2));
    *(s16 *)((unsigned char *)temp_t2 + (6842)) = (s16) *(s16 *)((unsigned char *)(&sp74) + (2));
    var_v1_3 = &D_8014A400;
    var_t2 = &D_8011ECD0;
    do {
        temp_t6_3 = *(s16 *)((unsigned char *)var_v1_3 + (26));
        var_v1_3 += 0x1C;
        temp_a0 = temp_t6_3 - D_8014A3DE;
        if ((temp_a0 >= 0x1E1) || (temp_a0 < -0xA0)) {
            *(s8 *)((unsigned char *)var_t2 + (6871)) = 0;
        } else {
            *(s8 *)((unsigned char *)var_t2 + (6871)) = 0xA4;
        }
        var_t2 += 0x44;
    } while (var_v1_3 != &D_8014A454);
}
