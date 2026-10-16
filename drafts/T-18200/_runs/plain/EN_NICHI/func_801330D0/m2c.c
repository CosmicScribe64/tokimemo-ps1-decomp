extern u8 D_800E71F4;
extern s32 D_800E7200;

void func_801330D0(void) {
    s32 temp_a0;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 var_at;
    s32 var_v1;

    var_v1 = 2;
    if (D_800E7200 & 0x40) {
        var_v1 = 3;
    }
    if (D_800E71F4 == 1) {
        temp_a0 = D_800E7200 & ~0x60;
        switch (temp_a0) {                          /* irregular */
        case 0x1000:
            D_8011ECF8 -= var_v1 << 0x10;
            break;
        case 0x2000:
            D_8011ECF4 += var_v1 << 0x10;
            break;
        case 0x4000:
            D_8011ECF8 += var_v1 << 0x10;
            break;
        case 0x8000:
            D_8011ECF4 -= var_v1 << 0x10;
            break;
        case 0x3000:
            temp_v0 = var_v1 * 0xB505;
            D_8011ECF4 += temp_v0;
            D_8011ECF8 -= temp_v0;
            break;
        case 0x6000:
            temp_v0_2 = var_v1 * 0xB505;
            D_8011ECF4 += temp_v0_2;
            D_8011ECF8 += temp_v0_2;
            break;
        case 0xC000:
            temp_v0_3 = var_v1 * 0xB505;
            D_8011ECF4 -= temp_v0_3;
            D_8011ECF8 += temp_v0_3;
            break;
        case 0x9000:
            temp_v0_4 = var_v1 * 0xB505;
            D_8011ECF4 -= temp_v0_4;
            D_8011ECF8 -= temp_v0_4;
            break;
        }
    }
    if (*(s16 *)((unsigned char *)D_8011ECD0 + (38)) < -0x8F) {
        *(s16 *)((unsigned char *)D_8011ECD0 + (38)) = -0x90;
    }
    if (*(s16 *)((unsigned char *)D_8011ECD0 + (38)) >= 0x90) {
        *(s16 *)((unsigned char *)D_8011ECD0 + (38)) = 0x90;
    }
    var_at = *(s16 *)((unsigned char *)D_8011ECD0 + (42)) < 0x68;
    if (*(s16 *)((unsigned char *)D_8011ECD0 + (42)) < -0x67) {
        *(s16 *)((unsigned char *)D_8011ECD0 + (42)) = -0x68;
        var_at = *(s16 *)((unsigned char *)D_8011ECD0 + (42)) < 0x68;
    }
    if (var_at == 0) {
        *(s16 *)((unsigned char *)D_8011ECD0 + (42)) = 0x68;
    }
    D_80121556 = *(s16 *)((unsigned char *)D_8011ECD0 + (38));
    D_8012155A = *(s16 *)((unsigned char *)D_8011ECD0 + (42));
}
