#include "common.h"
#include "ovl/KANGEI.h"

typedef struct {
    void (*f[22])();
} FnTbl22; /* size 0x58 */
extern FnTbl22 D_801399B0;

void func_80132090(void) {
    D_80139950 = 0x801973AC;
    D_80139954 = 0x80197910;
    D_80139958 = 0x80197C44;
    D_8013995C = 0x80197F34;
    D_80139960 = 0x80198130;
    D_80139964 = 0x801982DC;
    D_80139968 = 0x801984F4;
    D_8013996C = 0x8019865C;
    D_80139970 = 0x8019740C;
    D_80139974 = 0x80197930;
    D_80139978 = 0x80197C74;
    D_8013997C = 0x80197F4C;
    D_80139980 = 0x80198140;
    D_80139984 = 0x801982F4;
    D_80139988 = 0x8019850C;
    D_8013998C = 0x80198668;
    D_80139990 = 0x801977AC;
    D_80139994 = 0x80197A70;
    D_80139998 = 0x80197E44;
    D_8013999C = 0x80198034;
    D_801399A0 = 0x801981F0;
    D_801399A4 = 0x801983DC;
    D_801399A8 = 0x801985F4;
    D_801399AC = 0x801986DC;
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

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80132090", func_80132354);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80132090", func_801323A8);

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
    func_80046318(3, 0x80197000, 0xAF3C);
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

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80132090", func_8013260C);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80132090", func_801327DC);

INCLUDE_ASM("asm/ovl/KANGEI/nonmatchings/KANGEI/80132090", func_80132C24);
