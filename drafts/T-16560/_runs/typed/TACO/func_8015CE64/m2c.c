s32 func_8015CE64(void) {
    TcActor *temp_v0;
    TcActor *var_a2;
    s32 var_a0;
    s32 var_v1;

    var_v1 = 0;
    temp_v0 = D_8015EDB4 + 0x880;
    var_a0 = 0xA18;
    var_a2 = D_8015EDB4 + 0xA18;
    if (temp_v0->unk0[1] & 0x80) {
        var_v1 = 1;
    }
    if (*(u8 *)((unsigned char *)temp_v0 + (137)) & 0x80) {
        var_v1 += 1;
    }
    if (*(u8 *)((unsigned char *)temp_v0 + (273)) & 0x80) {
        var_v1 += 1;
    }
    do {
        var_a0 += 0x220;
        if (var_a2->unk0[1] & 0x80) {
            var_v1 += 1;
        }
        if (*(u8 *)((unsigned char *)var_a2 + (137)) & 0x80) {
            var_v1 += 1;
        }
        if (*(u8 *)((unsigned char *)var_a2 + (273)) & 0x80) {
            var_v1 += 1;
        }
        if (*(u8 *)((unsigned char *)var_a2 + (409)) & 0x80) {
            var_v1 += 1;
        }
        var_a2 += 0x220;
    } while (var_a0 != 0x2178);
    return var_v1;
}
