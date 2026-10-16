extern s32 D_800B3D10;
extern s32 D_800B3D14;
extern s32 D_800B3D18;
extern s32 D_800B3D1C;
extern u8 D_800E62B6;
extern u8 D_800E62B7;
extern u8 D_800E62B8;
extern u8 D_800E62B9;
extern ? D_800E62C0;
extern u8 D_800E7322;
extern s32 D_800E7368;
extern s32 D_800E7370;
extern u8 D_800E7388;
extern u8 D_800E7389;
extern u8 D_800E7392;
extern u8 D_800E7393;
extern u8 D_800E7394;

void func_80043510(s32 arg0) {
    ? sp70;
    ? sp50;
    GameState *var_s0;
    GameState *var_s0_2;
    Rec24 *var_s2;
    s32 var_s1;
    u32 *var_s0_3;
    u8 temp_v0;
    u8 temp_v0_2;

    D_800B3D14 = func_80098650();
    func_80049044();
    if (D_800E7392 == 1) {
        func_800476C0();
    }
    if (D_800E7322 != 0) {
        srn_set(0);
    }
    if (D_800E7394 != 0) {
        var_s2 = D_801217D0;
        if (D_800E7368 != 0) {
            var_s0 = &D_800E6280;
            do {
                temp_v0 = var_s0->unk_10A5[0];
                if (temp_v0 & 0x80) {
                    func_80098680((s32) var_s2, (s32) (&D_800E8C70 + (D_8011ECA0 * 0x14)), (temp_v0 - 1) & 0x7F);
                } else {
                    func_80098B80((s32) var_s2, (s32) (&D_800E8C70 + (D_8011ECA0 * 0x14)), (temp_v0 - 1) & 0x7F);
                }
                var_s0 += 1;
                var_s2 += 0x24;
            } while ((u32) var_s0 < (u32) &D_800E62C0);
            disp_string();
            var_s1 = 0;
            if (D_800E7368 != 0) {
                var_s0_2 = &D_800E6280;
                do {
                    temp_v0_2 = var_s0_2->unk_10EC[0];
                    if (temp_v0_2 < 0x80U) {
                        func_80098D00((s32) (&D_801220D0 + (var_s1 * 0x24)), (s32) (&D_800E8C70 + (D_8011ECA0 * 0x14)), (temp_v0_2 - 1) & 0xFFFF);
                    }
                    var_s1 += 1;
                    var_s0_2 += 1;
                } while (var_s1 != 2);
            }
        }
    }
    if (D_800E7393 != 0) {
        func_80099540(&D_80122740);
        var_s0_3 = D_80122640;
        do {
            func_80099B10(*(s32 *)((unsigned char *)var_s0_3 + (4)), (s32) &sp50, (s32) &sp70);
            func_80099F04((s32) &sp50);
            func_8009A200(*(s32 *)((unsigned char *)var_s0_3 + (4)), (s32) &sp70);
            func_80099ED8((s32) &sp70);
            func_8009A4D0((s32) var_s0_3, (s32) (&D_800E8C70 + (D_8011ECA0 * 0x14)), 6, 0x1F800000);
            var_s0_3 += 0x10;
        } while (var_s0_3 != &D_80122740);
        func_8005938C();
    }
    if (arg0 != 0) {
        D_800B3D18 = func_80098650();
        csr_load_vram(0);
        palette_load_vram(0);
        if ((D_800E7388 != 0x90) || ((u8) D_800E7389 < 0xAU) || (D_800E7389 == 0xC)) {
            func_8009C674(0);
        }
        D_800B3D1C = func_80098650();
        if ((D_800E7388 == 0x90) && ((u8) D_800E7389 >= 0xAU)) {
            func_800AD950(2);
            func_8009C210(1);
        } else if (func_80098650() < D_800E7370) {
            func_800AD950(0);
        }
        D_800B3D10 = func_80098650();
        func_80042A00();
    }
    func_8009C210(1);
    func_8009ABF0();
    func_8009AC20();
    if (D_800E62B9 != 0) {
        func_8009824C((s32) D_800E62B6, (s32) D_800E62B7, (s32) D_800E62B8, (s32) (&D_800E8C70 + (D_8011ECA0 * 0x14)));
    }
    func_8009AD00((s32) (&D_800E8C70 + (D_8011ECA0 * 0x14)));
}
