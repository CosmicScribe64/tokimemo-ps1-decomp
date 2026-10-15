? func_8004E58C();                                  /* extern */
? func_800570B8(s32, ?, s32, ?);                    /* extern */
? func_8006C760(?);                                 /* extern */
? k_disp_start(s32);                                /* extern */
? k_disp_switch(s32, ?, ?, ?);                      /* extern */
extern s32 D_800B58F8;
extern s32 D_800B5900;
extern u8 D_800B5904;
extern ? D_800B5908;
extern ? D_800B590D;
extern ? D_800B5919;
extern u8 D_800E7D35;

void func_8005715C(s32 arg0) {
    s32 sp34;
    u32 sp30;                                       /* compiler-managed */
    s32 temp_v0;
    s32 var_v0;
    u8 *var_v1;

    var_v1 = &D_800B5904;
    var_v0 = 0;
loop_1:
    if (arg0 >= (s32) ((u32) (*var_v1 * 0x50) / 15U)) {
        var_v0 += 1;
        var_v1 += 1;
        if (var_v0 != 0x16) {
            goto loop_1;
        }
    }
    if (var_v0 != 0) {
        sp34 = var_v0;
        sp30 = (u32) var_v1;
        k_disp_switch(var_v0 - 1, 0, 0x50, 0xF);
        k_disp_start(var_v0 + 1);
    }
    if (((u32) var_v1 >= (u32) &D_800B5908) && ((u32) var_v1 < (u32) &D_800B590D)) {
        sp30 = var_v1;
        func_8006C760(0);
    } else {
        sp30 = var_v1;
        func_8006C760(1);
    }
    if ((u32) var_v1 >= (u32) &D_800B5919) {
        func_8006C760(0);
        func_800570B8(0, 1);
        func_8004E58C();
        return;
    }
    if (D_800B58F8 != 0) {
        temp_v0 = ((u32) (*(u32) var_v1 * 0x50) / 15U) - arg0;
        if ((temp_v0 == 4) || (temp_v0 == 5)) {
            func_800570B8(((D_800B5900 + D_800E7D35) * 0xDF7FCF) + 0xFF208031, 1, 0x50, 0xF);
            return;
        }
        func_800570B8(((D_800B5900 + D_800E7D35) * 0xDF7FCF) + 0xFF208031, 0x81, 0x50, 0xF);
    }
}
