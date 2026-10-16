s32 func_80141DE0(s32 arg0) {
    Tc14 *temp_v0;
    Tc14 *temp_v0_2;
    Tc14 *var_v0_2;
    s16 temp_v0_3;
    s32 var_v0;
    u32 var_v1;

    if (func_80142220(arg0) == 0) {
        return 0;
    }
    if ((func_80142220(arg0) != 4) && (func_80142220(arg0) != 3)) {
        if (func_80142220(arg0) == 1) {
            if (!(D_800E7378 & 3) && (D_80160464 >= 0x41)) {
                func_8013CE78();
                func_801335A0(1, 0x505);
            }
        } else if (!(D_8015E770[0].unk12 & 3)) {
            var_v1 = (u8) D_8015E770[0].unk12 >> 2;
            if ((u8) D_8015E770[0].unk12 >= 0x81U) {
                func_8013CFD8();
                func_801335A0(1, 0x504);
                var_v1 = (u8) D_8015E770[0].unk12 >> 2;
            }
            if ((s32) var_v1 < 0x10) {
                temp_v0 = &D_8015E770[var_v1];
                temp_v0->unk13 = 0x90;
                temp_v0->unk0 = D_8015EDB4->unk74;
                temp_v0->unk2 = D_8015EDB4->unk76 + 0x10;
                temp_v0->unk8 = 0;
                temp_v0->unk4 = D_8015EDB4->unk78 - 0x64;
                temp_v0->unkA = 0;
                temp_v0->unkC = -0x20;
                temp_v0->unk12 = 0;
            }
            temp_v0_2 = &D_8015E770[var_v1];
            if (((u8) temp_v0_2->unk12 >= 0x11U) && ((u8) D_8015E770[0].unk12 < 0x80U)) {
                temp_v0_2->unkC = -0x100;
            }
            temp_v0_3 = D_8015EDB4[8].unk74;
            D_8015EDB4[8].unk74 = (s16) (temp_v0_3 + ((s32) (D_8015EDB4->unk74 - temp_v0_3) / 2));
            D_8015EDB4[8].unk76 = (s16) (D_8015EDB4[8].unk76 + ((s32) (D_8015EDB4->unk76 - D_8015EDB4[8].unk76) / 2));
            D_8015EDB4[8].unk78 = (s16) (D_8015EDB4[8].unk78 + ((s32) ((D_8015EDB4->unk78 - D_8015EDB4[8].unk78) - 0x200) / 2));
        }
    }
    if (D_80160460 != 0) {
        var_v0 = func_80154014();
        if (var_v0 == 0) {
            D_8015EDB4->unk2 &= 0xFF3F;
            func_8013CE78();
            return func_801420DC();
        }
        /* Duplicate return node #25. Try simplifying control flow for better match */
        return var_v0;
    }
    var_v0 = func_801423B0();
    if (var_v0 == 0) {
        var_v0_2 = D_8015E770;
        D_8015EDB4->unk2 &= 0xFF3F;
        do {
            var_v0_2 += 4;
            *(s8 *)((unsigned char *)var_v0_2 + (-41)) = 0;
            *(s8 *)((unsigned char *)var_v0_2 + (-21)) = 0;
            *(s8 *)((unsigned char *)var_v0_2 + (-1)) = 0;
            *(s8 *)((unsigned char *)var_v0_2 + (-61)) = 0;
        } while (var_v0_2 != D_8015E8B0);
        func_8013CFD8();
        var_v0 = func_801420DC();
    }
    return var_v0;
}
