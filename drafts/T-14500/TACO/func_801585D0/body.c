void func_801585D0(s32 arg0) {
    s32 sp44;
    s32 sp40;
    s16 sp3C;
    s16 sp3A;
    s16 sp38;
    s32 sp30;
    TcActor *temp_v1;
    TcActor *temp_v1_2;
    TcActor *temp_v1_3;
    TcActor *temp_v1_4;
    TcActor *temp_v1_5;
    TcActor *temp_v1_6;
    TcActor *temp_v1_7;
    TcActor *var_v1;
    s16 temp_a0;
    s16 temp_v0;
    s16 temp_v0_2;
    s32 temp_t0;
    s32 var_at;

    temp_t0 = arg0 * 0x88;
    temp_v1 = &D_8015EDB4[arg0];
    temp_v0 = temp_v1->unk7C;
    switch (temp_v0) {
    case 0:
        if (temp_v1->unk6A & 1) {
            temp_v1->unk74 = -0x1770;
        } else {
            temp_v1->unk74 = 0x1770;
        }
        temp_v1_2 = &D_8015EDB4[arg0];
        if (temp_v1_2->unk6A & 0x10) {
            temp_v1_2->unk76 = -0x898;
        } else {
            temp_v1_2->unk76 = 0x320;
        }
        D_8015EDB4[arg0].unk78 = 0;
        temp_v1_3 = &D_8015EDB4[arg0];
        temp_v1_3->unk7C += 1;
        break;
    case 1:
        if (temp_v1->unk6A & 1) {
            temp_v1->unk74 = (temp_v1->unk7A * 0x1E) - 0x1770;
        } else {
            temp_v1->unk74 = 0x1770 - (temp_v1->unk7A * 0x1E);
        }
        temp_v1_4 = &D_8015EDB4[arg0];
        if (temp_v1_4->unk6A & 0x10) {
            temp_v1_4->unk76 = (temp_v1_4->unk7A * 5) - 0x898;
        } else {
            temp_v1_4->unk76 = 0x320 - (temp_v1_4->unk7A * 5);
        }
        temp_v1_5 = &D_8015EDB4[arg0];
        temp_v1_5->unk78 = -temp_v1_5->unk7A * 0x1E;
        temp_v1_6 = &D_8015EDB4[arg0];
        if (temp_v1_6->unk7A >= 0xC9) {
            temp_v1_6->unk7A = 0;
            temp_v1_7 = &D_8015EDB4[arg0];
            temp_v1_7->unk7C += 1;
        }
        break;
    case 2:
        temp_v0_2 = temp_v1->unk6A;
        switch (temp_v0_2) {
        case 0:
            sp40 = -0x1F4;
            sp44 = temp_v1->unk7A * 0x14;
            break;
        case 1:
            sp40 = -0x1F4;
            sp44 = temp_v1->unk7A * -0x14;
            break;
        case 16:
            sp40 = 0x1F4;
            sp44 = temp_v1->unk7A * -0x14;
            break;
        case 17:
            sp40 = 0x1F4;
            sp44 = temp_v1->unk7A * 0x14;
            break;
        }
        sp30 = temp_t0;
        D_8015EDB4[arg0].unk74 = (s16) ((s32) (func_800A0070(sp44) * sp40) >> 0xC);
        D_8015EDB4[arg0].unk76 = ((s32) (func_800A0140(sp44) * -sp40) >> 0xC) - 0x2BC;
        var_v1 = &D_8015EDB4[arg0];
        temp_a0 = var_v1->unk7A;
        var_at = temp_a0 < 0x19A;
        if ((temp_a0 % 20) == 0) {
            sp38 = var_v1->unk74;
            sp3A = var_v1->unk76;
            sp3C = var_v1->unk78;
            sp30 = temp_t0;
            func_8013DB80(subroutine_arg0, subroutine_arg1, 0x100, 0xC0);
            var_v1 = &D_8015EDB4[arg0];
            var_at = var_v1->unk7A < 0x19A;
        }
        if (var_at == 0) {
            var_v1->unk7C += 1;
        }
        break;
    case 3:
        sp30 = temp_t0;
        func_801589B0();
        D_8015EDB4[arg0].unk2 = 0xFE;
        break;
    }
}
