? func_80048514(?, ?, ?);                           /* extern */

void func_80133010(void) {
    OptActor44 *var_v0;
    s32 var_a1;
    s32 var_a1_2;
    s32 var_s0;
    s32 var_s0_2;
    u8 temp_t7;
    u8 var_v0_2;
    u8 var_v1;

    if (D_800E6280.unk_110D == 0) {
        if (cd_read_sync_status() & 1) {
            D_800E6280.unk_110D = 1;
        }
    } else if (D_800E6280.unk_110D == 1) {
        func_80048E78();
        D_8013D300 = 1;
        var_v0 = D_8011ECD0 + 0x1980;
        do {
            var_v0 += 0x44;
            *(s8 *)((unsigned char *)var_v0 + (-67)) = 5;
            *(s8 *)((unsigned char *)var_v0 + (-66)) = 0x40;
            *(s32 *)((unsigned char *)var_v0 + (-12)) = 0x01000000;
            *(s8 *)((unsigned char *)var_v0 + (-65)) = 0x24;
            *(s32 *)((unsigned char *)var_v0 + (-56)) = 0x801C32D8;
            *(s32 *)((unsigned char *)var_v0 + (-52)) = 0x801C34E0;
            *(s32 *)((unsigned char *)var_v0 + (-16)) = 0x801C32C8;
            *(s8 *)((unsigned char *)var_v0 + (-64)) = 8;
            *(s16 *)((unsigned char *)var_v0 + (-44)) = 0;
            *(s8 *)((unsigned char *)var_v0 + (-1)) = 0;
            *(s8 *)((unsigned char *)var_v0 + (-63)) = 0x80;
            *(s16 *)((unsigned char *)var_v0 + (-30)) = -0x200;
            *(s16 *)((unsigned char *)var_v0 + (-26)) = -0x200;
            *(s8 *)((unsigned char *)var_v0 + (-61)) = 0x80;
        } while ((u32) var_v0 < (u32) D_80121750);
        var_s0 = 0;
        var_a1 = 0x10;
        do {
            func_80043914(0x801C34F0, var_a1 & 0xFF, 1, 1, var_s0);
            var_s0 += 1;
            var_a1 = var_s0 + 0x10;
        } while (var_s0 < 4);
        func_80043914(0x801B0000, 0x1F, 1, 1, 0);
        var_s0_2 = 0;
        var_a1_2 = 0x1A;
        do {
            func_80043914(0x801CBEF0, var_a1_2 & 0xFF, 1, 1, var_s0_2);
            var_s0_2 += 1;
            var_a1_2 = var_s0_2 + 0x1A;
        } while (var_s0_2 != 4);
        func_80048514(5, 0x801C7CF0, 1);
        func_801328C0();
        *(s16 *)((unsigned char *)D_8011ECD0 + (6552)) = 0;
        *(s16 *)((unsigned char *)D_8011ECD0 + (6566)) = -0xA0;
        *(s16 *)((unsigned char *)D_8011ECD0 + (6570)) = -0x78;
        *(void *)0x80120653 = (u8) (*(u8 *)0x80120653 | 0x80);
        *(s8 *)((unsigned char *)D_8011ECD0 + (6529)) = 8;
        *(s16 *)((unsigned char *)D_8011ECD0 + (6620)) = 0xA;
        *(s16 *)((unsigned char *)D_8011ECD0 + (6634)) = -0xA0;
        *(s16 *)((unsigned char *)D_8011ECD0 + (6638)) = -0x78;
        *(u8 *)((unsigned char *)D_8011ECD0 + (6599)) = (u8) (*(u8 *)((unsigned char *)D_8011ECD0 + (6599)) | 0x80);
        *(s8 *)((unsigned char *)D_8011ECD0 + (6597)) = 3;
        *(s16 *)((unsigned char *)D_8011ECD0 + (7232)) = 7;
        *(s16 *)((unsigned char *)D_8011ECD0 + (7246)) = -0xA0;
        *(s16 *)((unsigned char *)D_8011ECD0 + (7250)) = -0x78;
        *(s8 *)((unsigned char *)D_8011ECD0 + (7211)) = 0x24;
        *(s8 *)((unsigned char *)D_8011ECD0 + (7209)) = 1;
        *(s16 *)((unsigned char *)D_8011ECD0 + (10836)) = 7;
        *(s16 *)((unsigned char *)D_8011ECD0 + (10850)) = -0xA0;
        *(s16 *)((unsigned char *)D_8011ECD0 + (10854)) = -0x78;
        *(s8 *)((unsigned char *)D_8011ECD0 + (10815)) = 0x24;
        *(s8 *)((unsigned char *)D_8011ECD0 + (10813)) = 1;
    }
    var_v1 = D_800E6280.unk_110D;
    if (var_v1 != 0) {
        temp_t7 = (D_800E6280.unk_03A - 4) & 0xFF;
        var_v0_2 = temp_t7;
        if (temp_t7 < 0x81U) {
            var_v0_2 = 0x80;
            if (var_v1 == 1) {
                var_v1 = 2;
            }
        }
        D_800E6280.unk_03A = var_v0_2;
        D_800E6280.unk_110D = var_v1;
        if (var_v1 == 2) {
            func_8004284C();
        }
    }
    func_8004AC18((s32) D_800E6280.unk_03A);
}
