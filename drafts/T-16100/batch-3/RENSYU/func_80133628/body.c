void func_80133628(void) {
    u16 sp2C[11];
    s16 temp_v0;
    u32 temp_t6;
    u8 temp_t7;

    *(s32 *)sp2C = (s32) D_80134390;
    *(s32 *)((unsigned char *)(&sp2C[0]) + (4)) = (s32) *(s32 *)((unsigned char *)(&D_80134390) + (4));
    *(s32 *)((unsigned char *)(&sp2C[0]) + (8)) = (s32) *(s32 *)((unsigned char *)(&D_80134390) + (8));
    *(s32 *)((unsigned char *)(&sp2C[0]) + (12)) = (s32) *(s32 *)((unsigned char *)(&D_80134390) + (12));
    *(s32 *)((unsigned char *)(&sp2C[0]) + (16)) = (s32) *(s32 *)((unsigned char *)(&D_80134390) + (16));
    *(u16 *)((unsigned char *)(&sp2C[0]) + (20)) = (u16) *(u16 *)((unsigned char *)(&D_80134390) + (20));
    switch (D_800E738A) {
    case 0:
        if (D_80120652 == 0) {
            if ((u8) D_801342C0 < 5U) {
                if (D_801342C0 != 0) {
                    if (((u16) D_800E6374 >> 0xC) == 9) {
                        goto block_6;
                    }
                    goto block_8;
                }
block_8:
                D_80120653 = 0x84;
                D_80120652 = 5;
                D_80120668 = 0;
                D_80120658 = 0;
                if (*(&D_801342C8 + D_801342C0) == 1) {
                    D_80120666 = 0;
                } else {
                    D_80120666 = 1;
                }
                D_801342C0 += 1;
            } else {
block_6:
                func_8004284C();
            }
        }
        break;
    case 1:
        if ((u8) D_801342C4 >= 3U) {
            func_80044750(0x204);
            D_800CA148 = 0x11;
        } else {
            func_80044750(0x205);
            D_800CA148 = (*(&D_800E68EE + (((u16) D_800E6374 >> 0xC) * 4)) & 0xF) + 0x12;
        }
        k_reset(1);
        func_8004284C();
        break;
    case 2:
        normal_date_speak((u16 (*)[]) &sp2C[0]);
        break;
    case 3:
        temp_t6 = (u16) D_800E6374 >> 0xC;
        temp_v0 = D_800E6280.unk_66C[temp_t6].unk_00 - (&sp2C[0])[*(u16 *)((unsigned char *)(&D_800E6280.unk_66C[temp_t6]) + (2)) & 0xF];
        if (temp_v0 < -0x1E) {
            D_800CA148 = 0x1D;
        } else if (temp_v0 < -0x14) {
            D_800CA148 = 0x1E;
        } else if (temp_v0 < -0xA) {
            D_800CA148 = 0x1F;
        } else if (temp_v0 < 0) {
            D_800CA148 = 0x20;
        } else if (temp_v0 >= 0x1E) {
            D_800CA148 = 0x24;
        } else if (temp_v0 >= 0x14) {
            D_800CA148 = 0x23;
        } else if (temp_v0 >= 0xA) {
            D_800CA148 = 0x22;
        } else if (temp_v0 >= 0) {
            D_800CA148 = 0x21;
        }
        func_8004284C();
        break;
    case 4:
        normal_date_speak((u16 (*)[]) &sp2C[0]);
        break;
    case 5:
        temp_t7 = (D_80120657 - 4) & 0xFF;
        D_80120657 = temp_t7;
        if (temp_t7 < 3U) {
            D_80120657 = 0;
        }
        normal_date_bg_fadeout((u16 (*)[]) &sp2C[0]);
        break;
    case 6:
        func_80048F64(0x60);
        if ((D_801342D0 == 0xB) || (D_801342D0 == 0xFF)) {
            func_80042908(7);
        } else {
            func_80042808();
        }
        break;
    }
    Default_Disp();
}
