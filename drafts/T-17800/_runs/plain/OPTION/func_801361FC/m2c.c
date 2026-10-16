? func_8004F984(?, s16, s16);                       /* extern */
extern s8 D_800E71EF;
extern s8 D_800E71F4;
extern s32 D_800E7208;
extern s8 D_800E7314;
extern s8 D_800E738D;
extern u8 D_800E73A4;
extern ? D_801207A4;

s32 func_801361FC(void) {
    s32 var_v1;
    u8 *var_v0;

    D_801208FB = 0x80;
    func_8004F984(1, D_8011ECF6, D_8011ECFA);
    func_8004FC10(1);
    func_801320C0();
    var_v0 = D_80120650;
    var_v1 = 0;
    do {
        if (var_v0 == ((D_800E7314 * 0x44) + D_80120650)) {
            *(s16 *)((unsigned char *)var_v0 + (772)) = (s16) (var_v1 + 0xB);
        } else {
            *(s16 *)((unsigned char *)var_v0 + (772)) = (s16) (var_v1 + 0xC);
        }
        var_v0 += 0x44;
        var_v1 += 2;
    } while (var_v0 != &D_801207A4);
    if (D_800E7208 & 0x20) {
        switch (D_800E7314) {
        case 0:
            D_800E71EF = 1;
            D_800E71F4 = 0xFF;
            D_800E73A4 |= 8;
            func_8004284C();
            var_v0 = func_80044750(0x501);
            break;
        case 1:
            D_800E71EF = 2;
            D_800E71F4 = 0xFF;
            D_800E73A4 |= 8;
            func_8004284C();
            var_v0 = func_80044750(0x501);
            break;
        case 2:
            D_800E71EF = 3;
            D_800E71F4 = 0xFF;
            D_800E73A4 |= 8;
            func_8004284C();
            var_v0 = func_80044750(0x501);
            break;
        case 3:
            D_800E71EF = 4;
            D_800E71F4 = 0xFF;
            D_800E73A4 |= 8;
            func_8004284C();
            var_v0 = func_80044750(0x501);
            break;
        case 4:
            D_800E71EF = 6;
            D_800E71F4 = 0xFF;
            D_800E73A4 |= 8;
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
