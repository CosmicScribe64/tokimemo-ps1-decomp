void func_8005215C(void) {
    s32 sp34;
    u8 *sp30;
    s32 sp28;
    s32 temp_a0_2;
    s32 temp_t4;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_7;
    s32 temp_v1;
    s8 temp_t5;
    u32 temp_a0;
    u8 *temp_v0_4;
    u8 *temp_v0_5;
    u8 temp_v0_6;
    u8 temp_v0_8;
    void *temp_a3;

    sp34 = 0;
    temp_v0 = get_now_k();
    if ((u8) D_800E6280.unk_F62 & 0x80) {
        temp_v0_2 = (u8) D_800E6280.unk_F62 & 0x7F;
        if ((u32) ((u8) D_800E6280.unk_F61 + 2) >= (u32) temp_v0) {
            sp34 = k_disp_inc();
            temp_v0_3 = get_now_k();
            temp_a0 = (u8) D_800E6280.unk_F61 + 3;
            if ((temp_a0 == temp_v0_3) && ((u8) D_800E6280.unk_F60 >= temp_a0)) {
                k_disp_switch(temp_v0_3, 0);
            }
        } else if (temp_v0_2 == 0) {
            k_disp_switch((s32) (u8) D_800E6280.unk_F61, 0);
            sp30 = get_k_work((u8) D_800E6280.unk_F61 + 1);
            temp_v0_4 = get_k_work((u8) D_800E6280.unk_F61 + 2);
            *(s16 *)((unsigned char *)sp30 + (2)) = (s16) (*(s16 *)((unsigned char *)sp30 + (2)) - 2);
            *(s16 *)((unsigned char *)temp_v0_4 + (2)) = (s16) (*(s16 *)((unsigned char *)temp_v0_4 + (2)) - 2);
            D_800E6280.unk_F62 = (u8) D_800E6280.unk_F62 + 1;
        } else if (temp_v0_2 != 0) {
            sp30 = get_k_work((u8) D_800E6280.unk_F61 + 1);
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
        temp_v0_6 = (u8) D_800E6280.unk_03C;
        if (((temp_v0_6 == 1) && (D_80125C00 != 0)) || ((temp_v0_6 == 2) && (D_80125C00 != 0))) {
            sp34 = k_disp_inc();
        }
        if (temp_v0_6 == 0) {
            sp34 = k_disp_inc();
        }
        get_now_k();
    }
    if ((D_80125C00 == 0) && (func_800460EC() & 4)) {
        D_80125C00 = 1;
    }
    temp_v0_7 = func_80046284();
    if (D_80125C00 == 1) {
        sp28 = temp_v0_7;
        if (func_800460EC() & 2) {
            if (D_80125BFC != -1) {
                temp_t4 = D_80125BFC * 8;
                temp_a3 = D_80125BF8 + temp_t4;
                temp_v1 = *(D_80125BF4 + temp_t4);
                temp_a0_2 = temp_a3->unk0;
                if ((temp_v1 != temp_a0_2) && (temp_v1 == *(s32 *)((unsigned char *)sp28 + (8)))) {
                    func_80046290(temp_a0_2, *(s32 *)((unsigned char *)temp_a3 + (4)), (s32) D_800E6280.unk_F5F);
                    func_80044750(0x300);
                    D_80125C00 = 0;
                } else {
                    D_80125C00 = 2;
                }
            } else {
                D_80125C00 = 2;
            }
        }
    }
    if ((sp34 == 1) && (D_80125C00 == 2)) {
        if (*(u8 *)((unsigned char *)D_8011ECD0 + (139)) & 0x80) {
            func_8006C934();
        } else {
            func_8006C848(1);
        }
    }
    if ((sp34 == 1) && ((u8) D_800E6280.unk_03C == 0)) {
        if (*(u8 *)((unsigned char *)D_8011ECD0 + (139)) & 0x80) {
            func_8006C934();
        } else {
            func_8006C848(1);
        }
    }
    temp_v0_8 = (u8) D_800E6280.unk_03C;
    if (D_800E6280.unk_F88 & 0x40) {
        if (sp34 == 1) {
            if (temp_v0_8 == 2) {
                D_80125C00 = 2;
                func_80044750(0x74);
                func_80045414(9, 0, NULL);
            }
            D_800E6280.unk_F62 = 0;
        } else {
            k_disp_goto_line_end();
        }
    }
    if (D_800E6280.unk_F88 & 0x60) {
        if (sp34 == 1) {
            if (temp_v0_8 == 1) {
                if (D_80125C00 == 2) {
                    func_8006C848(0);
                    D_800E6280.unk_110D += 1;
                }
                D_800E6280.unk_F62 = 0;
                return;
            }
            if (temp_v0_8 == 2) {
                if (D_80125C00 == 2) {
                    func_8006C848(0);
                    D_800E6280.unk_110D += 1;
                }
                D_800E6280.unk_F62 = 0;
                return;
            }
            func_8006C848(0);
            D_800E6280.unk_110D += 1;
            D_800E6280.unk_F62 = 0;
            return;
        }
        k_disp_goto_line_end();
    }
}
