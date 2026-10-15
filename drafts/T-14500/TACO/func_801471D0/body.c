/* NON_MATCHING: T-11300, 4 lines: before the second loop the original copies arg0 to $s4 (`move s4,s3; lw s6,4(s4); addiu s4,s4,8`), IDO folds the copy and reads 4(s3); a pointer local for it is folded too (funcdiff also shows one lui row from its HI16 pairing, not a real difference) */
void func_801471D0(s32 arg0) {
    s32 i;
    u32 n;

    arg0 += 4;
    func_8009B3C0(arg0);
    for (i = 0; i < 64; i++) {
        func_8009B430(arg0 + 8, (u8 *)&D_80127080[i], 0);
        D_80127080[i].unk4[0] = (s32)&D_80127480[i];
        D_80127080[i].unk0 = 0x80000000;
    }
    func_8009B3C0(arg0);
    n = *(u32 *)(arg0 + 4);
    for (i = 0; i < n; i++) {
        func_8009B430(arg0 + 8, (u8 *)&D_80127080[i + 1], i);
        D_80127080[i + 1].unk4[0] = (s32)&D_80127480[i + 1];
        D_80127080[i + 1].unk0 = 0x80000000;
    }
}
