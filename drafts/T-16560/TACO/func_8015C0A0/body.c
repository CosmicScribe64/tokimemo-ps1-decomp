void func_8015C0A0(s32 arg0, s32 arg1) {
    TcActor *temp_v0;
    s32 var_v1;
    s32 var_v1_2;
    u8 *var_a0;
    u8 *var_a0_2;

    temp_v0 = &D_8015EDB4[arg1];
    if (temp_v0->unk0[1] & 0x80) {
        if (arg0 == 1) {
            var_v1 = 0;
            if ((u8) temp_v0->unk84[2] < 0xBU) {
                temp_v0->unk4.unk0 = 0;
                return;
            }
            temp_v0->unk4.unk0 = 0x80000000;
            var_a0 = D_80161D9C;
            do {
                if (arg1 == (*(s32 *)var_a0)) {
                    D_80127080[var_v1].unk0 = 0;
                }
                if (arg1 == *(u8 *)((unsigned char *)var_a0 + (1))) {
                    D_80127080[var_v1 + 1].unk0 = 0;
                }
                if (arg1 == *(u8 *)((unsigned char *)var_a0 + (2))) {
                    D_80127080[var_v1 + 2].unk0 = 0;
                }
                if (arg1 == *(u8 *)((unsigned char *)var_a0 + (3))) {
                    D_80127080[var_v1 + 3].unk0 = 0;
                }
                var_v1 += 4;
                var_a0 += 4;
            } while (var_v1 != 0x40);
            return;
        }
        var_v1_2 = 0;
        if ((u8) temp_v0->unk84[2] < 9U) {
            temp_v0->unk4.unk0 = 0x80000000;
            return;
        }
        temp_v0->unk4.unk0 = 0x80000000;
        var_a0_2 = D_80161D9C;
        do {
            if (arg1 == (*(s32 *)var_a0_2)) {
                D_80127080[var_v1_2].unk0 = 0x80000000;
            }
            if (arg1 == *(u8 *)((unsigned char *)var_a0_2 + (1))) {
                D_80127080[var_v1_2 + 1].unk0 = 0x80000000;
            }
            if (arg1 == *(u8 *)((unsigned char *)var_a0_2 + (2))) {
                D_80127080[var_v1_2 + 2].unk0 = 0x80000000;
            }
            if (arg1 == *(u8 *)((unsigned char *)var_a0_2 + (3))) {
                D_80127080[var_v1_2 + 3].unk0 = 0x80000000;
            }
            var_v1_2 += 4;
            var_a0_2 += 4;
        } while (var_v1_2 != 0x40);
    }
}
