#include "ovl/MASTER.h"

typedef struct {
    void (*f[4])();
} FnTbl4; /* size 0x10 */
extern FnTbl4 D_8013C2E4;

void func_80132140(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl4 tbl;

    tbl = D_8013C2E4;
    idx = D_800E6280.unk_1109;
    tbl.f[idx]();
}

void func_801321B0(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80044750(0xBF);
        func_80044890(1, 0xC043, 0xC01D, 0xC982, 0xC935, 0xC91B);
        func_80085E30(1, 0);
        func_800573AC();
        func_80057390(0);
        func_8008585C();
        func_80064E84();
        func_800649D4();
        func_80063930(0);
        func_80064FA4(0);
        func_8006764C(0);
        func_8004284C();
        break;
    case 1:
        if (func_80044E8C() == 1) {
            func_8004E9F4(0);
            switch ((u16) D_800E6280.unk_0F4.h >> 0xC) {
            case 0:
                func_8006612C((s32) "植木賞授賞式");
                func_8005742C(0x4644, 1);
                break;
            case 1:
                func_8006612C((s32) "演劇グランプリ");
                func_8005742C(0x464D, 1);
                break;
            case 2:
                func_8006612C((s32) "ノーベル賞受賞");
                func_8005742C(0x4656, 1);
                break;
            case 3:
                func_8006612C((s32) "ゲーム大賞");
                func_8005742C(0x465F, 1);
                break;
            case 4:
                func_8006612C((s32) "目展大賞");
                func_8005742C(0x4669, 1);
                break;
            case 5:
                func_8006612C((s32) "コンクール優勝");
                func_8005742C(0x4673, 1);
                break;
            default:
                func_80046500();
                break;
            }
            func_80065F34(0);
            func_80044750(0x202);
            func_8004284C();
        }
        break;
    case 2:
        func_8004284C();
        break;
    case 3:
        if (cd_read_sync_status() & 1) {
            func_80057418(1, func_8005751C(1));
            func_800847B8(0U);
            func_80084E4C();
            func_80065F34(1);
            func_8004284C();
        }
        break;
    case 4:
        func_8007E81C();
        break;
    case 5:
        func_80042808();
        break;
    default:
        func_80046500();
        break;
    }
    func_80086424();
}

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80132140", func_80132444);

INCLUDE_ASM("asm/ovl/MASTER/nonmatchings/MASTER/80132140", func_801326B4);

typedef struct {
    u32 pad0 : 25;
    u32 grade : 2;
    u32 pad1 : 3;
    u32 flag : 1;
    u32 pad2 : 1;
} MasterSlot; /* word of GameState.unk_66C[]: bit 30 is the flag */

void func_80133374(void) {
    s32 i;

    D_800E6280.unk_71E |= 0x10;
    D_800E6280.unk_1BC[0].unk_02 += 0x14;
    for (i = 1; i < 0xD; i++) {
        D_800E6280.unk_1BC[i].unk_02 += 0x14;
    }
    if (D_8013C2E0 != 0xFF) {
        D_800E6280.unk_1BC[D_8013C2E0].unk_06 += 2;
    }
    ((MasterSlot *)&D_800E6280.unk_66C[(u32)D_800E6280.unk_0F4.h >> 0xC])->flag = 1;
    func_8008585C();
    func_800573AC();
    func_80057390(0);
    func_8013348C();
    func_80072B5C(1);
    func_80086424();
}

/* bulk_m2c declarations (T-11190) */
extern u16 D_800E6374;
extern u8 D_800E62BE;
extern u8 D_800E62BF;
extern u8 D_800E62C0;
extern u8 D_800E62C1;
extern u8 D_800E644A;
extern u8 D_800E6482;
extern u8 D_800E71DE;
/* end bulk_m2c declarations */
s32 func_8013348C(void) {
    u8 sp2B;
    u32 temp_t7;
    u8 temp_t2;
    u8 var_v0;

    temp_t7 = (u16) D_800E6374 >> 0xC;
    switch (temp_t7) {
    case 0:
        sp2B = 0;
        break;
    case 1:
        if ((((u8) D_800E644A >> 4) == 1) && (D_8013C2E0 == 0)) {
            var_v0 = 0xB;
        } else if (((u8) D_800E6482 >> 4) == 1) {
            var_v0 = 1;
            if (D_8013C2E0 == 1) {
                var_v0 = 0xC;
            }
        } else {
            var_v0 = 1;
        }
block_15:
        sp2B = var_v0;
        break;
    case 2:
        var_v0 = 2;
        goto block_15;
    case 3:
        var_v0 = 3;
        goto block_15;
    case 4:
        var_v0 = 4;
        goto block_15;
    case 5:
        var_v0 = 5;
        goto block_15;
    }
    D_800E6280.unk_75E[D_800E71DE].unk_00 = (u8) D_8013C2E0;
    D_800E6280.unk_75E[D_800E71DE].unk_02 = 0x15;
    D_800E6280.unk_75E[D_800E71DE].unk_01 = sp2B;
    D_800E6280.unk_75E[D_800E71DE].unk_03 = get_g_zyotai_h(D_8013C2E0);
    D_800E6280.unk_75E[D_800E71DE].unk_04 = D_800E62BE;
    D_800E6280.unk_75E[D_800E71DE].unk_05 = D_800E62BF;
    D_800E6280.unk_75E[D_800E71DE].unk_06 = D_800E62C0;
    D_800E6280.unk_75E[D_800E71DE].unk_07 = D_800E62C1;
    temp_t2 = D_800E71DE + 1;
    D_800E71DE = temp_t2;
    if ((u32) (temp_t2 & 0xFF) >= 0xFFU) {
        D_800E71DE = 0xFE;
    }
}
