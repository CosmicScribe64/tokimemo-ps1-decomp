#include "common.h"
#include "ovl/GYOZI.h"

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80134000", func_80134000);

INCLUDE_ASM("asm/ovl/GYOZI/nonmatchings/GYOZI/80134000", func_801342A4);

void func_801343A4(void) {
    D_800D9234 = (u8 *) &D_80145EB4;
    D_800D9238 = (u8 *) &D_80145EB8;
    D_800D923C = D_80145EA8;
    D_800D9240 = D_80145EAC;
    D_800D9244 = D_80145EB0;
    func_8008D610(D_8012E66C, 1, 0);
}

void func_80134420(void) {
    D_800D9234 = (u8 *)&D_80145EB4;
    D_800D9238 = (u8 *)&D_80145EB8;
    D_800D923C = D_80145EA8;
    D_800D9240 = D_80145EAC;
    D_800D9244 = D_80145EB0;
    func_8008D610(0xFF, 1, 0);
}

void func_80134498(void) {
    D_800D9234 = (u8 *) &D_80145EB4;
    D_800D9238 = (u8 *) &D_80145EB8;
    D_800D923C = D_80145EA8;
    D_800D9240 = D_80145EAC;
    D_800D9244 = D_80145EB0;
    func_8008D610(D_8012E66C, 1, 2);
}
