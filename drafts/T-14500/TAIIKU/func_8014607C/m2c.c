extern u8 D_800E6375;
extern s16 D_800E638E;
extern s32 D_800E7374;
extern s8 D_8014A3D9;
extern s8 D_8014A3DA;
extern s8 D_8014A3DB;
extern ? D_8014A3E5;
extern s32 D_8014A3E8;
extern ? D_8014A3FC;
extern ? D_8014A456;
extern ? D_8014A468;
extern s16 D_8014A46C;
extern ? D_8014A476;
extern ? D_8014A47C;
extern ? D_8014A484;
extern s16 D_8014A488;
extern s16 D_8014A48E;
extern ? D_8014A492;
extern ? D_8014A498;
extern ? D_8014A4A0;
extern s16 D_8014A4A4;
extern ? D_8014A4AE;
extern ? D_8014A4B0;
extern s16 D_8014A4C0;

void func_8014607C(void) {
    s16 var_v0_2;
    s16 var_v1;
    s32 temp_v1;
    u8 *var_v0;

    D_8014A3E4.unk_00 = 0;
    D_8014A3E4.unk_08 = 0;
    D_8014A3F0 = 0;
    D_8014A3E8 = 0;
    D_8014A3FC.unk0 = 0;
    *(s32 *)((unsigned char *)(&D_8014A3FC) + (-4)) = 0;
    D_8014A3E5.unk0 = 0;
    *(s16 *)((unsigned char *)(&D_8014A3E5) + (1)) = 0;
    var_v0 = D_8014A400;
    var_v1 = 8;
    do {
        *(s16 *)((unsigned char *)var_v0 + (26)) = var_v1;
        var_v1 += 8;
        var_v0 += 0x1C;
        *(s32 *)((unsigned char *)var_v0 + (-16)) = 0;
        *(s32 *)((unsigned char *)var_v0 + (-24)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (-28)) = 0;
        *(s32 *)((unsigned char *)var_v0 + (-8)) = 0;
        *(s16 *)((unsigned char *)var_v0 + (-4)) = 0;
        *(s8 *)((unsigned char *)var_v0 + (-27)) = 0;
    } while (var_v1 < 0x20);
    *D_8014A3D8 = 0;
    D_8014A46C = 0;
    D_8014A456.unk0 = 0x37;
    *(s16 *)((unsigned char *)(&D_8014A456) + (4)) = 0x19;
    *(s32 *)((unsigned char *)(&D_8014A456) + (6)) = 0;
    D_8014A468.unk0 = 0;
    *(s16 *)((unsigned char *)(&D_8014A468) + (2)) = 0;
    *(s8 *)((unsigned char *)(&D_8014A468) + (-8)) = 0;
    *(s8 *)((unsigned char *)(&D_8014A468) + (-7)) = 0;
    *(s8 *)((unsigned char *)(&D_8014A468) + (-6)) = 0;
    *(s8 *)((unsigned char *)(&D_8014A468) + (-5)) = 0;
    D_8014A3D9 = 0;
    D_8014A488 = 0;
    D_8014A476.unk0 = 0x19;
    *(s16 *)((unsigned char *)(&D_8014A476) + (-4)) = 0x37;
    *(s32 *)((unsigned char *)(&D_8014A476) + (2)) = 0;
    D_8014A484.unk0 = 0;
    *(s16 *)((unsigned char *)(&D_8014A484) + (2)) = 0;
    D_8014A47C.unk0 = 0;
    *(s8 *)((unsigned char *)(&D_8014A47C) + (1)) = 0;
    *(s8 *)((unsigned char *)(&D_8014A47C) + (2)) = 0;
    *(s8 *)((unsigned char *)(&D_8014A47C) + (3)) = 0;
    D_8014A3DA = 0;
    D_8014A4A4 = 0;
    D_8014A48E = 0x37;
    D_8014A492.unk0 = 0x19;
    *(s32 *)((unsigned char *)(&D_8014A492) + (2)) = 0;
    D_8014A4A0.unk0 = 0;
    *(s16 *)((unsigned char *)(&D_8014A4A0) + (2)) = 0;
    D_8014A498.unk0 = 0;
    *(s8 *)((unsigned char *)(&D_8014A498) + (1)) = 0;
    *(s8 *)((unsigned char *)(&D_8014A498) + (2)) = 0;
    *(s8 *)((unsigned char *)(&D_8014A498) + (3)) = 0;
    D_8014A3DB = 0;
    D_8014A4C0 = 0;
    D_8014A4AE.unk0 = 0x19;
    *(s16 *)((unsigned char *)(&D_8014A4AE) + (-4)) = 0x37;
    D_8014A4B0.unk0 = 0;
    *(s16 *)((unsigned char *)(&D_8014A4B0) + (12)) = 0;
    *(s16 *)((unsigned char *)(&D_8014A4B0) + (14)) = 0;
    *(s8 *)((unsigned char *)(&D_8014A4B0) + (4)) = 0;
    *(s8 *)((unsigned char *)(&D_8014A4B0) + (5)) = 0;
    *(s8 *)((unsigned char *)(&D_8014A4B0) + (6)) = 0;
    *(s8 *)((unsigned char *)(&D_8014A4B0) + (7)) = 0;
    D_8014A3FE = -0xA0;
    D_8014A3E0 = 0;
    D_8014A3DC = 0;
    temp_v1 = D_800E6375 & 0xF;
    var_v0_2 = D_800E638E;
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
    func_800AE120(D_800E7374);
}
