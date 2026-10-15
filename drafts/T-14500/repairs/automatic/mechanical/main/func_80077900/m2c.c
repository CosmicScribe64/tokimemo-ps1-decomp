extern u8 D_800E62C1;
extern u16 D_800E62C2;
extern u16 D_800E6374;
extern u8 D_800E699C;
extern s8 D_800E69DD;
extern u32 D_800E7384;

void func_80077900(void) {
    s32 temp_t6;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 temp_v1_3;
    s8 temp_v0;
    u32 temp_t1;
    u32 temp_t5;

    if (D_800E7384 == 0x20) {
        temp_t6 = D_80120666 & 1;
        switch (temp_t6) {                          /* irregular */
        case 0:
            func_80044750(0x501);
            break;
        case 1:
            func_80044750(0x502);
            break;
        }
        parameter_up_down((s32) D_800E6280.unk_69C[D_800E699C].unk_01, 1, (s32) D_800E6280.unk_69C[D_800E699C].unk_02);
        parameter_show_init();
    } else if ((u32) D_800E7384 >= 0x41U) {
        func_80048F64(0x60);
        func_80048F64(0x61);
        if ((D_800E62C1 == 6) && (D_800E6280.unk_69C[D_800E699C].unk_01 == 4) && (temp_t5 = (u16) D_800E6374 >> 0xC, (temp_t5 != 0xB)) && (D_800E6280.unk_66C[temp_t5].unk_00 == 6) && (temp_v0 = func_80053564((s32) temp_t5), D_800E69DD = temp_v0, (temp_v0 != -1)) && ((u16) D_800E62C2 < 0x2DAU)) {
            temp_t1 = (u16) D_800E6374 >> 0xC;
            if (temp_t1 == 0xA) {
                if ((temp_v0 == 0) && (temp_v1 = D_800E6280.unk_1BC[temp_v0].unk_0C.w, (temp_v1 & 4)) && (temp_v1 & 2) && !((s32) D_800E6280.unk_66C[temp_t1] & 0x20000000) && !(temp_v1 & 0x40000)) {
                    D_800E6280.unk_1BC[temp_v0].unk_0C.b[2] |= 4;
                    main_state_set(0x60, (s32) temp_t1);
                } else {
                    temp_v1_2 = D_800E6280.unk_1BC[temp_v0].unk_0C.w;
                    if ((temp_v1_2 & 4) && (temp_v1_2 & 2) && !((s32) D_800E6280.unk_66C[temp_t1] & 0x20000000) && !(temp_v1_2 & 0x40000)) {
                        D_800E6280.unk_1BC[temp_v0].unk_0C.b[2] |= 4;
                        main_state_set(0x60, (s32) temp_t1);
                    } else {
                        goto block_31;
                    }
                }
            } else {
                temp_v1_3 = D_800E6280.unk_1BC[temp_v0].unk_0C.w;
                if ((temp_v1_3 & 2) && (temp_v1_3 & 4) && !((s32) D_800E6280.unk_66C[temp_t1] & 0x20000000) && !(temp_v1_3 & 0x40000)) {
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
