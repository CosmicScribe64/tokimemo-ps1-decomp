/* T-25142 draft, aligned cost 12, 45 positional rows (NOTES.md). Needs `s32 func_80134D94(void);` in include/ovl/BUNKA_SD.h.
   Grey level (0x68E - frame) * 0x10201, read through the frame counter pointer as in ETC func_80144224 (T-25146). */
s32 func_80134D94(void) {
    u32 q;
    s32 b;
    s32 col;
    s32 t;
    GsWord *fc;

    fc = &D_800E6280.unk_1104;
    if (fc->u == 0) {
        D_8012071E = 5;
    }
    if (fc->u == 0xFE) {
        D_80120697 = 0x80;
        D_80120696 = 5;
        D_8012069B = 0x80;
    }
    if ((D_80120696 & 1) == 0) {
        D_80120697 = 0;
    }
    if (fc->u == 0x29C) {
        D_801206DB = 0x80;
        D_801206DA = 5;
        D_801206DF = 0x80;
    }
    if (fc->u >= 0xFF && fc->u < 0x5A3) {
        b = ((u8 *)fc)[9];
        D_8012071E = 0x40;
        t = (u32)b * 1;
        if (t < 0) {
        }
        q = (u32)t / 14;
        if (q >= 2) {
            b = 0;
            q = 0;
        }
        /* FAKE: T-25142. `q * 1` is not a plain copy, so copy propagation keeps a range for the arm value (`move v0,a0` at the switch head); the empty test keeps it before the switch (as `q * 2` in func_80132708). */
        t = q * 1;
        if (t < 0) {
        }
        switch (D_80122EAC) {
        case 0:
            D_80120734 = t + 0x5D;
            break;
        case 1:
            D_80120734 = t + 0x72;
            break;
        case 2:
            D_80120734 = t + 0x87;
            break;
        case 3:
        default:
            if (fc->u < 0x4C2) {
                D_80120734 = t + 0x9D;
            } else {
                D_80120734 = t + 0x9F;
            }
            break;
        }
        D_800E6280.unk_110D = b + 1;
    }
    if (fc->u == 0x4C2 && D_80122EAC != 3) {
        D_8012071F = 0;
        D_80120763 = 0x80;
        D_80120762 = 5;
        D_80120767 = 0x80;
    }
    if (fc->u == 0x58F && D_80122EAC == 3) {
        D_8012071F = 0;
        D_80120763 = 0x80;
        D_80120762 = 5;
        D_80120767 = 0x80;
    }
    if (fc->u == 0x606 || fc->u == 0x609 || fc->u == 0x60C) {
        func_80049A40(-0xA0, -0x78, 0x140, 0xA0, 2, 0xFFFFFF, 0x81);
        func_800494BC(0, 0, 2, 0, 0);
    }
    if (fc->u == 0x606) {
        D_801206F0 = 0x48;
        D_801206E0 = 0;
    }
    if (fc->u >= 0x60F && fc->u < 0x68E) {
        col = 0x68E - fc->u;
        col += col << 8;
        col += col << 8;
        func_80049A40(-0x130, -0x78, 0x140, 0xA0, 2, col, 0x86);
        func_80049A40(0x10, -0x78, 0x140, 0xA0, 2, col, 0x85);
        func_800494BC(0, 0, 2, 1, 0);
    }
    if (fc->u == 0x67E) {
        D_80120657 = 0x80;
        D_801206DF = 0x80;
        D_80120767 = 0x80;
    }
    if (fc->u >= 0x723 && D_80122EAC == 3) {
        u32 w;

        D_80120762 = 0x40;
        /* FAKE: T-25142. u8 views of the s8 counter, as in func_80134850: uopt promotes it to a0 for the block. */
        w = *(u8 *)&D_800E6280.unk_110C / 14;
        if (w >= 2) {
            *(u8 *)&D_800E6280.unk_110C = 0;
            w = 0;
        }
        D_80120778 = w + 0xA2;
        if (D_80122EAC) {}
        /* FAKE: T-25142. On one line, as1 puts the `lui` before the add as in the original; the original still copies w to v0 before the store. */
        *(u8 *)&D_800E6280.unk_110C += 1; return w;
    }
}
