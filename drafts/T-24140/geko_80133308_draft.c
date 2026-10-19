/* T-24140 attempt at GEKO func_80133308: m2c baseline + types, unscored-ok but
 * 141 diff rows (register allocation: original keeps div operands/quotients in
 * $s2-$s5, draft uses temps; needs named locals for the /20 divisor and mflo
 * results, cf. matching-notes "constants in saved registers"). Restored to
 * INCLUDE_ASM in src/; continue here.
 *
 * TOOLCHAIN NOTE (important!): IDO 5.3 cfe rejects the combined
 * `((u32)(w << 0x1E) >> 0x1F)` bit-test when the function is preceded by other
 * content in the TU ("Syntax Error" even with a lone preceding blank line or
 * `void h(void){}`). Splitting the shifts through a temp (`tmp = w << 0x1E;
 * ... ((u32)tmp >> 0x1F) ...`) compiles. Bisected 2026-10-19; minimal trigger
 * unknown, likely cfe parser quirk. Any future draft MUST keep the shifts split.
 */
void func_80133308(void) {
    s16 i;
    s32 tmp;
    s32 v0;
    s32 k;

    v0 = func_80051A68(D_800E6280.unk_F5F);
    if (D_80122CDC != 0) {
        D_80144C54 = 0;
        tmp = (D_800E6280.unk_10F8 % (((D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_02 / 20) & 0xFF) + 1)) & 0xFF;
        if ((u32) (v0 & 0x7F) < 2U) {
            D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0A += tmp + 0xA;
        } else {
            D_800E6280.unk_1BC[D_800E6280.unk_F5F].unk_0A += tmp + 6;
        }
        D_80144C3C += 2;
    } else {
        D_80144C54 = 1;
        if ((D_800E6280.unk_F5F == 0) || (D_800E6280.unk_F5F == 7) || (D_800E6280.unk_F5F == 1) || (D_800E6280.unk_F5F == 2) || (D_800E6280.unk_F5F == 8) || (k = 4, (D_800E6280.unk_F5F == 9))) {
            k = 0;
            if ((u32) (v0 & 0x7F) >= 3U) {
                D_80144C54 = 0;
            } else {
                k = 4;
            }
        }
        i = 0;
        do {
            tmp = D_800E6280.unk_1BC[i].unk_0C.w << 0x1E;
            if ((((u32) tmp >> 0x1F) != 0) && (i != D_800E6280.unk_F5F)) {
                tmp = ((s16) D_800E6280.unk_1BC[i].unk_02 / 20) & 0xFF;
                D_800E6280.unk_1BC[i].unk_0A += k + ((func_800AE0D0() % (tmp + 1)) & 0xFF);
            }
            i += 1;
        } while (i < 0xB);
    }
    func_80084D3C();
    func_8004284C();
}
