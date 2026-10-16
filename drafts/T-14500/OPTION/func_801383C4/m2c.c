? func_8004F984(?, s16, s16, GameState *);          /* extern */
extern s8 D_800E691D;
extern s8 D_800E699C;
extern u8 D_800E71DF;
extern s32 D_800E7208;
extern s8 D_800E7314;
extern u8 D_800E738A;
extern ? D_800E7D24;
extern ? D_80120A08;

void func_801383C4(void) {
    s32 *var_v0;
    s32 var_a0;
    s32 var_v1;
    u8 *var_v0_2;
    u8 temp_v1;

    var_v0 = &D_800E7D10;
    var_a0 = 0;
    do {
        if ((*(u8 *)((unsigned char *)var_v0 + (20)) != 0) || (D_800E6280.unk_53C[var_a0] != 0)) {
            *(s8 *)((unsigned char *)(D_80120650 + (var_a0 * 0x44)) + (1023)) = 0x80;
        }
        var_a0 += 1;
        var_v0 += 1;
    } while (var_a0 < 0xE);
    D_8012170F = 0x80;
    D_80121726 = 0x1000;
    D_80121728 = 0x1000;
    func_8004F984(1, D_8011ECF6, D_8011ECFA, &D_800E6280);
    func_8004FC10(1);
    func_801320C0();
    var_v0_2 = D_80120650;
    var_v1 = 0;
    do {
        if (var_v0_2 == ((D_800E7314 * 0x44) + D_80120650)) {
            *(s16 *)((unsigned char *)var_v0_2 + (1044)) = (s16) (var_v1 + 0x5B);
        } else {
            *(s16 *)((unsigned char *)var_v0_2 + (1044)) = (s16) (var_v1 + 0x5C);
        }
        var_v0_2 += 0x44;
        var_v1 += 2;
    } while (var_v0_2 != &D_80120A08);
    if ((D_800E7208 & 0x20) && ((*(&D_800E7D24 + D_800E7314) != 0) || (D_800E6280.unk_53C[D_800E7314] != 0)) && (D_800E7314 >= 0)) {
        temp_v1 = D_800E7314 & 0xFF;
        if (D_800E7314 < 0xE) {
            D_800E691D = D_800E7314;
            D_800E699C = 0;
            D_800E71DF = temp_v1;
            if (temp_v1 < 0xDU) {
                func_80048EB8(0);
                func_8004E58C();
                func_80044890(1, 0xBF98, 0xBF79, 0xCE6D, 0xCE33, 0xCE1E);
                func_80044750(0x501);
                func_80042940(0x21);
            } else {
                func_80048EB8(0);
                func_8004E58C();
                func_80044750(0x501);
                func_80042940(0x23);
            }
        }
    }
    if (D_800E7208 & 0x40) {
        D_800E7314 = -1;
        func_80042940((D_800E738A + 1) & 0xFF);
    }
}
