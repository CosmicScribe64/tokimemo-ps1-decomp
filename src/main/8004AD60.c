#include "common.h"
#include "game.h"

void func_8004AD60(s32 arg0) {
    if ((arg0 & 0xFF) == 0) {
        D_800B3D80 = 0;
    } else {
        D_800B3D80 = D_800B3D80 - 1;
    }
    if ((u32)D_800B3D80 >= 9U) {
        D_800B3D80 = 8;
    }
}

void func_8004ADAC(u8 arg0) {
    D_800B3D80 = arg0;
    if ((u32)arg0 >= 9) {
        D_800B3D80 = 8;
    }
}

u8 func_8004ADD4(void) {
    return D_800B3D80;
}

void func_8004ADE4(void) {
    if (D_800B3220 != 0) {
        LoadSquare(0x2C0, 0x1E0, 0x40, 0x20, (void *)0x801F0020);
    }
}

/* FAKE: func_8004AE54 takes four arguments and the original leaves a0-a2 untouched; passing the
 * (unused) incoming a0-a2 through reproduces that, real source unknown. T-0800 */
void func_8004AE28(s32 a0, s32 a1, s32 a2) {
    if (D_800B3220 != 0) {
        func_8004AE54(a0, a1, a2, 0xF);
    }
}

INCLUDE_ASM("asm/nonmatchings/main/8004AD60", func_8004AE54);

void func_8004B19C(s32 arg0, s32 arg1, s32 arg2) {
    u8 *p;
    s32 n;
    u32 x;
    u8 *e;

    n = 0;
    if (D_800B3D80 >= 4 && D_800B3220 != 0) {
        x = arg0;
        for (;;) {
            p = (u8 *)GetWorkBase(0x10, D_8011ECA0);
            func_8009F07C(p);
            SetSemiTrans(p, 0);
            p[4] = 0x80;
            p[5] = 0x80;
            p[6] = 0x80;
            *(s16 *)(p + 8) = D_800E6280.unk_014[D_8011ECA0] + arg1 - n * 6;
            e = D_800B3D84 + (x & 0xF) * 2;
            *(s16 *)(p + 0xA) = D_800E6280.unk_018[D_8011ECA0] + arg2;
            p[0xC] = e[0];
            p[0xD] = e[1];
            *(s16 *)(p + 0xE) = func_8009ED74(0x100, 0x1FF);
            AddPrim(D_800E8CA0 + (D_8011ECA0 << 10) + 4, p);
            if (x < 0x10) {
                dtd_on_tpage(0x2C0, 0x100, 1, 0, 0);
                break;
            }
            x = x >> 4;
            n++;
        }
    }
}

void func_8004B338(s32 arg0, s32 arg1, s32 arg2) {
    func_8004B358(arg0, arg1, arg2, 0xF);
}

INCLUDE_ASM("asm/nonmatchings/main/8004AD60", func_8004B358);

INCLUDE_ASM("asm/nonmatchings/main/8004AD60", func_8004B590);

INCLUDE_ASM("asm/nonmatchings/main/8004AD60", make_color_bar);

INCLUDE_ASM("asm/nonmatchings/main/8004AD60", make_color_bar16);

INCLUDE_ASM("asm/nonmatchings/main/8004AD60", col2sepia);
