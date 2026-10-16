extern u8 D_800E62BA;
extern u8 D_800E71DF;

void func_80143F24(void) {
    s16 temp_v0;
    s32 *var_v1;

    func_80142AE8();
    if (D_800E62BA != 0) {
        var_v1 = &D_80121818;
        do {
            var_v1 += 0x24;
            *(u8 *)((unsigned char *)var_v1 + (-14)) = (u8) D_800E62BA;
            *(u8 *)((unsigned char *)var_v1 + (-15)) = (u8) D_800E62BA;
            *(u8 *)((unsigned char *)var_v1 + (-16)) = (u8) D_800E62BA;
        } while (var_v1 != &D_80121884);
        D_800E62BA -= 1;
    }
    temp_v0 = D_80150138 - 0xC8;
    D_80150138 = temp_v0;
    if (temp_v0 < -0x7530) {
        func_80059048();
        if ((u8) D_800E71DF >= 0xDU) {
            main_state_set(0x12);
        } else {
            main_state_set(0xC3);
        }
    }
}
