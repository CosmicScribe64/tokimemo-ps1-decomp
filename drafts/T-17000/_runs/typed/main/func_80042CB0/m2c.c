extern u8 D_800E8BF3;
extern s8 D_800E8BF4;
extern s8 D_800E8BF5;
extern u8 D_800E8C31;
extern u8 D_800E8C32;
extern u8 D_800E8C33;

void func_80042CB0(void) {
    s16 temp_a3;
    s16 var_a3;
    s16 var_t0;
    s32 var_at;
    s32 var_v0;
    u8 temp_t3;
    u8 temp_t8;
    u8 var_a1;

    temp_a3 = D_800E6280.unk_F7A + (s16) ((s32) (D_800E8BF4 * D_800E6280.unk_F6F) / 3);
    var_a3 = temp_a3;
    var_t0 = D_800E6280.unk_F7C + (s16) ((s32) (D_800E8BF5 * D_800E6280.unk_F6F) / 3);
    if (temp_a3 >= 0x8D) {
        var_a3 = 0x8C;
    }
    if (var_a3 < -0x96) {
        var_a3 = -0x96;
    }
    var_at = var_t0 < 0x65;
    if (var_t0 < -0x64) {
        var_t0 = -0x64;
        var_at = -0x64 < 0x65;
    }
    if (var_at == 0) {
        var_t0 = 0x64;
    }
    D_8011ECD0->unk_26 = var_a3;
    D_8011ECD0->unk_2A = var_t0;
    temp_t8 = ~D_800E8BF3 & 0xFF;
    D_800E6280.unk_F7A = var_a3;
    temp_t3 = temp_t8 & ~D_800E6280.unk0 & 0xFF;
    D_800E6280.unk_F7C = var_t0;
    if ((D_800E6280.unk0 != 0) && ((D_800E6280.unk0 == 0) || (temp_t3 != D_800E6280.unk_F72)) && (temp_t3 != 0)) {
        if (temp_t3 & 4) {
            D_800E6280.unk_F72 = temp_t3;
            D_800E6280.unk_F71 = (u8) D_800E6280.unk_F73 + 8;
        } else if (temp_t3 & 8) {
            D_800E6280.unk_F72 = temp_t3;
            D_800E6280.unk_F71 = (u8) D_800E6280.unk_F73 + 8;
        }
    }
    var_a1 = (u8) D_800E6280.unk_F71;
    var_v0 = 0;
    D_800E6280.unk_F77 = temp_t3;
    if ((D_800E6280.unk0 == 0) || ((D_800E6280.unk_F77 = temp_t3, (D_800E6280.unk0 != 0)) && (D_800E6280.unk_F77 = temp_t3, (var_a1 != 0)) && (D_800E6280.unk_F77 = temp_t3, (((u8) D_800E6280.unk_F73 + 8) != var_a1)) && (D_800E6280.unk_F77 = temp_t3, (temp_t3 == D_800E6280.unk_F72)))) {
        D_800E6280.unk_F72 = 0;
        var_a1 = 0;
        if (D_800E6280.unk_F75 != 0) {
            if (temp_t8 & 4) {
                var_v0 = 0x20;
            } else if (temp_t8 & 8) {
                var_v0 = 0x40;
            }
        } else if (temp_t8 & 8) {
            var_v0 = 0x20;
        } else if (temp_t8 & 4) {
            var_v0 = 0x40;
        }
    }
    if (var_a1 != 0) {
        D_800E6280.unk_F71 = var_a1 - 1;
    } else {
        D_800E6280.unk_F71 = (s8) var_a1;
        D_800E6280.unk_F72 = 0;
    }
    if ((*D_800E8C30 == 0) && ((D_800E8C31 & 0xF0) == 0x40)) {
        var_v0 = var_v0 | (~D_800E8C32 << 0x18) | ((~D_800E8C33 & 0xFF) << 0x10);
    }
    D_800E6280.unk_F88 = ~(s32) D_800E6280.unk0 & var_v0;
    D_800E6280.unk_F8C = (s32) D_800E6280.unk0 & ~var_v0;
    D_800E6280.unk_F78 = temp_t8;
    D_800E6280.unk_F80 = var_v0;
    D_800E6280.unk_F76 = temp_t8;
    D_800E6280.unk_F84 = (s32) D_800E6280.unk0;
}
