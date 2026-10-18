/* T-22022, 39 rows: needs `void func_80143314(void)` .. `func_801439F0` prototypes (80143314, 801433E0, 80143520, 801435C4, 80143940, 801439F0) and `extern s16 D_8012B8C8;` in include/ovl/GYOZI.h.
   Left: the second girl index load and the li 50 sit one temp register lower than in the original (t4/t3 for t5/t4), and as1 hoists the lh of D_8012B8D8 above the sb of D_8012B8C7 (original: sb first, lw ra in the bnez delay slot). */
s32 func_80143160(void) {
    if (D_800F647A == 0) {
        D_80148970 = *(Word4 *)&D_800F53A0.girl[D_800F62CF].unk_08;
        D_800F53A0.girl[D_800F53A0.unk_F2F].unk_0A = 0x32;
    }
    switch (D_800F62CF) {
    case 0:
        func_80052060();
        break;
    case 1:
        func_80143314();
        break;
    case 2:
        func_801433E0();
        break;
    case 3:
        func_80143520();
        break;
    case 4:
        func_801435C4();
        break;
    case 5:
        func_801436E4();
        break;
    case 6:
        func_80143780();
        break;
    case 7:
        func_80143858();
        break;
    case 8:
        func_80143940();
        break;
    case 9:
        func_801439F0();
        break;
    case 10:
        func_80052060();
        break;
    }
    D_8012B8C7 = D_801317EB;
    if ((D_8012B8D8 == 0) && (D_8012B8C8 == 0)) {
        D_8012B8D6 += 1;
        if (D_8012B8D6 >= 3) {
            D_8012B8D6 = 0;
        }
    }
}
