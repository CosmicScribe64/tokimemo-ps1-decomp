void xa_wait(void) {
    s32 sp2C;
    u8 *sp28;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s8 temp_t5;
    u32 temp_a0;
    u8 *temp_v0_4;
    u8 *temp_v0_5;

    sp2C = 0;
    temp_v0 = get_now_k();
    if ((u8) D_800E6280.unk_F62 & 0x80) {
        temp_v0_2 = (u8) D_800E6280.unk_F62 & 0x7F;
        if ((u32) ((u8) D_800E6280.unk_F61 + 2) >= (u32) temp_v0) {
            sp2C = k_disp_inc();
            temp_v0_3 = get_now_k();
            temp_a0 = (u8) D_800E6280.unk_F61 + 3;
            if (temp_a0 == temp_v0_3) {
                if ((u8) D_800E6280.unk_F60 >= temp_a0) {
                    k_disp_switch(temp_v0_3, 0);
                }
            }
        } else if (temp_v0_2 == 0) {
            k_disp_switch((s32) (u8) D_800E6280.unk_F61, 0);
            sp28 = get_k_work((u8) D_800E6280.unk_F61 + 1);
            temp_v0_4 = get_k_work((u8) D_800E6280.unk_F61 + 2);
            *(s16 *)((unsigned char *)sp28 + (2)) = (s16) (*(s16 *)((unsigned char *)sp28 + (2)) - 2);
            *(s16 *)((unsigned char *)temp_v0_4 + (2)) = (s16) (*(s16 *)((unsigned char *)temp_v0_4 + (2)) - 2);
            D_800E6280.unk_F62 = (u8) D_800E6280.unk_F62 + 1;
        } else if (temp_v0_2 != 0) {
            sp28 = get_k_work((u8) D_800E6280.unk_F61 + 1);
            temp_v0_5 = get_k_work((u8) D_800E6280.unk_F61 + 2);
            *(s16 *)((unsigned char *)sp28 + (2)) = (s16) (*(s16 *)((unsigned char *)sp28 + (2)) - 2);
            *(s16 *)((unsigned char *)temp_v0_5 + (2)) = (s16) (*(s16 *)((unsigned char *)temp_v0_5 + (2)) - 2);
            if (*(s16 *)((unsigned char *)sp28 + (2)) < 0x33) {
                *(s16 *)((unsigned char *)sp28 + (2)) = 0x32;
                *(s16 *)((unsigned char *)temp_v0_5 + (2)) = 0x42;
                D_800E6280.unk_F62 = (u8) D_800E6280.unk_F62 - 1;
                temp_t5 = (u8) D_800E6280.unk_F61 + 1;
                D_800E6280.unk_F61 = temp_t5;
                k_disp_switch((temp_t5 & 0xFF) + 2, 1);
            }
        }
    } else {
        sp2C = k_disp_inc();
        get_now_k();
    }
    if (sp2C == 1) {
        if (((DisplayActor44 *)D_8011ECD0)[2].unk_03 & 0x80) {
            func_8006C934();
        } else {
            func_8006C848(1);
        }
    }
    if (D_800E6280.unk_F88 & 0x60) {
        if (sp2C == 1) {
            func_8006C848(0);
            D_800E6280.unk_110D += 1;
            D_800E6280.unk_F62 = 0;
            return;
        }
        k_disp_goto_line_end();
    }
}
