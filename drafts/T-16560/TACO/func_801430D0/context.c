#include "common.h"
#include "ovl/TACO.h"

void func_801430B0(void) {
    func_8004ACC8(0x800000);
}

/* bulk_m2c declarations (T-11190) */
extern s32 D_800E7378;
extern s32 D_8015F5A0;
/* end bulk_m2c declarations */
void func_801430D0(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_v0;
    s32 var_s1;
    s32 var_s2;
    s32 var_v0;
    u8 *temp_v0_2;

    if (arg0 < 0) {
        var_s1 = -arg0;
        var_v0 = -1;
    } else {
        var_s1 = arg0;
        var_v0 = 1;
    }
    if ((var_v0 != -1) || !(D_800E7378 & 4)) {
        var_s2 = 0;
        if (var_s1 >= 0) {
loop_7:
            temp_v0 = GetWorkBase(0x14, D_8011ECA0);
            func_8009F0A4(temp_v0);
            *(s16 *)((unsigned char *)temp_v0 + (16)) = 0xC;
            *(s16 *)((unsigned char *)temp_v0 + (18)) = 0x10;
            SetSemiTrans(temp_v0, 0);
            *(s8 *)((unsigned char *)temp_v0 + (4)) = 0x80;
            *(s8 *)((unsigned char *)temp_v0 + (5)) = 0x80;
            *(s8 *)((unsigned char *)temp_v0 + (6)) = 0x80;
            *(s16 *)((unsigned char *)temp_v0 + (8)) = (s16) ((D_800E6280.unk_014[D_8011ECA0] + arg1) - var_s2);
            *(s16 *)((unsigned char *)temp_v0 + (10)) = (s16) (D_800E6280.unk_018[D_8011ECA0] + arg2);
            temp_v0_2 = ((var_s1 % 10) * 2) + &D_8015F5A0;
            *(s8 *)((unsigned char *)temp_v0 + (12)) = (s8) ((*(s32 *)temp_v0_2) + 0x80);
            *(u8 *)((unsigned char *)temp_v0 + (13)) = (u8) *(u8 *)((unsigned char *)temp_v0_2 + (1));
            *(s16 *)((unsigned char *)temp_v0 + (14)) = func_8009ED74(arg3 * 0x10, 0x1EF);
            AddPrim((D_8011ECA0 << 0xA) + 4 + D_800E8CA0, temp_v0);
            if (var_s1 >= 0xA) {
                var_s2 += 0xC;
                var_s1 = var_s1 / 10;
                if (var_s1 >= 0) {
                    goto loop_7;
                }
            }
        }
        dtd_on_tpage(0x340, 0x180, 1, 0, 0);
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/801430B0", func_801432F0);

void func_80143574(s32 arg0, s32 arg1, s32 arg2) {
    s32 pad; /* FAKE: unused local declared first puts p at sp+0x28 as in the original; real source unknown. T-6070 */
    u8 *p;

    p = func_800490F0(0xC, D_8011ECA0);
    func_8009F0B8(p);
    func_8009EF84(p, 0);
    /* FAKE: p[4] and p[5] on one source line give the original's store order 5, 4, 6 (line-based scheduling). T-6070 */
    p[4] = (arg2 >> 16) & 0xFF; p[5] = (arg2 >> 8) & 0xFF;
    p[6] = arg2 & 0xFF;
    *(s16 *)(p + 8) = D_800E6280.unk_014[D_8011ECA0] + arg0;
    *(s16 *)(p + 0xA) = D_800E6280.unk_018[D_8011ECA0] + arg1;
    func_8009EED0((D_8011ECA0 << 10) + 0x3C + D_800E8CA0, p);
}

void func_80143644(void) {
    D_800E6280.unk_1092 = 1;
    if (D_800E6280.unk_F88 & 0x1000) {
        if (D_800E6280.unk_1093 != 0) {
            D_800E6280.unk_1093 -= 1;
        } else {
            D_800E6280.unk_1093 = D_800E6280.unk_1090 - 1;
        }
    }
    if (D_800E6280.unk_F88 & 0x4000) {
        if ((u32)D_800E6280.unk_1093 < (u32)(D_800E6280.unk_1090 - 1)) {
            D_800E6280.unk_1093 += 1;
            return;
        }
        D_800E6280.unk_1093 = 0;
    }
}

void func_801436D4(void) {
    s32 pad; /* FAKE: unused 4-byte local puts rect at the original offset; real source unknown. T-2040 */
    RECT rect;

    rect.x = 0;
    rect.y = D_8011ECA0 * 0xF0;
    rect.w = 0x200;
    rect.h = 0xF0;
    func_8009C93C(&rect, 0x200, 0);
    func_8009C674(0);
}

void func_80143730(s32 arg0, s32 arg1) {
    s32 x;
    s32 y;

    x = (arg1 & 0xF) << 6;
    y = (arg1 & 0x10) << 4;
    func_8004435C(x, y, 0x40, 0x80, arg0);
    func_8004435C(x + 0x40, y, 0x40, 0x80, arg0 + 0x4000);
    func_8004435C(x, y + 0x80, 0x40, 0x80, arg0 + 0x8000);
    func_8004435C(x + 0x40, y + 0x80, 0x40, 0x80, arg0 + 0xC000);
}

void func_80143814(s32 arg0, s32 arg1) {
    s32 x;
    s32 y;

    x = (arg1 & 0xF) << 6;
    y = (arg1 & 0x10) << 4;
    func_8004435C(x, y, 0x20, 0x80, arg0);
    func_8004435C(x + 0x20, y, 0x20, 0x80, arg0 + 0x2000);
    func_8004435C(x, y + 0x80, 0x20, 0x80, arg0 + 0x4000);
    func_8004435C(x + 0x20, y + 0x80, 0x20, 0x80, arg0 + 0x6000);
}

void func_801438F0(s32 arg0, s32 arg1, u8 *arg2) {
    s32 pad[3]; /* FAKE: 12 bytes of unused locals above sp28 reproduce the original frame (0x38, sp28 at 0x28); real source unknown. T-4040 */
    s32 sp28;

    sp28 = arg0 + 4;
    func_8009B3C0(sp28);
    func_8009B430(sp28 + 8, arg2 + 4, arg1, sp28);
    *(u8 **)(arg2 + 8) = arg2 + 0x14;
    *(s32 *)(arg2 + 4) = 0;
}

void func_8014394C(void) {
    D_80122760 = 0;
    D_80122764 = 0;
    D_80122768 = -0x64;
    D_8012276C = 0xFF;
    D_8012276D = 0xFF;
    D_8012276E = 0xFF;
    func_8009AD70(0, &D_80122760);
    D_80122770 = 0;
    D_80122774 = 0x64;
    D_80122778 = -0x64;
    D_8012277C = 0xFF;
    D_8012277D = 0xFF;
    D_8012277E = 0xFF;
    func_8009AD70(1, &D_80122770);
    D_80122780 = -0x64;
    D_80122784 = 0;
    D_80122788 = -0x64;
    D_8012278C = 0xFF;
    D_8012278D = 0xFF;
    D_8012278E = 0xFF;
    func_8009AD70(2, &D_80122780);
    func_8009B310(0x800, 0x800, 0x800);
    func_8009B340(0);
}

void func_80143A74(void) {
    func_8009AD30(0x800);
    D_80122740 = 0;
    D_80122744 = 0;
    D_80122748 = 0x800;
    D_8012274C = 0;
    D_80122750 = 0;
    D_80122754 = 0;
    D_80122758 = 0;
    D_8012275C = 0;
    func_80099540(&D_80122740);
    func_8009AD50(-0x64);
    func_8009AD60(0x7FFF);
}

void func_80143AF4(void) {
    s32 i;

    for (i = 2; i != 0xA0; i += 2) {
        func_80059688(i);
    }
}

void func_80143B34(s32 arg0) {
    func_80059688((arg0 * 2) + 2);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/801430B0", func_80143B58);
