#include "common.h"
#include "ovl/OLH.h"

void func_80134F40(void) {
    switch (D_800E6280.unk_110A) {
    case 0x0:
        func_8013500C();
        break;
    case 0x1:
        func_80135190();
        break;
    case 0x10:
        func_801352A0();
        break;
    case 0x20:
        func_8013538C();
        break;
    case 0x30:
        func_80135494();
        break;
    case 0x40:
        func_8013559C();
        break;
    case 0x50:
        func_801356DC();
        break;
    case 0x60:
        func_80135828();
        break;
    case 0x70:
        func_80135974();
        break;
    }
}

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80134F40", func_8013500C);

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80134F40", func_80135190);

s32 func_801352A0(void) {
    switch (D_800E6280.unk_110D) {
    case 0:
        func_8004EAAC();
        func_8004E788(-0x88, -0x40, 1, D_8013C780, 0);
        func_8004E788(-0x88, -0x30, 0xF, (s32)"　画面左下の対地速度表示をよく見てスク", 0);
        func_8004E788(-0x88, -0x20, 0xF, (s32)"ロールを早くすることが、タイム短縮の秘", 0);
        func_8004E788(-0x88, -0x10, 0xF, (s32)"訣です。", 0);
        D_800E6280.unk_110D += 1;
        break;
    case 1:
        func_80052000();
        break;
    case 2:
        func_80042940(0);
        break;
    }
}

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80134F40", func_8013538C);

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80134F40", func_80135494);

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80134F40", func_8013559C);

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80134F40", func_801356DC);

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80134F40", func_80135828);

INCLUDE_ASM("asm/ovl/OLH/nonmatchings/OLH/80134F40", func_80135974);
