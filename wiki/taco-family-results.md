---
type: research
updated: 2026-10-16
---

# TACO boss actor transform family

Bounded diagnostic pass [[tickets/T-13800-taco-families]] / [[tickets/T-13801-taco-actor-family]], based on 97ec697f. **Zero accepted matches or bytes.** Production source and headers remain identical to the baseline. All tools ran in the existing tokimemo-decomp-arm64 Docker image, sequential compiles.

## Recovered source structure

The adjacent boss transforms func_8014A480 (796 bytes), func_801497EC (912), and func_801487A4 (948) share a MATRIX copy from D_80125C60, three rotation calls using D_80128880[0], a TcObj50 translation converted through TcPos input/output, then TcActor position/selector updates. Existing aggregate layouts suffice: m2c's split matrix stores and byte-offset actor fields need no new types.

Use the canonical D_80127480[0].unk18/unk1C/unk20 members for the common origin. Actor slots use arg1 + 16. Cached actor pointers, selector locals, and pair-index locals can alter the register web and stack homes even where their loads or additions are identical. In 497EC the selector advancement also writes D_8015EDB4[17].unk84[2]. Its final three dependent-object loops start at 4, 5 and 6 and advance by 3 to 22, 23 and 24 respectively.

## Measured boundaries

| Function | Best full-TU difference | Remaining issue |
| --- | ---: | --- |
| func_8014A480 | 3 rows | Final actor-base addition commuted; constant versus address `lui` scheduling pair |
| func_801497EC | 1 row | Final actor-base addition commuted (target base + index; candidate index + base) |
| func_801487A4 | 62 rows | Natural control flow correct, but register allocation and local layout still differ |

For 497EC, two loop-counter declarations before the position/index scalars recover the observed unused local slots. Reuse the second loop counter for the third loop; removing the cached selector then gives the exact frame and spill homes. This is diagnostic source reconstruction, not a verified match. The final direct array reread instead folds a load, changes the register web and shrinks the frame by eight bytes.

For 4A480, removing the cached pair index and selector gives the observed 128-byte frame. Mixed pointer-addition source forms recover all but the three rows above. Signed/unsigned casts, byte-address and integer-address final reads, xor-zero and line-grouping trials do not solve the residual. Do not repeat these variants without a new compiler/lifetime hypothesis.

The scratch-only named comparisons at D_801274D0, D_80127520, D_80127570, D_80127610 and D_801277F0 require canonical main declarations if a future match is accepted. None were added to production; no shared header expansion is justified by an unsuccessful candidate. The temporary forward declaration for existing overlay function 5163C was also removed.

## Reproduction and evidence

Isolated checkout: /Users/lila/Documents/Projects/tokimemo-wt/taco-13800, branch codex/taco-13800. Ignored scratch build/t13801 contains candidate bodies, full resolved diffs, bounded experiment scripts, and reprobe.py. From that checkout run `docker run --rm -v "$PWD:/work" -w /work tokimemo-decomp-arm64 python3 build/t13801/reprobe.py`. It compiles each complete translation unit and restores its original source in a finally block. Temporary declarations in this diagnostic probe are not production-approved header changes. Run the default overlay build again afterward because its objects may contain the last trial.

Best evidence: a480-storeorders.diff and 97ec-nocache.diff. Broader failures remain in the same ignored directory. No generated assembly was edited; no source guard or FAKE was installed. The actual 87A4 control flow was reconstructed from assembly because m2c's compound else-if incorrectly grouped the state-29 test with subsequent flag conditions.

## Final validation and review

The saved reprobe script reproduces the one-row and three-row results without compile errors. The restored default TACO build then exits 0 with SHA1 OK, 1419e93cbad6c0df790577701181be04e0747969; header/global checks pass. Evidence: build/t13801/final-default.log and final-verified.log. No whole-game rebuild/checksum claim is made. Independent standards review found no issues. Spec review confirms the primary matching goal is unmet; both tickets remain In Review for integration disposition. Source/header diff against 97ec697f is empty.

