#define RPG_BAT_OVERRIDE_func_8013E97C /* implicit int view: the call's $v0 moves the D_8015EC68 - (h - D_8015EC6C) * 2 range of func_80135358 to $v1 (T-17116) */
#include "common.h"
#include "ovl/RPG_BAT.h"

s32 func_8013E97C();

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

/* s32 without a return: func_80134D30 calls this function earlier as an implicit int, a void definition does not build. */
/* FAKE: *(s32 *)&D_8015EC74 stops IDO copypropagate from replacing the first read of D_8015EC74 with 0x300 so uopt keeps D_8015EC74 in $a1;
 * if (0) {} splits the basic block after r2 = r + 0x10 so uopt does not fold r + 0x10 into the +0x310 / -0x170 offsets; the words at
 * D_8015EBAC + 4 * (i + 60, 70, 80) are the same three 10-word rows as in func_80135358. T-24124 */
s32 func_80134E78(void) {
    s32 r;
    s32 r2;
    s32 i;

    D_8015EC78 = -0x1C0;
    D_8015EC7C = 0x280;
    D_8015EC80 = -0x140;
    ((DisplayActor44 *)D_8011ECD0)[146].unk_01 = 2;
    ((DisplayActor44 *)D_8011ECD0)[147].unk_01 = 2;
    ((DisplayActor44 *)D_8011ECD0)[150].unk_01 = 8;
    ((DisplayActor44 *)D_8011ECD0)[151].unk_01 = 8;
    D_8015EC74 = 0x300;
    func_8013E97C(0x32, *(s32 *)&D_8015EC74, 0x88);
    func_8013E97C(0x33, D_8015EC74, 0x88);
    func_8013E97C(0x36, D_8015EC7C, 0x50);
    func_8013E97C(0x37, D_8015EC7C, 0x50);
    func_8013E810(0x32, 6, 1, 1);
    func_8013E810(0x33, 5, 1, 1);
    func_8013E810(0x36, 6, 1, 1);
    func_8013E810(0x37, 5, 1, 1);
    r = func_8013E9A8(0x20);
    (&D_8015EBAC)[60] = 0x2F0;
    (&D_8015EBAC)[70] = -0x190;
    (&D_8015EBAC)[80] = 0x7D;
    r2 = r + 0x10;
    if (0) {}
    (&D_8015EBAC)[61] = r2 + 0x310;
    (&D_8015EBAC)[71] = r2 - 0x170;
    (&D_8015EBAC)[81] = 0x7A;
    r = func_8013E9A8(0x10) + 0x20;
    r2 = func_8013E9A8(0x10);
    (&D_8015EBAC)[62] = 0x2B0;
    (&D_8015EBAC)[72] = -0x190;
    (&D_8015EBAC)[82] = 0x6C;
    (&D_8015EBAC)[63] = r + 0x2C0;
    (&D_8015EBAC)[73] = r - 0x180;
    (&D_8015EBAC)[83] = 0x69;
    r2 += r;
    (&D_8015EBAC)[64] = r2 + 0x2F0;
    (&D_8015EBAC)[74] = r2 - 0x150;
    (&D_8015EBAC)[84] = 0x6F;
    r = func_8013E9A8(0x10) + 0x10;
    r2 = func_8013E9A8(0x10);
    (&D_8015EBAC)[65] = r + 0x2A0;
    (&D_8015EBAC)[75] = r - 0x160;
    (&D_8015EBAC)[85] = 0x61;
    r2 += r;
    (&D_8015EBAC)[66] = r2 + 0x2D0;
    (&D_8015EBAC)[76] = r2 - 0x130;
    (&D_8015EBAC)[86] = 0x5E;
    switch (D_8015EBAC) {
    case 0:
        D_8015ECB8 = 0x2F8;
        D_8015ECE0 = -0x148;
        D_8015ED08 = 0x64;
        D_8015EC68 = 0x77;
        D_8015EC6C = 0x3E;
        break;
    case 1:
        D_8015ECB8 = 0x2D0;
        D_8015ECE0 = -0x130;
        D_8015ED08 = 0x5B;
        D_8015EC68 = 0x3F;
        D_8015EC6C = 0x2E;
        break;
    case 2:
        D_8015ECB8 = 0x310;
        D_8015ECE0 = -0x170;
        D_8015ED08 = 0x85;
        D_8015EC68 = 0x37;
        D_8015EC6C = 0x5E;
        break;
    case 3:
        D_8015ECB8 = 0x310;
        D_8015ECE0 = -0x170;
        D_8015ED08 = 0x85;
        D_8015EC68 = 0x4C;
        D_8015EC6C = 0xE;
        break;
    case 4:
        D_8015ECB8 = 0x310;
        D_8015ECE0 = -0x170;
        D_8015ED08 = 0x85;
        D_8015EC68 = 0x54;
        D_8015EC6C = 0x2A;
        break;
    }
    for (i = 0; i != 8; i++) {
        func_8013E810(i + 0x28, 5, 1, 1);
        func_8013E97C(i + 0x28, (&D_8015EBAC)[i + 60], (&D_8015EBAC)[i + 80]);
        ((DisplayActor44 *)D_8011ECD0)[i + 136].unk_18 = func_8013E9A8(4);
        ((DisplayActor44 *)D_8011ECD0)[i + 136].unk_08 = func_8013E9A8(7);
        if ((&D_8015EBAC)[i + 80] >= 0x85) {
            ((DisplayActor44 *)D_8011ECD0)[i + 136].unk_01 = 3;
        } else if ((&D_8015EBAC)[i + 80] >= 0x66) {
            ((DisplayActor44 *)D_8011ECD0)[i + 136].unk_01 = 4;
        } else if ((&D_8015EBAC)[i + 80] >= 0x64) {
            ((DisplayActor44 *)D_8011ECD0)[i + 136].unk_01 = 5;
        } else if ((&D_8015EBAC)[i + 80] >= 0x5D) {
            ((DisplayActor44 *)D_8011ECD0)[i + 136].unk_01 = 6;
        } else {
            ((DisplayActor44 *)D_8011ECD0)[i + 136].unk_01 = 7;
        }
    }
    if (D_8015EDC4 & 0x100000) {
        ((DisplayActor44 *)D_8011ECD0)[155].unk_01 = 5;
        ((DisplayActor44 *)D_8011ECD0)[156].unk_01 = 7;
        ((DisplayActor44 *)D_8011ECD0)[157].unk_01 = 3;
    } else {
        ((DisplayActor44 *)D_8011ECD0)[155].unk_01 = 3;
    }
    func_8014EBA8();
}

/* s32 without a return: func_80134D30 calls this function earlier as an implicit int, a void definition does not build. */
/* FAKE: h + -h is the unfolded (h - h) of the original (subu s1,s1: uopt folds a plain h - h); the words at D_8015EBAC + 4 * (i + 60,
 * 70, 80) are three 10-word rows (from x, to x, y) reached through the first symbol, the real aggregate is unknown. Frame size and the
 * spill slot at sp+0x34 match only with these three declared locals. T-17116 */
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
