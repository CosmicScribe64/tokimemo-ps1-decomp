#include "common.h"
#include "ovl/BUNKAKEN.h"

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80144F70", func_80144F70);

void func_80145500(void) {
    if (D_800E6280.unk_110A != 0) {
        D_800E6280.unk_110A = 0;
        D_80122EC0 += 1;
    }
}

void func_80145530(void) {
    D_800E6280.unk_1109 = 0;
    D_800E6280.unk_110A = 0;
    D_80122EC0 = 8;
}

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80144F70", func_80145550);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/80144F70", func_80145844);

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80144F70.rodata", D_80154B84);

void func_801459FC(void) {
    k_reset(0);
    func_8006612C(&D_80154B84);
    k_disp_start(1);
}

INCLUDE_RODATA("asm/ovl/BUNKAKEN/data/BUNKAKEN/80144F70.rodata", D_80154B8C);

void func_80145A30(void) {
    k_reset(0);
    func_8006612C(&D_80154B8C);
    func_80065F34(0);
}

/* bulk_m2c declarations (T-11190) */
extern u8 D_800E62BE;
extern u8 D_800E62BF;
extern u8 D_800E62C0;
extern u8 D_800E62C1;
extern u8 D_800E644A;
extern u8 D_800E6482;
extern u8 D_800E64BA;
extern u8 D_800E64F2;
extern u8 D_800E71DE;
/* end bulk_m2c declarations */
void func_80145A64(void) {
    u8 sp2B;
    u8 temp_t8;
    u8 var_a1;

    if (func_8008667C(D_80122EBC) != 4) {
        if (D_80122EA4 != D_80122EBC) {
            switch (D_80122EB8) {
            case 0:
                if ((((u8) D_800E644A >> 4) == 0) && (D_80122EBC == 0)) {
                    var_a1 = 3;
                } else {
                    var_a1 = 4;
                    if ((((u8) D_800E6482 >> 4) == 0) && (D_80122EBC == 1)) {
                        var_a1 = 3;
                    }
                }
block_45:
                sp2B = var_a1;
                break;
            case 1:
                if ((((u8) D_800E644A >> 4) == 1) && (D_80122EBC == 0)) {
                    var_a1 = 7;
                } else {
                    var_a1 = 8;
                    if ((((u8) D_800E6482 >> 4) == 1) && (D_80122EBC == 1)) {
                        var_a1 = 7;
                    }
                }
                goto block_45;
            case 2:
                if ((((u8) D_800E644A >> 4) == 2) && (D_80122EBC == 0)) {
                    var_a1 = 0xB;
                } else {
                    var_a1 = 0xC;
                    if ((((u8) D_800E64BA >> 4) == 2) && (D_80122EBC == 2)) {
                        var_a1 = 0xB;
                    }
                }
                goto block_45;
            case 3:
                if ((((u8) D_800E644A >> 4) == 3) && (D_80122EBC == 0)) {
                    var_a1 = 0xF;
                } else {
                    var_a1 = 0x10;
                    if ((((u8) D_800E64BA >> 4) == 3) && (D_80122EBC == 2)) {
                        var_a1 = 0xF;
                    }
                }
                goto block_45;
            case 4:
                if ((((u8) D_800E644A >> 4) == 4) && (D_80122EBC == 0)) {
                    var_a1 = 0x13;
                } else {
                    var_a1 = 0x14;
                    if ((((u8) D_800E64F2 >> 4) == 4) && (D_80122EBC == 3)) {
                        var_a1 = 0x13;
                    }
                }
                goto block_45;
            case 5:
                if ((((u8) D_800E644A >> 4) == 5) && (D_80122EBC == 0)) {
                    var_a1 = 0x17;
                } else {
                    var_a1 = 0x18;
                    if ((((u8) D_800E64F2 >> 4) == 5) && (D_80122EBC == 3)) {
                        var_a1 = 0x17;
                    }
                }
                goto block_45;
            }
            if (D_80122EBC == 0xB) {
                D_800E6280.unk_75E[D_800E71DE].unk_00 = 0;
                D_800E6280.unk_75E[D_800E71DE].unk_03 = get_g_zyotai_h(0, sp2B);
            } else {
                D_800E6280.unk_75E[D_800E71DE].unk_00 = (u8) D_80122EBC;
                D_800E6280.unk_75E[D_800E71DE].unk_03 = get_g_zyotai_h(D_80122EBC, sp2B);
            }
            D_800E6280.unk_75E[D_800E71DE].unk_02 = 0x14;
            D_800E6280.unk_75E[D_800E71DE].unk_01 = sp2B;
            D_800E6280.unk_75E[D_800E71DE].unk_04 = D_800E62BE;
            D_800E6280.unk_75E[D_800E71DE].unk_05 = D_800E62BF;
            D_800E6280.unk_75E[D_800E71DE].unk_06 = D_800E62C0;
            D_800E6280.unk_75E[D_800E71DE].unk_07 = D_800E62C1;
            temp_t8 = D_800E71DE + 1;
            D_800E71DE = temp_t8;
            if ((u32) (temp_t8 & 0xFF) >= 0xFFU) {
                D_800E71DE = 0xFE;
            }
        }
    }
}
