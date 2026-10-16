extern s32 D_800E7208;
extern s32 D_8012071C;
extern s8 D_800E7314;
extern s8 D_800E738D;
extern u8 D_800E73A4;
s32 func_8013394C(void) {
    s32 var_v1;
    u8 *var_v0;

    D_801208FB = 0x80;
    menu_check(1, D_8011ECF6, D_8011ECFA);
    func_8004FC10(1);
    func_801320C0();
    var_v0 = D_80120650;
    var_v1 = 0;
    do {
        if (var_v0 == ((D_800E7314 * 0x44) + D_80120650)) {
            *(s16 *)((unsigned char *)var_v0 + (772)) = (s16) (var_v1 + 0xD);
        } else {
            *(s16 *)((unsigned char *)var_v0 + (772)) = (s16) (var_v1 + 0xE);
        }
        var_v0 += 0x44;
        var_v1 += 2;
    } while (var_v0 != &D_8012071C);
    if (D_800E7208 & 0x20) {
        var_v0 = (u8 *) D_800E7314;
        switch (D_800E7314) {
        case 0:
            k_speed_set(0, &D_8012071C, D_800E7314);
            D_800E73A4 |= 0x80;
            func_8004284C();
            var_v0 = func_80044750(0x501);
            break;
        case 1:
            k_speed_set(1, &D_8012071C, D_800E7314);
            D_800E73A4 |= 0x80;
            func_8004284C();
            var_v0 = func_80044750(0x501);
            break;
        case 2:
            k_speed_set(3, &D_8012071C, D_800E7314);
            D_800E73A4 |= 0x80;
            func_8004284C();
            var_v0 = func_80044750(0x501);
            break;
        }
    }
    if (D_800E7208 & 0x40) {
        var_v0 = func_8004284C();
        D_800E738D = 2;
    }
    return (s32) var_v0;
}
