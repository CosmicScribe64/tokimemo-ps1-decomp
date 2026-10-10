#include "common.h"
#include "ovl/SHUGAKU.h"

void func_80133D60(void) {
    D_8013BFD0 = (u8 *)0x801D2384;
    D_8013BFD4 = (u8 *)0x801D238C;
    D_8013BFD8 = (u8 *)0x801D23F0;
    D_8013BFDC = *(s16 *)0x801D240C;
    D_8013BFE0 = (u8 *)0x801B0000;
    D_8013BFE4 = (u8 *)0x801B2000;
    D_8013BFE8 = (u8 *)0x801B6000;
    D_8013BFEC = (u8 *)0x801BA000;
    D_8013BFF0 = (u8 *)0x801BE000;
    D_8013BFF4 = (u8 *)0x801C2000;
    D_8013BFF8 = (u8 *)0x801C6000;
}

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80133D60", func_80133E10);

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80133D60", func_80133F6C);

INCLUDE_ASM("asm/ovl/SHUGAKU/nonmatchings/SHUGAKU/80133D60", func_80134024);

void func_80134070(void) {
    D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_06 = D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_06 + 2;
    check_para_limit();
    func_80044750(0x201);
    func_80132BCC();
}

void func_801340C8(void) {
    func_80046318(0x45, 0x801B0000, 0x83FB);
    func_80133D60();
    func_8004284C();
}

void func_80134100(void) {
    if (D_800E6280.unk_100.unk_02 >= 0x78) {
        func_8004284C();
    }
    func_8004284C();
}

void func_80134138(void) {
    D_800E6280.unk_F5F = 3;
    D_80120666 = 2;
    D_801206EE = 4;
    func_800847B8(3);
    func_8004284C();
}

INCLUDE_RODATA("asm/ovl/SHUGAKU/data/SHUGAKU/80133D60.rodata", D_8013AD70);

void func_80134184(void) {
    D_800E6280.unk_F5F = 0xE;
    D_80120666 = 4;
    D_801206EE = 2;
    func_800AE0F0(&D_800CA188, &D_8013AD70);
    func_8004284C();
}

void func_801341DC(void) {
    if (D_800E6280.unk_100.unk_02 >= 0x78) {
        D_800CA2E4 += 3;
    }
    func_8004284C();
}

void func_80134220(void) {
    func_80044750(0x204);
    bg_read_sub2(0x476A);
    func_80085B3C(2, 0x1A);
    func_8004284C();
}

typedef struct {
    s32 w[0x11];
} Tb44; /* size 0x44 */

void func_8013425C(void) {
    func_801365F4();
    D_8013BFFC = D_8013C530;
    D_8013C000 = D_8013C66C;
    D_8013C004 = D_8013C7A8;
    D_800CA2E4 = 0;
    D_800CA2E8 = 0;
    func_80133D60();
    func_80043914(D_8013BFE0, 0x11, 1, 2, 0);
    func_80084E90(D_8013BFE4, D_8013BFE8, D_8013BFEC, D_8013BFF0, D_8013BFF4, D_8013BFF8);
    func_800850D4(D_8013BFD4, D_8013BFD8, D_8013BFD0, D_8013BFDC);
    D_80120650[0x48] = 8;
    D_80120650[4] = 8;
    *(Tb44 *) &D_80120650[0x88] = *(Tb44 *) D_80120650;
    *(s16 *) &D_80120650[0x9E] = 4;
    func_8004284C();
}

void func_801343B0(void) {
    D_800CA134 = &D_800CA2E4;
    D_800CA138 = &D_800CA2E8;
    D_800CA13C = D_8013BFFC;
    D_800CA140 = D_8013C000;
    D_800CA144 = D_8013C004;
    func_80082764(D_80122CDC, 1, 0);
}

void func_8013442C(void) {
    D_800CA134 = (u8 *) &D_800CA2E4;
    D_800CA138 = (u8 *) &D_800CA2E8;
    D_800CA13C = D_8013BFFC;
    D_800CA140 = D_8013C000;
    D_800CA144 = D_8013C004;
    func_80082764(0xFF, 1, 0);
}
