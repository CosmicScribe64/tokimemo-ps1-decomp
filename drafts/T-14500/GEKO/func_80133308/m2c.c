extern u8 D_800E71DF;
extern u32 D_800E7378;

void func_80133308(void) {
    s16 var_s1;
    s32 temp_s2;
    s32 temp_t3;
    s32 temp_v0;
    s32 var_s6;

    temp_v0 = func_80051A68(D_800E71DF);
    if (D_80122CDC != 0) {
        D_80144C54 = 0;
        temp_t3 = ((u32) D_800E7378 % (u32) ((((s16) D_800E6280.unk_1BC[D_800E71DF].unk_02 / 20) & 0xFF) + 1)) & 0xFF;
        if ((u32) (temp_v0 & 0x7F & 0xFF) < 2U) {
            D_800E6280.unk_1BC[D_800E71DF].unk_0A = D_800E6280.unk_1BC[D_800E71DF].unk_0A + temp_t3 + 0xA;
        } else {
            D_800E6280.unk_1BC[D_800E71DF].unk_0A = D_800E6280.unk_1BC[D_800E71DF].unk_0A + temp_t3 + 6;
        }
        D_80144C3C += 2;
    } else {
        D_80144C54 = 1;
        if ((D_800E71DF == 0) || (D_800E71DF == 7) || (D_800E71DF == 1) || (D_800E71DF == 2) || (D_800E71DF == 8) || (var_s6 = 4, (D_800E71DF == 9))) {
            var_s6 = 0;
            if ((u32) (temp_v0 & 0x7F & 0xFF) >= 3U) {
                D_80144C54 = 0;
            } else {
                var_s6 = 4;
            }
        }
        var_s1 = 0;
        do {
            if ((((u32) (D_800E6280.unk_1BC[var_s1].unk_0C.w << 0x1E) >> 0x1F) != 0) && (var_s1 != D_800E71DF)) {
                temp_s2 = ((s16) D_800E6280.unk_1BC[var_s1].unk_02 / 20) & 0xFF;
                D_800E6280.unk_1BC[var_s1].unk_0A = D_800E6280.unk_1BC[var_s1].unk_0A + var_s6 + ((func_800AE0D0() % (u32) (temp_s2 + 1)) & 0xFF);
            }
            var_s1 += 1;
        } while (var_s1 < 0xB);
    }
    func_80084D3C();
    func_8004284C();
}
