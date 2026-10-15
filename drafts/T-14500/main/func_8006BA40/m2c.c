extern u8 D_800E71EF;
extern s32 D_800E7200;
extern s32 D_800E7208;
extern u8 D_800E7312;

void func_8006BA40(void) {
    s16 var_v0_2;
    s16 var_v1_2;
    s32 temp_v1;
    s32 temp_v1_2;
    u8 var_v0;
    u8 var_v1;

    if ((u8) D_8011ECA4 == 0) {
        var_v1 = D_800E71EF;
        D_8011ECA4 = (u8) D_8011ECA4 + 1;
        if (var_v1 == 0) {
            var_v1 = 1;
        }
        D_800E71EF = var_v1;
        var_v0 = var_v1;
        if (!(D_800E7312 & 3) && (D_800E7200 & 0xFF)) {
            var_v0 = var_v1 * 2;
        }
        temp_v1 = D_800E7200 & 0x5000;
        if (D_800E7200 & 0xF) {
            var_v1_2 = D_8011ECF6;
        } else {
            switch (temp_v1) {                      /* irregular */
            case 0x1000:
                D_8011ECFA -= var_v0;
                break;
            case 0x4000:
                D_8011ECFA += var_v0;
                break;
            }
            temp_v1_2 = D_800E7200 & 0xA000;
            if (temp_v1_2 != 0x2000) {
                if (temp_v1_2 != 0x8000) {
                    var_v1_2 = D_8011ECF6;
                } else {
                    var_v1_2 = D_8011ECF6 - var_v0;
                }
            } else {
                var_v1_2 = D_8011ECF6 + var_v0;
            }
        }
        if (var_v1_2 >= 0x8D) {
            var_v1_2 = 0x8C;
        }
        D_8011ECF6 = var_v1_2;
        if (var_v1_2 < -0x96) {
            D_8011ECF6 = -0x96;
        }
        if (D_8011ECFA < -0x64) {
            D_8011ECFA = -0x64;
        }
        if (D_8011ECFA >= 0x65) {
            D_8011ECFA = 0x64;
        }
        if (D_800E7208 & 0x80) {
            if (D_800B6708 == 0) {
                var_v0_2 = 0x9B;
            } else if (D_800B6708 == 0xAC) {
                var_v0_2 = 0;
            } else {
                var_v0_2 = (D_800B6708 + 1) & 0xFF;
            }
            D_8011ECE8 = var_v0_2;
            D_800B6708 = (u8) var_v0_2;
        }
    }
}
