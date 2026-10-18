---
type: research
updated: 2026-10-17
---

# BUNKAKEN and BUNKASAI family reconstruction

Scope: [[tickets/T-12700-bunka-families]], from 95c3fc9f. Existing broad experiment evidence was read from the integration checkout's wiki/matching-breakthroughs.md and wiki/broad-coverage-results.md. That prior 190-target default sweep returned five functions / 5,036 bytes; this work does not repeat it.

BUNKASAI's types-first pass of wave 3 (T-18700) is on [[bunkasai-family-results]].

## Aggregate address form

The existing CharFlags bitfield view fixes BUNKAKEN flag tests/stores, but a direct global member expression still leaves aggregate-base register differences in several handlers. A local `GameState *g = &D_800E6280` used for all aggregate accesses follows already matched siblings in src/ovl/BUNKAKEN/8013BFE0.c and src/ovl/BUNKAKEN/8014A6A0.c. On the first two candidates this gives zero full-TU differences; replacing only the store with the pointer does not. This is source-form reuse, not a new compiler pass or independent type discovery.

The same form gives zero full-TU differences for func_80142A40, func_80142FD4 and func_801438F8. func_80141B80 remains nonmatching (134 rows). Literal proofs and accepted counts are recorded in [[tickets/T-12701-bunkaken-address-family]]. All unsuccessful scratch variants remain in build/bunka12701.

## Reload after calls

The BUNKASAI func_80146A78 archived draft saves a calendar state byte across calls. Reloading the existing GameState field for its final comparison removes the spill and reproduces the target's frame and instruction sequence. Local GameState pointer variants worsen this candidate. Literal proof is recorded in [[tickets/T-12702-bunkasai-family]]; scratch variants remain in build/b12702.

No compiler flags, generated assembly, or shared main types are changed by these experiments. A zero candidate score is not counted as an accepted match until linked bytes and the overlay SHA1 both pass.

## Verified checkpoint

Five BUNKAKEN handlers add 6,544 literal linked bytes, with BUNKAKEN SHA1 OK. Two BUNKASAI handlers add 2,520 bytes, with BUNKASAI SHA1 OK: func_80146A78 (1,216) and func_8013BB94 (1,304). The latter needs existing BunkaReturnFlags stores before the three continuation-state copies; bitfields alone reduce the mismatch but leave instruction ordering wrong. Total accepted at this checkpoint: seven functions / 9,064 bytes. Header validation passes. No whole-game checksum claim is made.

The continuation-state ordering also resolves BUNKAKEN func_80141B80: existing BunkakenModeFlags assignments before the three continuation copies reduce 134 -> 24 -> 0 rows (bitfields, then order). Its 1,860 linked bytes and BUNKAKEN SHA1 pass. BUNKAKEN now adds 8,404 bytes; both lanes total eight functions / 10,924 bytes.

Further transfer adds only the two still-assembly handlers func_80138A08 (1,200 bytes) and func_8013A8CC (804 bytes), both literal MATCH with BUNKAKEN SHA1 OK. Historical seeds for 3C0AC, 3C634 and 3CA6C were already matched controls in the baseline and contribute no new coverage. Current accepted total is ten functions / 12,928 bytes. The still-unmatched 41088 remains 228 rows; BUNKASAI 3B25C remains 122 rows after direct stores, and aggregate-pointer alternatives worsen it. The guarded BUNKASAI 51364 scheduling gap also remains; no rejected candidate enters production.

A targeted fresh test in BUNKAKEN 8014E440.c transfers the same source forms to func_8014E5E0 (688 bytes), func_8014E890 (608) and func_8014EAF0 (868). All three pass literal proof and BUNKAKEN SHA1, adding 2,164 bytes. Further adjacent handlers are tested sequentially with a two-miss stopping rule; this is a bounded family test, not another default sweep. Accepted cumulative gain: thirteen functions / 15,092 bytes.

All eight further handlers in BUNKAKEN 8014E440.c pass literal proof and overlay SHA1: 4EE54 (960), 4F214 (1,040), 4F624 (948), 4F9D8 (1,308), 4FEF4 (848), 50244 (600), 50680 (728), 50958 (880). New gain: 7,312 bytes. The file is entirely C. BUNKAKEN adds nineteen functions / 19,884 bytes; both lanes add twenty-one / 22,404 bytes. Evidence is build/bunka12701/eight-proof.txt and eight-build.log.

