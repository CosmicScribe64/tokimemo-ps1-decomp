void func_80077900(void) {
    s32 temp_t6;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 temp_v1_3;
    s8 temp_v0_2;
    u32 temp_t1;
    u32 temp_t5;
    u8 *temp_v0;
    u8 *temp_v0_3;
    u8 *temp_v0_4;
    u8 *temp_v0_5;

    if (D_800E7384 == 0x20) {
        temp_t6 = D_80120666 & 1;
        switch (temp_t6) {
        case 0:
            func_80044750(0x501);
            break;
        case 1:
            func_80044750(0x502);
            break;
        }
        temp_v0 = &D_800E6280 + (D_800E699C * 4);
        parameter_up_down(*(u8 *)((unsigned char *)temp_v0 + (1693)), 1, *(u8 *)((unsigned char *)temp_v0 + (1694)), &D_800E6280);
        parameter_show_init();
    } else if ((u32) D_800E7384 >= 0x41U) {
        func_80048F64(0x60);
        func_80048F64(0x61);
        if ((D_800E62C1 == 6) && (*(u8 *)((unsigned char *)(&D_800E6280 + (D_800E699C * 4)) + (1693)) == 4) && (temp_t5 = (u16) D_800E6374 >> 0xC, (temp_t5 != 0xB)) && (*(u16 *)((unsigned char *)(&D_800E6280 + (temp_t5 * 4)) + (1644)) == 6) && (temp_v0_2 = func_80053564(temp_t5, D_800E6374), D_800E69DD = temp_v0_2, (temp_v0_2 != -1)) && ((u16) D_800E62C2 < 0x2DAU)) {
            temp_t1 = (u16) D_800E6374 >> 0xC;
            if (temp_t1 == 0xA) {
                if ((temp_v0_2 == 0) && (temp_v1 = *(s32 *)((unsigned char *)(&D_800E6280 + (temp_v0_2 * 0x38)) + (456)), (temp_v1 & 4)) && (temp_v1 & 2) && !(*(s32 *)((unsigned char *)(&D_800E6280 + (temp_t1 * 4)) + (1644)) & 0x20000000) && !(temp_v1 & 0x40000)) {
                    temp_v0_3 = &D_800E6280 + (temp_v0_2 * 0x38);
                    *(u8 *)((unsigned char *)temp_v0_3 + (458)) = (u8) (*(u8 *)((unsigned char *)temp_v0_3 + (458)) | 4);
                    main_state_set(0x60, temp_t1, temp_v0_2, &D_800E6280);
                } else {
                    temp_v1_2 = *(s32 *)((unsigned char *)(&D_800E6280 + (temp_v0_2 * 0x38)) + (456));
                    if ((temp_v1_2 & 4) && (temp_v1_2 & 2) && !(*(s32 *)((unsigned char *)(&D_800E6280 + (temp_t1 * 4)) + (1644)) & 0x20000000) && !(temp_v1_2 & 0x40000)) {
                        temp_v0_4 = &D_800E6280 + (temp_v0_2 * 0x38);
                        *(u8 *)((unsigned char *)temp_v0_4 + (458)) = (u8) (*(u8 *)((unsigned char *)temp_v0_4 + (458)) | 4);
                        main_state_set(0x60, temp_t1, temp_v0_2, &D_800E6280);
                    } else {
                        goto block_31;
                    }
                }
            } else {
                temp_v1_3 = *(s32 *)((unsigned char *)(&D_800E6280 + (temp_v0_2 * 0x38)) + (456));
                if ((temp_v1_3 & 2) && (temp_v1_3 & 4) && !(*(s32 *)((unsigned char *)(&D_800E6280 + (temp_t1 * 4)) + (1644)) & 0x20000000) && !(temp_v1_3 & 0x40000)) {
                    temp_v0_5 = &D_800E6280 + (temp_v0_2 * 0x38);
                    *(u8 *)((unsigned char *)temp_v0_5 + (458)) = (u8) (*(u8 *)((unsigned char *)temp_v0_5 + (458)) | 4);
                    main_state_set(0x60, temp_t1, temp_v0_2, &D_800E6280);
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
