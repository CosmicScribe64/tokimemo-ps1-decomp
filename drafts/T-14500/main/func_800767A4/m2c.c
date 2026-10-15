extern u8 D_800B69BC;
extern s8 D_800E69A4;
extern s8 D_800E69A5;
extern s8 D_800E69A6;
extern s8 D_800E69A7;

void func_800767A4(void) {
    s32 sp190;
    s32 sp18C;
    s32 sp188;
    ? spFC;
    u8 spBC;
    u8 sp7C;
    u8 sp3C;
    s32 temp_hi;
    s32 var_v1_2;
    s32 var_v1_3;
    u8 *var_a0;
    u8 *var_a1;
    u8 *var_v1;

    var_v1 = &sp3C;
    var_a1 = &sp7C;
    var_a0 = &spBC;
    do {
        var_a0 += 4;
        *(s8 *)((unsigned char *)var_v1 + (1)) = 0;
        *(s8 *)((unsigned char *)var_a1 + (1)) = 0;
        *(s8 *)((unsigned char *)var_a0 + (-3)) = 0;
        *(s8 *)((unsigned char *)var_v1 + (2)) = 0;
        *(s8 *)((unsigned char *)var_a1 + (2)) = 0;
        *(s8 *)((unsigned char *)var_a0 + (-2)) = 0;
        *(s8 *)((unsigned char *)var_v1 + (3)) = 0;
        *(s8 *)((unsigned char *)var_a1 + (3)) = 0;
        *(s8 *)((unsigned char *)var_a0 + (-1)) = 0;
        var_v1 += 4;
        var_a1 += 4;
        *(s8 *)((unsigned char *)var_v1 + (-4)) = 0;
        *(s8 *)((unsigned char *)var_a1 + (-4)) = 0;
        *(s8 *)((unsigned char *)var_a0 + (-4)) = 0;
    } while (var_a0 != &spFC);
    if (func_800764AC((s32) &sp108[0]) >= 3) {
        temp_hi = rand() % 3;
        var_v1_2 = temp_hi;
        if (temp_hi < 0) {
            var_v1_2 = 0;
        }
        sp188 = (&sp108[0])[var_v1_2 % 3];
        sp18C = (&sp108[0])[(s32) (var_v1_2 + 1) % 3];
        D_800E69A4 = 2;
        sp190 = (&sp108[0])[(s32) (var_v1_2 + 2) % 3];
        x_taku_menu_set(1, 3);
        strcpy(&spBC, "『");
        strcat(&spBC, D_800B342C[sp188]);
        strcat(&spBC, &D_800B69BC);
        strcpy(&sp7C, "『");
        strcat(&sp7C, D_800B342C[sp18C]);
        strcat(&sp7C, &D_800B69BC);
        strcpy(&sp3C, "『");
        strcat(&sp3C, D_800B342C[sp190]);
        strcat(&sp3C, &D_800B69BC);
        x_taku_string_set((s32) &spBC, (s32) &sp7C, (s32) &sp3C, 3);
    } else {
        var_v1_3 = rand() & 1;
        if (var_v1_3 < 0) {
            var_v1_3 = 0;
        }
        sp188 = (&sp108[0])[var_v1_3 % 2];
        sp190 = 0xFF;
        D_800E69A4 = 1;
        sp18C = (&sp108[0])[(s32) (var_v1_3 + 1) % 2];
        x_taku_menu_set(1, 2);
        strcpy(&spBC, "『");
        strcat(&spBC, D_800B342C[sp188]);
        strcat(&spBC, &D_800B69BC);
        strcpy(&sp7C, "『");
        strcat(&sp7C, D_800B342C[sp18C]);
        strcat(&sp7C, &D_800B69BC);
        x_taku_string_set((s32) &spBC, (s32) &sp7C, 0, 2);
    }
    D_800E69A5 = (s8) sp188;
    D_800E69A6 = (s8) sp18C;
    D_800E69A7 = (s8) sp190;
}
