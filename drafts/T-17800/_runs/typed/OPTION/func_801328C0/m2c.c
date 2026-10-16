extern s16 D_801220D8;
extern s16 D_801220DA;
extern s16 D_801220DC;
extern s16 D_801220DE;
extern s8 *D_801220E4;
extern s8 D_80122120;
extern s8 D_80122121;
extern s16 D_80122122;
extern s16 D_80122124;
extern ? *D_80122128;
extern ? *D_8012212C;
extern ? D_80122140;
extern ? D_80122540;
extern ? D_8012254A;

void func_801328C0(void) {
    ? *var_s1;
    s16 *var_s2;
    s32 var_s5;
    s32 var_s6;
    void *var_s0;

    D_801220D0 = 0x01000000;
    D_801220D4 = -0xA0;
    D_801220D6 = -0x78;
    D_801220D8 = 0x200;
    D_801220DA = 0x140;
    D_801220DC = 0;
    D_801220DE = 0;
    D_801220E0 = D_800E6280.unk_03A;
    D_801220E1 = D_800E6280.unk_03A;
    D_801220E2 = D_800E6280.unk_03A;
    D_801220E4 = &D_80122120;
    D_801220E8 = 0;
    D_801220EA = 0;
    D_801220EC = 0x1000;
    D_801220EE = 0x1000;
    D_801220F0 = 0;
    D_80122120 = 0x80;
    D_80122121 = 0x40;
    D_80122122 = 4;
    D_80122124 = 5;
    D_80122128 = &D_80122140;
    D_8012212C = &D_80122540;
    var_s5 = 0;
    var_s6 = 0;
    do {
        var_s1 = &D_80122540;
        var_s0 = var_s5 + &D_80122140;
        var_s2 = var_s6 + &D_80122540;
loop_2:
        *var_s2 = 0;
        var_s1 += 2;
        var_s0 += 8;
        var_s2 += 2;
        *(s16 *)((unsigned char *)var_s0 + (-6)) = func_8009ED74(0, 0x1FA);
        *(s8 *)((unsigned char *)var_s0 + (-8)) = 0;
        *(s8 *)((unsigned char *)var_s0 + (-7)) = 0;
        *(s16 *)((unsigned char *)var_s0 + (-2)) = 5;
        *(s16 *)((unsigned char *)var_s0 + (-4)) = 0;
        if (var_s1 != &D_8012254A) {
            goto loop_2;
        }
        var_s5 += 0x20;
        var_s6 += 8;
    } while (var_s5 != 0x80);
    D_800E6280.unk_10EC[0] = 0xD;
}
