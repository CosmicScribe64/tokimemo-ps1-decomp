? func_8004F984(?, s16, s16);                       /* extern */
extern u8 D_800E71F3;
extern u8 D_800E71F5;
extern u8 D_800E71F7;
extern s32 D_800E7208;
extern s8 D_800E7314;
extern u8 D_800E73A4;

void func_80135110(void) {
    s32 var_v1;
    u8 *var_v0;
    u8 temp_t9;
    u8 var_v0_2;

    func_8004F984(1, D_8011ECF6, D_8011ECFA);
    func_8004FC10(1);
    func_801320C0();
    var_v0 = D_80120650;
    var_v1 = 0;
    do {
        if (var_v0 == ((D_800E7314 * 0x44) + D_80120650)) {
            *(s16 *)((unsigned char *)var_v0 + (1248)) = (s16) (var_v1 + 0x29);
        } else {
            *(s16 *)((unsigned char *)var_v0 + (1248)) = (s16) (var_v1 + 0x2A);
        }
        var_v0 += 0x44;
        var_v1 += 2;
    } while (var_v0 != D_801206D8);
    if (((D_800E71F5 != 0) && (D_800E71F7 & 4)) || ((D_800E71F5 == 0) && (D_800E71F7 & 8))) {
        switch (D_800E7314) {                       /* irregular */
        case 0:
            D_800E73A4 |= 0x20;
            D_800E71F3 = 0x20;
            func_80044750(0x501);
            func_8004284C();
            break;
        case 1:
            if ((u8) D_8013D180 != 0) {
                D_800E73A4 |= 0x20;
                temp_t9 = (0xFF - (u8) D_8013D180) & 0xFF;
                var_v0_2 = temp_t9;
                if (temp_t9 < 0x10U) {
                    var_v0_2 = 0x10;
                }
                D_800E71F3 = var_v0_2;
                if (var_v0_2 >= 0x41U) {
                    D_800E71F3 = 0x40;
                }
                func_80044750(0x501);
                func_8004284C();
                D_8013D180 = 0;
            } else {
                D_8013D180 = -1;
            }
            break;
        }
    }
    if (D_800E7208 & 0x40) {
        D_80120BA3 &= 0xFF7F;
        func_8004284C();
    }
    func_801352FC();
    if ((u8) D_8013D180 != 0) {
        D_8013D180 = (u8) D_8013D180 - 1;
    }
}
