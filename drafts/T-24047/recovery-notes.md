# T-24047 source recovery (no compiler run)

Read original `func_80135F4C.s`, T-20930 body, ENDING family page, and coding standards. No production edits or compilation.

## Definite loop facts

All three loops use a signed 16-bit induction variable. Original resets s1 at 80136100, 801361B8, 801364F8, and explicitly sign-extends after every increment. A single `s16 i` used in ordinary `for (i = 0; i < N; i++)` loops is the simplest plausible source. Three separate m2c counters, plus the final `temp_s1`/`temp_at_2` comparison temporary, express register snapshots rather than source structure.

First loop (80136120-36198): three Rec24 records 16..18, old i used for every field, x = (i << 7) - 160, y = -188. Separate menu byte is `D_800E6280.unk_10A5[i + 16] = 13`. The multiplication/addition computes D_801217D0 + 36*i; member stores have constant +0x240 offsets, so preserve the idea of the same i plus constant record index, rather than walking an incremented pointer or introducing a second record counter. All fields in the existing draft are semantically correct.

Second loop (801361C0-362D0): fourteen display records 96..109, each reset through func_80048F64(i+96), then populated. Compute/retain a display pointer after this call, not across it. Three separate signed random remainders are `% 5`, `% 7`, `% 60`. The pointer is D_8011ECD0 + 68*i, and member stores carry +0x1980 offsets. The two stack tables are indexed by i, and the final store is `D_8013CA1C[i] = func_800AE0D0() % 60`. Do not create the final halfword destination pointer before rand: original materializes D_8013CA1C's high address after the last call (362B0), while retaining 2*i in s2 from the coordinate lookups (36254). The T-20930 `temp_at = &D_8013CA1C[i]; i++; *temp_at = rand()%60` needlessly extends an address across the call and makes the increment part of the body. Also delete the unused `temp_s2` declaration/assignment.

Third loop (80136514-36608): twelve Rec24 records 4..15. `unk_0F = 128 - ((i / 6) << 7)`, `unk_0C = i % 6 + (i / 6) * 16 + 5`, x = ((i % 3) << 7)-160, y = ((i / 3) << 7)-392; menu byte `unk_10A5[i+4] = 11`. Correct source uses old i for all fields and the menu byte, increments once at loop end. No need for `temp_s1`, `temp_at_2`, temp x/y snapshots, or explicit assignment of the saved increment. Preserve the source store ordering of the original draft, which agrees with asm, while letting scheduling put independent calculations early.

## Strong candidate source-lifetime correction

The original retains 128 in s3 from 8013610C through the later texture transfers and third loop. Every later texture height is `andi a3,s3,0xFFFF`; transfers to y=128 also use `andi a1,s3,0xFFFF` (e.g. 80136304/3630C). Plain `0x80 & 0xFFFF` in T-20930 is a constant expression, so it cannot itself preserve a source variable lifetime. A plausible source experiment is one `s16 size = 128` reused for Rec24 width/height and RGB fields and for these LoadSquare y/height arguments. First three LoadSquare heights should use the same size as well. This is a hypothesis about original source, not proof: IDO may create common constant registers on its own.

The third loop uses signed `div s1,a0` with a0=6 and `div s1,a1` with a1=3, including complete runtime zero/overflow checks, unlike random remainder divisions using literal divisors in at. This suggests ordinary scalar divisor locals for the 6-column and 3-column calculations, or compiler hoisting of repeated literal expressions. Before adding locals, try clean repeated `/ 6`, `% 6`, `/ 3`, `% 3`; if divisor-register setup remains wrong, a bounded plausible follow-up is initialized scalar columns=6 and columns=3. Do not force registers or retain m2c machine temporaries.

## Other definite draft cleanup

`DisplayActor44.unk_0C`, `.unk_10`, `.unk_34` already are `u8 *`: replace casts through `s32 *` with ordinary pointer-field assignments using suitable pointer conversions from EdScene30's scalar address fields. The location-record halfword is already s16. `D_8013C620` is now an array of actual EdScene44 records, so `D_8013C620[0]` is an explicit readable first record. The guarded source should use the existing GameState menu array, not byte casts to whole GameState.

No numerical match improvement is claimed. This note identifies source-recovery leads; the parent's compiler pass must decide their effect.
