void func_801449D4(void) {
    GameState *g = &D_800E6280;
    s32 pad0, pad1, pad2; /* FAKE: unused words give the frame of the original; real source unknown. T-17207 */
    u8 sel; /* FAKE: copy of D_800CA2F8 reproduces the hoisted lbu; real source unknown. T-17207 */
    FnTbl76 tbl;

    tbl = D_8015BF50;
    if (&func_80133DE8 == tbl.f[g->unk_110A]) {
        switch (D_800CA2F8) {
        case 1:
            tbl.f[g->unk_110A] = func_80133EA0;
            break;
        case 2:
            tbl.f[g->unk_110A] = func_80135B4C;
            break;
        case 3:
            tbl.f[g->unk_110A] = func_8013426C;
            break;
        case 4:
            tbl.f[g->unk_110A] = func_801344C8;
            break;
        case 5:
            tbl.f[g->unk_110A] = func_801347B8;
            break;
        case 6:
            tbl.f[g->unk_110A] = func_801349A0;
            break;
        case 7:
            tbl.f[g->unk_110A] = func_80134C64;
            break;
        case 8:
            tbl.f[g->unk_110A] = func_8013518C;
            break;
        case 9:
            tbl.f[g->unk_110A] = func_801353F8;
            break;
        case 10:
            tbl.f[g->unk_110A] = func_80135620;
            break;
        case 11:
            tbl.f[g->unk_110A] = func_80135804;
            break;
        case 12:
            tbl.f[g->unk_110A] = func_80135A34;
            break;
        case 13:
            tbl.f[g->unk_110A] = func_80135C14;
            break;
        case 14:
            tbl.f[g->unk_110A] = func_80135E7C;
            break;
        case 15:
            tbl.f[g->unk_110A] = func_80136340;
            break;
        case 16:
            tbl.f[g->unk_110A] = func_801366A4;
            break;
        case 17:
            tbl.f[g->unk_110A] = func_80136870;
            break;
        case 18:
            tbl.f[g->unk_110A] = func_80136A74;
            break;
        case 19:
            tbl.f[g->unk_110A] = func_80136B70;
            break;
        case 20:
            tbl.f[g->unk_110A] = func_80136D3C;
            break;
        }
    }
    sel = D_800CA2F8;
    if (g->unk_110A != 0) {
        if (((g->unk_03E == 0x5F) && (g->unk_03F == 7) && (g->unk_040 == 0x17)) || ((g->unk_03E == 0x60) && (g->unk_03F == 7) && (g->unk_040 == 0x1C)) || ((g->unk_03E == 0x61) && (g->unk_03F == 7) && (g->unk_040 == 0x1B))) {
            if (D_800B593C == 0x80) {
                func_8006B900();
                D_80122D44 = 1;
            }
        } else if ((sel == 0x11) && (D_800B593C == 0x80) && (g->unk_110A != 0x14)) {
            if (g->unk_110A != 0x15) {
                func_8006B900();
                D_80122D44 = 1;
            } else {
                goto block_42;
            }
        } else {
block_42:
            if ((sel == 0x10) && (D_800B593C == 0x80) && (g->unk_110A != 0x14) && (g->unk_110A != 0x15)) {
                func_8006B900();
                D_80122D44 = 1;
            } else if ((g->unk_110A == 0x14) && (g->unk_724 == 0x11)) {
                func_8006B900();
                D_80122D44 = 1;
            } else if ((g->unk_110A == 0x14) && (g->unk_724 == 0x10)) {
                func_8006B900();
                D_80122D44 = 1;
            }
        }
    }
    tbl.f[g->unk_110A](0x80);
    if ((D_800CA2F8 == 0xB) && (g->unk_110A >= 0x2CU) && (g->unk_110A < 0x30U) && (D_800B3CFC == 1) && (D_800CA2FC != 2)) {
        D_800CA2FC = 2;
        func_80044750(0x601);
    }
}
