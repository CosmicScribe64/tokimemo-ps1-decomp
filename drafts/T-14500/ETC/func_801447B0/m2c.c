? func_8004EC24();                                  /* extern */
extern u8 D_800E738D;
extern u8 D_800E7395;
extern u32 D_800E73A0;

void func_801447B0(void) {
    switch (D_800E738D) {
    case 0:
        if (func_80053F04() != 0) {
            if (D_800E7395 == 1) {
                func_80053D10();
                D_800E738D += 1;
            } else if (D_800E7395 == 0xF1) {
                func_80053D10();
                func_80042940(0xA0);
            } else if (D_800E7395 == 0xFF) {
                func_80053D10();
                func_80042940(0xFF);
            }
        } else if ((u32) D_800E73A0 >= 0x201U) {
            func_80053D10();
            func_80042940(0xFF);
        }
        break;
    case 1:
        func_80053CE0();
        D_800E738D += 1;
        break;
    case 2:
        if (func_80054140() == 2) {
            D_800E738D += 1;
        } else if ((u32) D_800E73A0 >= 0x201U) {
            func_80042940(0xFF);
        }
        break;
    case 3:
        if ((u8) D_800E8BEE != 0) {
            func_80053D10();
            func_80053CE0();
            D_800E738D += 1;
        } else {
            func_80053D10();
            func_8004E58C();
            func_8004E788(-0x80, 0x32, 0, (s32) "セーブするアルバムを選んでください", 0);
            func_8004E884(1);
            func_8004284C();
        }
        break;
    case 4:
        if (func_80054284() == 3) {
            func_800552DC(M2C_ERROR(/* Read from unset register $a0 */));
            func_8004E58C();
            func_8004E788(-0x80, 0x32, 0, (s32) "セーブするアルバムを選んでください", 0);
            func_8004E884(1);
            func_8004284C();
            func_80053D10();
        } else if (((u32) D_800E73A0 >= 0x201U) || (D_800E7395 == 0xF0)) {
            func_80053D10();
            func_80042940(0xFF);
        }
        break;
    }
    func_8004EC24();
}
