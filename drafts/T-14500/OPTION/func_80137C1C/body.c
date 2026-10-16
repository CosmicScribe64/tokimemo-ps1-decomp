void func_80137C1C(void) {
    s32 sp2C;
    s32 *var_v0;
    s32 var_a0;
    s32 var_v1;
    s8 temp_a3;
    u32 *temp_v0;
    u8 *var_v0_2;

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
    menu_check(1, D_8011ECF6, D_8011ECFA);
    func_8004FC10(1);
    var_v0_2 = D_80120650;
    var_v1 = 0;
    do {
        if (var_v0_2 == ((D_800E7314 * 0x44) + D_80120650)) {
            *(s16 *)((unsigned char *)var_v0_2 + (1044)) = (s16) (var_v1 + 0x3F);
        } else {
            *(s16 *)((unsigned char *)var_v0_2 + (1044)) = (s16) (var_v1 + 0x40);
        }
        var_v0_2 += 0x44;
        var_v1 += 2;
    } while (var_v0_2 != &D_80120A08);
    if (D_800E7208 & 0x40) {
        func_80044750(0x74);
        D_800E7314 = -1;
        func_80042940((D_800E738A + 2) & 0xFF);
    } else if (D_800E7208 & 0x20) {
        temp_a3 = D_800E7314;
        if ((*(&D_800E7D24 + D_800E7314) != 0) || (D_800E6280.unk_53C[D_800E7314] != 0)) {
            sp2C = (s32) temp_a3;
            if ((func_800460EC() & 2) && (temp_a3 >= 0) && (temp_a3 < 0xE)) {
                temp_v0 = D_8013D2C4[temp_a3];
                if ((*(u32 *)temp_v0) != 0) {
                    D_801230D0 = 1;
                    D_8013D2FC = 0;
                    func_80046290(*(s32 *)((unsigned char *)temp_v0 + (4)), *(s32 *)((unsigned char *)temp_v0 + (8)), (s32) temp_a3);
                    func_80044750(0x300);
                    func_8004284C();
                }
            }
        } else {
            func_80044750(0x74);
        }
    }
    func_801320C0();
}
