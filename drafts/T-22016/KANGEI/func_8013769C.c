void func_8013769C(void) {
    KangeiNames20 sp13C;
    KangeiNames21 sp54;
    s16 var_s0;

    sp13C = D_80139FAC;
    sp54 = D_8013A088;
    D_800E6280.unk_0F6.b[0] = D_800E6280.unk_0F6.b[0] & 0xFFEF;
    D_80139ADC = 0xD;
    D_80139AE0 = 0;
    D_80139AD0 = (KObj *) D_80139A5C[0][6];
    D_80139AD4 = D_80139A5C[1][6];
    D_80139AD8 = D_80139A5C[2][6];
    load_palette(D_80139DEC, 0x10, 1, 0, 0);
    load_palette(D_80139DEC, 0x11, 1, 0, 1);
    func_800AE0F0(D_800CA25C, sp13C.name[(u16) D_800E6280.unk_0F4.h >> 0xC]);
    func_800AE0F0(&D_800CA280, sp54.name[(u16) D_800E6280.unk_0F4.h >> 0xC]);
    func_80048F64(0x60);
    ((DisplayActor44 *)D_8011ECD0)[96].unk_01 = 0xB;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_02 = 0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_38 = 0x01000000;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_03 = 4;
    *(s32 *)&((DisplayActor44 *)D_8011ECD0)[96].unk_0C = (s32) D_80139DD4;
    *(s32 *)&((DisplayActor44 *)D_8011ECD0)[96].unk_10 = (s32) D_80139DDC;
    *(s32 *)&((DisplayActor44 *)D_8011ECD0)[96].unk_34 = (s32) D_80139DCC;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_14 = (s16) D_80139DE4;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_16 = 0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_18 = 0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_06 = 1;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_43 = 0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_26 = -0xA0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_2A = -0x78;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_07 = 0;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_05 = 0x80;
    ((DisplayActor44 *)D_8011ECD0)[96].unk_04 = 0;
    func_800AE120((s32) D_800E6280.unk_10F8);
    ((DisplayActor44 *)D_8011ECD0)[97] = ((DisplayActor44 *)D_8011ECD0)[96];
    ((DisplayActor44 *)D_8011ECD0)[97].unk_02 = 0x40;
    *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_0C = (s32) D_80139DD8;
    *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_10 = (s32) D_80139DE0;
    *(s32 *)&((DisplayActor44 *)D_8011ECD0)[97].unk_34 = (s32) D_80139DD0;
    ((DisplayActor44 *)D_8011ECD0)[97].unk_14 = (s16) D_80139DE8;
    ((DisplayActor44 *)D_8011ECD0)[97].unk_26 = (s16) ((func_800AE0D0() % 320) - 0xA0);
    ((DisplayActor44 *)D_8011ECD0)[97].unk_2A = 0x3C;
    for (var_s0 = 0x62; var_s0 < 0x74; var_s0++) {
        ((DisplayActor44 *)D_8011ECD0)[var_s0] = ((DisplayActor44 *)D_8011ECD0)[97];
        ((DisplayActor44 *)D_8011ECD0)[var_s0].unk_18 = (s16) (func_800AE0D0() % 10);
    }
    for (var_s0 = 0; var_s0 < 0xA; var_s0++) {
        D_80139F38[var_s0] = (func_800AE0D0() % 5) + 1;
        D_80139F10[var_s0].x = (func_800AE0D0() % 320) - 0xA0;
        D_80139F10[var_s0].y = 0x3C;
    }
    func_8004284C();
}
