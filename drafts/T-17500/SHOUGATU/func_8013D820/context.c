#include "common.h"
#include "ovl/SHOUGATU.h"

void func_8013D660(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl33 tbl;

    tbl = D_80145F40;
    idx = D_800E6280.unk_110A;
    tbl.f[idx](0x80);
}

void func_8013D6D4(void) {
    func_80044890(1, 0xC621, 0xC613, 0xD93B, 0xD8F0, 0xD8D0);
    if (func_80044E8C() == 1) {
        func_8004284C();
    }
    func_8004284C();
}

void func_8013D734(void) {
    if (func_80044E8C() == 1) {
        func_80044750(0x202);
        func_8004284C();
    }
    if ((u32) D_800E6280.unk_1104.w++ >= 0x400U) {
        func_800452C4();
        func_8004482C();
        func_80042940((D_800E6280.unk_110A - 1) & 0xFF);
    }
}

void func_8013D7BC(void) {
    cd_read_sectors(0xE, 0x80197000, 0xAFBC);
    func_8013C070();
    func_8004284C();
}

void func_8013D7F8(void) {
    D_80122CFC = 0x1E;
    func_8004284C();
}

void func_8013D820(void) {
    s32 var_t6;
    u32 temp_v1;

    temp_v1 = (u32) (D_800E6280.unk_1BC[0xC].unk_0C.w << 0x14) >> 0x1D;
    if (D_800E6280.unk_10C.unk_02 >= (s16) ((temp_v1 * 0x19) + 0x96)) {
        var_t6 = temp_v1 < 2U;
        if (((u32) (D_800E6280.unk_1BC[0xE].unk_0C.w << 0x14) >> 0x1D) != 0) {
            func_8004284C();
            func_8004284C();
            func_8004284C();
            func_8004284C();
            return;
        }
        goto block_4;
    }
    var_t6 = temp_v1 < 2U;
block_4:
    if (D_800E6280.unk_114.unk_02 >= (s16) (((temp_v1 * 0x1E) - ((var_t6 ^ 1) * 0xA)) + 0x46)) {
        func_8004284C();
        return;
    }
    func_8004284C();
    func_8004284C();
    func_8004284C();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013D660", func_8013D92C);

void func_8013DA5C(void) {
    if (((u32) (D_800E6280.unk_1BC[14].unk_0C.w << 0x14) >> 0x1D) == 1) {
        func_8013D2A8();
        return;
    }
    func_8004284C();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/8013D660", func_8013DAAC);

void func_8013DCFC(void) {
    bg_read_sub2(0x41CA);
    func_8004284C();
}

void func_8013DD24(void) {
    func_80062CD0(0x6B24);
    func_8004284C();
}

void func_8013DD4C(void) {
    func_80042808();
}

void func_8013DD6C(void) {
    func_80042908(4);
}