## Types-first pass (T-16560)

Pilot of the types-first idea in [[speedup-research-2026-10-16]]: recover TACO's shared types first, then
redraft every unmatched function ([[tickets/T-16560-taco-types-first]], branch `pt-taco`). The drafts and the
driver are in `drafts/T-16560/` (README there).

### Type map

| Type / global | Layout and evidence |
| --- | --- |
| `TcActor` (0x88), 79 records at `TcActor *D_8015EDB4` | `unk0[2]` (byte 1: flag bits 0x80, 0x40), `unk2`, `unk3`; `Tc10 unk4` at +0x04 (object handle); `TcObj50 unk14` at +0x14 (coordinate object: `func_8013801C` passes `actor + 0x14` to `func_80099E30`); s16 `unk64`..`unk82`; `u8 unk84[4]`. The old `pad4[0x60]` had no users. `unk74`..`unk7A` and `unk7C`..`unk82` are copied as two 8-byte `TcPos` (lwl/lwr pairs) into by-value arguments; they stay s16 members because 245 matched lines read them one by one (310 uses). |
| `TcObj50` (0x50), `D_80127480[64]` and `actor->unk14` | GsCOORDINATE2 layout: flag word `unk0`, matrix rotation `s16 unk4[3][3]`, translation `unk18`/`unk1C`/`unk20` (kept as scalars, matched code reads them so), world `MATRIX unk24`, `unk44`, super/sub links `unk48`/`unk4C`. |
| `Tc10` (0x10), `D_80127080[64]` and `actor->unk4` | GsDOBJ2 layout: attribute (`0x80000000` hides it), `unk4[0]` the coordinate object, model, id; filled by `func_8009B430` (GsLinkObject4). |
| `Tc14` (0x14), nine 16-record tables `D_8015E270`..`D_8015EC70` | same record as `TacoParticle14`: two `TcPos` (copied whole with lwl/lwr), then four bytes. Holes filled (`unk6`, `unkE`, `unk10`, `unk11`). `D_8015EC70` is the ninth table (`Tc14[16]`), also the end address of the walks over `D_8015EB30`. |
| `MATRIX D_80129C40` (main bss, in `main_api.h`, which now includes `libgte.h`) | copied whole to the stack in 13 TACO functions; only TACO uses it. |

