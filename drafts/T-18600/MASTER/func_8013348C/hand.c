void func_8013348C(void) {
    u32 sel; /* declared before kind: its slot sits above kind's stack home (sp+0x2B) */
    u8 kind;

    sel = (u16) D_800E6280.unk_0F4.h >> 0xC;
    switch (sel) {
    case 0:
        kind = 0;
        break;
    case 1:
        if ((((u8) D_800E6280.unk_1BC[0].unk_0C.b[2] >> 4) == 1) && (D_8013C2E0 == 0)) {
            kind = 0xB;
        } else if (((u8) D_800E6280.unk_1BC[1].unk_0C.b[2] >> 4) == 1) {
            kind = 1;
            if (D_8013C2E0 == 1) {
                kind = 0xC;
            }
        } else {
            kind = 1;
        }
        break;
    case 2:
        kind = 2;
        break;
    case 3:
        kind = 3;
        break;
    case 4:
        kind = 4;
        break;
    case 5:
        kind = 5;
        break;
    }
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_00 = D_8013C2E0;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_02 = 0x15;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_01 = kind;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_03 = get_g_zyotai_h(D_8013C2E0);
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_04 = D_800E6280.unk_03E;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_05 = D_800E6280.unk_03F;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_06 = D_800E6280.unk_040;
    D_800E6280.unk_75E[D_800E6280.unk_F5E].unk_07 = D_800E6280.unk_041;
    D_800E6280.unk_F5E++;
    if (D_800E6280.unk_F5E >= 0xFF) {
        D_800E6280.unk_F5E = 0xFE;
    }
}
