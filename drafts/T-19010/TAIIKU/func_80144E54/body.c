void func_80144E54(void) {
    s32 i;
    u8 *p;

    func_80048E78();
    for (i = 0; i < 8; i++) {
        DISP(96 + i).unk_03 = 0xA4;
        DISP(96 + i).unk_02 = 0x40;
        DISP(96 + i).unk_05 = 0x80;
        DISP(96 + i).unk_01 = 3;
        DISP(96 + i).unk_38 = 0x01000000;
        DISP(96 + i).unk_07 = 0x80;
        DISP(96 + i).unk_0C = (u8 *)D_80149FC8;
        DISP(96 + i).unk_10 = (u8 *)D_8014A000;
        DISP(96 + i).unk_34 = (u8 *)D_8014A228;
        DISP(96 + i).unk_14 = D_8014A038;
        DISP(96 + i).unk_1A = 0x1000;
        *(s16 *)DISP(96 + i).pad_1C = 0x1000;
        DISP(96 + i).unk_06 = 2;
    }
    p = D_8014A120;
    i = 0;
    do {
        DISP(96 + i).unk_26 = i * 0x40 - 0x78;
        DISP(96 + i).unk_18 = *p * 8 + 0x1A;
        DISP(96 + i).unk_2A = -8;
        if (DISP(96 + i).unk_18 == 0x22 && D_8014A110 == 1) {
            DISP(96 + i).unk_18 = 0x42;
        }
        p++;
        i++;
    } while (p < (u8 *)&D_8014A124);
    DISP(100).unk_26 = -0x78;
    DISP(100).unk_2A = -8;
    DISP(100).unk_18 = D_8014A120[0] * 5 + 0xD;
    DISP(101).unk_26 = -0x38;
    DISP(101).unk_2A = -8;
    DISP(101).unk_18 = D_8014A120[1] * 5 + 0xD;
    DISP(102).unk_26 = 8;
    DISP(102).unk_2A = -8;
    DISP(102).unk_18 = D_8014A120[2] * 5 + 0xD;
    DISP(103).unk_26 = 0x48;
    DISP(103).unk_2A = -8;
    DISP(103).unk_18 = D_8014A120[3] * 5 + 0xD;
}
