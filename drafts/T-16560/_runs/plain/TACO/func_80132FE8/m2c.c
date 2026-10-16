extern s32 D_800E7200;
extern s32 D_800E7208;
extern s32 D_800E7378;

void func_80132FE8(s32 arg0, s32 arg1) {
    s32 sp30;
    TcActor *sp2C;
    s16 temp_v0;
    s16 temp_v0_2;
    s32 var_a0;
    s32 var_a1;
    s32 var_a1_2;
    s32 var_a2;
    s32 var_t8;
    s32 var_v1;

    sp2C = D_8015EDB4;
    if (D_8015EDB4->unk2 & 0x40) {
        var_a1 = sp30;
    } else {
        func_8013240C(arg0, arg1);
        func_801328B0();
        func_80140000();
        func_80140DF0();
        var_a1_2 = 0;
        if (D_8015EDD8 != 0) {
            if ((D_800E7200 & 0x20) && (((u8) D_8015EDE0 % 3U) == 0) && !(D_800E7378 & 3)) {
                var_a1_2 = 0x20;
            }
            if (D_800E7200 & 0x40) {
                var_t8 = D_800E7200 & 0x80;
                if (((u8) D_8015EDE0 % 3U) == 1) {
                    var_t8 = D_800E7200 & 0x80;
                    if (!(D_800E7378 & 3)) {
                        var_a1_2 |= 0x20;
                        goto block_11;
                    }
                }
            } else {
block_11:
                var_t8 = D_800E7200 & 0x80;
            }
            if ((var_t8 != 0) && (((u8) D_8015EDE0 % 3U) == 2) && !(D_800E7378 & 3)) {
                var_a1_2 |= 0x20;
            }
            var_v1 = D_800E7208 & 0x80;
            var_a0 = D_800E7208 & 0x40;
            var_a2 = D_800E7208 & 0x20;
        } else {
            var_a2 = D_800E7208 & 0x20;
            var_a0 = D_800E7208 & 0x40;
            if ((var_a2 != 0) && (((u8) D_8015EDE0 % 3U) == 0)) {
                var_a1_2 = 0x20;
            }
            if (var_a0 != 0) {
                var_v1 = D_800E7208 & 0x80;
                if (((u8) D_8015EDE0 % 3U) == 1) {
                    var_a1_2 |= 0x20;
                    goto block_23;
                }
            } else {
block_23:
                var_v1 = D_800E7208 & 0x80;
            }
            if ((var_v1 != 0) && (((u8) D_8015EDE0 % 3U) == 2)) {
                var_a1_2 |= 0x20;
            }
        }
        if ((var_a0 != 0) && (((u8) D_8015EDE4 % 3U) == 0)) {
            var_a1_2 |= 0x40;
        }
        if ((var_v1 != 0) && (((u8) D_8015EDE4 % 3U) == 1)) {
            var_a1_2 |= 0x40;
        }
        if ((var_a2 != 0) && (((u8) D_8015EDE4 % 3U) == 2)) {
            var_a1_2 |= 0x40;
        }
        temp_v0 = sp2C->unk72;
        if (temp_v0 == 0) {
            sp30 = var_a1_2;
            func_80141170(0, var_a1_2);
            var_a1 = var_a1_2;
        } else if (temp_v0 == 1) {
            sp30 = var_a1_2;
            func_80141290(0, var_a1_2);
            var_a1 = var_a1_2;
        } else {
            sp30 = var_a1_2;
            func_80141050(0, var_a1_2);
            var_a1 = var_a1_2;
        }
        temp_v0_2 = sp2C->unk82;
        if (temp_v0_2 == 0) {
            sp30 = var_a1;
            func_801404FC(0, var_a1);
        } else if (temp_v0_2 == 1) {
            sp30 = var_a1;
            func_80140364(0, var_a1);
        }
    }
    func_80141DE0(var_a1);
    func_8013B990();
    func_8013D348();
    func_8013D3F4();
    func_8013DDE4();
    func_8013C6D4();
    func_8013E4FC();
}
