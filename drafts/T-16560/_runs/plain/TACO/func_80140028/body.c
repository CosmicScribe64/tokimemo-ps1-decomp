void func_80140028(s32 arg0) {
    s32 sp6C;
    s32 sp8C;
    s32 sp80;
    s32 sp7C;
    s32 sp78;
    s32 sp68;
    Tc14 *var_s0;
    Tc14 *var_s3;
    TcActor *temp_v0_2;
    TcActor *var_s4;
    s32 var_s1;
    u8 temp_t5;
    u8 temp_t7;
    u8 temp_v0;
    u8 temp_v1;

    if (arg0 == 0) {
        var_s4 = D_8015EDB4;
        var_s3 = D_8015E4F0;
    } else {
        var_s3 = D_8015E630;
        var_s4 = D_8015EDB4 + 0x440;
    }
    var_s1 = 0;
    var_s0 = var_s3;
    do {
        temp_t7 = var_s0->unk12 + 1;
        var_s0->unk12 = temp_t7;
        if ((temp_t7 & 0xFF) == 0xFF) {
            var_s0->unk13 = 0;
        }
        if (var_s0->unk4 < -0x7D00) {
            var_s0->unk13 = 0;
        }
        if ((var_s4->unk78 + 0x400) < var_s0->unk4) {
            var_s0->unk13 = 0;
        }
        temp_v1 = var_s0->unk13;
        if (temp_v1 & 0x80) {
            temp_v0 = var_s0->unk12;
            if ((temp_v1 & 1) && (temp_v0 >= 0x11U)) {
                if (!(D_800E7378 & 3)) {
                    temp_t5 = var_s0->unk11;
                    sp8C = (s32) temp_t5;
                    if (temp_t5 != -1U) {
                        temp_v0_2 = &D_8015EDB4[temp_t5];
                        if ((temp_v0_2->unk78 < var_s0->unk4) && (temp_v0_2->unk0[1] & 0x80) && (temp_v0_2->unk84[3] & 1)) {
                            sp78 = temp_v0_2->unk74 - var_s0->unk0;
                            sp80 = 0;
                            sp7C = temp_v0_2->unk76 - var_s0->unk2;
                            func_800AA460((s32) &sp78, (s32) &sp68);
                            var_s0->unk8 = (s16) ((s32) (var_s0->unk8 + (sp68 / 64)) / 2);
                            if (var_s0->unk8 >= 0x41) {
                                var_s0->unk8 = 0x40;
                            }
                            var_s0->unkA = (s16) ((s32) (var_s0->unkA + (sp6C / 64)) / 2);
                            if (var_s0->unkA >= 0x41) {
                                var_s0->unk8 = 0x40;
                            }
                        }
                    } else {
                        func_801407D4((s32) &sp8C, subroutine_arg1, subroutine_arg2);
                        var_s0->unk11 = (u8) sp8C;
                    }
                }
            } else if ((temp_v0 >= 0x11U) && (var_s0->unk11 == 0)) {
                var_s0->unk11 = 1;
                var_s0->unk12 = 0;
                var_s0->unkC = -0x200;
                var_s0->unk8 = (s16) ((s16) var_s0->unk8 / 16);
                var_s0->unkA = (s16) ((s16) var_s0->unkA / 16);
            }
            var_s0->unk0 += var_s0->unk8;
            var_s0->unk2 += var_s0->unkA;
            var_s0->unk4 += var_s0->unkC;
            func_80140ADC((s32) var_s3, var_s1);
        }
        var_s1 += 1;
        var_s0 += 0x14;
    } while (var_s1 != 0x10);
}