Not changed: the `TcPos` view of actor +0x74/+0x7C (245 matched lines would change); the
pair tables `D_8015FEF0`/`D_8015FFE0` and their siblings (matched code indexes them as `s32[]`/`u8[]` with
`i * 2 - 2`; they are `TcPair8`/`TcPair2` records read at `i - 1`); the `D_8015EDB0`..`D_8015EDF0`
scalars (type_recovery's 0x44-byte struct proposal rests on two ordered pairs only; not audited); the GPU packets from `GetWorkBase`, whose
shared prototype returns `s32`.

### Draft rewrites the types made possible

`drafts/T-16560/redraft.py` scores two drafts per function through the T-14500 census path: `plain` (m2c
with the new headers, nothing else) and `typed` (plain plus these rewrites), then `tools/draft_fixes.py`.

- **pointer-step**: m2c prints pointer steps in bytes for every pointer type: `Tc14 *p; p += 0x14`,
  `s16 **q; q += 4`, `s32 *r; r += 8`. In C that is 20, 4 and 8 elements. Written as element steps.
  46 chosen drafts. This m2c defect is not TACO-specific: a detector in `tools/draft_fixes.py` would help
  every unit.
- **actor-offset**: `*(u8 *)((unsigned char *)D_8015EDB4 + (1090))` is `D_8015EDB4[8].unk2`; the same for
  the other typed tables. A store keeps no signedness cast (`*(s8 *)&...` on a byte store changes the
  schedule; dropping it matched `func_8013822C`).
- **matrix-copy**: eight word copies from `D_80129C40`, `D_80125C60`, a stack matrix or a coordinate
  object, in any order: one struct copy (`m = *(MATRIX *)D_80125C60;`, `*(MATRIX *)o->unk4 = m;`).
  Up to 67 rows less per function.
- **tcpos-args**: m2c splits a by-value `TcPos` into two words and loses the source of the register
  words (`subroutine_arg2`). The asm names the source of each copy (stack local, global, or the register
  m2c named `temp_s0`); `func_8015ACCC(3, 0x21, D_801604D4, sp50, sp48, 8, 0)` with `TcPos sp50, sp48`.
  `func_80159878` 595 to 147 rows, `func_801563E4` 435 to 108, `func_80159260` 308 to 34.
- **field-symbols**: splat names inside a declared aggregate (`D_8015F5F2` in `TacoCam D_8015F5F0`,
  GameState fields) written as fields, as `tools/migrate_globals.py` does for `src/`.

### Idioms found while matching

- A `switch` on `s8 D_8015EDE8` reads it with `lbu`: write `switch (*(u8 *)&D_8015EDE8)`. m2c's
  `(u8) D_8015EDE8` gives `lb` + `andi`. Matched `func_8013555C` and `func_80135638`.
- `func_8015A948` masks its first argument on entry and switches on it: a `u8` parameter
  (`void func_8015A948(u8 arg0, s32 arg1)`), not m2c's `s32` plus `& 0xFF`. It has no prototype in the
  header; a `u8` prototype may change the matched caller in `8015ABF0.c` (not tried).
- `func_80159260`: the tcpos-args draft matched once one unused `TcPos` was declared after the two
  position locals (FAKE, the same frame idiom as `func_80156F50`).
- `func_8013822C` is an implicit-int function (called before its definition): `s32` with no return.
- `func_80133870` (18 rows): `func_801430D0(D_8015EDEC--, 0, 0x32, 6)`; only the slot of `move a1,zero`
  differs (`drafts/T-16560/TACO/func_80133870/hand.c`).
- `func_80147488` is a clone of matched `func_8015B3B0` (same opcodes, locals 4 bytes higher): with
  `s16 *arg1` in the prototype and the MATRIX declared first, 10 rows remain, all the spill homes of the
  object and position pointers (`drafts/T-16560/TACO/func_80147488/sibling.c`).
- `func_80147928` (32 rows): with the coordinate and position pointers declared before `slot` and `base`
  the frame and the spill homes of those two match; one temporary home and the tail registers differ
  (`drafts/T-16560/TACO/func_80147928/hand.c`).

### What the redraft changed

Against the census (`drafts/T-14500/best-quality-catalog.jsonl`), over the 116 functions without a guarded
draft (the census scored the hand-written guard body of the other 10): 103 fresh drafts compile (the
census had 116, 13 only after manual repair); 48 score better, 53 the same, 2 worse; median 166 to 150
diff rows, sum 21,752 to 19,983 (-8%). The headers alone changed 7 of 98 scores (3 better, 4 worse);
the rewrites gave the rest and made 5 more drafts compile. No draft scored 0; five matched after a
small hand edit (2,032 bytes, about 25 minutes of hand work): `func_8013822C`, `func_8013555C`,
`func_80135638`, `func_8015A948` and `func_80159260` (1,048 bytes, a tcpos-args draft). The 67%-zero rate of
the BUNKA families (T-12700) did not repeat: TACO's remaining functions are large handlers whose distance
comes from register allocation, loop shape and local layout, not from missing types.

Work packages for the matching stage: T-16561..T-16564 (`pt-taco-p1`..`p4`), about 170 minutes each,
best expected bytes first (expected bytes = size / (1 + score / 30)); the gcc-built
`8015CF30.c`, `8015D0D0.c` and `8015D270.c` are left out.

## Matching stage, package 2 (T-16562)

[[tickets/T-16562-taco-types-first-p2]] worked the first ten functions of the T-16560 list for `80154230.c`, `801430B0.c` and `80133870.c`: seven matched in plain C (`func_801430D0`, `func_801347F4`, `func_80143B58`, `func_801342CC`, `func_80133870`, `func_801432F0`, `func_80154820`, 3,120 bytes, about one and a half hours), three guarded (the entry-copy trio) plus `func_801345D4` guarded and `func_80133E98` left. Method and blockers: [[matching-notes]], "T-16562". New types and declarations: the font UV tables `D_8015F5A0[20]`, `D_8015F5B4[52]` and eight scalar glyph bytes (`D_8015F5E8`..`D_8015F5EF`), prototypes for the loading-screen state handlers (`func_80133980`, `func_80133AF4`, `func_80133C08`, `func_80133E98`, `func_80134500`), `func_801347F4(s32 *)`. The three functions with the entry copy (`func_80154230`, `func_801546FC`, `func_8015443C`) stay at 8-9 lines each. Closer pass: those three and `func_801345D4` matched too (two-step header walk; flag test read through the same base), 11 functions and 4,728 bytes in all; only `func_80133E98` is left.
## Matching stage, package p1 (T-16561)

[[tickets/T-16561-taco-types-first-p1]] took the best redrafts of `801335A0.c`, `80137590.c`, `80159090.c` and `80146D60.c` from [[tickets/T-16560-taco-types-first]] and worked them to the byte: 8 of 12 functions matched (5,612 bytes, about 2 hours, 4 of them within 10 minutes of the first compile). The pattern: the new types fix the data flow, the rest is the shape of the locals.

| Function | Bytes | Result | What decided it |
| --- | ---: | --- | --- |
| `func_80147488` | 492 | matched | clone of `func_8015B3B0`: MATRIX first, `pos` local, no `obj` local (a pointer local adds a home, spills move 4 bytes) |
| `func_80146F74` | 604 | matched | same shape plus the interpolation `m.t[0] += (D_8015EDB4->unk74 - m.t[0]) / arg7` |
| `func_80147674` | 644 | matched | same shape; the parameter itself walks the table (`arg1 += 3; *arg1; arg1++`), which gives `$s0` |
| `func_80147C98` | 488 | matched | no pointer locals at all; declaration order MATRIX, SVECTOR in, SVECTOR out, TcPos out |
| `func_80159878` | 1,768 | matched | `default: break;` (keeps `arg0` out of `$s0`), FAKE unused `TcPos`, one-line `sp48.y = ..; sp50.x = ..;` pairs |
| `func_8013788C` | 468 | matched | range tests as `D_800E6280.unk_F80 |= k;` on the global instead of a local accumulator |
| `func_8013760C` | 640 | matched | `s32` without return, `*(u8 *)&` selector, empty `default`, FAKE `(u8 *)i + (u32)ptr` operand order |
| `func_80147928` | 508 | matched | `p`, `pos`, `slot`, `base` locals, index-first sums, and a FAKE empty `do { } while (0);` that gives arg2 its `$a3` range (found by the permuter) |
| `func_80133738` | 312 | matched (closer) | the pair table read as flat words: `p = &D_8015E238[idx * 2]; func_80046290(p[0], p[1], 8);` (`D_8015E238` is an `s32[]`); the `TcPair8` element form allocates one ugen temporary less |
| `func_801472D4` | 300 | 20 lines | block-copy temporaries one ugen register low |
| `func_801471D0` | 260 | matched (T-19015 closer) | pointer parameter and one `dop` walker restarted from it for the second pass (libgs model-setup shape); callers keep an `s32` view through `TACO_OVERRIDE_func_801471D0` |
| `func_80147B24` | 372 | 63 lines | unrolled loop: pointer-end test instead of the counter |

The closer pass (Opus, about 35 minutes) matched `func_80133738`: 9 of 12 functions, 5,924 bytes. `func_801472D4` and `func_801471D0` stay guarded with unchanged line counts; what the closer learned is in their `NON_MATCHING` comments and in [[matching-notes]].

Idioms and dead ends are in [[matching-notes]] (T-16561 section); the open rows are in [[data/t0018-cases]].
### Matching stage, package p3 (T-16563)

[[tickets/T-16563-taco-types-first-p3]] (`80151C30.c`, `8015ABF0.c`): 9 of the 10 listed functions and the unlisted `func_8015BD30` matched from the T-16560 redrafts, 4,740 bytes: `func_8015BD30` (576 bytes, FAKE empty test), `func_8015C0A0`, `func_80151D1C`, `func_80152944`, `func_8015BF70`, `func_80152808`, `func_80152DB4`, `func_801523D0`, `func_801521EC`, `func_8015CE64`. The m2c drafts were not usable as printed; each match came from reading the asm for the loop the compiler turns into the draft's pointer variables, with the new types (`TcPair8`/`TcPair2` at `D_80160290`/`D_801603A8`, `D_80160444` as a pointer table). Idioms and misses: [[matching-notes]], "T-16563: TACO package p3". `func_8015B1B8` (8 lines, struct-copy temporary) stays guarded.

New in `TACO.h`: `D_80160290` (`TcPair8[]`) and `D_801603A8` (`TcPair2[]`) read at `i - 1`; `D_80160444` is `s16 *[]` now (the scalar uses became `[0]`); `D_801603D4` as an `s16 *[]` view base (FAKE); prototypes of `func_80152944`, `func_80152DB4`, `func_801523D0`.
### Matching stage, package p4 (T-16564)

[[tickets/T-16564-taco-types-first-p4]]: from the redrafts, 6 of 8 functions matched in about an hour (`func_801563E4`, `func_80154960`, `func_80138D08`, `func_801557E8`, `func_80138964`, `func_801553AC`, 5,852 bytes), two register-only near misses left (`func_80138B2C`, `func_80137D00`). The types that carried them: `POLY_F4` for the fade packet in `func_80154960`, the `TcPos` by-value arguments (`func_801563E4`, `func_801553AC`), a 10-entry selector table of `GameState.unk_110D`, and `D_8015F490[5]` (the five score words) with `D_8015F4A4`/`D_8015F4A8` (target and shown score) as scalars. Details and the idioms (m2c temporaries inlined, `case 8: case 9: default:`, per-statement `idx + base` spelling, a reset arm without a value) in [[matching-notes]], "T-16564". The T-16560 numbers hold: the remaining distance of these handlers was register allocation and local layout, and for the functions with no sibling it took 8 to 15 minutes each once the draft's temporaries were removed.

## Matching stage, package w3-d-p02 (T-19002)

[[tickets/T-19002-w3-d-p02]] (`801447D0.c`, `80137A60.c`): `func_801449D8` (star-field packets, FAKE frame pad), `func_80145DC4` (eight model-select loops over `s16 *D_8015F82C[2]`) and `func_80137D00` (pointer loaded through its absolute address, FAKE) matched, 2,008 bytes; `func_8014616C` stays a 3-row draft (frame counter promoted by IDO, T-0018 `reverse` row). Notes: [[matching-notes]], "T-19002".

## Matching stage, package w3-d-p06 (T-19006)

[[tickets/T-19006-w3-d-p06]] (`8013B990.c`): all five listed functions matched, 2,636 bytes (`func_8013B990`, `func_8013CFD8`, `func_8013CC40`, `func_8013C1F4`, `func_8013C440`), three with an `s32`-without-return FAKE and one with a branch-over guard FAKE. New declarations in `TACO.h`: `func_8013BAC4(Tc14 *, s32, s32)`, `func_8013C1F4`, `func_8013C440`, `D_8015F500`. Idioms and the preheader-load finding: [[matching-notes]], "T-19006".
## Matching stage, package w3-d-p09 (T-19009)

[[tickets/T-19009-w3-d-p09]] (`80142820.c`, `80143E80.c`): `func_801428DC` (the settings writer, 532 bytes), `func_80144430` (672) and `func_80142AF0` (752) matched, 1,956 bytes; `func_80144094` (924) stays `INCLUDE_ASM`. New types and declarations: `TcSetAll`/`TcSetBits` (the 0x58-byte settings block at `D_800E7D10`, file-local in `80142820.c` with `MAIN_API_OVERRIDE_D_800E7D10`; the loader's view is `TcSave` in `80133870.c`), `void func_800A1070(MATRIX *)` in `main_api.h`, `case 0xFF:` and a `GameState *gs` for the memory-card state machine. The rules that matter beyond TACO (declared scalars own stack slots, one struct instead of splat names for scheduling, `void` for lib calls) are in [[matching-notes]], "T-19009", and [[register-cheatsheet]].

## Matching stage, package w3-d-p11 (T-19011)

[[tickets/T-19011-w3-d-p11]] (`80134930.c`, `80133870.c`): no function matched. `func_8013506C` (29 lines) and `func_80134A18` (36 lines) stay as guarded drafts, `func_80133E98` stays `INCLUDE_ASM` (226 lines, four forms tried). New declarations in `TACO.h`: `D_8015F3E0[2]` (replaces the scalars `D_8015F3E0`/`D_8015F3E4`) and `s32 func_80134A18()`. Findings (chain stores, pointer-with-offset loops, call-saved quotient): [[matching-notes]], "T-19011".
## Matching stage, package w3-d-p14 (T-19014)

[[tickets/T-19014-w3-d-p14]] (`8013F250.c`): `func_801404FC` (316 bytes), `func_80140364` (408), `func_8013F9C0` (560) and `func_801407D4` (776) matched, 2,060 bytes; `func_80140ADC` (788) matched in the closer pass (the UV stores on one line in `setUVWH` order). New declaration in `TACO.h`: `s32 func_801407D4(s32 *arg0, TcPos arg1);` (the first eight bytes of the particle record by value). FAKEs: unused locals for frame homes, a one-line pair of UV stores, byte-offset element address (index first) and an empty `if (count) {}`. Details and idioms: [[matching-notes]], "T-19014".
## Matching stage, package w3-d-p13 (T-19013)

[[tickets/T-19013-w3-d-p13]] (`801480B0.c`, the 1ST BOSS scene): `func_80148184` (init, 388 bytes) matched in plain C from its 2ST BOSS twin `func_801490B4`; 1,336 bytes matched in all; `func_801484E4` (state 3 handler, 392 bytes) is a 6-row guarded draft after the closer pass (counter web, two FAKEs; the volatile final access leaves the address and one spill register); `func_801487A4` (actor transform, 948 bytes) matched too: the typed draft had the frame and all 237 instructions with 140 register rows, one ugen temporary short, until the `arg2` records were indexed as `((s16 *)arg2)[arg1 * 4 + k]` (two-step scaling) and the `arg4` pair index-first. New tables: `TcPair8 D_8015F840[]` and `TcPair2 D_8015F930[]` (both read at `i - 1`, linker symbols in `config/symbols_TACO.txt`). Notes: [[matching-notes]], "T-19013"; open rows in [[data/t0018-cases]].

## Matching stage, package w3-d-p21 (T-19021)

[[tickets/T-19021-w3-d-p21]] (`80139170.c`, the name entry screens): `func_8013A580` (484 bytes, three 9-argument calls) and `func_8013AD64` (632, colour dot packet) matched, 1,116 bytes; both were old frame rows of [[data/t0018-cases]] and needed a different number of named locals (three temporaries beside `k`; two single-use shifts), no pad or FAKE. `func_80139BCC` (1,012, cursor and character table) is a 14-line guard: one register swap in a priority tie. `func_80139FB8` (1,480, character carousel) stays `INCLUDE_ASM` (402 lines, the sine values take `$s1` first). New declaration in `TACO.h`: `u8 D_8015F4B0[]` (the 40-character table). `D_800E7200`/`D_800E7208` are `D_800E6280.unk_F80`/`unk_F88`. Notes: [[matching-notes]], "T-19021".
