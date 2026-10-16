void func_8007A080(u16 arg0) {
    s32 var_t9;
    u16 var_a1;
    u32 temp_v0;

    var_a1 = arg0 & 0xFFFF;
    temp_v0 = var_a1 & 0xF0;
    switch (temp_v0) {
    case 0x10:
        func_8007A254(var_a1 & 0xFFFF);
        break;
    case 0x20:
        func_8007A354(var_a1 & 0xFFFF);
        break;
    case 0x70:
        if (var_a1 & 1) {
            arg0 = var_a1;
            func_8007B144(0x71);
            var_a1 = arg0;
        }
        var_t9 = var_a1 & 4;
        if (var_a1 & 2) {
            arg0 = var_a1;
            func_8007B7E0();
            var_t9 = arg0 & 4;
        }
        if (var_t9 != 0) {
            func_8007A98C(0x74);
            func_8007A868();
        }
        break;
    case 0xE0:
        if (var_a1 & 1) {
            arg0 = var_a1;
            func_8007B144(0xE1);
            var_a1 = arg0;
        }
        if (var_a1 & 4) {
            func_8007A98C(0xE4);
            func_8007A868();
        }
        break;
    case 0xF0:
        if (var_a1 & 1) {
            arg0 = var_a1;
            func_8007B144(0xF1);
            var_a1 = arg0;
        }
        if (var_a1 & 4) {
            func_8007A98C(0xF4);
            func_8007A868();
        }
        break;
    case 0xB0:
    case 0xC0:
    case 0xD0:
        if (var_a1 & 1) {
            arg0 = var_a1;
            func_8007B2B4(var_a1 & 0xFFF1);
            var_a1 = arg0;
        }
        if (var_a1 & 4) {
            func_8007AB24(var_a1 & 0xFFF4);
        }
        break;
    default:
        func_8007A43C(var_a1 & 0xFF);
        break;
    }
}
