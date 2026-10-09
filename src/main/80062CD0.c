#include "common.h"
#include "game.h"

void func_80062CD0(s32 arg0) {
    func_80046318(0x2D, 0x80180000, arg0);
    D_8011F50F = 0;
    D_8011F4CB = 0;
    D_800B5A60 = 0;
}

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80062D0C);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80062DBC);

void func_800634FC(void) {
    if ((u32)D_800B5A64 >= 0xA5) {
        D_800B5A64 = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80063520);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80063668);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", bustup_speech);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", bustup_wink);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", parameter_change);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", parameter_disp_switch);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", parameter_show_init);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", parameter_show);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", hizuke_disp_switch);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", hizuke_init);

void hizuke_show(void) {
    if (D_8011F113 & 0x80) {
        func_80049A40(0x86, -0x4B, 0x14, 0x72, 5, 0x30E00, 0);
        dtd_on(5);
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", message_disp_switch);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", message_window_init);

void message_window_show(void) {
    if (D_8011ED17 & 0x80) {
        func_80049A40(-0x85, 0x2E, 0x10A, 0x34, 8, 0x30E00, 0);
        dtd_on(8);
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", icon_disp_switch);

void icon_can_use_set(s32 idx, s32 on) {
    if (on) {
        D_80125C90[idx] = 1;
    } else {
        D_80125C90[idx] = 0;
    }
}

s32 get_icon_can_use(s32 arg0) {
    if (D_80125C90[arg0] == 1) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_8006509C);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_800654D0);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80065900);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80065964);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80065B0C);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80065F34);

void func_80066104(void) {
    func_8006612C();
    D_80125CA0 = 0xB8;
}

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_8006612C);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80066334);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80066A2C);

s32 func_80066A68(void) {
    return 1;
}

s32 func_80066A70(void) {
    return 2;
}

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80066A78);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80066A84);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80066AC0);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80066ACC);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80066B40);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80066C08);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_800673B8);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80067438);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_800674B0);

void func_80067610(void) {
    if (func_80044E8C() == 1) {
        func_8004500C(0, 0);
        func_80042808();
    }
}

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_8006764C);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_800676AC);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80067870);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80067C28);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80067DD4);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80067DFC);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80067E34);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80067F04);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80068898);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_800688F0);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80068938);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80068BE4);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_80068EC0);

void cal_base_show(void) {
    func_80049A40(-0xA0, -0x57, 0x11B, 0x7D, 5, 0xE0E0E0, 2);
    dtd_on(5);
}

void appraisal_base_show(void) {
    func_80049A40(-0xA0, -0x68, 0x11B, 0x8E, 5, 0xE0E0E0, 2);
    dtd_on(5);
}

void magazine_base_show(void) {
    func_80049A40(-0xA0, -0x78, 0x140, 0x9E, 5, 0xE0E0E0, 2);
    dtd_on(5);
}

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", cal_sprite_disp_switch);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", cal_sprite_init);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", data_save_load_class_init);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_800696DC);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_8006A044);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_8006A2CC);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_8006AEC4);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_8006B014);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_8006B0C8);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_8006B5BC);

void func_8006B640(void) {
}

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_8006B648);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_8006B900);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_8006BA40);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_8006BC28);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_8006BD6C);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_8006BDA8);

void func_8006C308(void) {
    func_8006C334(1, 1, 1, 2);
}

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_8006C334);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_8006C700);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_8006C760);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_8006C848);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_8006C934);

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_8006C988);

u8 *func_8006CA9C(void) {
    if (D_800E71F4 == 0) {
        if (D_800E71F5 != 0) {
            return D_800B672C;
        }
        return D_800B6730;
    }
    return D_800B6724;
}

INCLUDE_ASM("asm/nonmatchings/main/80062CD0", func_8006CAE0);
