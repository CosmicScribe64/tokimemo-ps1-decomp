void func_8007A6AC(s32 arg0) {
    s32 sp30;
    s32 sp2C;
    s32 temp_a0;
    s32 temp_v0;
    s32 temp_v1;
    s32 var_a0;
    u8 *temp_v0_2;

    temp_v0 = arg0 & 0xF000;
    switch (temp_v0) {
    case 0x0:
        temp_v0_2 = func_80046284();
        temp_a0 = (*(s32 *)temp_v0_2);
        temp_v1 = *(s32 *)((unsigned char *)temp_v0_2 + (4));
        D_80125D10.unk_10 = *(s32 *)((unsigned char *)temp_v0_2 + (8));
        D_80125D10.unk_14 = *(s32 *)((unsigned char *)temp_v0_2 + (12));
        D_80125D10.unk_48[5] = (u8) ((u32) D_80125D10.unk_10 >> 0x18);
        D_80125D10.unk_48[7] = (u8) ((u32) D_80125D10.unk_14 >> 0x18);
        D_80125D10.unk_10 &= 0xFFFFFF;
        D_80125D10.unk_14 &= 0xFFFFFF;
        sp30 = temp_a0;
        sp2C = temp_v1;
        func_8007A618(temp_a0);
        if (temp_a0 == 0xF) {
            var_a0 = 8;
        } else {
            var_a0 = 0x20;
        }
        D_80125D10.unk_10 = ((D_80125D10.unk_00 * var_a0) + D_80125D10.unk_48[5] + temp_v1) - 0x10;
        D_80125D10.unk_14 = (D_80125D10.unk_14 * var_a0) + D_80125D10.unk_48[5] + temp_v1 + 0x10;
        if (D_80125D10.unk_00 & 0x400) {
            func_8007AB24(0xB4U);
        } else if (!(D_80125D10.unk_00 & 0x200) && !(D_80125D10.unk_00 & 0x4000)) {
            func_8007A868();
            func_8007A924(D_80125D10.unk_08);
        }
        break;
    case 0x1000:
        func_8007A98C(0x74);
        func_8007A868();
        break;
    case 0x2000:
        func_8007A98C(0xE4);
        break;
    case 0x4000:
        func_8007A98C(0xF4);
        break;
    }
}
