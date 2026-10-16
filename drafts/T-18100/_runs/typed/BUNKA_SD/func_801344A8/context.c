#include "common.h"
#include "ovl/BUNKA_SD.h"

void func_80134260(void) {
    func_8008585C();
    func_80048E78();
    k_reset(1);
    tpage_buf_clear();
    D_800E6280.unk_036 = 0;
    D_800E6280.unk_038 = 0;
    D_800E6280.unk_037 = 0;
    if (D_80122EB8 == 1) {
        back_clear_switch(0);
    } else {
        back_clear_switch(1);
    }
    draw2d3d(1, 0);
    func_80048DAC(1);
    hizuke_disp_switch(0);
    func_80041584();
    D_8013C700 = get_k_speed();
    func_8004284C();
}

s32 func_80134314(void) {
    if (D_800E6280.unk_110D == 0) {
        func_80044890(1, 0xC478, 0xC44B, 0xD258, 0xD208, 0xD200);
        D_800E6280.unk_110D += 1;
    } else if (D_800E6280.unk_110D == 1) {
        if (func_80044E8C() == 1) {
            D_800E6280.unk_110D += 1;
        }
    } else {
        func_80048390();
        func_80048E78();
        if (D_80122EB8 == 1) {
            func_800573AC();
            func_800438F0(1);
        }
        func_8006492C(1);
        func_8004EBEC(*(u8 *)&D_8013C700);
        func_80042908(0x10);
        func_80044750(0x200);
    }
}

/* NON_MATCHING: T-12090, 26 differing listing rows; selector/base allocation and shared 0x80 constant. */
#ifdef NON_MATCHING
void func_80134418(void) {
    s32 i;
    s32 sel;

    sel = *(u8 *)&D_800E6280.unk_110B;
    for (i = 0; i < 64; i++) {
        D_80120650[i * 0x44 + 7] = sel;
    }
    sel = (u8)(sel + 2);
    D_800E6280.unk_110B = sel;
    if (sel > 0x80) {
        for (i = 0; i < 64; i++) {
            D_80120650[i * 0x44 + 7] = 0x80;
        }
        *(u8 *)&D_800E6280.unk_110B = 0x80;
        func_8004284C();
    }
}
#else
INCLUDE_ASM("asm/ovl/BUNKA_SD/nonmatchings/BUNKA_SD/80134260", func_80134418);
#endif

void func_801344A8(void) {
    DisplayActor44 *var_v0;
    DisplayActor44 *var_v0_2;
    s8 temp_t6;
    u8 var_v1;

    var_v1 = (u8) D_800E6280.unk_110B;
    var_v0 = &((DisplayActor44 *)D_8011ECD0)[96];
    do {
        var_v0 += 1;
        *(u8 *)((unsigned char *)var_v0 + (-61)) = var_v1;
    } while ((u32) var_v0 < (u32) D_80121750);
    if (var_v1 == 0) {
        var_v1 = 2;
    }
    temp_t6 = (var_v1 - 2) & 0xFF;
    D_800E6280.unk0 = temp_t6;
    if (temp_t6 == 0) {
        var_v0_2 = &((DisplayActor44 *)D_8011ECD0)[96];
        do {
            var_v0_2 += 4;
            *(s8 *)((unsigned char *)var_v0_2 + (-197)) = 0;
            *(s8 *)((unsigned char *)var_v0_2 + (-129)) = 0;
            *(s8 *)((unsigned char *)var_v0_2 + (-61)) = 0;
            *(s8 *)((unsigned char *)var_v0_2 + (-265)) = 0;
        } while (var_v0_2 != D_80121750);
        D_800E6280.unk_110B = 0;
        func_8004284C();
    }
}
