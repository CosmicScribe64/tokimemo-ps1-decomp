void func_801022C0(void) {
    s16 i;
    u8 c;
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    EvFnTbl53 tbl;

    tbl = D_801225A8;
    func_80078950("%d %d %d\n", D_800B1AF6, D_80094714, D_80094718);
    idx = D_800B1AF6;
    tbl.f[idx](0x80);
    if (D_800B1AF6 == 0xE && !(D_800B1AE4 & 1)) {
        if (D_800F19AB == 0x80) {
            c = 0x60;
        } else {
            c = 0x80;
        }
        D_800EAFA7 = c;
        D_800EAFEB = c;
        for (i = 0; i < 6; i++) {
            ((Rec24 *)D_800EC190)[i + 2].unk_14 = ((Rec24 *)D_800EC190)[i + 2].unk_15 = ((Rec24 *)D_800EC190)[i + 2].unk_16 = c;
        }
        D_800F19AB = c;
    }
}
