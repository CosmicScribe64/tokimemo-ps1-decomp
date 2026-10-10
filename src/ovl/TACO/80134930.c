#include "common.h"
#include "ovl/TACO.h"

void func_80134930(void) {
    D_800E6280.unk_037 = 0;
    D_800E6280.unk_036 = 0;
    D_800E6280.unk_038 = 0;
    draw2d3d(1, 1);
    initView();
    func_80146D60(-0xFA0);
    initModelingData_init(0x8019C800);
    func_801471D0(0x8019C800);
    func_80147488(1, D_8015F3E0, 0);
    D_80127090.unk0 = 0;
    D_80128880[1].x = 0x32;
    D_80128880[1].z = 0x32;
    D_80128880[1].y = 0xD2;
    func_80147488(2, D_8015F3E4, 0);
    D_801270A0.unk0 = 0;
    D_80128880[2].x = 0x4B;
    D_80128880[2].z = -0x32;
    D_80128880[2].y = 0xC3;
    D_8015F3E8 = 0;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80134930", func_80134A18);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80134930", func_80134D8C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80134930", func_80134EBC);

void func_80134FB8(void) {
    func_80048F64(1);
    D_8011ED15 = 8;
    D_8011ED4C = 0;
    D_8011ED16 = 0x40;
    D_8011ED17 = 0x84;
    D_8011ED20 = D_800E6280.unk_1A4C;
    D_8011ED24 = D_800E6280.unk_1A5C;
    D_8011ED48 = D_800E6280.unk_1A3C;
    D_8011ED18 = 8;
    D_8011ED19 = 0x81;
    D_8011ED57 = 0xF;
    D_8011ED3A = -0xA0;
    D_8011ED3E = -0x64;
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80134930", func_8013506C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80134930", func_8013515C);

void func_80135220(void) {
    if ((u32)D_800E6280.unk_1104.w >= 0xD1U) {
        func_8004284C();
    } else if (D_800E6280.unk_F88 & 0x860) {
        func_80042940(5);
    }
}

void func_80135278(void) {
    if (func_8013515C() == 0) {
        func_8004284C();
    } else if (D_800E6280.unk_F88 & 0x860) {
        func_80042940(5);
    }
}

void func_801352CC(void) {
    if ((u32)D_800E6280.unk_1104.w < 0xFFU) {
        if (D_800E6280.unk_F88 != 0) {
            func_80042940(5);
            return;
        }
        func_8004AC18(0xFF - D_800E6280.unk_1104.w);
        return;
    }
    func_8004284C();
}

void func_80135330(void) {
    func_8004284C();
}

void func_80135350(void) {
    func_80059048();
    load_palette(D_800E6280.unk_1A6C, 0x1F, 1, 1, 0);
    func_80042908(3);
}

void func_80135394(void) {
    D_800E6280.unk_1104.w += 1;
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80135418();
        break;
    case 1:
        func_8013546C();
        break;
    case 2:
        func_80135638();
        break;
    }
    func_8013506C();
}
void func_80135418(void) {
    func_8009C210(0);
    func_80059048();
    func_800591D8(0);
    func_80134FB8();
    D_8011ED1B = 0x40;
    D_8011ED3E = -0x78;
    func_8004284C();
}

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80134930", func_8013546C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80134930", func_8013555C);

INCLUDE_ASM("asm/ovl/TACO/nonmatchings/TACO/80134930", func_80135638);
