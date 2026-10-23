# T-25139: KANGEI func_801327DC (matched)

Result: literal default-C match, 1,096 bytes (274 words). The body lives in
`src/ovl/KANGEI/80132090.c`; no draft body is kept here.

## Start state

`INCLUDE_ASM`. Best known body: `drafts/T-24516/kangei-327dc.c` (T-24516: 270 rows, cost 264).
Under the current tools it measured 270 rows with `funcdiff --resolve --version c`.

## First divergences and the change that closed each

Rows are `funcdiff --resolve --version c` row counts after each step.

1. **Halfword reads in ugen registers (270 rows).** The original reads the club halfword
   `D_800E6280.unk_0F4.h` (0x800E6374) as `lui v1; lhu v1` at every read: before the first
   call, at the first switch, and in cases 0, 3 and 4. The draft had `lhu a1; srl t6; move a1,t6`
   and `lhu t7`. regdiff: no uopt range held the load, so it was a ugen temporary. The global is
   read only through `>> 12`, so `tools/cvt_pass.py` keeps the widening CVT of every read, and uopt
   sees no plain use of the variable. Fix: empty truth tests `if (D_800E6280.unk_0F4.h) {}` (FAKE).
   A truth test gets no CVT, so it is a plain use. With them, the global is one range
   (10 uses in the trace) coloured `$v1`, as in the original. Measured: no test, 175 rows; any
   single test, 180 to 197 rows; any two of the five tried placements, 0 rows (all 10 pairs);
   all five, 0 rows. The source keeps two, before the first call and before the first switch.
   Spellings that do not help: `/ 0x1000`, `(u32)`, `(s32)` (gives `sra`), `(u16)(h >> 12)`
   (adds an `andi`), and a scalar `extern u16 D_800E6374`.
2. **Default arm `lui a0`/`addiu a0` hoisted into the first switch head (205 rows).** as1 moved
   the default arm's `D_800CA19C` address into the `lhu` stall and the range-check delay slot. The
   original has `nop` there and the dispatch `sll` in the slot. This is the T-25150 rule 8 case
   (wiki/as1-scheduler-rule.md). The object's `.data` table `D_801399B0` names this function and
   seven others of the file, so it has to be defined in C. Done with
   `tools/data_island.py KANGEI/80132090` (island 0x7950..0x7A50 in config/overlays/KANGEI.yaml)
   and a C initializer for `D_801399B0`. The other 28 words stay `INCLUDE_RODATA`
   (`func_80132090` stores to each `D_80139950` word through its own `lui $at`, so defining that
   array in C could change it). KANGEI.bin stayed byte-identical. Control: the final body with the
   table back as `INCLUDE_RODATA` gives 29 rows, all from this hoist.
3. **Flag tests `andi 4; bnez` instead of `sll 29; bltz` (176 rows).** Use the bit-fields
   `unk_0C.f.b2` and `unk_0C.f.b19` (as main `func_8005B43C`). `((s32)(w << 29) >> 31) == 0`
   gives the same code (150 rows either way).
4. **D_800CA148 not promoted (150 rows).** The original keeps `D_800CA148` in `$v1` across each
   case (`move v1,zero` / `li v1,k` then `sh v1`, constants stored as `sh zero`, case 6 reloads
   with `lh v1` after the call). The increments are `addiu t4,v1,1` with one ring slot spent
   before. Fix: drop m2c's `var_v1` locals. Assign the global itself in every arm and write the
   increments as `D_800CA148 += 1` / `+= 2`. Give the default arm its own `D_800CA148 = 0`
   (no shared `goto block_44`). 150 to 17 rows.
5. **Last 17 rows.** (a) Case 0's chain has `b L_A30; li v1,4` before the join. This is an
   `else if (club == 7) ... else D_800CA148 = 6;` chain, not `= 6; if (== 7) = 4;`. (b) Case 6:
   `andi t1`/`addiu t3` against `t0`/`t2` in the draft, one ring slot short. Fix:
   `((u8) get_g_zyotai_s(6) & 0x7F) < 2` (FAKE: as1 deletes the `andi 0xFF`, but it spent the
   slot). Both together: MATCH.

## Final checks

`funcdiff --resolve --version c` MATCH, then guard removed and `--version default` MATCH
(1,096 linked bytes). Clean `rm -rf build && configure && ninja`: 27/27 sha1 OK. check_headers,
migrate_globals --check and sync_protos --check-branch pass.

## FAKEs in the source

- Two empty tests of `D_800E6280.unk_0F4.h` (range priority of the halfword global).
- `(u8)` on the `get_g_zyotai_s(6)` result (one ugen ring slot).

## Negatives added

- One empty halfword test alone is not enough (180 to 197 rows). Two are needed; the placement
  does not matter.
- Spelling the halfword read differently (`/ 0x1000`, `(u32)`, `(s32)`, `(u16)(x >> 12)`, scalar
  `D_800E6374`) changes nothing or adds instructions.
- The table definition is required (29 rows without it).
