void func_801359FC(void) {
    UnalignedBytes35 sp28;
    s32 var_t5;
    u8 temp_t2;
    u8 temp_v0;
    u8 var_v0;

    sp28 = D_80143E80;
    temp_t2 = D_800E7378 & 7;
    var_v0 = temp_t2;
    if (temp_t2 != 0) {
        var_v0 = (temp_t2 - 1) & 0xFF;
    }
    if (D_800E71DF == 3) {
        var_t5 = var_v0 * 4;
        if (var_v0 == 3) {
            var_v0 = 4;
            goto block_5;
        }
    } else {
block_5:
        var_t5 = var_v0 * 4;
    }
    D_80143DD8 = var_v0;
    func_800AE0F0(&D_800CA248, sp28.b + (var_t5 + var_v0));
    temp_v0 = D_80143DD8;
    if (temp_v0 == 0) {
        D_800E639E -= 5;
    }
    D_80143DD8 = temp_v0;
    if (temp_v0 == 4) {
        D_800E639E += 5;
    }
    check_para_limit();
}
