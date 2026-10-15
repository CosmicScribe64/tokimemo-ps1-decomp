/* NON_MATCHING: T-10320, seventeen instruction rows differ in saved constants and end-pointer allocation; T-12100 recheck retains the existing 3-argument func_8009B430 view */
void func_8014AF88(void) {
    s32 i;

    func_8009B3C0(0x801F8004);
    for (i = 0; i < 2; i++) {
        func_8009B430(0x801F800C, (u8 *)&D_80122640[i * 4], i);
        D_80122640[i * 4 + 1] = (u32)&D_801227A0[i];
        D_80122640[i * 4] = 0x80000000;
    }
    func_8009B3C0(0x801F9004);
    for (i = 0; i < 9; i++) {
        func_8009B430(0x801F900C, D_80122660 + i * 0x10, 0);
        D_80122640[i * 4 + 9] = (u32)&D_80122840[i];
        D_80122640[i * 4 + 8] = 0x80000000;
    }
}
