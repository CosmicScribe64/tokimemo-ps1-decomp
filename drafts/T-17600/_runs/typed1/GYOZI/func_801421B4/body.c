void func_801421B4(void) {
    s16 temp_t9;
    s16 var_v0;
    s16 var_v1;
    s16 var_v1_2;
    s32 temp_v0_2;
    s32 var_v0_2;
    u8 *var_a0;
    u8 temp_t4;
    u8 temp_t6;
    u8 temp_t7;
    u8 temp_t8;
    u8 temp_v0;
    u8 temp_v1;

    temp_t6 = D_800F6468 & 3;
    var_v1 = 1;
loop_1:
    temp_t8 = ((D_800F6468 * 2) + var_v1) & 3 & 0xFF;
    var_v0 = var_v1;
    if (temp_t6 == temp_t8) {
        var_v1 += 1;
        goto loop_1;
    }
loop_4:
    temp_t7 = (((u32) D_800F6468 / 10U) + var_v0) & 3 & 0xFF;
    if (temp_t6 == temp_t7) {
block_6:
        temp_t9 = var_v1 + 1;
        var_v1 = temp_t9;
        var_v0 = temp_t9;
        goto loop_4;
    }
    if (temp_t8 == temp_t7) {
        goto block_6;
    }
loop_7:
    temp_t4 = (var_v1 % 4) & 0xFF;
    if (temp_t6 == temp_t4) {
block_11:
        var_v1 += 1;
        goto loop_7;
    }
    if (temp_t8 == temp_t4) {
        goto block_11;
    }
    D_80148828[3] = temp_t4;
    if (temp_t7 == temp_t4) {
        goto block_11;
    }
    (*(s32 *)D_80148828) = temp_t6;
    D_80148828[1] = temp_t8;
    D_80148828[2] = temp_t7;
    var_v1_2 = 0;
    var_a0 = D_80148828;
    do {
        temp_v0 = *var_a0;
        if (temp_v0 == 1) {
            *var_a0 = 0xB;
        } else if (temp_v0 == 2) {
            *var_a0 = D_80148754;
        } else if (temp_v0 == 3) {
            *var_a0 = D_80148758;
        }
        var_v1_2 += 1;
        var_a0 = &D_80148828[var_v1_2];
    } while (var_v1_2 < 4);
    func_8005BE34(0, ((DisplayActor44 *)D_80129F40)->unk_26, ((DisplayActor44 *)D_80129F40)->unk_2A, D_80148828);
    func_8005C1B8(0);
    if (D_800F62F8 & 0x600060) {
        if (func_8005B32C() != 0) {
            if (D_800F6306 != 0) {
                D_80148818 = 0;
                goto block_31;
            }
            if (D_800F6316 != 0) {
                D_80148818 = 1;
                goto block_31;
            }
            if (D_800F6326 != 0) {
                D_80148818 = 2;
                goto block_31;
            }
            if (D_800F6336 != 0) {
                D_80148818 = 3;
block_31:
                ((DisplayActor44 *)D_80129F40)->unk_03 = ((DisplayActor44 *)D_80129F40)->unk_03 & 0xFF7F;
                D_8012E6C0 = 0;
                func_8005B06C(1);
                ((DisplayActor44 *)D_80129F40)[97].unk_03 = (u8) (((DisplayActor44 *)D_80129F40)[97].unk_03 | 0x80);
                temp_v1 = D_80148828[D_80148818];
                var_v0_2 = temp_v1 == 0xB;
                if (var_v0_2 != 0) {
                    var_v0_2 = D_800F53DE == 0x5F;
                }
                ((DisplayActor44 *)D_80129F40)[97].unk_18 = (s16) (((temp_v1 + ((D_800F53DE == 0x60) * 0xB) + ((D_800F53DE == 0x61) * 0x17)) - var_v0_2) + 1);
                ((DisplayActor44 *)D_80129F40)[97].unk_26 = (s16) ((D_80148818 * 0x4D) - 0x75);
                ((DisplayActor44 *)D_80129F40)[97].unk_2A = -0x3C;
                *(s16 *)((unsigned char *)((DisplayActor44 *)D_80129F40) + (6624)) = 0;
                D_80148620 = (D_80148620 + (temp_v1 * 3) + D_800F53DE) - 0x5F;
                D_80148820 = 1;
                D_80148818 = D_80148818;
                if (((D_800F53DE == 0x5F) && (temp_v1 == 0) && (D_800F54A2 >= 0x64)) || (((temp_v1 == 0) & (D_800F53DE == 0x5F)) && ((u16) D_800F5A10 >= 0x64U)) || ((temp_v0_2 = temp_v1 == 1, ((temp_v0_2 & (D_800F53DE == 0x5F)) != 0)) && (D_800F549A >= 0x46)) || ((temp_v0_2 & (D_800F53DE == 0x61)) && (D_800F549A >= 0x64)) || (((temp_v1 == 2) & (D_800F53DE == 0x60)) && (D_800F549A >= 0x64))) {
                    D_80148624 = 1;
                }
                func_8004DE1C();
            }
        }
    }
}
