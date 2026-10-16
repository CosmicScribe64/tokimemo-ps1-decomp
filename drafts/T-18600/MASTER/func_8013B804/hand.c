void func_8013B804(void) {
    u32 sel;

    sel = (u16) D_800E6280.unk_0F4.h >> 0xC;
    switch (sel) {
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
    D_800E6280.unk_F5E++;
    if (D_800E6280.unk_F5E >= 0xFF) {
        D_800E6280.unk_F5E = 0xFE;
    }
}
