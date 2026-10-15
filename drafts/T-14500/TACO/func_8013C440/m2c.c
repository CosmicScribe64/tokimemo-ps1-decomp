extern s32 D_8015F500;

void func_8013C440(void) {
    TcActor *var_v1;
    TcActor *var_v1_2;
    s32 temp_s1;
    s32 temp_s1_2;
    s32 temp_s2;
    s32 temp_s2_2;
    s32 temp_t1;
    s32 temp_t4;
    s32 var_s0;
    s32 var_s0_2;
    u8 temp_v0;
    u8 temp_v1;

    D_8015F500 = 0;
    var_s0 = 0x880;
    if (!(D_8015EDB4->unk2 & 0x80)) {
        var_v1 = D_8015EDB4 + 0x880;
loop_2:
        if ((var_v1->pad0[1] & 0xC0) && (var_v1->unk84[1] != 0) && (temp_s2 = func_800AE0C0(var_v1->unk74 - D_8015EDB4->unk74), temp_s1 = func_800AE0C0(*(s16 *)((unsigned char *)(&D_8015EDB4->pad0[var_s0]) + (118)) - D_8015EDB4->unk76), var_v1 = (TcActor *) &D_8015EDB4->pad0[var_s0], temp_t4 = var_v1->unk84[0] * 0x10, ((func_800AE0C0(*(s16 *)((unsigned char *)(&D_8015EDB4->pad0[var_s0]) + (120)) - D_8015EDB4->unk78) < 0x100) != 0)) && (temp_s1 < temp_t4) && (temp_s2 < temp_t4)) {
            func_801335A0(1, 0x506);
            D_8015EDB4->unk2 |= 4;
            temp_v1 = D_8015EDB4->unk84[1];
            if (temp_v1 >= 0xDU) {
                D_8015EDB4->unk84[1] = temp_v1 - 0xC;
            } else {
                D_8015EDB4->unk84[1] = 0;
            }
            D_8015EDB4->unk7A = 0x10;
        } else {
            var_s0 += 0x88;
            var_v1 += 0x88;
            if (var_s0 != 0x2178) {
                goto loop_2;
            }
        }
    }
    var_s0_2 = 0;
    if (!(*(u8 *)((unsigned char *)D_8015EDB4 + (1090)) & 0x80)) {
        var_v1_2 = D_8015EDB4;
loop_14:
        if ((var_v1_2->pad0[1] & 0xC0) && (var_v1_2->unk84[1] != 0)) {
            temp_s2_2 = func_800AE0C0(var_v1_2->unk74 - *(s16 *)((unsigned char *)D_8015EDB4 + (1204)));
            temp_s1_2 = func_800AE0C0(*(s16 *)((unsigned char *)(&D_8015EDB4->pad0[var_s0_2]) + (118)) - *(s16 *)((unsigned char *)D_8015EDB4 + (1206)));
            var_v1_2 = (TcActor *) &D_8015EDB4->pad0[var_s0_2];
            temp_t1 = var_v1_2->unk84[0] * 0x10;
            if ((func_800AE0C0(*(s16 *)((unsigned char *)(&D_8015EDB4->pad0[var_s0_2]) + (120)) - *(s16 *)((unsigned char *)D_8015EDB4 + (1208))) < 0x100) && (temp_s1_2 < temp_t1)) {
                var_s0_2 += 0x88;
                if (temp_s2_2 < temp_t1) {
                    temp_v0 = *(u8 *)((unsigned char *)D_8015EDB4 + (1221));
                    if (temp_v0 >= 0xDU) {
                        *(u8 *)((unsigned char *)D_8015EDB4 + (1221)) = (u8) (temp_v0 - 0xC);
                    } else {
                        *(u8 *)((unsigned char *)D_8015EDB4 + (1221)) = 0U;
                    }
                    *(s16 *)((unsigned char *)D_8015EDB4 + (1210)) = 0x10;
                    return;
                }
                goto block_24;
            }
        }
        var_s0_2 += 0x88;
block_24:
        var_v1_2 += 0x88;
        if (var_s0_2 == 0x880) {

        } else {
            goto loop_14;
        }
    }
}
