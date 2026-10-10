#include "common.h"
#include "ovl/SHOUGATU.h"

void func_801390C0(void) {
    D_80145270 = (u8 *)0x801E8088;
    D_80145274 = (u8 *)0x801E808C;
    D_80145278 = (u8 *)0x801E80A0;
    D_8014527C = *(s16 *)0x801E80B4;
    D_80145280 = (u8 *)0x801B0000;
    D_80145284 = (u8 *)0x801B2000;
    D_80145288 = (u8 *)0x801B4000;
    D_8014528C = (u8 *)0x801B8000;
    D_80145290 = (u8 *)0x801BC000;
    D_80145294 = (u8 *)0x801C0000;
    D_80145298 = (u8 *)0x801C4000;
    D_8014529C = (u8 *)0x801C8000;
    D_801452A0 = (u8 *)0x801D0000;
    D_801452A4 = (u8 *)0x801D4000;
    D_801452A8 = (u8 *)0x801D8000;
    D_801452AC = (u8 *)0x801DC000;
    D_801452B0 = (u8 *)0x801E0000;
    D_801452B4 = (u8 *)0x801E4000;
}

void func_801391D4(void) {
    func_801391F4();
}

typedef struct {
    void (*f[62])();
} FnTbl62; /* size 0xF8 */
extern FnTbl62 D_801452B8;

void func_801391F4(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl62 tbl;

    tbl = D_801452B8;
    idx = D_800E738A;
    tbl.f[idx](0x80);
    if (D_80144E08 == 0) {
        if (D_80144E0C == 3) {
            D_80122CF4 = 0;
        }
    }
}

void func_801392A4(void) {
    k_reset(1);
    func_8004284C();
}

void func_801392CC(void) {
    func_80046318(0x71, 0x801B0000, 0x85B6);
    func_801390C0();
    func_8004284C();
}

void func_80139304(void) {
    func_80044750(0x24);
    func_80044750(0x501);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/801390C0", func_80139334);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/801390C0", func_80139514);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/801390C0", func_8013960C);

void func_80139734(void) {
    D_80144E08 = 0;
    D_80144E0C = 0;
    func_800AE0F0(D_800CA19C, "廊下");
    func_8004284C();
}

void func_80139778(void) {
    bg_read_sub2(0x4045);
    func_8004284C();
}

void func_801397A0(void) {
    bg_read_sub2(0x46F0);
    func_8004284C();
}
