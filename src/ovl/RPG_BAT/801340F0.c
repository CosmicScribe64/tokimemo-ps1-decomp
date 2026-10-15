#include "common.h"
#include "ovl/RPG_BAT.h"

void func_801340F0(void) {
    switch (D_8015EDF0) {
    case 1:
        func_8013415C();
        return;
    case 2:
        func_80134570();
        return;
    case 3:
        func_80134984();
        return;
    }
}

void func_8013415C(void) {

    switch (D_8015EE04) {
    case 0:
        func_800AE0F0(D_8015E904, "好雄「いくぞっ！");
        func_800AE0F0(D_8015E92C, "スペシャル好雄パーンチ！");
        func_8014B738((s32) D_8015E9CC, 2, 0x3C);
        func_8013E7C0(0x1D, 5, 5, 1);
        func_8013F1C4(0x1F0000A1, 0x1D0000A4, 0);
        func_8014F230();
        return;
    case 1:
        func_8014F448(1);
        return;
    case 2:
        func_8013F1C4(0x0700001F, 0x0600002B, 1);
        D_8015EE48 = 3;
        func_8014F350(0x1E);
        return;
    case 3:
        func_8013E7C0(0x1D, 7, 1, 1);
        func_8014F230();
        return;
    case 4:
        func_8013E97C(0x1D, (s32) (D_8015EE0C * -0xA8) / 1000, (s32) (D_8015EE0C * -0x30) / 1000);
        D_8015EE0C += 5;
        if (D_8015EE0C >= 0x3E9) {
            func_8013E97C(0x1D, -0xA8, -0x30);
            func_8013E97C(0x1E, -0xA8, -0x30);
            func_8013E7C0(0x1D, 8, 1, 1);
            func_8013E7C0(0x1E, 0xC, 1, 1);
            func_8014BFE4(0, 0x48, 0x50, 0, 1);
            func_8013F15C(0x503, 1, 0);
            D_8015EE48 = 1;
            func_8014F230();
            return;
        }
    default:
        return;
    case 5:
        func_8014F350(0x3C);
        return;
    case 6:
        func_8014BFE4(1, 0x64, 0x60, D_8015EDEC - 7, 1);
        D_8015EDEC = 7;
        func_8014F230();
        return;
    case 7:
        func_8014F350(0x3C);
        return;
    case 8:
        func_8013E7C0(0x1D, 3, 1, 1);
        func_8013E7C0(0x1E, 0xFF, 1, 0);
        func_8014F230();
        return;
    case 9:
        func_8013E97C(0x1D, ((s32) (D_8015EE0C * 0xA8) / 100) - 0xA8, ((s32) (D_8015EE0C * 0x30) / 100) - 0x30);
        D_8015EE0C += 2;
        if (D_8015EE0C >= 0x65) {
            func_8013E97C(0x1D, 0, 0);
            func_8013E7C0(0x1D, 0, 1, 1);
            func_8014F230();
            return;
        }
        break;
    case 10:
        func_8014F448(0);
        return;
    case 11:
        func_8014F350(0x14);
        return;
    case 12:
        func_8014F278();
        break;
    }
}


/* NON_MATCHING: T-12040, 22 listing lines; phase/divisor v0-v1 swap. */
#ifdef NON_MATCHING
void func_80134570(void) {
    s32 temp_v0;
    s32 temp_v0_2;

    switch (D_8015EE04) {
    case 0:
        func_800AE0F0(D_8015E904, "好雄「食らえ！");
        func_800AE0F0(D_8015E92C, "グレート好雄キーック！");
        func_8014B738((s32) D_8015E9CC, 2, 0x3C);
        func_8013E7C0(0x1D, 5, 5, 1);
        func_8013F1C4(0x1F0000A5, 0x1E0000A8, 0);
        func_8014F230();
        return;
    case 1:
        func_8014F448(1);
        return;
    case 2:
        func_8013F1C4(0x1C0000A8, 0x0F0000B7, 1);
        D_8015EE48 = 3;
        func_8014F350(0x1E);
        return;
    case 3:
        func_8013E7C0(0x1D, 0xA, 1, 1);
        func_8014F230();
        return;
    case 4:
        func_8013E97C(0x1D, (s32) (D_8015EE0C * -0xA8) / 1000, (s32) (D_8015EE0C * -0x30) / 1000);
        temp_v0 = D_8015EE0C + 5;
        D_8015EE0C = temp_v0;
        if (temp_v0 >= 0x3E9) {
            func_8013E97C(0x1D, -0xA8, -0x30);
            func_8013E97C(0x1E, -0xA8, -0x30);
            func_8013E7C0(0x1D, 0xB, 1, 1);
            func_8013E7C0(0x1E, 0xD, 1, 1);
            func_8014BFE4(0, 0x48, 0x5C, 0, 1);
            func_8013F15C(0x504, 1, 0);
            D_8015EE48 = 1;
            func_8014F230();
            return;
        }
    default:
        return;
    case 5:
        func_8014F350(0x3C);
        return;
    case 6:
        func_8014BFE4(1, 0x60, 0x58, D_8015EDEC - 7, 1);
        D_8015EDEC = 7;
        func_8014F230();
        return;
    case 7:
        func_8014F350(0x3C);
        return;
    case 8:
        func_8013E7C0(0x1D, 3, 1, 1);
        func_8013E7C0(0x1E, 0xFF, 1, 0);
        func_8014F230();
        return;
    case 9:
        func_8013E97C(0x1D, ((s32) (D_8015EE0C * 0xA8) / 100) - 0xA8, ((s32) (D_8015EE0C * 0x30) / 100) - 0x30);
        temp_v0_2 = D_8015EE0C + 2;
        D_8015EE0C = temp_v0_2;
        if (temp_v0_2 >= 0x65) {
            func_8013E97C(0x1D, 0, 0);
            func_8013E7C0(0x1D, 0, 1, 1);
            func_8014F230();
            return;
        }
        break;
    case 10:
        func_8014F448(0);
        return;
    case 11:
        func_8014F350(0x14);
        return;
    case 12:
        func_8014F278();
        break;
    }
}
#else
INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801340F0", func_80134570);
#endif

void func_80134984(void) {
    switch (D_8015EE04) {
    case 0:
        func_800AE0F0(D_8015E904, "　　　　　　好雄の秘薬　　　　　　");
        func_8014B738((s32) D_8015E9CC, 1, 0x3C);
        func_8014F230();
        return;
    case 1:
        func_8014F448(1);
        return;
    case 2:
        func_8013EA60(2, 0x23, 0x270F, 0);
        D_8015EDEC = D_8015EDF8;
        func_8013F15C(0x502, 1, 0);
        func_8014F230();
        return;
    case 3:
        func_8014F350(0x1E);
        return;
    case 4:
        func_8014F278();
        /* fallthrough */
    default:
        return;
    }
}
