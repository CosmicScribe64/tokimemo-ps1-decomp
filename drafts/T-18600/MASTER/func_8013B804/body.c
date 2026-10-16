void func_8013B804(void) {
    u32 temp_t6;
    u8 temp_t0;

    temp_t6 = (u16) D_800E6280.unk_0F4.h >> 0xC;
    switch (temp_t6) {
    case 7:
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_01 = 7;
        break;
    case 8:
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_01 = 8;
        break;
    case 9:
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_01 = 9;
        break;
    case 10:
        D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_01 = 0xA;
        break;
    }
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_00 = D_800E6280.unk_F5F;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_02 = 0x15;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = 5;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_04 = D_800E6280.unk_03E;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_05 = D_800E6280.unk_03F;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_06 = D_800E6280.unk_040;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_07 = D_800E6280.unk_041;
    temp_t0 = D_800E6280.unk_F5E + 1;
    D_800E6280.unk_F5E = temp_t0;
    if ((u32) (temp_t0 & 0xFF) >= 0xFFU) {
        D_800E6280.unk_F5E = 0xFE;
    }
}
