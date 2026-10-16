/* T-16560: the redraft with the post-decrement in the call argument; 18 rows remain, all the
 * placement of `move a1,zero` (the original fills the branch delay slot with it). Not in src. */
void func_80133870(void) {

    D_800E6280.unk_1104.u += 1;
    switch (D_800E6280.unk_110A) {
    case 0:
        func_80133980();
        break;
    case 1:
        func_80133AF4();
        break;
    case 2:
        func_80133C08();
        break;
    case 3:
        func_80133E98();
        break;
    case 4:
        func_80134500();
        break;
    }
    if (D_800E6280.unk_1104.u & 0x10) {
        func_801432F0((s32) "NOW LOADING DATA ", -0x60, 0, 1);
    }
    if ((D_800E6280.unk_1104.u != 0) && ((u32) D_800E6280.unk_1104.u < 0x400U)) {
        func_801430D0(D_8015EDEC--, 0, 0x32, 6);
    }
}
