void func_8007B144(s32 arg0) {
    s32 temp_v0;
    u32 temp_v0_2;

    temp_v0 = arg0 & 0xFF;
    switch (temp_v0) {                              /* irregular */
    case 0x71:
        temp_v0_2 = D_80125D10.unk_00;
        if ((temp_v0_2 & 0x100) || (temp_v0_2 & 0x1000)) {
            func_8008C720((s32) D_80125D38, 0, 0);
            func_80090580(D_80125D38);
            func_800907E0(D_80125D38);
        }
        D_80125D3A = D_80125D3C;
        D_80125D3C = -1;
        D_80125D14 &= ~1;
        D_80125D10.unk_00 = temp_v0_2 & ~0x100 & ~0x1000;
        break;
    case 0xE1:
        if (D_80125D10.unk_00 & 0x100) {
            func_80090AB0((s32) D_80125D38);
            D_80125D10.unk_00 = (D_80125D10.unk_00 & ~0x100) | 0x1000;
        }
        break;
    case 0xF1:
        if (D_80125D10.unk_00 & 0x1000) {
            SsSeqReplay((s32) D_80125D38);
            D_80125D10.unk_00 = (D_80125D10.unk_00 & ~0x1000) | 0x100;
        }
        break;
    }
}
