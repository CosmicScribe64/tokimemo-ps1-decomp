extern ? D_800C6020;
extern s32 D_800C6200;
extern s32 D_800C62C0;
extern ? D_800C6340;

void func_80059EF0(void) {
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
