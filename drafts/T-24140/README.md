# T-24140 scratch (subagent work, 2026-10-19)

## Kept in src/ (production green)
- OPTION `func_80133010` guarded at **21 rows** (`src/ovl/OPTION/80132000.c`,
  `NON_MATCHING` + `INCLUDE_ASM` fallback; production `--version default`
  MATCHes). Proto added to `include/ovl/OPTION.h`.
- OPTION `func_8013A7EC` (`src/ovl/OPTION/8013A550.c`): pre-existing C from
  prior worker, verified MATCH via funcdiff, kept as-is.

## Probe
- `probe_33010.py`: applies edit-forms (single or chained `[old,new]` pairs)
  to the current `func_80133010` body in src/ and scores with
  `funcdiff_verify.compile_unit` (no ninja; safe alongside other builds).
  Usage: `tools/docker.sh python3 drafts/T-24140/probe_33010.py <forms.json>`.
  Saves per-label `.diff` files next to the forms.
- `forms_33010_*.json`: batches tried. Path 89 -> 55 (inline loop masks,
  drop `a1` local) -> 49 (`(u8)` narrow) -> 38/24 raw/aligned (drop `v0`/`v1`
  named locals) -> 21 (narrow store). Dead ends: decl order (inert), `*0`
  boosts, `register`, s2-early, store swaps, index loop, named `t7`.
- Remaining 21: loop-1 hoist order (4 rows), loop-3 `$s1/$s2` swap (6),
  tail `bne` delay `nop` vs `addiu v0,0x80` + target shift (~11).

## Archived drafts (restored to INCLUDE_ASM in src/)
- `geko_80144198.c`: date-rollover helper draft, ~65 rows (arg0 spill,
  temp homing). Struct-copy + `days.day[month-1]` shape is right.
- `geko_80133308_draft.c`: 141 rows (needs named locals for /20 divisor and
  mflo results in $s2-$s5).
- `geko_80132B24_draft.c`: 184 rows (frame 0x60 vs 0x50, spills).
- `m2c_geko_801358CC.c`: raw m2c, needs saved-reg locals + shift idioms.
- T-24002 `func_80139A30` draft re-tested: 223 rows (floor, restored).

## Findings
- IDO 5.3 cfe rejects combined `((u32)(w << K) >> K)` bit-tests when the
  function is preceded by other TU content ("Syntax Error"); split shifts
  through a temp. Bisected; affects GEKO 33308/32B24/358CC drafts.
- GEKO `func_80136C20` (4-row `lui`-in-delay gap): `s32` implicit-int,
  girl-local drop (frame!), volatile load all fail. Guard untouched.
- No dupes fillable for any owned function (`dupes.py -v`: 0).
- Permuter 600s on 33010 never beat score 810 (stuck far from the 21-row
  hand draft); hand probing beat it 89 -> 21.
