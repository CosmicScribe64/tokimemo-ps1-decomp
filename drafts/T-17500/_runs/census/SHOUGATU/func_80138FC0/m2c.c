extern u16 D_800E6374;
extern u8 D_800E64F2;
extern u8 D_800E71DF;
void func_80138FC0(void) {
    s32 var_v0;
    s32 var_v1;

    var_v1 = 0;
    D_800E6280.unk_1BC[D_800E71DF].unk_0C.b[0] |= 4;
    var_v0 = 0;
    if (((u16) D_800E6374 >> 0xC) == 4) {
        var_v1 = 1;
    }
    if (((u8) D_800E64F2 >> 4) == 4) {
        var_v0 = 1;
    }
    if ((var_v1 == 0) || (var_v0 == 0)) {
        if (var_v0 != 0) {
            D_80144E08 += 2;
        } else {
            D_80144E08 += 4;
            D_800E6280.unk_1BC[D_800E71DF].unk_0C.b[0] &= 0xFFFB;
        }
    }
    func_8004284C();
}
