#include "common.h"
#include "ovl/SHOUGATU.h"

void func_801330C0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl60 tbl;

    tbl = D_80143BC4;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_80133134(void) {
    func_80044750(0x603);
    func_8004284C();
}

s32 func_8013315C(void) {
    func_801323D4();
    if (D_800E6280.unk_1104.w++ == 0) {
        if (((u8 **)D_80143AF8)[D_80143B00][D_80143B04 - 1] == 7) {
            if (func_800AEEB0(((s32 **)D_80143AF4)[D_80143B00][D_80143B04 - 1], "ガチャ") == 0) {
                func_80044750(0x60C);
            }
        }
    }
}

/* bulk_m2c declarations (T-11190) */
extern s32 D_800B3778;
/* end bulk_m2c declarations */
s32 func_80133208(void) {
    s16 temp_v0;
    s32 var_v0;

    if ((D_800E6280.unk_F88 & 0x400040) && (D_80143B04 > 0) && (D_80143B04 < 5) && ((u32) D_800E6280.unk_1104.w >= 2U)) {
        func_80044750(0x1508);
        D_80122D00 = 0;
        k_reset(1);
        D_800CA2AC = 0;
        D_80143B04 = 4;
        goto block_19;
    }
    if ((check_end_k() != 0) && (D_80143B04 < 5)) {
        if (D_80143B04 == 0) {
            temp_v0 = D_80143BC0 + 1;
            if (D_80143BC0 < 0x5A) {
                D_80143BC0 = temp_v0;
                if (temp_v0 == 1) {
                    func_80044750(0x60A);
                }
                if (D_80143BC0 == 0x1E) {
                    func_80044750(0x604);
                }
                return 0;
            }
            if (!(D_800E6280.unk0 & 7)) {
                D_800E6280.unk_110D = D_800E6280.unk0;
                func_80044750((((*(&D_800B3778 + ((D_800E6280.unk_F5F * 7) + ((u8) D_800E6280.unk0 >> 3))) + 0x505) & 0xFF) | 0x600) & 0xFFFF);
            }
            goto block_16;
        }
block_16:
        if ((u8) D_800E6280.unk_110D < 0x28U) {
            D_800E6280.unk_110D = (D_800E6280.unk_110D + 1) & 0xFF;
            return 0;
        }
        D_800E6280.unk_110D = 0;
        k_reset(1);
        goto block_19;
    }
block_19:
    var_v0 = func_801323D4();
    if ((D_80143B04 < 5) && (D_800E6280.unk_1104.w == 1)) {
        var_v0 = func_80044750(0x608);
    }
    D_800E6280.unk_1104.w += 1;
    return var_v0;
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/801330C0", func_801333F0);

void func_80133610(void) {
    func_800AE0F0(D_800CA1DC, "正月");
    D_80143BC0 = 0;
    D_80143B20 = 0;
    D_80143B18 = 1;
    D_80143B00 = 6;
    D_80143AF4 = D_80143A48;
    D_80143AF8 = D_80143A84;
    D_80143AFC = D_80143AC0;
    func_8004284C();
}

void func_80133698(void) {
    if (D_80143B14 == 0) {
        func_8007ED84(0x4167);
        func_800AE0F0(D_800CA19C, "正月");
        func_8004E9F4(0);
        func_8006612C(D_800CA19C);
    } else {
        D_800E6280.unk_110A += 3;
    }
    func_8004284C();
}

void func_80133710(void) {
    bg_read_sub2(0x4167);
    func_8004284C();
}

void func_80133738(void) {
    bg_read_sub2(0x4144);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/801330C0", func_80133760);

void func_80133874(void) {
    D_80143B00 = 7;
    D_80143AF4 = D_80143A48;
    D_80143AF8 = D_80143A84;
    D_80143AFC = D_80143AC0;
    func_8004284C();
    D_800E6280.unk_110A -= 5;
}

void func_801338DC(void) {
    /* FAKE: the (u8) cast reserves the extra temp the original has; real prototype unknown. T-2020 */
    if (((u8)func_80051A68(D_800E6280.unk_F5F) & 0x7F) != 4) {
        D_80143B00 += 1;
        func_8004284C();
        func_8004284C();
        func_8004284C();
        return;
    }
    func_8004284C();
}

void func_80133948(void) {
    if (D_80122CDC == 0) {
        func_8004284C();
    }
    func_8004284C();
}

void func_8013397C(void) {
    D_80143B20 = 1;
    D_80143B00 = 0;
    D_80143AF4 = D_80143A40;
    D_80143AF8 = D_80143A7C;
    D_80143AFC = D_80143AB8;
    func_80042808();
}

void func_801339DC(void) {
    s32 temp_t6;

    /* FAKE: the (u8) cast reserves the extra temp the original has; real prototype unknown. T-2020 */
    temp_t6 = (u8)func_80051A68(D_800E6280.unk_F5F) & 0x7F;
    switch (temp_t6) {
    case 0:
        break;
    case 1:
    case 2:
        D_80143B00 += 1;
        break;
    case 3:
        D_80143B00 += 2;
        break;
    }
    func_8004284C();
}

void func_80133A60(void) {
    D_80143B00 = 0x27;
    func_8004284C();
}

void func_80133A88(void) {
    if (D_80122CDC != 0) {
        D_80143B00 += 1;
        D_800E6280.unk_1BC[0].unk_02 -= 1;
        D_800E6280.unk_1BC[0].unk_06 -= 1;
        D_800E6280.unk_1BC[0].unk_0A += 0xA;
        D_80143B24 = 0;
        func_8004284C();
        func_8004284C();
        func_8004284C();
    } else {
        D_800E6280.unk_1BC[0].unk_02 += 1;
    }
    func_80084D3C();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/801330C0", func_80133B44);

void func_80133CDC(void) {
    D_80143B00 = 0xF;
    func_8004284C();
}
