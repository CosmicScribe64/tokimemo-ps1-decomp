#include "common.h"
#include "ovl/RPG_BAT.h"

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 func_801468B0(void) {
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E828, "？？？「拙僧にまかせなされ。");
        func_8014B738(D_8015E8F0, 1, 0xC8);
        D_8015ED8C[0] += 1;
        return;
    case 1:
        func_8013F1C4(0x05000045, 0x0700004E, 0);
        D_8015ED8C[0] += 1;
        return;
    case 2:
        func_8013F250(0);
        return;
    case 3:
        func_8013F220();
        func_8014EF3C();
        return;
    }
}

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 func_8014698C(void) {
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E828, "虚無僧「迷える大鹿よ、");
        func_800AE0F0(D_8015E850, "怒りを静め給え。");
        func_8014B738(D_8015E8F0, 2, 0xA0);
        D_8015ED8C[0] += 1;
        return;
    case 1:
        func_8013F1C4(0x1D00009C, 0x1D0000A2, 0);
        D_8015ED8C[0] += 1;
        return;
    case 2:
        func_8013F250(1);
        return;
    case 3:
        func_8013F1C4(0x1D0000A3, 0x1E0000A9, 1);
        D_8015ED8C[0] += 1;
        return;
    case 4:
        func_8013F250(0);
        return;
    case 5:
        func_8013F220();
        func_8014EF3C();
        return;
    }
}

/* returns int without a value: the switch temporary stays live in $v0, as in the original (T-8080) */
s32 func_80146AB4(void) {
    switch (D_8015ED8C[0]) {
    case 0:
        func_800AE0F0(D_8015E828, "虚無僧「はしたないところを");
        func_800AE0F0(D_8015E850, "お見せしてしまった。");
        func_800AE0F0(D_8015E878, "では、拙僧はこれにて。");
        func_8014B738(D_8015E8F0, 3, 0xC8);
        D_8015ED8C[0] += 1;
        return;
    case 1:
        func_8013F1C4(0x1D0000AA, 0x1F0000AE, 0);
        D_8015ED8C[0] += 1;
        return;
    case 2:
        func_8013F250(1);
        return;
    case 3:
        func_8013F1C4(0x0D0000B2, 0x0D0000B7, 1);
        D_8015ED8C[0] += 1;
        return;
    case 4:
        func_8013F250(2);
        return;
    case 5:
        func_8013F1C4(0x1000008A, 0x05000092, 2);
        D_8015ED8C[0] += 1;
        return;
    case 6:
        func_8013F250(0);
        return;
    case 7:
        func_8013F220();
        func_8014EF3C();
        return;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801468B0", func_80146C30);
