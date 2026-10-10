#include "ovl/OMIMAI.h"

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI/801320A0", func_801320A0);

typedef struct {
    void (*f[25])();
} FnTbl25; /* size 0x64 */
extern FnTbl25 D_80134AF0;

void func_801322E4(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl25 tbl;

    tbl = D_80134AF0;
    idx = D_800E738A;
    tbl.f[idx](0x80);
    if (D_800E738A < 0xEU) {
        func_80066C08(1);
        parameter_show();
        return;
    }
    func_80066C08(2);
}

void func_80132398(void) {
    if ((D_800E62C0 == ((u32)(D_800E6378 << 0x17) >> 0x1B)) && (D_800E62BF == (D_800E6378 & 0xF))) {
        func_80042808();
        return;
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI/801320A0", func_801323F8);

void func_801324BC(void) {
    func_80044890(1, 0xBF98, 0xBF79, D_800B3688[D_800E69DD], D_800B36C8[D_800E69DD], D_800B3708[D_800E69DD]);
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    func_8004284C();
}

void func_80132544(void) {
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    if ((u32) D_800E7384++ >= 0x400U) {
        func_800452C4();
        func_8004482C();
        func_80042940((D_800E738A - 1) & 0xFF);
    }
}

void func_801325C4(void) {
    func_80044750(0x200);
    func_8004284C();
}

void func_801325EC(void) {
    func_80046318(4, 0x80197000, 0xAF3F);
    func_801320A0();
    func_80044750(0x603);
    func_8004284C();
}

void func_80132630(void) {
    func_8004E58C();
    k_reset(1);
    addr_init_bustup();
    func_80084E4C();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI/801320A0", func_80132670);

void func_80132788(void) {
    bg_read_sub2(0x4144);
    func_80085B3C(0xF, 0);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/OMIMAI/nonmatchings/OMIMAI/801320A0", func_801327BC);

void func_8013285C(void) {
    switch (D_800E71DF) {
    case 0:
        D_800CA160 = D_80134A60[1];
        D_800CA164 = D_80134A90[1];
        D_800CA168 = D_80134AC0[1];
        return;
    case 1:
        D_800CA160 = D_80134A60[5];
        D_800CA164 = D_80134A90[5];
        D_800CA168 = D_80134AC0[5];
        return;
    case 2:
        D_800CA160 = D_80134A60[2];
        D_800CA164 = D_80134A90[2];
        D_800CA168 = D_80134AC0[2];
        return;
    case 3:
        D_800CA160 = D_80134A60[3];
        D_800CA164 = D_80134A90[3];
        D_800CA168 = D_80134AC0[3];
        return;
    case 4:
        D_800CA160 = D_80134A60[9];
        D_800CA164 = D_80134A90[9];
        D_800CA168 = D_80134AC0[9];
        return;
    case 5:
        D_800CA160 = D_80134A60[6];
        D_800CA164 = D_80134A90[6];
        D_800CA168 = D_80134AC0[6];
        return;
    case 6:
        D_800CA160 = D_80134A60[7];
        D_800CA164 = D_80134A90[7];
        D_800CA168 = D_80134AC0[7];
        return;
    case 7:
        D_800CA160 = D_80134A60[4];
        D_800CA164 = D_80134A90[4];
        D_800CA168 = D_80134AC0[4];
        return;
    case 8:
        D_800CA160 = D_80134A60[0];
        D_800CA164 = D_80134A90[0];
        D_800CA168 = D_80134AC0[0];
        return;
    case 9:
        D_800CA160 = D_80134A60[8];
        D_800CA164 = D_80134A90[8];
        D_800CA168 = D_80134AC0[8];
        return;
    case 10:
        D_800CA160 = D_80134A60[10];
        D_800CA164 = D_80134A90[10];
        D_800CA168 = D_80134AC0[10];
    }
}
