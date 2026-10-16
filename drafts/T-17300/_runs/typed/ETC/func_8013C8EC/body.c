s32 func_8013C8EC(void) {
    u32 temp_hi;
    u32 temp_hi_2;
    u32 temp_hi_3;
    u32 temp_hi_4;
    u32 temp_hi_5;
    u32 temp_hi_6;
    u8 temp_a0;
    u8 *temp_v0;

    temp_a0 = *(u8 *)((unsigned char *)(&D_800E6280) + (3935));
    if ((temp_a0 == 0xC) || (temp_a0 == 0xB)) {
        func_8004284C(temp_a0);
    } else {
        switch (*(u8 *)((unsigned char *)(&D_800E6280) + (4365))) {
        case 0:
            if (temp_a0 == 0) {
                do {
loop_9:
                    temp_hi = func_800AE0D0(temp_a0) % (u8) *(&D_8014E0A4 + *(u8 *)((unsigned char *)(&D_800E6280) + (3935)));
                    D_801500C0 = temp_hi;
                    if (temp_hi == D_80150078) {
                        goto loop_9;
                    }
                    D_801500C0 = temp_hi;
                    if (temp_hi == 0x14) {
                        goto loop_9;
                    }
                    D_801500C0 = temp_hi;
                } while (temp_hi == 0);
            } else if (temp_a0 == 2) {
                do {
loop_15:
                    temp_hi_2 = func_800AE0D0(temp_a0) % (u8) *(&D_8014E0A4 + *(u8 *)((unsigned char *)(&D_800E6280) + (3935)));
                    D_801500C0 = temp_hi_2;
                    if (temp_hi_2 == D_80150078) {
                        goto loop_15;
                    }
                    D_801500C0 = temp_hi_2;
                } while (temp_hi_2 == 8);
            } else if (temp_a0 == 3) {
                do {
loop_20:
                    temp_hi_3 = func_800AE0D0(temp_a0) % (u8) *(&D_8014E0A4 + *(u8 *)((unsigned char *)(&D_800E6280) + (3935)));
                    D_801500C0 = temp_hi_3;
                    if (temp_hi_3 == D_80150078) {
                        goto loop_20;
                    }
                    D_801500C0 = temp_hi_3;
                } while (temp_hi_3 == 9);
            } else if (temp_a0 == 4) {
                do {
loop_25:
                    temp_hi_4 = func_800AE0D0(temp_a0) % (u8) *(&D_8014E0A4 + *(u8 *)((unsigned char *)(&D_800E6280) + (3935)));
                    D_801500C0 = temp_hi_4;
                    if (temp_hi_4 == D_80150078) {
                        goto loop_25;
                    }
                    D_801500C0 = temp_hi_4;
                } while (temp_hi_4 == 0xD);
            } else if (temp_a0 == 9) {
                do {
loop_30:
                    temp_hi_5 = func_800AE0D0(temp_a0) % (u8) *(&D_8014E0A4 + *(u8 *)((unsigned char *)(&D_800E6280) + (3935)));
                    D_801500C0 = temp_hi_5;
                    if (temp_hi_5 == D_80150078) {
                        goto loop_30;
                    }
                    D_801500C0 = temp_hi_5;
                } while (temp_hi_5 == 0);
            } else {
                do {
                    temp_hi_6 = func_800AE0D0(temp_a0) % (u8) *(&D_8014E0A4 + *(u8 *)((unsigned char *)(&D_800E6280) + (3935)));
                    D_801500C0 = temp_hi_6;
                } while (temp_hi_6 == D_80150078);
            }
            D_80150078 = D_801500C0;
            temp_v0 = D_801500C4 + (D_801500C0 * 8);
            func_80046290((*(s32 *)temp_v0), *(s32 *)((unsigned char *)temp_v0 + (4)), *(u8 *)((unsigned char *)(&D_800E6280) + (3935)));
            func_80044750(0x300);
            *(u8 *)((unsigned char *)(&D_800E6280) + (4365)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4365)) + 1);
            break;
        case 1:
            if (func_800460EC(temp_a0) & 4) {
                *(u8 *)((unsigned char *)(&D_800E6280) + (4365)) = (u8) (*(u8 *)((unsigned char *)(&D_800E6280) + (4365)) + 1);
            }
            break;
        case 2:
            if (func_800460EC(temp_a0) & 2) {
                func_8004284C();
            }
            break;
        }
    }
}
