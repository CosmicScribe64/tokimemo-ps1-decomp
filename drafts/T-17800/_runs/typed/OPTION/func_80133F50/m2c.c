? func_8004F984(?, s16, s16, s8);                   /* extern */

void func_80133F50(void) {
    s32 sp30;
    s32 sp2C;
    OptActor44 *temp_a0;
    OptActor44 *var_v0;
    s16 temp_t0;
    s16 temp_t3;
    s16 temp_t4;
    s16 temp_t5;
    s16 temp_t6;
    s16 temp_t7;
    s16 temp_t8;
    s16 temp_t9;
    s32 var_v1;
    s8 temp_a3;
    u8 *var_v0_2;
    u8 *var_v0_3;

    *(s8 *)((unsigned char *)D_8011ECD0 + (7211)) = 0x80;
    sp2C = (s32) D_800E6280.unk_1094;
    func_8004F984(1, D_8011ECD0->unk_26, D_8011ECD0->unk_2A, D_800E6280.unk_1094);
    func_8004FC10(1);
    func_801320C0();
    temp_a3 = D_800E6280.unk_1094;
    var_v0 = D_8011ECD0 + 0x1980;
    temp_a0 = &var_v0[temp_a3];
    var_v1 = 0;
    do {
        if (var_v0 == temp_a0) {
            *(s16 *)((unsigned char *)var_v0 + (772)) = (s16) (var_v1 + 0x15);
        } else {
            *(s16 *)((unsigned char *)var_v0 + (772)) = (s16) (var_v1 + 0x16);
        }
        var_v0 += 0x44;
        var_v1 += 2;
    } while ((u32) var_v0 < (u32) (D_8011ECD0 + 0x1A4C));
    if (sp2C != temp_a3) {
        switch (temp_a3) {                          /* switch 1; irregular */
        case 0:                                     /* switch 1 */
            D_800E6280.unk_F6C = 0;
            sp30 = (s32) D_800E6280.unk0;
            func_8006509C();
            var_v0_2 = D_80125C10;
            do {
                temp_t3 = var_v0_2->unk0;
                temp_t5 = *(s16 *)((unsigned char *)var_v0_2 + (4));
                temp_t8 = *(s16 *)((unsigned char *)var_v0_2 + (8));
                temp_t9 = *(s16 *)((unsigned char *)var_v0_2 + (12));
                var_v0_2 += 0x10;
                *(s16 *)((unsigned char *)var_v0_2 + (-4)) = (s16) (temp_t9 + 0xC8);
                *(s16 *)((unsigned char *)var_v0_2 + (-8)) = (s16) (temp_t8 + 0xC8);
                *(s16 *)((unsigned char *)var_v0_2 + (-12)) = (s16) (temp_t5 + 0xC8);
                *(s16 *)((unsigned char *)var_v0_2 + (-16)) = (s16) (temp_t3 + 0xC8);
            } while (var_v0_2 != &D_80125C50);
            D_800E6280.unk_F6D = 0xE;
            func_800654D0(0xE);
            D_800E6280.unk_F6C = (s8) sp30;
            break;
        case 1:                                     /* switch 1 */
            D_800E6280.unk_F6C = 1;
            sp30 = (s32) D_800E6280.unk0;
            func_8006509C();
            var_v0_3 = D_80125C10;
            do {
                temp_t4 = var_v0_3->unk0;
                temp_t6 = *(s16 *)((unsigned char *)var_v0_3 + (4));
                temp_t7 = *(s16 *)((unsigned char *)var_v0_3 + (8));
                temp_t0 = *(s16 *)((unsigned char *)var_v0_3 + (12));
                var_v0_3 += 0x10;
                *(s16 *)((unsigned char *)var_v0_3 + (-4)) = (s16) (temp_t0 + 0xC8);
                *(s16 *)((unsigned char *)var_v0_3 + (-8)) = (s16) (temp_t7 + 0xC8);
                *(s16 *)((unsigned char *)var_v0_3 + (-12)) = (s16) (temp_t6 + 0xC8);
                *(s16 *)((unsigned char *)var_v0_3 + (-16)) = (s16) (temp_t4 + 0xC8);
            } while (var_v0_3 != &D_80125C50);
            D_800E6280.unk_F6D = 0xE;
            func_800654D0(0xE);
            D_800E6280.unk_F6C = (s8) sp30;
            break;
        }
    }
    if (D_800E6280.unk_F88 & 0x20) {
        switch (temp_a3) {                          /* switch 2; irregular */
        case 0:                                     /* switch 2 */
            D_800E6280.unk_F6C = 3;
            func_80134210();
            func_8004284C();
            func_80044750(0x501);
            break;
        case 1:                                     /* switch 2 */
            D_800E6280.unk_F6C = 4;
            func_80134210();
            func_8004284C();
            func_80044750(0x501);
            break;
        case 2:                                     /* switch 2 */
            D_800E6280.unk_F6C = 5;
            func_80134210();
            func_8004284C();
            func_80044750(0x501);
            break;
        }
    }
    if (D_800E6280.unk_F88 & 0x40) {
        func_80134210();
        func_8004284C();
        D_800E6280.unk_110D = 2;
    }
}
