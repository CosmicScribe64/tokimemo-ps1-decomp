extern u8 D_800E71DF;
extern u8 D_800E738D;
extern ? D_8014E0A4;
extern u32 D_80150078;
extern u32 D_801500C0;

void func_8013C8EC(void) {
    u32 temp_hi;
    u32 temp_hi_2;
    u32 temp_hi_3;
    u32 temp_hi_4;
    u32 temp_hi_5;
    u32 temp_hi_6;
    void *temp_v0;

    if ((D_800E71DF == 0xC) || (D_800E71DF == 0xB)) {
        func_8004284C();
    } else {
        switch (D_800E738D) {                       /* irregular */
        case 0:
            if (D_800E71DF == 0) {
                do {
loop_9:
                    temp_hi = func_800AE0D0() % (u8) *(&D_8014E0A4 + D_800E71DF);
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
            } else if (D_800E71DF == 2) {
                do {
loop_15:
                    temp_hi_2 = func_800AE0D0() % (u8) *(&D_8014E0A4 + D_800E71DF);
                    D_801500C0 = temp_hi_2;
                    if (temp_hi_2 == D_80150078) {
                        goto loop_15;
                    }
                    D_801500C0 = temp_hi_2;
                } while (temp_hi_2 == 8);
            } else if (D_800E71DF == 3) {
                do {
loop_20:
                    temp_hi_3 = func_800AE0D0() % (u8) *(&D_8014E0A4 + D_800E71DF);
                    D_801500C0 = temp_hi_3;
                    if (temp_hi_3 == D_80150078) {
                        goto loop_20;
                    }
                    D_801500C0 = temp_hi_3;
                } while (temp_hi_3 == 9);
            } else if (D_800E71DF == 4) {
                do {
loop_25:
                    temp_hi_4 = func_800AE0D0() % (u8) *(&D_8014E0A4 + D_800E71DF);
                    D_801500C0 = temp_hi_4;
                    if (temp_hi_4 == D_80150078) {
                        goto loop_25;
                    }
                    D_801500C0 = temp_hi_4;
                } while (temp_hi_4 == 0xD);
            } else if (D_800E71DF == 9) {
                do {
loop_30:
                    temp_hi_5 = func_800AE0D0() % (u8) *(&D_8014E0A4 + D_800E71DF);
                    D_801500C0 = temp_hi_5;
                    if (temp_hi_5 == D_80150078) {
                        goto loop_30;
                    }
                    D_801500C0 = temp_hi_5;
                } while (temp_hi_5 == 0);
            } else {
                do {
                    temp_hi_6 = func_800AE0D0() % (u8) *(&D_8014E0A4 + D_800E71DF);
                    D_801500C0 = temp_hi_6;
                } while (temp_hi_6 == D_80150078);
            }
            D_80150078 = D_801500C0;
            temp_v0 = D_801500C4 + (D_801500C0 * 8);
            func_80046290(temp_v0->unk0, *(s32 *)((unsigned char *)temp_v0 + (4)), (s32) D_800E71DF);
            func_80044750(0x300);
            D_800E738D += 1;
            break;
        case 1:
            if (func_800460EC() & 4) {
                D_800E738D += 1;
            }
            break;
        case 2:
            if (func_800460EC() & 2) {
                func_8004284C();
            }
            break;
        }
    }
}
