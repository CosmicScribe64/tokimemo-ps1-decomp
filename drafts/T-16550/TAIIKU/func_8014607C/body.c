void func_8014607C(void) {
    TaiikuController *var_v0;
    s16 var_v0_2;
    s16 var_v1;
    s32 temp_v1;

    D_8014A3E4.unk_00 = 0;
    D_8014A3E4.unk_08 = 0;
    D_8014A3E4.unk_0C = 0;
    D_8014A3E4.unk_04 = 0;
    D_8014A3E4.position.h[0] = 0;
    D_8014A3E4.unk_14 = 0;
    D_8014A3E4.unk_01 = 0;
    D_8014A3E4.unk_02 = 0;
    var_v0 = D_8014A400;
    var_v1 = 8;
    do {
        var_v0->position.h[1] = var_v1;
        var_v1 += 8;
        var_v0 += 1;
        *(s32 *)((unsigned char *)var_v0 + (-16)) = 0;
        *(s32 *)((unsigned char *)var_v0 + (-24)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (-28)) = 0;
        *(s32 *)((unsigned char *)var_v0 + (-8)) = 0;
        *(s16 *)((unsigned char *)var_v0 + (-4)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (-27)) = 0;
    } while (var_v1 < 0x20);
    *D_8014A3D8 = 0;
    D_8014A454->unk_18 = 0;
    D_8014A454->unk_02 = 0x37;
    D_8014A454->unk_06 = 0x19;
    D_8014A454->unk_08 = 0;
    D_8014A454->unk_14 = 0;
    D_8014A454->unk_16 = 0;
    D_8014A454->unk_0C = 0;
    D_8014A454->unk_0D = 0;
    D_8014A454->unk_0E = 0;
    D_8014A454->unk_0F = 0;
    D_8014A3D9 = 0;
    D_8014A454[1].unk_18 = 0;
    D_8014A454[1].unk_06 = 0x19;
    D_8014A454[1].unk_02 = 0x37;
    D_8014A454[1].unk_08 = 0;
    D_8014A454[1].unk_14 = 0;
    D_8014A454[1].unk_16 = 0;
    D_8014A454[1].unk_0C = 0;
    D_8014A454[1].unk_0D = 0;
    D_8014A454[1].unk_0E = 0;
    D_8014A454[1].unk_0F = 0;
    D_8014A3DA = 0;
    D_8014A454[2].unk_18 = 0;
    D_8014A454[2].unk_02 = 0x37;
    D_8014A454[2].unk_06 = 0x19;
    D_8014A454[2].unk_08 = 0;
    D_8014A454[2].unk_14 = 0;
    D_8014A454[2].unk_16 = 0;
    D_8014A454[2].unk_0C = 0;
    D_8014A454[2].unk_0D = 0;
    D_8014A454[2].unk_0E = 0;
    D_8014A454[2].unk_0F = 0;
    D_8014A3DB = 0;
    D_8014A454[3].unk_18 = 0;
    D_8014A454[3].unk_06 = 0x19;
    D_8014A454[3].unk_02 = 0x37;
    D_8014A454[3].unk_08 = 0;
    D_8014A454[3].unk_14 = 0;
    D_8014A454[3].unk_16 = 0;
    D_8014A454[3].unk_0C = 0;
    D_8014A454[3].unk_0D = 0;
    D_8014A454[3].unk_0E = 0;
    D_8014A454[3].unk_0F = 0;
    D_8014A3E4.position.h[1] = -0xA0;
    D_8014A3E0 = 0;
    D_8014A3DC = 0;
    temp_v1 = D_800E6280.unk_0F4.b[1] & 0xF;
    var_v0_2 = D_800E6280.unk_10C.unk_02;
    D_8014A3C4 = 0;
    if ((temp_v1 & 1) || (temp_v1 & 2)) {
        var_v0_2 = (s16) (var_v0_2 >> 1);
    }
    if (var_v0_2 >= 0xA0) {
        D_8014A3D0 = 0x280;
        D_8014A3D4 = 0x20000;
    } else if (var_v0_2 >= 0x7D) {
        D_8014A3D0 = 0x220;
        D_8014A3D4 = 0x1E000;
    } else if (var_v0_2 >= 0x64) {
        D_8014A3D0 = 0x1C0;
        D_8014A3D4 = 0x1C000;
    } else if (var_v0_2 >= 0x4B) {
        D_8014A3D0 = 0x160;
        D_8014A3D4 = 0x1A000;
    } else {
        D_8014A3D0 = 0x100;
        D_8014A3D4 = 0x18000;
    }
    func_800AE120(D_800E6280.unk0);
}
