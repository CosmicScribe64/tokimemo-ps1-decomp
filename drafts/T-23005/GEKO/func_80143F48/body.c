/* T-23005 hand draft (not built in the tree): 106 rows against the original; GekoDateEntry (GEKO.h) is the calendar word of func_8006B5BC. The locals are declared kind, entry, tbl to put entry at sp+0x48 and tbl at sp+0x30 as in the original. */
void func_80143F48(void) {
    u8 kind;
    GekoDateEntry entry;
    GekoBytes21 tbl;

    tbl = D_80147C3C;
    if (D_80122CDC != 0) {
        D_801476CC = D_8014765C;
        D_801476D0 = D_80147690;
        D_801476D4 = D_801476C4;
        D_801476D8 = D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0A / 20;
        if (D_801476D8 >= 5) {
            D_801476D8 = 4;
        }
    } else {
        entry.bits.place = tbl.v[D_801476E0];
        entry.bits.year = func_80144198(0);
        entry.bits.month = func_80144198(1);
        entry.bits.day = func_80144198(2);
        entry.bits.unk_24 = 1;
        entry.bits.girl = D_800E6280.unk_F5F;
        entry.bits.active = 1;
        entry.bits.unk_21 = 6;
        entry.bits.unk_30 = 0;
        func_8006B5BC(entry.word);
        kind = tbl.v[D_801476E0];
        if (kind == 0xE) {
            D_800E6280.unk_74C[D_800E6280.unk_F5F] |= 1;
        }
        if (kind == 0xF) {
            D_800E6280.unk_74C[D_800E6280.unk_F5F] |= 2;
        }
        if (kind == 7) {
            D_800E6280.unk_74C[D_800E6280.unk_F5F] |= 4;
        }
    }
    func_8004284C();
}
