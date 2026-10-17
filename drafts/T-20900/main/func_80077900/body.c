void func_80077900(void) {
    s32 temp_t6;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 temp_v1_3;
    u32 temp_t1;
    u32 temp_t5;
    u8 temp_v0;

    if (D_800E6280.unk_1104.w == 0x20) {
        temp_t6 = ((DisplayActor44 *)D_8011ECD0)[96].unk_16 & 1;
        switch (temp_t6) {
        case 0:
            func_80044750(0x501);
            break;
        case 1:
            func_80044750(0x502);
            break;
        }
        parameter_up_down((s32) D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01, 1, (s32) D_800E6280.unk_69C[D_800E6280.unk_71C].unk_02);
        parameter_show_init();
    } else if ((u32) D_800E6280.unk_1104.w >= 0x41U) {
        func_80048F64(0x60);
        func_80048F64(0x61);
        if ((D_800E6280.unk_041 == 6) && (D_800E6280.unk_69C[D_800E6280.unk_71C].unk_01 == 4) && (temp_t5 = (u16) D_800E6280.unk_0F4.h >> 0xC, (temp_t5 != 0xB)) && (D_800E6280.unk_66C[temp_t5].unk_00 == 6) && (temp_v0 = func_80053564((s32) temp_t5), D_800E6280.unk_75D = temp_v0, (temp_v0 != -1U)) && ((u16) D_800E6280.unk_042 < 0x2DAU)) {
            temp_t1 = (u16) D_800E6280.unk_0F4.h >> 0xC;
            if (temp_t1 == 0xA) {
                if ((temp_v0 == 0) && (temp_v1 = D_800E6280.unk_1BC[temp_v0].unk_0C.w, (temp_v1 & 4)) && (temp_v1 & 2) && !(*(s32 *)&D_800E6280.unk_66C[temp_t1] & 0x20000000) && !(temp_v1 & 0x40000)) {
                    D_800E6280.unk_1BC[temp_v0].unk_0C.b[2] |= 4;
                    main_state_set(0x60, (s32) temp_t1);
                } else {
                    temp_v1_2 = D_800E6280.unk_1BC[temp_v0].unk_0C.w;
                    if ((temp_v1_2 & 4) && (temp_v1_2 & 2) && !(*(s32 *)&D_800E6280.unk_66C[temp_t1] & 0x20000000) && !(temp_v1_2 & 0x40000)) {
                        D_800E6280.unk_1BC[temp_v0].unk_0C.b[2] |= 4;
                        main_state_set(0x60, (s32) temp_t1);
                    } else {
                        goto block_31;
                    }
                }
            } else {
                temp_v1_3 = D_800E6280.unk_1BC[temp_v0].unk_0C.w;
                if ((temp_v1_3 & 2) && (temp_v1_3 & 4) && !(*(s32 *)&D_800E6280.unk_66C[temp_t1] & 0x20000000) && !(temp_v1_3 & 0x40000)) {
                    D_800E6280.unk_1BC[temp_v0].unk_0C.b[2] |= 4;
                    main_state_set(0x60, (s32) temp_t1);
                } else {
                    goto block_31;
                }
            }
        } else {
block_31:
            func_80072B5C(0);
        }
    }
}
