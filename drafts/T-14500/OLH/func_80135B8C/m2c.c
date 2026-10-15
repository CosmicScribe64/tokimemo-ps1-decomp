extern ? D_8013CEBC;

void func_80135B8C(void) {
    s16 spA4;
    s16 sp94;
    s16 sp84;
    s16 sp74;
    s16 *var_s0;
    s16 *var_s1;
    s16 *var_s4;
    s16 *var_s5;
    s16 var_s3;
    s32 temp_v0;
    u8 **var_s2;

    func_8004EAAC();
    func_8004E788(-0x50, -0x40, 0xF, (s32) "ミニ知識", 0);
    var_s2 = &D_8013CEA0;
    var_s0 = &spA4;
    var_s1 = &sp94;
    var_s3 = -0x32;
    var_s4 = &sp84;
    var_s5 = &sp74;
    do {
        *var_s0 = -0x60;
        *var_s1 = var_s3;
        *var_s4 = 0xE8;
        *var_s5 = 0x10;
        temp_v0 = func_8004E788(*var_s0, *var_s1, 0xF, (s32) *var_s2, 0);
        var_s2 += 4;
        var_s0 += 2;
        var_s1 += 2;
        var_s3 += 0x10;
        var_s4 += 2;
        var_s5 += 2;
    } while (var_s2 != &D_8013CEBC);
    func_8004F870(0, 7, &spA4, &sp94, &sp84, &sp74);
    func_8004E884(temp_v0);
    func_8004284C();
}
