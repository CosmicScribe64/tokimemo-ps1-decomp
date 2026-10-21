/* T-24923 best draft, variant D (149 diff rows with funcdiff --resolve).
 *
 * What this body gets right (verified in the compiled NON_MATCHING unit):
 *  - `D_8013D304` is read directly in the loop expressions. uopt hoists the load
 *    once to before the first loop (one `lw`), keeps it in one register, promotes
 *    the final `D_8013D304 += 1` from that value, and leaves the signed `% 4`
 *    (bgez/andi/beqz/addiu -4) inside both loops, recomputed per copy, exactly as
 *    the target does. This was the key finding of the run.
 *  - The one symbol-based destination store form keeps the load alive across the
 *    loop stores (a pointer-based store `*(s32 *)(p + 0x163C)` makes uopt treat
 *    the stores as aliasing and it reloads D_8013D304 three times in loop 2).
 *
 * What is still wrong: the target's induction pointer. The target walks
 * v0 = &D_800E6280 + i (v0 += 4, `sltu v0,a0`, store at 0x1638(v0) with the
 * +0x163C folded into the post-increment displacement), i.e. a strength-reduced
 * pointer compared against 0x800E6480. uopt here keeps the base in a register
 * and recomputes `base + i` per iteration (`addu v0,a3,v1`), and adds one
 * redundant `addu` in the branch delay slot. The p-form draft (body_pform.c)
 * reproduces the target's loop 1 instruction for instruction but hoists the
 * `% 4` out of both loops, because the row comes from a local (`pal`).
 */
extern s16 D_801220DC;
extern s16 D_801220DE;

void func_801326BC(void) {
    u8 *src;
    s32 i;
    s32 m;
    s32 n;

    if (D_8013D300 == 0) {
        return;
    }
    D_801220E0 = D_800E6280.unk_03A;
    D_801220E1 = D_800E6280.unk_03A;
    D_801220E2 = D_800E6280.unk_03A;
    if (D_800E6280.unk_10F8 & 1) {
        D_801220DC += 1;
        D_801220DE -= 1;
    }
    i = 0;
    n = D_8013D308 + 1;
    if (n >= 0x101) {
        src = (u8 *)0x801CBEF0;
        i = 0;
        do {
            *(s32 *)((u8 *)&D_800E6280 + 0x163C + i) = *(s32 *)(src + (D_8013D304 % 4) * 0x200 + i);
            i += 4;
        } while ((u8 *)&D_800E6280 + i < (u8 *)&D_800E6280.unk_1BC[1].unk_0C);
        m = 0;
        if (n - 0x100 >= 0) {
            i = 0;
            do {
                *(s32 *)((u8 *)&D_800E6280 + 0x167C + i) = *(s32 *)(src + ((D_8013D304 + 1) % 4) * 0x80 * 4 + i + 0x40);
                *(s32 *)((u8 *)&D_800E6280 + 0x171C + i) = *(s32 *)(src + ((D_8013D304 + 1) % 4) * 0x200 + i + 0xE0);
                if (m + 0x60 < 0x80) {
                    *(s32 *)((u8 *)&D_800E6280 + 0x17BC + i) = *(s32 *)(src + ((D_8013D304 + 1) % 4) * 0x200 + i + 0x180);
                }
                m += 1;
                i += 4;
            } while (n - 0x100 >= m);
        }
        D_8013D308 = n;
        if (n >= 0x129) {
            D_8013D304 += 1;
            D_8013D308 = 0;
        }
        load_palette(D_800E6280.unk_163C, 0x1A, 1, 1, 0);
        return;
    }
    D_8013D308 = n;
}