Adjacent BUNKAKEN func_8013EC98 adds 780 literal verified bytes. Its remaining argument-narrowing differences disappear with the existing per-TU old-style declaration of func_80083440; the callee narrows its own argument. The override is explained at the include site and leaves main_api.h unchanged. BUNKAKEN SHA1 passes with all prior matching functions retained. Cumulative accepted gain: twenty-two functions / 23,184 bytes.

Eight adjacent BUNKAKEN handlers add 6,112 literal verified bytes with overlay SHA1 OK; 8013EB30.c is now fully C. The local K&R view preserves all pre-existing C in the object. BUNKAKEN cumulative gain is twenty-eight functions / 26,776 bytes. Proof: build/bunka12701/adjacent-proof.txt and adjacent-build.log.

BUNKASAI held-out 8014DD90.c now has four newly verified handlers: 4DF04 (936), 4E2AC (832), 4E9D0 (1,364), 4FC70 (884), totaling 4,016 bytes. All literal proofs, overlay SHA1 and headers pass (build/b12703/final-proof.log). Existing CharFlags b12/b13 stores close the first candidate's last 14 rows. The fourth needs direct switch stores instead of m2c's artificial goto across case labels (128 -> 0). No header changes or FAKE workarounds. Both lanes now add thirty-four functions / 33,312 bytes.

Five handlers in BUNKAKEN 8013CF30.c add 3,704 literal verified bytes: 3D048 (428), 3D4D8 (852), 3D82C (956), 3E2C0 (696), 3E7C4 (772), with BUNKAKEN SHA1 OK. The remaining 3DBE8 candidate is not counted. An exact function-body text audit against 95c3fc9f finds all 37 pre-existing C bodies in touched BUNKAKEN files unchanged (build/bunka12701/audit.py). Cumulative both-lane gain: thirty-nine functions / 37,016 bytes.

## Final accepted result

Forty-one functions add **40,252 literal linked bytes**: BUNKAKEN 35 / 33,716 and BUNKASAI 6 / 6,536. Final bounded fixes are BUNKAKEN 3DBE8 (1,752 bytes), using mode-3 bitfield stores before state copies, and 41088 (1,484), using ordinary switch stores and the target statement order. Both pass literal proof and BUNKAKEN SHA1 (build/bunka12701/final-proof.txt, final-build.log). All 37 pre-existing BUNKAKEN bodies and 16 pre-existing BUNKASAI bodies across touched files are text-identical to 95c3fc9f. Final review and combined validation are recorded in the umbrella ticket.

Final validation: 5,848/6,958 game functions, 1,385,776/2,279,368 game bytes (60.8%); BUNKAKEN 82.9%, BUNKASAI 88.9% byte coverage. Both final overlay SHA1 hashes were independently recalculated and match references. Headers/globals pass, prototypes have 0 new / 15 known disagreements. Both review axes pass all source changes. This bounded family pass is complete; the game-wide 80% target is not yet reached. All failures remain local in the documented ignored experiment directories.

## Types-first pass (T-18500)

T-20900 update: `BunkakenModeFlags` is `Flags7D4F` (main_api.h; `mode` is `unk_2_4`) and `BunkaDispatch8` moved to game.h ([[data-types]], "Shared-type unification").

[[tickets/T-18500-types-first-bunkaken]], branch `w3-bunkaken-types` from `w2-good`. Method of
[[speedup-research-2026-10-16]]; driver and drafts in `drafts/T-18500/` (README there).

### Survey

- 12 unmatched BUNKAKEN functions, 14,256 bytes (`tools/siblings.py`), 1 guarded (`func_801428D0`, 12 rows).
- Siblings at 0.90 or more: `func_80147914` (1,920 B) to matched `func_8013FD40` (0.987) and
  `func_80151A78` (1,096 B) to matched `func_80138A08` (0.913); the other ten have none above 0.75.
- `tools/type_recovery.py --unit BUNKAKEN`: mostly jump tables; the real proposals were the 0xC-stride
  "arrays" over `D_801562A0`.. (in fact three 13-word tables) and small tables copied to the stack.
