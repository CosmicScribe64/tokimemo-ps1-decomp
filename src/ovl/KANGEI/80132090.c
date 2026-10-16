#include "common.h"
#include "ovl/KANGEI.h"

typedef struct {
    void (*f[22])();
} FnTbl22; /* size 0x58 */
extern FnTbl22 D_801399B0;

void func_80132090(void) {
    D_80139950[0][0] = 0x801973AC;
    D_80139950[0][1] = 0x80197910;
    D_80139950[0][2] = 0x80197C44;
    D_80139950[0][3] = 0x80197F34;
    D_80139950[0][4] = 0x80198130;
    D_80139950[0][5] = 0x801982DC;
    D_80139950[0][6] = 0x801984F4;
    D_80139950[0][7] = 0x8019865C;
    D_80139950[1][0] = 0x8019740C;
    D_80139950[1][1] = 0x80197930;
    D_80139950[1][2] = 0x80197C74;
    D_80139950[1][3] = 0x80197F4C;
    D_80139950[1][4] = 0x80198140;
    D_80139950[1][5] = 0x801982F4;
    D_80139950[1][6] = 0x8019850C;
    D_80139950[1][7] = 0x80198668;
    D_80139950[2][0] = 0x801977AC;
    D_80139950[2][1] = 0x80197A70;
    D_80139950[2][2] = 0x80197E44;
    D_80139950[2][3] = 0x80198034;
    D_80139950[2][4] = 0x801981F0;
    D_80139950[2][5] = 0x801983DC;
    D_80139950[2][6] = 0x801985F4;
    D_80139950[2][7] = 0x801986DC;
}

void func_80132214(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl22 tbl;

    tbl = D_801399B0;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

s32 func_80132290(void) {
    if (D_800E6280.unk_110D == 0) {
        func_800674B0();
        D_800E6280.unk_110D += 1;
    }
    if (D_800E6280.unk_110D == 1) {
        if (D_800E6280.unk_1104.u++ >= 0x400U) {
            func_800452C4();
            func_8004482C();
            D_800E6280.unk_110D = 0;
        }
        if (func_80044E8C() == 1) {
            D_800E6280.unk_110D += 1;
        } else {
            return 0;
        }
    }
    return func_80072B5C(1);
}

void func_80132354(void) {
    u32 v;
    s32 lo;

    v = func_80051B48(D_800E6280.unk_F5F);
    lo = v & 0xFF;
    if (v >= 3U) {
        func_80083440(3);
    } else {
        func_80083440(lo);
    }
    func_8004284C();
}

void func_801323A8(void) {
    func_80044890(1, 0xBF98, 0xBF79, (&D_800B3688)[D_800E6280.unk_F5F], (&D_800B36C8)[D_800E6280.unk_F5F], (&D_800B3708)[D_800E6280.unk_F5F]);
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    func_8004284C();
}

void func_80132430(void) {
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    if ((u32) D_800E6280.unk_1104.w++ >= 0x400U) {
        func_800452C4();
        func_8004482C();
        func_80042940((D_800E6280.unk_110A - 1) & 0xFF);
    }
}

void func_801324B0(void) {
    func_80044750(0x200);
    func_8004284C();
}

void func_801324D8(void) {
    cd_read_sectors(3, 0x80197000, 0xAF3C);
    func_80132090();
    func_8004284C();
}

void func_80132514(void) {
    draw2d3d(1, 0);
    func_80048DAC(1);
    func_80041584();
    func_80048EB8(0);
    back_clear_switch(1);
    tpage_buf_clear();
    func_8004E58C();
    k_reset(1);
    D_800E6280.unk_10A2 = 0;
    D_800E6280.unk_10E8 = 1;
    func_8008585C();
    D_800E6280.unk_03A = 0x80;
    D_800B593C = 0;
    D_800B5940 = 0;
    hizuke_init();
    message_window_init();
    addr_init_bustup();
    func_80084E4C();
    func_801325D0();
    func_8004284C();
}

void func_801325D0(void) {
    D_800E6280.unk_F5F = D_800E6280.unk_75D;
    func_800847B8(D_800E6280.unk_75D);
    func_80132090();
    func_8013260C();
}

void func_8013260C(void) {
    switch (D_800E6280.unk_F5F) {
    case 0:
        D_800CA160 = D_80139950[0][0];
        D_800CA164 = D_80139950[1][0];
        D_800CA168 = D_80139950[2][0];
        return;
    case 1:
        D_800CA160 = D_80139950[0][3];
        D_800CA164 = D_80139950[1][3];
        D_800CA168 = D_80139950[2][3];
        return;
    case 2:
        D_800CA160 = D_80139950[0][1];
        D_800CA164 = D_80139950[1][1];
        D_800CA168 = D_80139950[2][1];
        return;
    case 3:
        D_800CA160 = D_80139950[0][2];
        D_800CA164 = D_80139950[1][2];
        D_800CA168 = D_80139950[2][2];
        return;
    case 4:
        D_800CA160 = D_80139950[0][6];
        D_800CA164 = D_80139950[1][6];
        D_800CA168 = D_80139950[2][6];
        return;
    case 5:
        D_800CA160 = D_80139950[0][4];
        D_800CA164 = D_80139950[1][4];
        D_800CA168 = D_80139950[2][4];
        return;
    case 6:
        D_800CA160 = D_80139950[0][5];
        D_800CA164 = D_80139950[1][5];
        D_800CA168 = D_80139950[2][5];
        return;
    default:
        D_800CA160 = D_80139950[0][7];
        D_800CA164 = D_80139950[1][7];
        D_800CA168 = D_80139950[2][7];
        return;
    }
}

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80132090", func_801327DC);

void func_80132C24(void) {
    u8 girl;
    KangeiWords11 all;
    KangeiWords2 first;
    KangeiWords2 second;
    KangeiWords2 third;

    all = D_80139A08;
    first = D_80139A34;
    second = D_80139A3C;
    third = D_80139A44;
    if ((D_800E6280.unk_03F < 6) || (D_800E6280.unk_03F >= 10)) {
        all.unk_00[0] = 0x544A;
        all.unk_00[3] = 0x544A;
        all.unk_00[5] = 0x544A;
        first.unk_00[0] = 0x5D41;
        second.unk_00[1] = 0x522E;
        third.unk_00[1] = 0x58AF;
    }
    girl = D_800E6280.unk_F5F;
    switch (girl) {
    case 0:
        func_80062CD0(all.unk_00[D_800E6280.unk_0F4.h >> 12]);
        break;
    case 1:
        func_80062CD0(first.unk_00[D_800E6280.unk_0F4.h >> 12]);
        break;
    case 2:
        func_80062CD0(second.unk_00[(D_800E6280.unk_0F4.h >> 12) - 2]);
        break;
    case 3:
        func_80062CD0(third.unk_00[(D_800E6280.unk_0F4.h >> 12) - 4]);
        break;
    case 4:
        func_80062CD0(0x66EC);
        break;
    case 5:
        func_80062CD0(0x5FE4);
        break;
    case 6:
        func_80062CD0(0x625A);
        break;
    default:
        func_80062CD0(0x6962);
        break;
    }
    func_8004284C();
}
