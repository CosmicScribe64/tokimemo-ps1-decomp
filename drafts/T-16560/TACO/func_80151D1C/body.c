s32 func_80151D1C(u32 arg0) {
    s32 *var_s1;
    s32 *var_s1_2;
    s32 *var_s2;
    s32 *var_s2_2;
    s32 temp_s0;
    s32 temp_s0_2;
    s32 var_s0;
    s32 var_s0_2;
    u8 var_v1;

    var_s0 = 0x16;
    if (arg0 < (u8) D_8015EDB4[16].unk3) {
        var_s2 = &D_801603D4;
        var_s1 = &D_80160340;
        do {
            func_801472D4(var_s0, (s32) D_8015EDB4[16].unk2, *(s32 *)((unsigned char *)var_s1 + (-8)), *(s32 *)((unsigned char *)var_s1 + (-4)), (s32) *(u8 *)((unsigned char *)var_s2 + (-2)), (s32) *(u8 *)((unsigned char *)var_s2 + (-1)));
            var_s0 += 1;
            var_s1 += 2;
            var_s2 += 2;
        } while (var_s0 != 0x1C);
        var_v1 = D_8015EDB4[16].unk2;
        if ((var_v1 % 10U) == 0) {
            func_801335A0(0, 0x502);
            var_v1 = D_8015EDB4[16].unk2;
        }
        D_8015EDB4[16].unk2 = (u8) (var_v1 + 1);
        D_8015EDB4[16].unk3 = 0U;
        if (((u8) D_8015EDB4[16].unk2 % 3U) == 0) {
            temp_s0 = func_800AE0D0();
            func_80147C98(0, 0x16, (s16) ((temp_s0 & 0x1FF) - 0x100), (s16) ((func_800AE0D0() & 0x1FF) - 0x100), 0x3E8, 0xC8, 0xC4);
            temp_s0_2 = func_800AE0D0();
            func_80147C98(0, 0x17, (s16) ((temp_s0_2 & 0x1FF) - 0x100), (s16) ((func_800AE0D0() & 0x1FF) - 0x100), 0x3E8, 0xC8, 0xC4);
        }
    }
    if ((u8) D_8015EDB4[16].unk2 >= 0x1EU) {
        D_8015EDB4[16].unk2 = 0U;
        var_s2_2 = &D_801603D4;
        var_s1_2 = &D_80160340;
        var_s0_2 = 0x16;
        D_8015EDB4[16].unk3 = (u8) D_8015EDB4[16].unk2;
        do {
            func_801472D4(var_s0_2, 0, *(s32 *)((unsigned char *)var_s1_2 + (-8)), *(s32 *)((unsigned char *)var_s1_2 + (-4)), (s32) *(u8 *)((unsigned char *)var_s2_2 + (-2)), (s32) *(u8 *)((unsigned char *)var_s2_2 + (-1)));
            var_s0_2 += 1;
            var_s1_2 += 2;
            var_s2_2 += 2;
        } while (var_s0_2 != 0x1C);
        if (D_8015EDB4[17].unk84[2] == 1) {
            *(s8 *)&D_8015EDB4[16].unk84[2] = 1;
        }
    } else {
        D_8015EDB4[16].unk3 = (u8) (D_8015EDB4[16].unk3 + 1);
    }
}
