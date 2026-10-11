#include "common.h"
#include "ovl/RPG_BAT.h"

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/801563A0", func_801563A0);

/* FAKE: typed 0x44-byte record view coexists with unresolved shared byte-table/scalar declarations. T-9550 */
void func_801564F0(void) {
    func_8013E7C0(0x1D, 8, 1, 1);
    switch (*D_8015ED8C) {
    case 0:
        if (D_8015EDC4 & 0x100000) {
            func_800AE0F0(D_8015E904, "不良達「なんだ、なんだ？");
        } else {
            func_800AE0F0(D_8015E904, "番長「なんだ、なんだ？");
        }
        func_8014B738((s32) D_8015E9CC, 1, 0x82);
        *D_8015ED8C += 1;
        return;
    case 1:
        if (D_8015EDC4 & 0x100000) {
            func_8013F1C4(0x1E00006F, 0x17000072, 0);
        } else {
            func_8013F1C4(0x10000093, 0x06000098, 0);
        }
        *D_8015ED8C += 1;
        return;
    case 2:
        ((RpgFlagRecord44 *)D_8011ECD0)[155].unk_03 |= func_8013E9A8(2);
        ((RpgFlagRecord44 *)D_8011ECD0)[156].unk_03 |= func_8013E9A8(2);
        ((RpgFlagRecord44 *)D_8011ECD0)[157].unk_03 |= func_8013E9A8(2);
        *D_8015ED8C += 1;
        return;
    case 3:
        if (((s32) D_8015EE0C % 15) == 0xE) {
            ((RpgFlagRecord44 *)D_8011ECD0)[155].unk_03 ^= 1;
            ((RpgFlagRecord44 *)D_8011ECD0)[156].unk_03 ^= 1;
            ((RpgFlagRecord44 *)D_8011ECD0)[157].unk_03 ^= 1;
        }
        D_8015EE0C += 1;
        func_8013F250(0);
        return;
    case 4:
        ((RpgFlagRecord44 *)D_8011ECD0)[155].unk_03 &= 0xFE;
        ((RpgFlagRecord44 *)D_8011ECD0)[156].unk_03 &= 0xFE;
        ((RpgFlagRecord44 *)D_8011ECD0)[157].unk_03 &= 0xFE;
        func_8013E7C0(0x1D, 0, 1, 1);
        func_8013F220();
        func_8014F230();
        /* fallthrough */
    default:
        return;
    }
}

void func_8015675C(void) {
    switch (D_8015EE04) {
    case 0:
        func_8013E7C0(0x1D, 5, 5, 1);
        func_8014F230();
        break;
    case 1:
        if (D_80120E1C == 2) {
            func_8013F15C(0x513, 0, 1);
        }
        func_8014F3C8(0x1D);
        break;
    case 2:
        func_80143C28(6, 0);
        break;
    case 3:
        func_801563A0();
        break;
    case 4:
        func_801564F0();
        break;
    case 5:
        func_8014F350(0x14);
        break;
    case 6:
        func_80143CE8(3, 0);
        break;
    case 7:
        func_8014F350(0x14);
        break;
    case 8:
        func_8014F278();
        break;
    }
}
