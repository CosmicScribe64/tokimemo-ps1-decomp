#include "common.h"
#include "ovl/DATE.h"

typedef struct {
    void (*f[44])();
} FnTbl44; /* size 0xB0 */
extern FnTbl44 D_80160890;

void func_801594B0(void) {
    D_801607E0 = 0x801CE0F4;
    D_801607E4 = 0x801CE0F8;
    D_801607E8 = 0x801CE118;
    D_801607EC = *(s16 *)0x801CE12C;
    D_801607F0 = 0x801B0000;
    D_801607F4 = 0x801B2000;
    D_801607F8 = 0x801B6000;
    D_801607FC = 0x801BA000;
    D_80160800 = 0x801BE000;
    D_80160804 = 0x801C2000;
    D_80160808 = 0x801C6000;
}

void func_80159560(void) {
    D_8016080C = 0x801CE0F4;
    D_80160810 = 0x801CE0F8;
    D_80160814 = 0x801CE118;
    D_80160818 = *(s16 *)0x801CE12C;
    D_8016081C = 0x801B0000;
    D_80160820 = 0x801B2000;
    D_80160824 = 0x801B6000;
    D_80160828 = 0x801BA000;
    D_8016082C = 0x801BE000;
    D_80160830 = 0x801C2000;
    D_80160834 = 0x801C6000;
}

void func_80159610(void) {
    D_80160838 = 0x801CE0F4;
    D_8016083C = 0x801CE0F8;
    D_80160840 = 0x801CE118;
    D_80160844 = *(s16 *)0x801CE12C;
    D_80160848 = 0x801B0000;
    D_8016084C = 0x801B2000;
    D_80160850 = 0x801B6000;
    D_80160854 = 0x801BA000;
    D_80160858 = 0x801BE000;
    D_8016085C = 0x801C2000;
    D_80160860 = 0x801C6000;
}

void func_801596C0(void) {
    D_80160864 = 0x801D2470;
    D_80160868 = 0x801D2478;
    D_8016086C = 0x801D24B8;
    D_80160870 = *(s16 *)0x801D24D0;
    D_80160874 = 0x801B0000;
    D_80160878 = 0x801B2000;
    D_8016087C = 0x801B6000;
    D_80160880 = 0x801BA000;
    D_80160884 = 0x801BE000;
    D_80160888 = 0x801C2000;
    D_8016088C = 0x801C6000;
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/801594B0", func_80159770);

void func_80159804(void) {
    D_8015E208 = D_8015DF64;
    D_8015E20C = D_8015E0A0;
    D_8015E210 = D_8015E1DC;
    D_800E643C[10].unk_02 += 1;
    D_800E643C[10].unk_06 += 1;
    D_800E643C[10].unk_0A -= 0xA;
    check_para_limit();
    func_801594B0();
    func_80043914(D_801607F0, 0x11, 1, 2, 0);
    func_80084E90(D_801607F4, D_801607F8, D_801607FC, D_80160800, D_80160804, D_80160808);
    func_800850D4(D_801607E4, D_801607E8, D_801607E0, D_801607EC);
    D_800CA360 = 1;
    D_800CA224 = 2;
    D_800CA226 = 2;
    D_800CA228 = 2;
    D_800CA234 = 1;
    D_800CA236 = 1;
    D_800CA238 = 1;
    func_8004284C();
}

void func_8015996C(void) {
    D_8015E208 = D_8015DF68;
    D_8015E20C = D_8015E0A4;
    D_8015E210 = D_8015E1E0;
    func_800AE0F0(D_800CA1DC, "池");
    D_800E643C[10].unk_02 += 3;
    D_800E643C[10].unk_06 += 2;
    D_800E643C[10].unk_0A -= 0x14;
    check_para_limit();
    func_80159560();
    func_80043914(D_8016081C, 0x11, 1, 2, 0);
    func_80084E90(D_80160820, D_80160824, D_80160828, D_8016082C, D_80160830, D_80160834);
    func_800850D4(D_80160810, D_80160814, D_8016080C, D_80160818);
    func_8004284C();
}

void func_80159A98(void) {
    D_8015E208 = D_8015DF6C;
    D_8015E20C = D_8015E0A8;
    D_8015E210 = D_8015E1E4;
    D_800E643C[10].unk_06 += 2;
    D_800E643C[10].unk_0A -= 0xA;
    check_para_limit();
    func_80159610();
    func_80043914(D_80160848, 0x11, 1, 2, 0);
    func_80084E90(D_8016084C, D_80160850, D_80160854, D_80160858, D_8016085C, D_80160860);
    func_800850D4(D_8016083C, D_80160840, D_80160838, D_80160844);
    D_800CA360 = 1;
    func_8004284C();
}

void func_80159BA4(void) {
    D_8015E208 = D_8015DF70;
    D_8015E20C = D_8015E0AC;
    D_8015E210 = D_8015E1E8;
    D_800E643C[10].unk_02 += 2;
    D_800E643C[10].unk_06 += 1;
    D_800E643C[10].unk_0A -= 0x14;
    check_para_limit();
    func_801596C0();
    func_80043914(D_80160874, 0x11, 1, 2, 0);
    func_80084E90(D_80160878, D_8016087C, D_80160880, D_80160884, D_80160888, D_8016088C);
    func_800850D4(D_80160868, D_8016086C, D_80160864, D_80160870);
    D_800CA360 = 1;
    D_800CA224 = 2;
    D_800CA226 = 2;
    D_800CA228 = 2;
    D_800CA234 = 1;
    D_800CA236 = 1;
    D_800CA238 = 1;
    func_8004284C();
}

void func_80159D0C(void) {
    bg_read_sub2(0x4821);
    func_8004284C();
}

void func_80159D34(void) {
    bg_read_sub2(0x432B);
    func_8004284C();
}

void func_80159D5C(void) {
    bg_read_sub2(0x4335);
    func_8004284C();
}

void func_80159D84(void) {
    bg_read_sub2(0x42FD);
    func_8004284C();
}

void func_80159DAC(void) {
    bg_read_sub2(0x47E8);
    func_8004284C();
}

void func_80159DD4(void) {
    bg_read_sub2(0x42EB);
    func_8004284C();
}

void func_80159DFC(void) {
    bg_read_sub2(0x4913);
    func_8004284C();
}

void func_80159E24(void) {
    bg_read_sub2(0x45F8);
    func_8004284C();
}

void func_80159E4C(void) {
    bg_read_sub2(0x489A);
    func_8004284C();
}

void func_80159E74(void) {
    bg_read_sub2(0x4562);
    func_8004284C();
}

void func_80159E9C(void) {
    bg_read_sub2(0x4558);
    func_8004284C();
}

void func_80159EC4(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl44 tbl;

    tbl = D_80160890;
    idx = D_800E738A;
    tbl.f[idx](0x80);
}

void func_80159F4C(void) {
    func_80044750(0x508);
    func_8004284C();
}

void func_80159F74(void) {
    func_80046318(0x3D, 0x801B0000, 0x8E02);
    func_801594B0();
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/801594B0", func_80159FAC);

INCLUDE_ASM("asm/ovl/DATE/nonmatchings/DATE/801594B0", func_8015A114);

void func_8015A170(void) {
    func_80044750(0x500);
}
