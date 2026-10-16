#include "common.h"
#include "ovl/TACO.h"

void func_80137F90(void) {
    D_800E6280.unk_1104.w += 1;
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8013801C();
        break;
    case 1:
        func_80138160();
        break;
    case 2:
        func_8013822C();
        break;
    }
    func_8013F250();
    func_80132FE8();
}
void func_8013801C(void) {
    s32 var_s0;

    func_8013AFDC();
    D_8015F4F0 = 0x80;
    D_8015EDCC = 0;
    D_8015EDF0 = 0;
    D_8015EDB0 = 0x80;
    D_8015EDEC = 0x4650;
    func_80134884();
    D_8015EDBC = 0x10;
    D_800E6280.unk_036 = 0;
    D_800E6280.unk_037 = 0;
    D_800E6280.unk_038 = 0;
    func_801335A0(0, 0x503);
    func_800591D8(1);
    k_reset(0);
    back_clear_switch(1);
    func_80143A74();
    func_8014394C();
    func_80059048();
    func_801347A8();
    func_80143AF4();
    /* FAKE: shared source line schedules the loop setup (T-9780). */
    var_s0 = 0; do {
        /* FAKE: integer address arithmetic preserves index-first addu (T-9780). */
        func_80099E30(0, (void *)(var_s0 + (u32)D_8015EDB4 + 0x14));
        func_801438F0(0x801AC000, 0, var_s0 + (u32)D_8015EDB4);
        var_s0 += 0x88;
    } while (var_s0 != 0x29F8);
    func_801322AC();
    func_80132220();
    func_801334BC();
    func_8004284C();
}

void func_80138160(void) {
    func_8013A790(1, 1);
    if (D_8015EDBC == 0) {
        func_800450F4(0, 0x203);
        load_csr_tp(0x80162000, 0xE, 1, 0, 0, 0x100, 0x100);
        load_csr_tp(0x80172000, 0xC, 1, 0, 0, 0x100, 0x100);
        load_csr_tp(0x80182000, 0xA, 1, 0, 0, 0x80, 0x80);
        func_8004284C();
    }
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80137F90", func_8013822C);
