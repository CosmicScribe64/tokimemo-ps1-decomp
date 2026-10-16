void prologue_line_run(void) {
    s32 sp34;
    u8 *sp30;
    s32 sp28;
    NameCommandPair8 *temp_v1_2;
    s32 temp_a0_3;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_6;
    s8 temp_t5;
    u32 temp_a0_2;
    u8 temp_a0;
    u8 temp_v1;
    u8 *temp_v0_4;
    u8 *temp_v0_5;

    sp34 = 0;
    temp_v0 = get_now_k();
    temp_a0 = (u8) D_800E6280.unk_F62;
    if (temp_a0 & 0x80) {
        temp_v1 = (u8) D_800E6280.unk_F61;
        temp_v0_2 = temp_a0 & 0x7F;
        if ((u32) (temp_v1 + 2) >= (u32) temp_v0) {
            sp34 = k_disp_inc(temp_a0);
            temp_v0_3 = get_now_k();
            temp_a0_2 = (u8) D_800E6280.unk_F61 + 3;
            if ((temp_a0_2 == temp_v0_3) && ((u8) D_800E6280.unk_F60 >= temp_a0_2)) {
                k_disp_switch(temp_v0_3, 0);
            }
        } else if (temp_v0_2 == 0) {
            k_disp_switch((s32) temp_v1, 0);
            sp30 = get_k_work((u8) D_800E6280.unk_F61 + 1);
            temp_v0_4 = get_k_work((u8) D_800E6280.unk_F61 + 2);
            *(s16 *)((unsigned char *)sp30 + (2)) = (s16) (*(s16 *)((unsigned char *)sp30 + (2)) - 2);
            *(s16 *)((unsigned char *)temp_v0_4 + (2)) = (s16) (*(s16 *)((unsigned char *)temp_v0_4 + (2)) - 2);
            D_800E6280.unk_F62 = (u8) D_800E6280.unk_F62 + 1;
        } else if (temp_v0_2 != 0) {
            sp30 = get_k_work(temp_v1 + 1);
            temp_v0_5 = get_k_work((u8) D_800E6280.unk_F61 + 2);
            *(s16 *)((unsigned char *)sp30 + (2)) = (s16) (*(s16 *)((unsigned char *)sp30 + (2)) - 2);
            *(s16 *)((unsigned char *)temp_v0_5 + (2)) = (s16) (*(s16 *)((unsigned char *)temp_v0_5 + (2)) - 2);
            if (*(s16 *)((unsigned char *)sp30 + (2)) < 0x33) {
                *(s16 *)((unsigned char *)sp30 + (2)) = 0x32;
                *(s16 *)((unsigned char *)temp_v0_5 + (2)) = 0x42;
                D_800E6280.unk_F62 = (u8) D_800E6280.unk_F62 - 1;
                temp_t5 = (u8) D_800E6280.unk_F61 + 1;
                D_800E6280.unk_F61 = temp_t5;
                k_disp_switch((temp_t5 & 0xFF) + 2, 1);
            }
        }
    } else {
        sp34 = k_disp_inc(temp_a0);
        get_now_k();
    }
    if (func_800460EC() & 4) {
        D_8014D188 = 1;
    }
    temp_v0_6 = func_80046284();
    if ((D_800E6280.unk_F5F & 0xF) != 0xF) {
        if (D_8014D188 != 0) {
            sp28 = temp_v0_6;
            if (func_800460EC() & 2) {
                temp_v1_2 = &D_8014DB88[(u8) D_8014D180];
                temp_a0_3 = temp_v1_2->unk_00;
                if ((D_8014DA30[(u8) D_8014D180].unk_00 != temp_a0_3) && (temp_a0_3 != *(s32 *)((unsigned char *)sp28 + (8)))) {
                    func_80046290(temp_a0_3, temp_v1_2->unk_04, (s32) D_8014DA04[(u8) D_8014D180]);
                    func_80044750(0x300);
                } else if (sp34 == 1) {
                    D_800E6280.unk_1104.w = D_800E6280.unk_1104.w + 1;
                    D_800E6280.unk_F62 = 0;
                }
            } else {
                goto block_25;
            }
        }
    } else if (sp34 == 1) {
        D_800E6280.unk_1104.w = D_800E6280.unk_1104.w + 1;
        D_800E6280.unk_F62 = 0;
    } else {
block_25:
        D_800E6280.unk_1104.w = 0;
    }
    if ((u32) D_800E6280.unk_1104.w >= 0x3DU) {
        D_800E6280.unk_1104.w = 0;
        D_800E6280.unk_110D = D_800E6280.unk_110D + 1;
    }
}
