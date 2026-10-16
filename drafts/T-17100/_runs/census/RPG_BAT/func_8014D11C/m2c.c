s32 func_800AE100();
void func_8014D11C(void *arg0, void *arg1, s32 *arg2, s32 arg3, s32 *arg4) {
    s32 temp_lo;
    s32 var_s1;
    u32 temp_hi;

    func_800AE0F0(arg0, arg1);
    func_800AE100(arg0, arg2);
    if (arg3 == 0) {
        func_800AE100(arg0, "０");
    }
    var_s1 = 0x3E8;
    do {
        temp_lo = arg3 / var_s1;
        if (temp_lo != 0) {
            temp_hi = (u32) temp_lo % 10U;
            switch (temp_hi) {
            case 0:
                func_800AE100(arg0, "０");
                break;
            case 1:
                func_800AE100(arg0, "１");
                break;
            case 2:
                func_800AE100(arg0, "２");
                break;
            case 3:
                func_800AE100(arg0, "３");
                break;
            case 4:
                func_800AE100(arg0, "４");
                break;
            case 5:
                func_800AE100(arg0, "５");
                break;
            case 6:
                func_800AE100(arg0, "６");
                break;
            case 7:
                func_800AE100(arg0, "７");
                break;
            case 8:
                func_800AE100(arg0, "８");
                break;
            case 9:
                func_800AE100(arg0, "９");
                break;
            }
        }
        var_s1 = var_s1 / 10;
    } while (var_s1 > 0);
    func_800AE100(arg0, arg4);
}
