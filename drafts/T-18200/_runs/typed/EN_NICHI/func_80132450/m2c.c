? func_80048514(?, s32, ?);                         /* extern */

void func_80132450(void) {
    DisplayActor44 *var_s0;
    DisplayActor44 *var_s0_2;
    s32 var_s1;

    func_80048514(5, D_80139204->unk0, 1);
    func_80048514(0x25, *(s32 *)((unsigned char *)D_80139204 + (4)), 1);
    func_80048514(6, *(s32 *)((unsigned char *)D_80139204 + (8)), 1);
    func_80048514(7, 0x801BA000, 1);
    func_80048514(8, 0x801BA000, 1);
    func_80048514(9, 0x801BA000, 1);
    func_80048514(0x27, 0x801BA000, 1);
    func_80048514(0x28, 0x801BA000, 1);
    func_80048514(0x29, 0x801BA000, 1);
    var_s0 = D_8011ECD0;
    var_s1 = 0;
    do {
        func_80048F64(var_s1 + 0x60);
        *(s8 *)((unsigned char *)var_s0 + (6529)) = 7;
        *(s32 *)((unsigned char *)var_s0 + (6584)) = 0x01000000;
        *(s8 *)((unsigned char *)var_s0 + (6535)) = 0;
        *(s8 *)((unsigned char *)var_s0 + (6530)) = 0x40;
        *(s8 *)((unsigned char *)var_s0 + (6531)) = 4;
        *(s8 *)((unsigned char *)var_s0 + (6534)) = 1;
        *(s16 *)((unsigned char *)var_s0 + (6566)) = -0xA0;
        *(s16 *)((unsigned char *)var_s0 + (6570)) = -0x78;
        *(s8 *)((unsigned char *)var_s0 + (6533)) = 0;
        *(s8 *)((unsigned char *)var_s0 + (6595)) = 0x11;
        *(s8 *)((unsigned char *)var_s0 + (6532)) = 0;
        *(s32 *)((unsigned char *)var_s0 + (6540)) = (s32) D_80139208;
        *(s32 *)((unsigned char *)var_s0 + (6544)) = (s32) D_8013920C;
        var_s1 += 1;
        var_s0 += 0x44;
        *(s32 **)((unsigned char *)var_s0 + (6512)) = (s32 *) D_80139204;
    } while (var_s1 < 0x40);
    *(s8 *)((unsigned char *)D_8011ECD0 + (10475)) = 0x80;
    *(s16 *)((unsigned char *)D_8011ECD0 + (10496)) = 0x22;
    *(s8 *)((unsigned char *)D_8011ECD0 + (10407)) = 0x80;
    *(s16 *)((unsigned char *)D_8011ECD0 + (10428)) = 0x20;
    *(s8 *)((unsigned char *)D_8011ECD0 + (10337)) = 3;
    *(s8 *)((unsigned char *)D_8011ECD0 + (10339)) = 0x84;
    *(s16 *)((unsigned char *)D_8011ECD0 + (10360)) = 0x1F;
    *(s16 *)((unsigned char *)D_8011ECD0 + (10374)) = (s16) D_8011ECD0->unk_26;
    var_s0_2 = D_8011ECD0 + 0x1980;
    *(s16 *)((unsigned char *)D_8011ECD0 + (10378)) = (s16) D_8011ECD0->unk_2A;
    do {
        var_s0_2 += 0x110;
        *(s8 *)((unsigned char *)var_s0_2 + (-203)) = 3;
        *(s16 *)((unsigned char *)var_s0_2 + (-180)) = 0x2E;
        *(s8 *)((unsigned char *)var_s0_2 + (-135)) = 3;
        *(s16 *)((unsigned char *)var_s0_2 + (-112)) = 0x2E;
        *(s8 *)((unsigned char *)var_s0_2 + (-67)) = 3;
        *(s16 *)((unsigned char *)var_s0_2 + (-44)) = 0x2E;
        *(s8 *)((unsigned char *)var_s0_2 + (-271)) = 3;
        *(s16 *)((unsigned char *)var_s0_2 + (-248)) = 0x2E;
    } while (var_s0_2 != (D_8011ECD0 + 0x1ED0));
    *(s8 *)((unsigned char *)D_8011ECD0 + (10541)) = 2;
    *(s16 *)((unsigned char *)D_8011ECD0 + (10564)) = 0x2D;
    *(s32 *)((unsigned char *)D_8011ECD0 + (10596)) = 0x51000000;
    *(s8 *)((unsigned char *)D_8011ECD0 + (10607)) = 0x1E;
    *(s8 *)((unsigned char *)D_8011ECD0 + (9793)) = 2;
    *(s16 *)((unsigned char *)D_8011ECD0 + (9830)) = 0x10;
    *(s16 *)((unsigned char *)D_8011ECD0 + (9834)) = -8;
    *(s8 *)((unsigned char *)D_8011ECD0 + (9861)) = 2;
    *(s16 *)((unsigned char *)D_8011ECD0 + (9898)) = 0;
    *(s16 *)((unsigned char *)D_8011ECD0 + (9902)) = -8;
    *(s8 *)((unsigned char *)D_8011ECD0 + (9929)) = 2;
    *(s16 *)((unsigned char *)D_8011ECD0 + (9966)) = 0x10;
    *(s16 *)((unsigned char *)D_8011ECD0 + (9970)) = 0x18;
    *(s8 *)((unsigned char *)D_8011ECD0 + (9997)) = 2;
    *(s16 *)((unsigned char *)D_8011ECD0 + (10034)) = 0;
    *(s16 *)((unsigned char *)D_8011ECD0 + (10038)) = 0x18;
}
