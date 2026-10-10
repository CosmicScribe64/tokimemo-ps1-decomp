#define MAIN_API_OVERRIDE_D_80122EB8 /* matched as u32 (main_api.h: s32) */
#include "common.h"
#include "ovl/BUNKAKEN.h"

extern u32 D_80122EB8;

/* Bit 2 of the flag word of a Rec38 record (tested with sll 29 / bltz). */
typedef struct BunkakenFlagBits {
    u32 pad0 : 2;
    u32 flag : 1;
    u32 rest : 29;
} BunkakenFlagBits;

void func_8013BFE0(void) {
    D_80122EBC = 1;
    switch (D_80122EB8) {
    case 0:
        if (D_800E6280.unk_03E == 0x61) {
            func_80042908(0x2E);
            return;
        }
        if ((func_8008667C(1) == 4) && !((BunkakenFlagBits *)&D_800E6280.unk_1BC[1].unk_0C)->flag) {
            D_80122EBC = 0xFF;
            func_80042908(5);
            return;
        }
        func_80042908(0x2D);
        return;
    case 1:
        func_80042908(0x2F);
        return;
    default:
        func_80046500();
        return;
    }
}

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8013BFE0", func_8013C0AC);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8013BFE0", func_8013C634);

INCLUDE_ASM("asm/ovl/BUNKAKEN/nonmatchings/BUNKAKEN/8013BFE0", func_8013CA6C);

void func_8013CEC0(void) {
    func_800847B8(1);
    func_80132104();
    D_800CA14C = 0;
    D_800CA160 = D_80155E90;
    D_800CA164 = D_80155E94;
    D_800CA168 = D_80155E98;
    func_8004284C();
}
