s32 func_80139E3C(arg0)
    s32 arg0;
{
    u16 v;
    s32 *t;
    u8 g;

    if (arg0 > 0) {
        D_8013D3B8 = arg0;
    }
    v = D_8013D3B8;
    if (arg0 < 0) {
        v++;
    }
    if (D_800E6280.unk_F80 & 4) {
        v++;
    }
    if ((D_800E6280.unk_F80 & 1) && (v != 0)) {
        v--;
    }
    g = D_800E6280.unk_F5F;
    t = D_8013D360[g & 0xF];
    D_8013D3B8 = v;
    if (t != 0) {
        t += v * 2;
        D_8013D3B8 = v;
        func_80046290(t[0], t[1], g);
    }
    func_80044750(0x300);
}
