extern s16 D_800E6382;
extern s16 D_800E638A;
extern s32 D_800E7208;
extern s8 D_8014616B;
extern u16 D_800E6900;
extern u32 D_800E7378;
extern u8 D_800E62BE;
extern u8 D_800E7216;
extern u8 D_800E7226;
extern u8 D_800E7236;
extern u8 D_800E7246;
extern u8 D_800E7312;
extern u8 D_80146054;
extern u8 D_80146058;
void func_8013FB28(void) {
    s16 temp_t9;
    s16 var_v0;
    s16 var_v1;
    s16 var_v1_2;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 var_v0_2;
    s8 temp_t4;
    s8 temp_t7;
    s8 temp_t8;
    u8 *var_a0;
    u8 temp_t6;
    u8 temp_v0;
    u8 temp_v1;

    temp_t6 = D_800E7378 & 3;
    var_v1 = 1;
loop_1:
    temp_t8 = ((D_800E7378 * 2) + var_v1) & 3 & 0xFF;
    var_v0 = var_v1;
    if (temp_t6 == temp_t8) {
        var_v1 += 1;
        goto loop_1;
    }
loop_4:
    temp_t7 = (((u32) D_800E7378 / 10U) + var_v0) & 3 & 0xFF;
    if (temp_t6 == temp_t7) {
block_6:
        temp_t9 = var_v1 + 1;
        var_v1 = temp_t9;
        var_v0 = temp_t9;
        goto loop_4;
    }
    if (temp_t8 == temp_t7) {
        goto block_6;
    }
loop_7:
    temp_t4 = (var_v1 % 4) & 0xFF;
    if (temp_t6 == temp_t4) {
block_11:
        var_v1 += 1;
        goto loop_7;
    }
    if (temp_t8 == temp_t4) {
        goto block_11;
    }
    D_8014616B = temp_t4;
    if (temp_t7 == temp_t4) {
        goto block_11;
    }
    (*(s32 *)D_80146168) = temp_t6;
    *(s8 *)((unsigned char *)D_80146168 + (1)) = temp_t8;
    *(s8 *)((unsigned char *)D_80146168 + (2)) = temp_t7;
    var_v1_2 = 0;
    var_a0 = D_80146168;
    do {
        temp_v0 = *var_a0;
        if (temp_v0 == 1) {
            *var_a0 = 0xB;
        } else if (temp_v0 == 2) {
            *var_a0 = D_80146054;
        } else if (temp_v0 == 3) {
            *var_a0 = D_80146058;
        }
        var_v1_2 += 1;
        var_a0 = &D_80146168[var_v1_2];
    } while (var_v1_2 < 4);
    menu_check(0, D_8011ECF6, D_8011ECFA, D_80146168);
    func_8004FC10(0);
    if ((D_800E7208 & 0xF000) && ((temp_v0_2 = D_800E7312 & 3, (temp_v0_2 == 1)) || (temp_v0_2 == 3))) {
        func_80044750(0x502);
    }
    if (D_800E7208 & 0x200020) {
        if (check_end_k() != 0) {
            if (D_800E7216 != 0) {
                D_80146158 = 0;
                goto block_35;
            }
            if (D_800E7226 != 0) {
                D_80146158 = 1;
                goto block_35;
            }
            if (D_800E7236 != 0) {
                D_80146158 = 2;
                goto block_35;
            }
            if (D_800E7246 != 0) {
                D_80146158 = 3;
block_35:
                *(u8 *)((unsigned char *)D_8011ECD0 + (3)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (3)) & 0xFF7F);
                D_80122D30 = 0;
                k_reset(1);
                D_80120697 |= 0x80;
                temp_v1 = D_80146168[D_80146158];
                var_v0_2 = temp_v1 == 0xB;
                if (var_v0_2 != 0) {
                    var_v0_2 = D_800E62BE == 0x5F;
                }
                D_801206AC = ((temp_v1 + ((D_800E62BE == 0x60) * 0xB) + ((D_800E62BE == 0x61) * 0x17)) - var_v0_2) + 1;
                D_801206BA = (D_80146158 * 0x4D) - 0x75;
                D_801206BE = -0x3C;
                D_801206B0 = 0;
                D_80145F2C = (D_80145F2C + (temp_v1 * 3) + D_800E62BE) - 0x5F;
                D_80146160 = 1;
                D_80146158 = D_80146158;
                if (((D_800E62BE == 0x5F) && (temp_v1 == 0) && (D_800E638A >= 0x64)) || (((temp_v1 == 0) & (D_800E62BE == 0x5F)) && ((u16) D_800E6900 >= 0x64U)) || ((temp_v0_3 = temp_v1 == 1, ((temp_v0_3 & (D_800E62BE == 0x5F)) != 0)) && (D_800E6382 >= 0x46)) || ((temp_v0_3 & (D_800E62BE == 0x61)) && (D_800E6382 >= 0x64)) || (((temp_v1 == 2) & (D_800E62BE == 0x60)) && (D_800E6382 >= 0x64))) {
                    D_80145F30 = 1;
                }
                func_8004284C();
            }
        }
    }
}
