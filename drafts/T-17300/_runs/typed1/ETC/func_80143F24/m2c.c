? func_80059048();                                  /* extern */
? func_80142AE8();                                  /* extern */
? main_state_set(?);                                /* extern */
extern ? D_800E6280;
extern ? D_801217D0;
extern s16 D_80150138;

void func_80143F24(void) {
    s16 temp_v0;
    void *var_v1;

    func_80142AE8();
    if (*(u8 *)((unsigned char *)(&D_800E6280) + (58)) != 0) {
        var_v1 = &D_801217D0 + 0x48;
        do {
            var_v1 += 0x24;
            *(u8 *)((unsigned char *)var_v1 + (-14)) = (u8) *(u8 *)((unsigned char *)(&D_800E6280) + (58));
            *(u8 *)((unsigned char *)var_v1 + (-15)) = (u8) *(u8 *)((unsigned char *)(&D_800E6280) + (58));
            *(u8 *)((unsigned char *)var_v1 + (-16)) = (u8) *(u8 *)((unsigned char *)(&D_800E6280) + (58));
        } while (var_v1 != (&D_801217D0 + 0xB4));
        *(u8 *)((unsigned char *)(&D_800E6280) + (58)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (58)) - 1);
    }
    temp_v0 = D_80150138 - 0xC8;
    D_80150138 = temp_v0;
    if (temp_v0 < -0x7530) {
        func_80059048();
        if ((u8) *(u8 *)((unsigned char *)(&D_800E6280) + (3935)) >= 0xDU) {
            main_state_set(0x12);
        } else {
            main_state_set(0xC3);
        }
    }
}
