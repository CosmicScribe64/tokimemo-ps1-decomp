void func_800FAA04(void) {
    s16 temp_s1;
    s16 var_s1;
    s32 temp_hi;
    void *temp_s0;

    func_800F9900();
    func_80012D64(D_80120C00, 0x11, 1, 2, 0);
    func_8004C46C(D_80120C08, D_80120C0C, D_80120C10, D_80120C14, D_80120C18, D_80120C1C);
    func_8004C6B0(D_80120BF4, D_80120BF8, D_80120BF0, (s32) D_80120BFC);
    func_80012D64(D_80120C04, 0x12, 1, 3, 0);
    var_s1 = 4;
    do {
        temp_s0 = D_800EC190 + (var_s1 * 0x24);
        temp_s0->unk0 = 0xC1000000;
        temp_hi = func_800789A0() % 320;
        *(s16 *)((unsigned char *)temp_s0 + (6)) = -0x78;
        *(s16 *)((unsigned char *)temp_s0 + (8)) = 8;
        *(s16 *)((unsigned char *)temp_s0 + (10)) = 8;
        *(s16 *)((unsigned char *)temp_s0 + (12)) = 5;
        *(s8 *)((unsigned char *)temp_s0 + (14)) = 0;
        *(s8 *)((unsigned char *)temp_s0 + (15)) = 0;
        *(s16 *)((unsigned char *)temp_s0 + (16)) = 0;
        *(s16 *)((unsigned char *)temp_s0 + (18)) = 0x1F2;
        *(s8 *)((unsigned char *)temp_s0 + (20)) = 0;
        *(s8 *)((unsigned char *)temp_s0 + (21)) = 0;
        *(s8 *)((unsigned char *)temp_s0 + (22)) = 0;
        *(s16 *)((unsigned char *)temp_s0 + (24)) = 0;
        *(s16 *)((unsigned char *)temp_s0 + (26)) = 0;
        *(s16 *)((unsigned char *)temp_s0 + (28)) = 0x1000;
        *(s16 *)((unsigned char *)temp_s0 + (4)) = (s16) (temp_hi - 0x9F);
        *(s16 *)((unsigned char *)temp_s0 + (30)) = 0x1000;
        *(s32 *)((unsigned char *)temp_s0 + (32)) = 0;
        D_800B188D[var_s1] = 0xC;
        *(&D_80120CE8 + (var_s1 * 2)) = 0;
        temp_s1 = var_s1 + 1;
        D_80120D08[var_s1] = 0;
        var_s1 = temp_s1;
    } while (temp_s1 < 0x13);
    func_80011DFC();
}
