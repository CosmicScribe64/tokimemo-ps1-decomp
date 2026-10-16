void func_80079B34(void) {
    s32 temp_v1;
    s32 var_v0;
    u16 var_a0;

    var_v0 = func_8007B64C();
    var_a0 = var_v0 & 0xFFFF;
    if (var_v0 != 0) {
        do {
            temp_v1 = var_v0 & 0xF00;
            switch (temp_v1) {
            case 0x0:
                func_8007A080((s32) var_a0);
                break;
            case 0x200:
                func_8007ADD8((s32) var_a0);
                break;
            case 0x300:
                func_8007A6AC((s32) var_a0);
                break;
            case 0x500:
                func_8007B358((s32) var_a0);
                break;
            case 0x600:
                func_8007B3DC(var_a0);
                break;
            case 0x700:
                func_8007B460(var_a0);
                break;
            case 0x800:
                func_8007B4E4(var_a0);
                break;
            }
            var_v0 = func_8007B64C();
            var_a0 = var_v0 & 0xFFFF;
        } while (var_v0 != 0);
    }
    func_80079E4C((s32) var_a0);
    func_8007A008();
    func_8007B640();
}