- Census drafts: almost no byte-offset casts; the distance came from main-exe aggregates (GameState
  fields read through splat names, 99 uses of `D_80122EBC`), stack tables typed as the wrong shape
  (`ByteTable54` for a `[9][6]` table, `UnalignedBytes11`), a synthesized stack byte (`spDA`) and
  hand-scaled indexing of the heroine tables (`*(&D_80155F0C + (i * 4))`).

### Type map (`include/ovl/BUNKAKEN.h`)

| Type / globals | Layout and evidence |
| --- | --- |
| `s32 D_801562A0[3][13]` | replaces 39 scalars `D_801562A0`..`D_80156338`: text addresses in the loaded scene data (0x801B....), filled by matched `func_80148ED0`; `func_8014A0E0` copies element `[0][k]`, `[1][k]`, `[2][k]` to `D_800CA160/164/168` per heroine. Matched code rewritten as elements, same bytes |
| `s32 D_80155EA0[12]`, `void (*D_80155ED0[12])(void)`, `u8 D_80155F00[12]`, `s32 D_80155F0C[12]`, `u8 D_80155F3C[20]` | per-heroine tables of the date loaders `func_8013B510` and `func_80143DD0`, indexed by `D_80122EBC` (`lw %lo(sym)(base + i * 4)`); sizes from the next data label (`D_80155F3C`: 12 entries and 8 zero bytes) |
| `BunkakenBytes11 D_80155F90` (0x0B) | copied whole to the stack by `func_80144F70` (`lw`, `lw`, `lwl`/`lwr`); the census called it `UnalignedBytes11` |
| `BunkakenBytes9x6 D_80155F9C` (0x36), `BunkakenBytes11x6 D_80155FD4`, `D_80156018` (0x42) | copied to the stack by `func_80145550` (word runs plus an `lhu`/`sh` tail), read at `[D_80122EBC][D_80122EB8]` (`i * 6` as `sll 2; subu; sll 1`) |
| `BunkakenNames3x3 D_8015633C` (0x129) | three by three 33-byte strings, copied whole by `func_8014A5EC` and passed to `strcpy` at `[GameState.unk_03E - 0x5F][D_80122CDC]` |

Not changed: `D_80155E90`..`D_80155E98` (three words that matched code reads one by one), the
`BunkakenTexts*` tables of `func_80152680`, the jump tables.

### Idioms found on the way

- **Sibling ports first**: `func_80147914` is `func_8013FD40` with other constants, one extra case and the
  mode bits (`BunkakenModeFlags` `mode = 2` for `| 8`, `b0 = 1` for `|= 1`); `func_80151A78` is
  `func_80138A08` (GameState local `g`, same bit-field stores). Both matched at the first compile once
  m2c's `D_800E7D4F = ((D_800E7D4F | 2) & 0xFFE3) | 8` was written as the sibling's bit-field stores.
- **`GameState *g = &D_800E6280;` also owns a stack word**: in `func_8014A5EC` it puts the 0x129-byte
  table at sp+0x28 as in the original, where an unused `s32` pad would be a FAKE (the gs-local step of
  the driver found it: census 2 rows to 0).
- **Uninitialized byte local with a stack home**: `func_80145550` and `func_80145A64` read a `u8` from
  the stack (`lbu v0,0xDA(sp)`, `sp2B`) in the switch arm that sets nothing: the original has no
  `default` and reads `v` uninitialized, so IDO gives `v` a home. One local for both branches. In
  `func_80145550` the home sits one byte low (0xDA, ours 0xDB with `g`, `v`, tables): an unused `u8`
  before `v` (FAKE) matches.
- **K&R calls in the 80148ED0 object**: `func_8014A0E0` passes `D_80122EBC` to `func_800847B8` and
  `get_g_zyotai_h(...) & 0x7F` to `func_80083440` without narrowing in the caller; file overrides
  `MAIN_API_OVERRIDE_func_800847B8` and `MAIN_API_OVERRIDE_func_80083440` (as `80152680.c` and
  `8013EB30.c` already do). The other functions of the file match unchanged.
