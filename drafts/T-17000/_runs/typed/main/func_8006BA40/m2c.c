void func_8006BA40(void) {
    s16 var_v0_2;
    s16 var_v1_2;
    s32 temp_v1;
    s32 temp_v1_2;
    u8 var_v0;
    u8 var_v1;

    if ((u8) D_8011ECA4 == 0) {
        var_v1 = D_800E6280.unk_F6F;
        D_8011ECA4 = (u8) D_8011ECA4 + 1;
        if (var_v1 == 0) {
            var_v1 = 1;
        }
        D_800E6280.unk_F6F = var_v1;
        var_v0 = var_v1;
        if (!(D_800E6280.unk_1092 & 3) && (D_800E6280.unk_F80 & 0xFF)) {
            var_v0 = var_v1 * 2;
        }
        temp_v1 = D_800E6280.unk_F80 & 0x5000;
        if (D_800E6280.unk_F80 & 0xF) {
            var_v1_2 = D_8011ECD0->unk_26;
        } else {
            switch (temp_v1) {                      /* irregular */
            case 0x1000:
                D_8011ECD0->unk_2A = D_8011ECD0->unk0 - var_v0;
                break;
            case 0x4000:
                D_8011ECD0->unk_2A = D_8011ECD0->unk0 + var_v0;
                break;
            }
            temp_v1_2 = D_800E6280.unk_F80 & 0xA000;
            if (temp_v1_2 != 0x2000) {
                if (temp_v1_2 != 0x8000) {
                    var_v1_2 = D_8011ECD0->unk_26;
                } else {
                    var_v1_2 = D_8011ECD0->unk_26 - var_v0;
                }
            } else {
                var_v1_2 = D_8011ECD0->unk_26 + var_v0;
            }
        }
        if (var_v1_2 >= 0x8D) {
            var_v1_2 = 0x8C;
        }
        D_8011ECD0->unk_26 = var_v1_2;
        if (var_v1_2 < -0x96) {
            D_8011ECD0->unk_26 = -0x96;
        }
        if (D_8011ECD0->unk0 < -0x64) {
            D_8011ECD0->unk_2A = -0x64;
        }
        if (D_8011ECD0->unk0 >= 0x65) {
            D_8011ECD0->unk_2A = 0x64;
        }
        if (D_800E6280.unk0 & 0x80) {
            if (D_800B6708 == 0) {
                var_v0_2 = 0x9B;
            } else if (D_800B6708 == 0xAC) {
                var_v0_2 = 0;
            } else {
                var_v0_2 = (D_800B6708 + 1) & 0xFF;
            }
            D_8011ECD0->unk_18 = var_v0_2;
            D_800B6708 = (u8) var_v0_2;
        }
    }
}
