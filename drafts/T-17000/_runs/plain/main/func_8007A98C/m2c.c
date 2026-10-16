void func_8007A98C(s32 arg0) {
    s32 temp_v0;
    s32 temp_v0_2;

    temp_v0 = arg0 & 0xFF;
    switch (temp_v0) {                              /* irregular */
    case 0x74:
        func_8008BEE0(0, 0, 0);
        func_80045414(9, 0, NULL);
        D_80125D10.unk_00 = D_80125D10.unk_00 & ~0x400 & ~0x4000;
        D_80125D14 = D_80125D14 & ~0x200 & ~0x100;
        break;
    case 0xE4:
        if (D_80125D14 & 0x200) {
            D_80125D10.unk_00 |= 0x4000;
            func_80045414(9, 0, NULL);
        } else if (D_80125D10.unk_00 & 0x400) {
            temp_v0_2 = func_80046274();
            D_80125D28 = temp_v0_2;
            if ((u32) D_80125D18 >= (u32) temp_v0_2) {
                D_80125D28 = D_80125D18;
            }
            D_80125D10.unk_00 = (D_80125D10.unk_00 & ~0x400) | 0x4000;
            func_8007AB24(0xB4U);
        }
        break;
    case 0xF4:
        if (D_80125D10.unk_00 & 0x4000) {
            D_80125D10.unk_00 &= ~0x4000;
            if (D_80125D20 != 0) {
                func_8007A5BC();
            } else {
                func_8007A924(D_80125D28);
            }
        }
        break;
    }
}
