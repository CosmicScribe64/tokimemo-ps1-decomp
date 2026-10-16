s32 func_8013348C(void) {
    u8 sp2B;
    u32 temp_t7;
    u8 temp_t2;
    u8 var_v0;

    temp_t7 = (u16) D_800E6280.unk_0F4.h >> 0xC;
    switch (temp_t7) {
    case 0:
        sp2B = 0;
        break;
    case 1:
        if ((((u8) D_800E6280.unk_1BC[0].unk_0C.h[1] >> 4) == 1) && (D_8013C2E0 == 0)) {
            var_v0 = 0xB;
        } else if (((u8) D_800E6280.unk_1BC[1].unk_0C.h[1] >> 4) == 1) {
            var_v0 = 1;
            if (D_8013C2E0 == 1) {
                var_v0 = 0xC;
            }
        } else {
            var_v0 = 1;
        }
block_15:
        sp2B = var_v0;
        break;
    case 2:
        var_v0 = 2;
        goto block_15;
    case 3:
        var_v0 = 3;
        goto block_15;
    case 4:
        var_v0 = 4;
        goto block_15;
    case 5:
        var_v0 = 5;
        goto block_15;
    }
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_00 = (u8) D_8013C2E0;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_02 = 0x15;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_01 = sp2B;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = get_g_zyotai_h(D_8013C2E0);
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_04 = D_800E6280.unk_03E;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_05 = D_800E6280.unk_03F;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_06 = D_800E6280.unk_040;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_07 = D_800E6280.unk_041;
    temp_t2 = D_800E6280.unk_F5E + 1;
    D_800E6280.unk_F5E = temp_t2;
    if ((u32) (temp_t2 & 0xFF) >= 0xFFU) {
        D_800E6280.unk_F5E = 0xFE;
    }
}
