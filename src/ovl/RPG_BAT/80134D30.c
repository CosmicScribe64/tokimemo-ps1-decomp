#include "common.h"
#include "ovl/RPG_BAT.h"

void func_80134D30(void) {
    switch (D_8015EDAC) {
    case 0:
        func_800AE0F0(D_8015E74C, "　　　　ハーメルンのホラ吹き　　　　");
        func_8014B738((s32) &D_8015E814, 1, 0x5A);
        func_80144480();
        func_8013EEAC(D_8015EBAC);
        func_80148F70();
        func_8014EBA8();
        func_8014EBA8();
        return;
    case 2:
        func_8013D418();
        return;
    case 3:
        func_801359FC();
        return;
    case 4:
        func_8013D1D0();
        return;
    case 5:
        func_80134E78();
        return;
    case 6:
        func_80135358();
        return;
    case 7:
        func_8013D350();
        func_8013E7C0(0x34, 2, 1, 0);
        func_8014EBA8();
        return;
    case 8:
        func_8013D290();
        return;
    case 9:
        func_8013D59C();
        return;
    case 10:
        func_8014EBEC();
        /* fallthrough */
    default:
        return;
    }
}

INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80134D30", func_80134E78);

/* NON_MATCHING: T-17116, 10 differing lines (5 instructions, registers only). The value D_8015EC68 - (h - D_8015EC6C) * 2 (compared
 * with cx at three places, recomputed in both arms of the second block) sits in $v0 here, the original has it in $v1 (the slt, the
 * bne and both recomputations); no variant (named local, operand order, shift form, void type, call-result locals, order_search,
 * autotweak, permuter) moved it. Frame size and the spill slot at sp+0x34 match with three declared locals.
 * FAKE: h + -h is the unfolded (h - h) of the original (subu s1,s1: uopt folds a plain h - h); the words at D_8015EBAC + 4 * (i + 60,
 * 70, 80) are three 10-word rows (from x, to x, y) reached through the first symbol, the real aggregate is unknown. T-17116 */
#ifdef NON_MATCHING
s32 func_80135358(void) {
    s32 cx;
    s32 h;
    s32 sp34;

    if (D_8015EDB0.unk_04 >= 0x68 && D_8015EDB0.unk_04 < 0x450) {
        if ((D_8015EDB0.unk_04 & 7) == 0) {
            func_8013F3F0(0, 1);
        } else {
            func_8013F3F0(0, -1);
        }
    }
    if (D_8015EDB0.unk_04 >= 0x130 && D_8015EDB0.unk_04 < 0x3B8) {
        if ((D_8015EDB0.unk_04 & 7) == 0) {
            func_8013F3F0(0, 1);
        } else {
            func_8013F3F0(0, -1);
        }
    }
    if (D_8015EDB0.unk_04 >= 0x1F8 && D_8015EDB0.unk_04 < 0x320) {
        if ((D_8015EDB0.unk_04 & 7) == 0) {
            func_8013F3F0(0, 1);
        } else {
            func_8013F3F0(0, -1);
        }
    }
    func_8013F0F4(0x50A, 0x11, 1);
    func_8013E97C(0x32, (D_8015EC78 - D_8015EC74) * D_8015EDB0.unk_04 / 1000 + D_8015EC74, 0x88);
    func_8013E97C(0x33, (D_8015EC78 - D_8015EC74) * D_8015EDB0.unk_04 / 1000 + D_8015EC74, 0x88);
    func_8013E97C(0x36, (D_8015EC80 - D_8015EC7C) * D_8015EDB0.unk_04 / 1000 + D_8015EC7C, 0x50);
    func_8013E97C(0x37, (D_8015EC80 - D_8015EC7C) * D_8015EDB0.unk_04 / 1000 + D_8015EC7C, 0x50);
    for (h = 0; h != 8; h++) {
        func_8013E97C(h + 0x28, (&D_8015EBAC)[h + 60] + ((&D_8015EBAC)[h + 70] - (&D_8015EBAC)[h + 60]) * D_8015EDB0.unk_04 / 1000, (&D_8015EBAC)[h + 80]);
    }
    cx = (D_8015ECE0 - D_8015ECB8) * D_8015EDB0.unk_04 / 1000 + D_8015ECB8;
    h = D_8015ED08;
    if (cx >= D_8015EC68 && (h - D_8015EC6C) + D_8015EC68 >= cx) {
        func_8013F0F4(0x50C, 0, 3);
        func_8013E810(0x2F, 4, 1, 1);
        if (D_8015EDC4 & 0x100000) {
            func_8013E97C(0x2F, cx, (h + -h + D_8015EC6C + cx) - D_8015EC68 - (func_800A0070((cx - D_8015EC68) * 0x800 / (h - D_8015EC6C)) >> 7));
        } else {
            func_8013E97C(0x2F, cx, (h + -h + D_8015EC6C + cx) - D_8015EC68 - (func_800A0070((cx - D_8015EC68) * 0x800 / (h - D_8015EC6C)) >> 6));
        }
    }
    if (D_8015EC68 - (h - D_8015EC6C) * 2 < cx && cx < D_8015EC68) {
        sp34 = h + -h;
        func_8013F0F4(0x50B, 0, 2);
        func_8013E810(0x2F, 4, 1, 1);
        if (D_8015EDC4 & 0x100000) {
            func_8013E97C(0x2F, cx, (sp34 + D_8015EC6C + (D_8015EC68 - cx) / 2) - (func_800A0070((D_8015EC68 - cx) * 0x800 / ((h - D_8015EC6C) * 2)) >> 7));
        } else {
            func_8013E97C(0x2F, cx, (sp34 + D_8015EC6C + (D_8015EC68 - cx) / 2) - (func_800A0070((D_8015EC68 - cx) * 0x800 / ((h - D_8015EC6C) * 2)) >> 6));
        }
    }
    if (cx == D_8015EC68 - (h - D_8015EC6C) * 2) {
        func_8013E810(0x2F, 5, 1, 1);
    }
    D_8015EDB0.unk_04 += 4;
    if (D_8015EDB0.unk_04 >= 0x4B1) {
        func_8014EBA8();
    }
}
#else
INCLUDE_ASM("asm/ovl/RPG_BAT/nonmatchings/RPG_BAT/80134D30", func_80135358);
#endif

void func_801359FC(void) {
    switch (D_8015EDB0.unk_00) {
    case 0:
        func_8013E7C0(0x34, 3, 5, 1);
        func_8014EBD0();
        break;
    case 1:
        func_8014ED80(0xF);
        break;
    case 2:
        func_8014EBD0();
        break;
    case 3:
        func_8014EDFC(0x34);
        break;
    case 4:
        func_8013E7C0(0x34, 7, 1, 1);
        func_8013F0F4(0x50D, 1, 0);
        func_8014EBD0();
        break;
    case 5:
        func_8014ED80(0x4B);
        break;
    case 6:
        func_8014EBA8();
        break;
    }
}
