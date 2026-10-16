#include "common.h"
#include "ovl/TACO.h"

void func_8013B170(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_8013B2A0();
        break;
    case 1:
        func_8013B398();
        break;
    case 2:
        func_8013B3C4();
        break;
    }
    if (D_800E6280.unk_110A != 0) {
        if (D_800E6280.unk_10F8 & 0x10) {
            if ((u8) D_8015EDCC >= 3U) {
                func_801432F0((s32) "MISSION COMPLETE", -0x40, 0, 2);
            } else if (D_8015EDEC == 0) {
                func_801432F0((s32) "TIME OVER", -0x40, 0, 1);
            } else {
                func_801432F0((s32) "GAME OVER", -0x40, 0, 1);
            }
        }
        func_8013B460(0x64);
    } else {
        func_8013F250();
    }
    func_80143B58();
}

/* bulk_m2c declarations (T-11190) */
void func_8013B404(void);
/* end bulk_m2c declarations */
void func_8013B2A0(void) {
    s32 sp38;
    s32 sp30[2];

    if (D_8015EDBC == 0) {
        func_8013B404();
        func_801347A8();
        func_80059048();
        (*(s32 *)&sp30) = (s32) *(s32 *)((unsigned char *)D_8015EDB4 + (124));
        *(s32 *)((unsigned char *)(&sp30) + (4)) = (s32) *(s32 *)((unsigned char *)D_8015EDB4 + (128));
        sp38 = (s32) D_8015EDB4->unk84[1];
        func_80143AF4();
        *(s32 *)((unsigned char *)D_8015EDB4 + (124)) = (*(s32 *)&sp30);
        *(s32 *)((unsigned char *)D_8015EDB4 + (128)) = *(s32 *)((unsigned char *)(&sp30) + (4));
        D_8015EDB4->unk84[1] = (u8) sp38;
        D_800E6280.unk_038 = 0x10;
        D_800E6280.unk_037 = 0x10;
        D_800E6280.unk_036 = 0x10;
        D_8015EDB0 = 0;
        D_8015EDBC = 0x44;
        func_800450F4(0, 0x202);
        func_8004284C();
    }
}

void func_8013B398(void) {
    if (D_800E6280.unk_F88 != 0) {
        func_8004284C();
    }
}

void func_8013B3C4(void) {
    if (D_8015EDBC == 0) {
        if (func_80044C98() == 1) {
            func_80042908(8);
        }
    }
}

void func_8013B404(void) {
    s32 pad; /* FAKE: unused 4-byte local puts rect at the original offset; real source unknown. T-2040 */
    RECT rect;

    rect.x = 0;
    rect.y = D_8011ECA0 * 0xF0;
    rect.w = 0x200;
    rect.h = 0xF0;
    func_8009C93C(&rect, 0x200, 0);
    func_8009C674(0);
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/8013B170", func_8013B460);