- **Two-argument `get_g_zyotai_h`**: `func_80145A64` calls it with `(girl, v)`; `main_api.h` now wraps
  the one-argument prototype in `#ifndef MAIN_API_OVERRIDE_get_g_zyotai_h` and `80144F70.c` declares
  `s32 get_g_zyotai_h();`.
- **Load order of a combined test**: `if (func_8008667C(D_80122EBC) != 4 && D_80122EA4 != D_80122EBC)`
  in one condition loads `D_80122EA4` before `D_80122EBC` as the original does; two nested `if`s load
  them the other way round.

### Redraft

`drafts/T-18500/redraft.py` is the T-18100 driver with three T-18300 rewrites and three new ones
(`header-arrays`, `array-symbols`, `gs-local`; README there). Scores are full-unit diff rows as in the
census (`drafts/T-14500/best-quality-catalog.jsonl`), 12 functions:

| | value |
| --- | --- |
| better than the census | 5: `func_80143DD0` 449 to 330, `func_80144F70` 426 to 335, `func_80145550` 302 to 161 (gs-local), `func_80145A64` 256 to 223, `func_8014A5EC` 2 to 0 (gs-local) |
| same | 6 (the two sibling clones, `func_8014A0E0`, `func_8013BD84`, the two selector cases) |
| worse | 1: `func_8013B510` 297 to 337 (m2c prints two `default` labels in one switch, so only the census body compiles, and it loses rows with the typed header) |
| median, sum | 210 to 191, 2,477 to 2,131 rows |
| headers alone (plain m2c) | changed 2 of 9 compiling drafts: `func_80143DD0` 449 to 386; `func_801428D0` 12 to 44 (its census score is the guarded hand body) |
| score 0 | `func_8014A5EC` |

Hand drafts in `drafts/T-18500/BUNKAKEN/<func>/hand.c`: `func_8013BD84` 87 rows (from 228; clean loop over
the ten heroines, the flag tests inline; one temporary register too many from the first block on),
`func_80144F70` 118 (from 335; the two loops of the matched siblings, `for (i = 0; i < 11; i++)
D_800CA2B0[i] = ...unk_0C.f.b1;` and a 64-word clear of `D_80122DD0`; the frame is 8 bytes too big and the
original stores `D_80122EBC` back with its own value right before both `func_8008667C(D_80122EBC)` calls,
which m2c prints as `D_80122EBC = temp_a0` and no plain C form tried reproduces).

### Matched during the pass (6 functions, 6,232 bytes)

`func_80147914` (1,920), `func_80151A78` (1,096), `func_8014A0E0` (1,292), `func_80145550` (756, FAKE
unused byte), `func_80145A64` (988), `func_8014A5EC` (180). BUNKAKEN sha1 OK after each. Left: 6 functions,
8,024 bytes: `func_80143DD0` (3,232) and `func_8013B510` (2,164), the two date loaders; `func_80144F70`
(1,424); `func_8013BD84` (540); `func_801428D0` (368, guarded) and `func_80140F60` (296), the selector
`$v1` cases of [[data/t0018-cases]].

### Packages

Built with the wave-3 inventory scripts (`inventory2.py`, `mklists3.py`; 6 functions, 8,604 bytes by
their size measure):

| Package | Ticket | Files | Functions | Bytes |
| --- | --- | --- | --- | --- |
| `w3-bunkaken-p01` | T-18501 | `80144F70.c`, `80143DD0.c`, `801428D0.c` | 3: `func_801428D0` (guarded, 12), `func_80144F70` (hand draft, 118), `func_80143DD0` (330) | 5,368 |
| `w3-bunkaken-p02` | T-18502 | `80140F60.c`, `8013B510.c` | 3: `func_80140F60` (37), `func_8013BD84` (hand draft, 87), `func_8013B510` (census, 297) | 3,236 |

## Completion pass T-24003

The matched BunkaDispatch8 view from func_801428D0 transfers directly to func_80140F60: write unk_04 through the view, switch on unk_00, and use the existing heroine-2 CharFlags b2. The scalar-global draft allocated the outer selector in v0 and delayed the nested calendar load; the aggregate form restores v1 and the original delay slot. Literal default linked proof passes for 296 bytes (`build/t24003/selector-proof.txt`). The two date-loader functions remain under investigation in [[tickets/T-24003-complete-bunkaken]].
