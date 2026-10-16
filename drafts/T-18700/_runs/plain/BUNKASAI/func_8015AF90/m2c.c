? func_800848AC();                                  /* extern */
extern s8 D_800CA2B1;
extern s8 D_800CA2B2;
extern ? D_800CA2B3;
extern ? D_800E6328;
extern ? D_800E6354;
extern ? D_800E635C;
extern u16 D_800E6374;
extern s32 D_800E6378;
extern s32 D_800E6448;
extern u8 D_800E644A;
extern s32 D_800E644C;
extern s32 D_800E6480;
extern s32 D_800E64B8;
extern u8 D_800E64BA;
extern ? D_800E64E8;
extern s32 D_800E7208;
extern u8 D_800E738A;

void func_8015AF90(void) {
    ? sp78;
    ? *var_v0;
    ? *var_v1;
    s32 temp_a3_2;
    u32 temp_t1;
    u32 temp_t2;
    u32 temp_t4;
    u8 *temp_a3;
    u8 temp_t2_2;
    u8 temp_t3;
    u8 temp_t3_2;
    u8 temp_t4_2;
    u8 temp_t5;
    u8 temp_t5_2;
    u8 temp_t8;

    M2C_MEMCPY_ALIGNED(&sp78, &D_80160AE0, 0x30);
    M2C_MEMCPY_ALIGNED(&sp4C[0], &D_80160B10, 0x2C);
    sp3C[0].unk0 = (s32) D_80160B3C.text[0];
    *(u8 **)((unsigned char *)(&sp3C[0]) + (4)) = (u8 *) D_80160B3C.text[1];
    *(u8 **)((unsigned char *)(&sp3C[0]) + (8)) = (u8 *) D_80160B3C.text[2];
    *(u8 **)((unsigned char *)(&sp3C[0]) + (12)) = (u8 *) D_80160B3C.text[3];
    switch (D_800E738A) {
    case 0:
        cd_read_sectors(1U, 0x801E8000, 0x4B80);
        *D_800CA2B0 = (u8) ((u32) (D_800E6448 << 0x1E) >> 0x1F);
        D_800CA2B1 = (s8) ((u32) (D_800E6480 << 0x1E) >> 0x1F);
        var_v0 = &D_800E6328;
        var_v1 = &D_800CA2B3;
        D_800CA2B2 = (s8) ((u32) (D_800E64B8 << 0x1E) >> 0x1F);
        do {
            var_v1->unk0 = (s8) ((u32) (*(s32 *)((unsigned char *)var_v0 + (456)) << 0x1E) >> 0x1F);
            temp_t4 = (u32) (*(s32 *)((unsigned char *)var_v0 + (568)) << 0x1E) >> 0x1F;
            temp_t2 = (u32) (*(s32 *)((unsigned char *)var_v0 + (624)) << 0x1E) >> 0x1F;
            temp_t1 = (u32) (*(s32 *)((unsigned char *)var_v0 + (512)) << 0x1E) >> 0x1F;
            var_v0 += 0xE0;
            *(s8 *)((unsigned char *)var_v1 + (1)) = (s8) temp_t1;
            *(s8 *)((unsigned char *)var_v1 + (3)) = (s8) temp_t2;
            *(s8 *)((unsigned char *)var_v1 + (2)) = (s8) temp_t4;
            var_v1 += 4;
        } while (var_v0 != &D_800E64E8);
        func_8004284C();
        break;
    case 1:
        if (cd_read_sync_status() & 1) {
            func_80132410();
            D_800CA14C = 0;
            D_800CA160 = (s32) D_801604F0;
            D_800CA164 = (s32) D_801604F4;
            D_800CA168 = (s32) D_801604F8;
            if ((D_80122EBC != 0xFF) && ((((u16) D_800E6374 >> 0xC) == 3) || (func_8008667C(0) != 4) || (((u8) D_800E644A >> 4) != 3))) {
                if (((u16) D_800E6374 >> 0xC) != 3) {
                    if ((func_8008667C(2) == 4) && (((u8) D_800E64BA >> 4) == 3)) {
                        goto block_13;
                    }
                    goto block_15;
                }
block_15:
                D_800CA148 = 1;
                func_80042940(5);
            } else {
block_13:
                D_800CA148 = 0;
                func_8004284C();
            }
        }
        break;
    case 2:
        func_80085A60();
        break;
    case 3:
        func_800848AC();
        break;
    case 4:
        D_80122EBC = D_80122CDC;
        func_800847B8((u8) D_80122CDC);
        func_8004284C();
        break;
    case 5:
        func_80085A60();
        break;
    case 6:
        func_8004E9F4(1);
        func_8004E788(-0x80, 0x32, 0, (s32) "＊パ\x83\\コン　ＮＭＳ＿９０００＊", 0);
        func_8004E884(2);
        func_8004284C();
        break;
    case 7:
        func_80044750(0x50A);
        func_8004284C();
        break;
    case 8:
        if (D_80122EBC == 0xB) {
            func_8004E788(-0x80, 0x42, 0, (s32) *girl_short_name_table, 0);
        } else {
            temp_a3 = girl_short_name_table[D_80122EBC];
            D_80122EBC = D_80122EBC;
            func_8004E788(-0x80, 0x42, 0, (s32) temp_a3, 0);
        }
        func_8004E788(-0x48, 0x42, 0, *(s32 *)((unsigned char *)(sp + (D_80122EBC * 4)) + (120)), 0);
        func_8004E884(4);
        D_80160B4C = 0;
        func_8004284C();
        break;
    case 9:
        temp_t3 = (D_80160B4C + 1) & 0xFF;
        D_80160B4C = temp_t3;
        if (temp_t3 >= 0x15U) {
            D_80160B4C = 0;
            func_8004284C();
        }
        break;
    case 10:
        func_8004E788(-0x10, 0x42, 0, (s32) "血液型", 0);
        func_8004E884(5);
        func_8004284C();
        break;
    case 11:
        temp_t5 = (D_80160B4C + 1) & 0xFF;
        D_80160B4C = temp_t5;
        if (temp_t5 >= 0x15U) {
            D_80160B4C = 0;
            func_8004284C();
        }
        break;
    case 12:
        if ((D_80122EBC == 0) || (D_80122EBC == 0xB)) {
            func_8004E788(0x28, 0x42, 0, (&sp3C[0])[(u32) (D_800E644C << 0x15) >> 0x1E], 0);
        } else {
            temp_a3_2 = (&sp4C[0])[D_80122EBC];
            D_80122EBC = D_80122EBC;
            func_8004E788(0x28, 0x42, 0, temp_a3_2, 0);
        }
        func_8004E884(6);
        func_8004284C();
        break;
    case 13:
        temp_t3_2 = (D_80160B4C + 1) & 0xFF;
        D_80160B4C = temp_t3_2;
        if (temp_t3_2 >= 0x15U) {
            D_80160B4C = 0;
            func_8004284C();
        }
        break;
    case 14:
        func_80044750(0x50A);
        func_8004284C();
        break;
    case 15:
        func_8004E788(-0x80, 0x52, 0, (s32) &D_800E6354, 0);
        func_8004E788(-0x48, 0x52, 0, (s32) &D_800E635C, 0);
        func_8004E884(8);
        D_80160B4C = 0;
        func_8004284C();
        break;
    case 16:
        temp_t5_2 = (D_80160B4C + 1) & 0xFF;
        D_80160B4C = temp_t5_2;
        if (temp_t5_2 >= 0x15U) {
            D_80160B4C = 0;
            func_8004284C();
        }
        break;
    case 17:
        func_8004E788(-0x10, 0x52, 0, (s32) "血液型", 0);
        func_8004E884(9);
        func_8004284C();
        break;
    case 18:
        temp_t2_2 = (D_80160B4C + 1) & 0xFF;
        D_80160B4C = temp_t2_2;
        if (temp_t2_2 >= 0x15U) {
            D_80160B4C = 0;
            func_8004284C();
        }
        break;
    case 19:
        func_8004E788(0x28, 0x52, 0, (&sp3C[0])[(u32) (D_800E6378 << 0x15) >> 0x1E], 0);
        func_8004E884(0xA);
        func_8004284C();
        break;
    case 20:
        temp_t4_2 = (D_80160B4C + 1) & 0xFF;
        D_80160B4C = temp_t4_2;
        if ((temp_t4_2 >= 0x10U) && (func_8004ECB4() != 0)) {
            D_8011ED5B ^= 0x80;
            D_80160B4C = 0;
        }
        if ((D_800E7208 & 0xF0) && (func_8004ECB4() != 0)) {
            func_8004E9F4(1);
            D_8011ED5B &= 0x7F;
            func_8004284C();
        }
        break;
    case 21:
        func_80085A60();
        break;
    case 22:
        func_80044750(0x508);
        func_8004284C();
        break;
    case 23:
        temp_t8 = (D_80160B4C + 1) & 0xFF;
        D_80160B4C = temp_t8;
        if (temp_t8 >= 0x3DU) {
            D_800CA148 = func_8015B828() + 3;
            D_80122EA8 = func_8015B828();
            func_80044750(0x50C);
            func_8004284C();
        }
        break;
    case 24:
        func_80085A60();
        break;
    case 25:
        if ((func_8008667C(0) == 4) && (((u8) D_800E644A >> 4) == 3)) {
            D_80122EBC = 0;
        } else if ((func_8008667C(2) == 4) && (((u8) D_800E64BA >> 4) == 3)) {
            D_80122EBC = 2;
        }
        func_80042908((s32) D_80122EB0);
        func_80042940((s32) D_80122EB4);
        break;
    default:
        func_80046500();
        break;
    }
}
