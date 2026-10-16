? func_8007E5A0();                                  /* extern */
extern u8 D_800E71DF;
extern s32 D_800E7384;
extern u8 D_800E738A;
extern u8 D_800E738D;
extern s16 D_8013C970;
extern u8 D_8013C980;

void func_80138E40(void) {
    s32 sp30;
    s32 sp2C;

    if ((D_800E71DF != 4) || (D_8013C980 != 0x36)) {
        func_8007E934();
        return;
    }
    sp30 = (s32) D_800E738A;
    if ((D_800E7384 == 0) && (D_800E738D == 0)) {
        D_800CA2DC = 0x10;
    }
    if (D_800E738D == 0) {
        D_800E7384 += 1;
    }
    sp2C = (s32) D_800E738D;
    if (D_800E738D == 0) {
        func_801386C4();
    } else if (D_800E738D == 1) {
        func_8007E390();
    } else if (D_800E738D == 2) {
        func_801386C4();
    } else if (D_800E738D == 3) {
        func_8007E5A0();
    } else {
        func_8007E934();
    }
    if (sp30 != D_800E738A) {
        if (sp2C < 4) {
            D_800E738A = (u8) sp30;
            D_800E738D = (sp2C + 1) & 0xFF;
            return;
        }
        D_800CA2DC = D_8013C970;
        D_800E738D = (sp2C + 1) & 0xFF;
    }
}
