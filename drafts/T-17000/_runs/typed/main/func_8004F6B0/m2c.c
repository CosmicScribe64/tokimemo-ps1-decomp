extern s16 D_80125BF0;
extern s16 D_80125BF2;

void func_8004F6B0(void) {
    s16 var_t0;
    s16 var_v1;
    s32 temp_a0;
    s32 temp_a1;
    s32 temp_v0;

    D_800E6280.unk_F88 = 0x80000;
    temp_v0 = D_800E6280.unk_F80;
    var_v1 = 0;
    var_t0 = 0;
loop_1:
    if (temp_v0 & 0x10000000) {
        var_t0 -= 0x10;
    }
    if (temp_v0 & 0x40000000) {
        var_t0 += 0x10;
    }
    if (temp_v0 & 0x20000000) {
        var_v1 += 0x10;
    }
    if (temp_v0 & 0x80000000) {
        var_v1 -= 0x10;
    }
    D_80125BF0 = var_v1;
    if (var_v1 < 0) {
        D_80125BF0 = 0;
    }
    D_80125BF2 = var_t0;
    if (var_t0 < 0) {
        D_80125BF2 = 0;
    }
    func_80042A00();
    func_80098380();
    temp_a1 = D_80125BF2 & 0xFFFF;
    temp_a0 = D_80125BF0 & 0xFFFF;
    func_80098490(temp_a0, temp_a1, temp_a0 & 0xFFFF, temp_a1 & 0xFFFF);
    func_800422C8();
    func_8009C674(0);
    func_800AD950(0);
    func_8009AC20();
    if (!(D_800E6280.unk_F80 & 0x08000000)) {
        var_t0 = D_80125BF2;
        var_v1 = D_80125BF0;
        goto loop_1;
    }
    func_80097D90((s32) D_800E6280.unk_000, (s32) D_800E6280.unk_002, (s32) D_800E6280.unk_004, 0, 0);
    func_80098380();
    func_80098490(0, 0, 0, (s32) D_800E6280.unk_002);
    func_80098530();
}
