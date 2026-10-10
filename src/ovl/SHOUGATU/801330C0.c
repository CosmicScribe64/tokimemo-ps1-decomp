#include "common.h"
#include "ovl/SHOUGATU.h"

typedef struct {
    void (*f[60])();
} FnTbl60; /* size 0xF0 */
extern FnTbl60 D_80143BC4;

void func_801330C0(void) {
    s32 idx; /* declared before tbl: its stack slot sits above tbl (T-3330) */
    FnTbl60 tbl;

    tbl = D_80143BC4;
    idx = D_800E738A;
    tbl.f[idx](0x80);
}

void func_80133134(void) {
    func_80044750(0x603);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/801330C0", func_8013315C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/801330C0", func_80133208);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/801330C0", func_801333F0);

void func_80133610(void) {
    func_800AE0F0(D_800CA1DC, "正月");
    D_80143BC0 = 0;
    D_80143B20 = 0;
    D_80143B18 = 1;
    D_80143B00 = 6;
    D_80143AF4 = D_80143A48;
    D_80143AF8 = D_80143A84;
    D_80143AFC = D_80143AC0;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/801330C0", func_80133698);

void func_80133710(void) {
    bg_read_sub2(0x4167);
    func_8004284C();
}

void func_80133738(void) {
    bg_read_sub2(0x4144);
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/801330C0", func_80133760);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/801330C0", func_80133874);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/801330C0", func_801338DC);

void func_80133948(void) {
    if (D_80122CDC == 0) {
        func_8004284C();
    }
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/801330C0", func_8013397C);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/801330C0", func_801339DC);

void func_80133A60(void) {
    D_80143B00 = 0x27;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/801330C0", func_80133A88);

INCLUDE_ASM("asm/ovl/SHOUGATU/nonmatchings/SHOUGATU/801330C0", func_80133B44);

void func_80133CDC(void) {
    D_80143B00 = 0xF;
    func_8004284C();
}
