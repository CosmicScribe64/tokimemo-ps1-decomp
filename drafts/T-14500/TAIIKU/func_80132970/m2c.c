extern u8 D_800E62BE;
extern u8 D_800E62BF;
extern u8 D_800E62C0;
extern u8 D_800E62C1;
extern u8 D_800E71DE;
extern u8 D_80148EC0;

void func_80132970(void) {
    u8 *var_s1;
    u8 temp_t3;

    var_s1 = &D_80148EC0;
    do {
        D_800E6280.unk_75E[D_800E71DE].unk_00 = *var_s1;
        D_800E6280.unk_75E[D_800E71DE].unk_02 = 0x13;
        D_800E6280.unk_75E[D_800E71DE].unk_01 = 0;
        D_800E6280.unk_75E[D_800E71DE].unk_03 = func_80051B48(*var_s1);
        if ((var_s1 == (u8 *)0x80148EC2) && (D_800E6280.unk_1BC[*var_s1].unk_0A >= 0x4C)) {
            D_800E6280.unk_75E[D_800E71DE].unk_03 = 4;
        }
        D_800E6280.unk_75E[D_800E71DE].unk_04 = D_800E62BE;
        D_800E6280.unk_75E[D_800E71DE].unk_05 = D_800E62BF;
        D_800E6280.unk_75E[D_800E71DE].unk_06 = D_800E62C0;
        D_800E6280.unk_75E[D_800E71DE].unk_07 = D_800E62C1;
        temp_t3 = D_800E71DE + 1;
        D_800E71DE = temp_t3;
        var_s1 += 1;
        if ((u32) (temp_t3 & 0xFF) >= 0xFFU) {
            D_800E71DE = 0xFE;
        }
    } while (var_s1 != (u8 *)0x80148EC3);
}
