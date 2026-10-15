extern s32 D_800E7208;
extern u8 D_800E738D;

void func_80145318(void) {
    switch (D_800E738D) {
    case 0:
        func_80053D10();
        func_80044750(0x502);
        func_8004EAAC();
        func_8004E788(-0x80, 0x32, 1, (s32) "メモリーカードに空きがありません", 0);
        func_8004EAD4(1);
        D_800E738D = (D_800E738D + 1) & 0xFF;
    default:
        break;
    case 1:
        if (D_800E7208 & 0x860) {
            D_800E738D = (D_800E738D + 1) & 0xFF;
        }
        break;
    case 2:
        func_8004EAAC();
        func_8004E788(-0x80, 0x32, 1, (s32) "別のメモリーカードに", 0);
        func_8004E788(-0x80, 0x42, 1, (s32) "セーブしますか？", 0);
        func_8004E788(-0x18, 4, 0xF, (s32) D_801500F0, 0);
        func_8004E788(0x3C, 4, 1, func_80140F50(), 0);
        func_8004E788(-0x18, 0x14, 0xF, (s32) D_8015010C, 0);
        func_8004E788(0x3C, 0x14, 2, func_80140F94(), 0);
        func_8004EAD4(6);
        D_800E738D = (D_800E738D + 1) & 0xFF;
        break;
    case 3:
        if (D_800E7208 & 0x20) {
            D_800E738D = (D_800E738D + 1) & 0xFF;
        } else if (D_800E7208 & 0x40) {
            func_80042908(2);
        }
        break;
    case 4:
        func_8004EAAC();
        func_8004E788(-0x80, 0x32, 1, (s32) "別のメモリーカードを", 0);
        func_8004E788(-0x80, 0x42, 1, (s32) "差し込み口１にセットして、", 0);
        func_8004E788(-0x80, 0x52, 1, (s32) "ボタンを押してください。", 0);
        func_8004EAD4(3);
        D_800E738D = (D_800E738D + 1) & 0xFF;
        break;
    case 5:
        if (D_800E7208 != 0) {
            func_80053CE0();
            func_80042940(0);
        }
        break;
    }
}
