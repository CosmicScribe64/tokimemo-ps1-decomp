#include "common.h"
#include "game.h"

void func_80059B40(void) {
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80059E00();
        return;
    case 1:
        func_80059BC0();
        return;
    case 2:
        func_8004ADE4();
        func_8004284C();
        return;
    default:
        func_80042808();
        return;
    }
}

void func_80059BC0(void) {
    func_80059BE8();
    func_8004284C();
}

INCLUDE_ASM("asm/nonmatchings/main/80059B40", func_80059BE8);

/* FAKE: u8 views give the original's global access (LOD): `andi`/`or v0` copy of the increment; the s32 return keeps that copy live (T-5010) */
s32 func_80059E00(void) {
    if (*(u8 *)&D_800E6280.unk_110D == 0) {
        printf("LDVI");
        back_clear_switch(1);
        func_80041168(0xFF);
        cd_read_sectors(0x35, 0x80180000, 0x9D85);
        *(u8 *)&D_800E6280.unk_110D += 1;
    } else if (*(u8 *)&D_800E6280.unk_110D == 1) {
        if (cd_read_sync_status() & 1) {
            func_80059EF0();
            func_800462C8(8, 0x801F0000, 0x9C97);
            *(u8 *)&D_800E6280.unk_110D += 1;
        }
    } else if (*(u8 *)&D_800E6280.unk_110D == 2 && (cd_read_sync_status() & 1)) {
        func_8004284C();
    }
}

/* bulk_m2c declarations (T-11190) */
extern s32 D_800C6020;
extern s32 D_800C6200;
extern s32 D_800C62C0;
extern s32 D_800C6340;
/* end bulk_m2c declarations */
s32 func_80059EF0(void) {
    s32 *var_v1;
    s32 *var_v1_2;
    s32 temp_a1;
    s32 temp_t1;
    s32 temp_t7;
    s32 var_s4;
    s32 var_v0;
    s32 var_v0_2;

    var_s4 = 0;
    do {
        var_v1 = &D_800C62C0;
loop_2:
        var_v0 = *var_v1;
        temp_t7 = var_v0 | 0x80000000;
        if (var_v0 & 0x7FFF0000) {
            *var_v1 = temp_t7;
            var_v0 = temp_t7;
        }
        if (var_v0 & 0x7FFF) {
            *var_v1 = var_v0 | 0x8000;
        }
        var_v1 += 4;
        if ((u32) var_v1 < (u32) &D_800C6340) {
            goto loop_2;
        }
        var_v1_2 = &D_800C6200;
loop_8:
        var_v0_2 = *var_v1_2;
        temp_t1 = var_v0_2 | 0x80000000;
        if (var_v0_2 & 0x7FFF0000) {
            *var_v1_2 = temp_t1;
            var_v0_2 = temp_t1;
        }
        if (var_v0_2 & 0x7FFF) {
            *var_v1_2 = var_v0_2 | 0x8000;
        }
        var_v1_2 += 4;
        if (var_v1_2 != D_800C6220) {
            goto loop_8;
        }
        temp_a1 = var_s4 & 0xFF;
        load_palette((s32) &D_800C6020, temp_a1, 1, 1, /* extra? */ temp_a1);
        var_s4 += 1;
    } while (var_s4 != 0x10);
    hizuke_init();
    hizuke_disp_switch(0);
    parameter_show_init();
    parameter_show_init();
    parameter_disp_switch(0);
    func_8006509C();
    icon_disp_switch(0);
    message_window_init();
    message_disp_switch(0);
    func_800673B8();
    func_8006764C(0);
    func_80065F34(0);
}

void func_8005A06C(void) {
    if (D_800E6280.unk_1109 == 0) {
        func_80059B40();
        return;
    }
    main_state_set(0x12);
}
