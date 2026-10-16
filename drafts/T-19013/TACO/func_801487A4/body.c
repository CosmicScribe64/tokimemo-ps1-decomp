void func_801487A4(s32 arg0, s32 arg1, void *arg2, void *arg3, void *arg4) {
    u8 st;
    MATRIX m;
    SVECTOR vec;
    SVECTOR rot;
    s32 sel;
    s32 px;
    s32 py;

    sel = arg1 + 0x10;
    if (D_8015EDB4[17].unk2 == 1) {
        px = ((TcPos *)arg2)[arg1].x;
        py = ((TcPos *)arg2)[arg1].y;
        D_8015EDB4[sel].unk84[0] = *(arg1 * 2 + (u8 *)arg3);
        if (arg0 == 0x1C && D_8015EDB4[16].unk84[2] == 1) {
            D_8015EDB4[sel].unk84[0] = *(arg1 * 2 + (u8 *)arg3 + 1);
        }
        if ((arg0 == 0x1C || arg0 == 0x1B || arg0 == 0x15) && D_8015EDB4[21].unk84[2] == 1) {
            D_8015EDB4[sel].unk84[0] = *(arg1 * 2 + (u8 *)arg3 + 1);
        }
    } else if (D_8015EDB4[17].unk2 == 0x1D) {
        px = ((TcPos *)arg2)[arg1].z;
        py = ((TcPos *)arg2)[arg1].unk6;
        D_8015EDB4[sel].unk84[0] = *(arg1 * 2 + (u8 *)arg3 + 1);
        if ((arg0 == 0x1B || arg0 == 0x1C) && D_8015EDB4[22].unk84[2] != 0 && D_8015EDB4[23].unk84[2] == 0) {
            D_8015EDB4[sel].unk84[0] = 0;
        }
    }
    m = *(MATRIX *)D_80125C60;
    func_8015D0D0((D_80128880[0].x << 12) / 360, &m);
    func_8015CF30((D_80128880[0].z << 12) / 360, &m);
    func_8015D270((D_80128880[0].y << 12) / 360, &m);
    vec.vx = D_80127480[arg0].unk18;
    vec.vy = D_80127480[arg0].unk1C;
    vec.vz = D_80127480[arg0].unk20;
    func_800A0ECC(&m, &vec, &rot);
    D_8015EDB4[sel].unk74 = D_80127480[0].unk18 + rot.vx + px;
    D_8015EDB4[sel].unk76 = D_80127480[0].unk1C + rot.vy + py;
    D_8015EDB4[sel].unk78 = D_80127480[0].unk20 + rot.vz;
    st = (sel + D_8015EDB4)->unk84[1];
    if (st < 0x15U && (sel + D_8015EDB4)->unk82 < (sel + D_8015EDB4)->unk7A) {
        (sel + D_8015EDB4)->unk84[1] = ((u8 *)arg4)[arg1 * 2];
        (sel + D_8015EDB4)->unk82 += 1;
        st = D_8015EDB4[sel].unk84[1];
    }
    if (st == 0) {
        if ((D_800E6280.unk_1104.w & 1) == 0) {
            D_80127080[arg0].unk0 = 0x40000000;
        } else {
            D_80127080[arg0].unk0 = 0x80000000;
        }
    }
}
