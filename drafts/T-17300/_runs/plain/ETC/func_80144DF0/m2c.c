? func_8004EC24();                                  /* extern */
? func_8004F984(?, s16, s16);                       /* extern */
extern s32 D_800E7208;
extern s8 D_800E7314;
extern u8 D_800E7398;

void func_80144DF0(void) {
    s32 sp28;
    s32 temp_v1;
    u8 temp_v0;
    u8 temp_v0_2;

    func_8004F984(1, D_8011ECF6, D_8011ECFA);
    func_8004FC10(1);
    func_80050B54(1);
    temp_v1 = D_800E7314 + 1;
    sp28 = temp_v1;
    func_8004EC24();
    if (D_800E7208 & 0x20) {
        if (D_800E7314 != -1) {
            if (*(u8 *)((unsigned char *)(&D_800E7D14[D_800E7314]) + (1)) == 0) {
                temp_v0 = temp_v1 & 0xFF;
                D_800E7398 = temp_v0;
                if (func_80054694((s32) temp_v0) == 1) {
                    func_80053CE0();
                    func_8004284C();
                } else {
                    func_80042940(0xF0);
                }
            } else if (temp_v1 == D_800E7398) {
                func_80053CE0();
                func_8004284C();
            } else {
                temp_v0_2 = temp_v1 & 0xFF;
                if ((u8) D_800E8BEE != 0) {
                    D_800E7D36 = temp_v0_2;
                    D_800E7398 = temp_v0_2;
                    func_800696DC(0, 0);
                    func_8005C7C0();
                    D_800E7314 = D_800E7D36 - 1;
                } else {
                    D_800E7398 = temp_v0_2;
                    func_80042940(0xC0);
                }
            }
        }
    } else if (D_800E7208 & 0x40) {
        func_80042908(2);
    }
    func_8004EC24();
    func_8005C7FC();
}
