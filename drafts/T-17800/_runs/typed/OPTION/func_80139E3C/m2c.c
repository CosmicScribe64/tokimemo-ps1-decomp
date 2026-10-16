void func_80139E3C(s32 arg0) {
    s32 *temp_v1;
    u16 var_a3;
    void *temp_v0;

    if (arg0 > 0) {
        D_8013D3B8 = (u16) arg0;
    }
    var_a3 = D_8013D3B8;
    if (arg0 < 0) {
        var_a3 = (var_a3 + 1) & 0xFFFF;
    }
    if (D_800E6280 & 4) {
        var_a3 = (var_a3 + 1) & 0xFFFF;
    }
    if ((D_800E6280 & 1) && (var_a3 != 0)) {
        var_a3 = (var_a3 - 1) & 0xFFFF;
    }
    temp_v1 = D_8013D360[(u8) D_800E6280 & 0xF];
    D_8013D3B8 = var_a3;
    if (temp_v1 != NULL) {
        temp_v0 = temp_v1 + (var_a3 * 8);
        D_8013D3B8 = var_a3;
        func_80046290(temp_v0->unk0, *(s32 *)((unsigned char *)temp_v0 + (4)), (s32) (u8) D_800E6280);
    }
    func_80044750(0x300);
}
