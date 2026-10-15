extern u8 D_800E71EF;
extern u8 D_800E71F0;
extern u8 D_800E71F1;
extern u8 D_800E71F2;
extern u8 D_800E71F3;
extern u8 D_800E71F5;
extern u8 D_800E71F6;
extern u8 D_800E71F7;
extern u8 D_800E71F8;
extern s16 D_800E71FA;
extern s16 D_800E71FC;
extern s32 D_800E7200;
extern s32 D_800E7204;
extern s32 D_800E7208;
extern s32 D_800E720C;
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
    s32 temp_a0;
    s32 var_at;
    s32 var_v0;
    u8 temp_t3;
    u8 temp_t8;
    u8 var_a1;

    temp_a3 = D_800E71FA + (s16) ((s32) (D_800E8BF4 * D_800E71EF) / 3);
    var_a3 = temp_a3;
    var_t0 = D_800E71FC + (s16) ((s32) (D_800E8BF5 * D_800E71EF) / 3);
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
    D_8011ECF6 = var_a3;
    D_8011ECFA = var_t0;
    temp_a0 = D_800E7200;
    temp_t8 = ~D_800E8BF3 & 0xFF;
    D_800E71FA = var_a3;
    temp_t3 = temp_t8 & ~D_800E71F8 & 0xFF;
    D_800E71FC = var_t0;
    if ((D_800E71F0 != 0) && ((D_800E71F1 == 0) || (temp_t3 != D_800E71F2)) && (temp_t3 != 0)) {
        if (temp_t3 & 4) {
            D_800E71F2 = temp_t3;
            D_800E71F1 = D_800E71F3 + 8;
        } else if (temp_t3 & 8) {
            D_800E71F2 = temp_t3;
            D_800E71F1 = D_800E71F3 + 8;
        }
    }
    var_a1 = D_800E71F1;
    var_v0 = 0;
    D_800E71F7 = temp_t3;
    if ((D_800E71F0 == 0) || ((D_800E71F7 = temp_t3, (D_800E71F0 != 0)) && (D_800E71F7 = temp_t3, (var_a1 != 0)) && (D_800E71F7 = temp_t3, ((D_800E71F3 + 8) != var_a1)) && (D_800E71F7 = temp_t3, (temp_t3 == D_800E71F2)))) {
        D_800E71F2 = 0;
        var_a1 = 0;
        if (D_800E71F5 != 0) {
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
        D_800E71F1 = var_a1 - 1;
    } else {
        D_800E71F1 = var_a1;
        D_800E71F2 = 0;
    }
    if ((*D_800E8C30 == 0) && ((D_800E8C31 & 0xF0) == 0x40)) {
        var_v0 = var_v0 | (~D_800E8C32 << 0x18) | ((~D_800E8C33 & 0xFF) << 0x10);
    }
    D_800E7208 = ~temp_a0 & var_v0;
    D_800E720C = temp_a0 & ~var_v0;
    D_800E71F8 = temp_t8;
    D_800E7200 = var_v0;
    D_800E71F6 = temp_t8;
    D_800E7204 = temp_a0;
}
