/* T-24923 variant E (146 diff rows): pointer-form loops with a `pal` local.
 *
 * Loop 1 compiles instruction for instruction like the target:
 *   addiu v0,v0,4 / sltu at,v0,a0 / addiu v1,v1,4 / bnez / sw t8,0x1638(v0)
 * with v0 = &D_800E6280, a0 = 0x800E6480, v1 = source byte offset, a1 = 0x801CBEF0.
 * The one structural defect: `pal = D_8013D304` makes uopt's loop optimizer hoist
 * `(pal % 4) * 0x200` into the preheader of both loops (target recomputes the
 * signed modulo inside both loops, per copy). Everything else (single load, final
 * reuse of the loaded value) follows from the local.
 */
extern s16 D_801220DC;
extern s16 D_801220DE;

void func_801326BC(void) {
    u8 *p;
    u8 *src;
    s32 i;
    s32 m;
    s32 n;
    s32 pal;

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
        pal = D_8013D304;
        src = (u8 *)0x801CBEF0;
        p = (u8 *)&D_800E6280;
        do {
            *(s32 *)(p + 0x163C) = *(s32 *)(src + (pal % 4) * 0x200 + i);
            p += 4;
            i += 4;
        } while (p < (u8 *)&D_800E6280.unk_1BC[1].unk_0C);
        m = 0;
        if (n - 0x100 >= 0) {
            p = (u8 *)&D_800E6280;
            i = 0;
            do {
                *(s32 *)(p + 0x167C) = *(s32 *)(src + ((pal + 1) % 4) * 0x80 * 4 + i + 0x40);
                *(s32 *)(p + 0x171C) = *(s32 *)(src + ((pal + 1) % 4) * 0x200 + i + 0xE0);
                if (m + 0x60 < 0x80) {
                    *(s32 *)(p + 0x17BC) = *(s32 *)(src + ((pal + 1) % 4) * 0x200 + i + 0x180);
                }
                m += 1;
                i += 4;
                p += 4;
            } while (n - 0x100 >= m);
        }
        D_8013D308 = n;
        if (n >= 0x129) {
            D_8013D304 = pal + 1;
            D_8013D308 = 0;
        }
        load_palette(D_800E6280.unk_163C, 0x1A, 1, 1, 0);
        return;
    }
    D_8013D308 = n;
}
