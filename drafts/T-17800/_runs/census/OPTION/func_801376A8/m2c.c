extern s32 D_800E7208;
extern s8 D_800E7314;
extern s8 D_80120A4F;
extern s8 D_80120A93;
void func_801376A8(void) {
    s32 temp_t3;
    s32 temp_t9;
    s32 var_v1;
    u8 *var_v0;

    D_8012170F = 0x80;
    D_80121726 = 0x1000;
    D_80121728 = 0x1000;
    menu_check(1, D_8011ECF6, D_8011ECFA);
    func_8004FC10(1);
    func_801320C0();
    temp_t9 = D_800E7D4F & 1;
    if (temp_t9 == 1) {
        D_80120A4F = 0x80;
    }
    temp_t3 = D_800E7D50 & 1;
    if (temp_t3 == 1) {
        D_80120A93 = 0x80;
    }
    var_v0 = D_80120650;
    var_v1 = 0;
    do {
        if (var_v0 == ((D_800E7314 * 0x44) + D_80120650)) {
            *(s16 *)((unsigned char *)var_v0 + (1044)) = (s16) (var_v1 + 0x3B);
        } else {
            *(s16 *)((unsigned char *)var_v0 + (1044)) = (s16) (var_v1 + 0x3C);
        }
        var_v0 += 0x44;
        var_v1 += 2;
    } while (var_v0 != D_801206D8);
    if (D_800E7208 & 0x20) {
        switch (D_800E7314) {
        case 0:
            if (temp_t9 == 1) {
                D_800E7D4F &= 0xFFFD;
                func_80044750(0x501);
                func_80048EB8(0);
                func_8004E58C();
                main_state_set(0xD0);
            }
            break;
        case 1:
            if (temp_t3 == 1) {
                D_800E7D50 = (u8) D_800E7D50 & 0xFFFD;
                func_80044750(0x501);
                func_80048EB8(0);
                func_8004E58C();
                main_state_set(0x90);
            }
            break;
        }
    } else if (D_800E7208 & 0x40) {
        func_8004284C();
    }
}
