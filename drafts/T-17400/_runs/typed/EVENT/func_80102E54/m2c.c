extern s16 D_800B0B0A;
extern s16 D_800B0B12;

void func_80102E54(void) {
    DisplayActor44 *temp_v0;
    s16 temp_s0;
    s16 var_s0;
    s16 var_s0_2;
    s32 temp_at;
    s32 temp_v1;
    s8 *temp_t2;
    s8 *temp_t7;

    D_80120678 = D_80120450;
    D_8012067C = D_8012046C;
    D_80120680 = D_80120488;
    D_800B0B0A += 2;
    *D_800B0B0E += 1;
    D_800B0B12 -= 0x14;
    func_8004C250();
    func_80102610();
    func_80012D64(D_801226E8, 0x11, 1, 2, 0);
    func_8004C46C(D_801226EC, D_801226F0, D_801226F4, D_801226F8, D_801226FC, D_80122700);
    func_8004C6B0(D_801226DC, D_801226E0, D_801226D8, (s32) D_801226E4);
    *(s8 *)((unsigned char *)D_800E9620 + (6532)) = 8;
    *(s8 *)((unsigned char *)D_800E9620 + (6600)) = 8;
    var_s0 = 0;
    do {
        temp_t7 = &D_8012274C + var_s0;
        var_s0 += 1;
        *temp_t7 = 0;
    } while (var_s0 < 8);
    var_s0_2 = 0;
    do {
        temp_v1 = var_s0_2 * 2;
        temp_v0 = &D_800E9620[var_s0_2];
        M2C_MEMCPY_ALIGNED(temp_v0 + 0x1A08, D_800E9620 + 0x1980, 0x44);
        *(s16 *)((unsigned char *)temp_v0 + (6686)) = 5;
        *(s16 *)((unsigned char *)temp_v0 + (6702)) = (s16) (*(&D_80122754 + temp_v1) - 0xA0);
        *(s16 *)((unsigned char *)temp_v0 + (6706)) = (s16) (*(&D_80122764 + temp_v1) - 0x78);
        temp_t2 = &D_80122774 + var_s0_2;
        temp_s0 = var_s0_2 + 1;
        temp_at = temp_s0 < 8;
        var_s0_2 = temp_s0;
        *temp_t2 = (s8) (func_800789A0() % 120);
    } while (temp_at != 0);
    *(s16 *)((unsigned char *)D_800E9620 + (6686)) = 4;
    func_80011DFC();
}
