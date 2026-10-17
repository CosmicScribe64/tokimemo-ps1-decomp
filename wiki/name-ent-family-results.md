---
type: research
updated: 2026-10-17
---

# NAME_ENT bounded family reconstruction

[[tickets/T-13700-name-ent-families]], baseline origin/main 5ccb8765, isolated codex/name-13700. Result: **zero accepted functions / zero verified bytes**. Production C and headers are unchanged. All compilation and scoring used the existing tokimemo-decomp-arm64 Docker image, IDO 5.3, one CPU and 1 GiB; no dependencies installed or compiler options changed.

Read [[tickets/T-11570-w1-ovl-name_ent-80139740]], [[bunka-family-results]], [[broad-coverage-results]] and the NAME_ENT historical sections of [[matching-notes]] first. The exhausted selector-copy family was not retested.

## Prologue setup probes

func_80142A80 (932 list bytes): straightforward reconstruction has 18 full-TU resolved differing rows. m2c produced a malformed Shift-JIS escape for the character 予; restoring the proper Unicode literal removes the string difference. A 4-byte local buffer leaves 12 rows, all frame/saved-local positions. Target: frame 72, saved byte at67, saved word at60, buffer at52. Buffer4 produces frame64, saved63/56, buffer52; buffer8 produces frame64, saved63/56, buffer48; buffer16 produces frame72, saved71/64, buffer48. Buffer12 with an offset4 address also remains12 rows. None accepted; do not infer the real buffer extent from these experiments.

func_80142E24 (1,652 list bytes): the target initializes two rows of four image addresses. A typed word-pointer reconstruction scores328 rows; reordered initialization and pointer-valued induction variables score324. Natural nested 2x4 and four-field row loops score357 and331 respectively. These are worse than the historical56-row broad-sweep default. No new source form is established.

func_8014472C (476 list bytes): a typed draft scores126 rows. The target computes a four-entry signed-halfword average whose result appears unused; the m2c draft omits it. Its draft return/local narrowing also introduces conversions. No further search performed because this isolated small helper lacks family throughput.

## Memory-card menu family

Fresh drafts inspected for func_80139F30 (2,712 list bytes), func_80140F90 (2,192) and func_80141820 (1,924). The latter two share sixteen-byte clear/copy/accumulation loops and slot-status scanning. m2c misidentifies several byte pointers as word pointers or GameState pointers, so its increments must be checked against original strides before reuse. func_80054E6C reads globals and takes no live incoming arguments; m2c's inferred argument-count override introduces unset-register placeholders at its call sites. Existing callers in this TU already call it without arguments.

A handwritten array reconstruction for41820 scores361 full-TU resolved rows. Local GameState alias364; infinite-loop scan336; for-loop scan369; aliases for both NameMetricWork and GameState347, and with the infinite scan332. Preserving the m2c control flow while changing incorrect pointer types to byte pointers scores474. All remain distant misses; transfer to40F90 was stopped.39F30 was inspected but not compiled/scored: its record-flag rendering and fixed-stride actor operations require broader reconstruction, not the unproven menu scan form.

All scores above are full-TU diagnostics, not matches. No literal proof or overlay checksum claim is made because no candidate reached zero. All failures remain assembly in production. No blind mutation sweeps, toolchain emulation changes, generated assembly edits or FAKE source were accepted.

## Local evidence and handoff

Worktree: /Users/lila/Documents/Projects/tokimemo-wt/name-13700. Drafts, compiler objects and selected diffs: build/name13700/. Reproduction scripts are copied there from /private/tmp/name13700.py, name13701.py, name13702.py and name13703.py. The scripts mount as /probe.py inside Docker and execute from /work; output paths are relative to the worktree. They preserve source as scratch whole-TU candidates rather than editing production files. Other owners should not treat these negative results as proof that the entire overlay is exhausted.

## T-13701 semantic record/actor reconstruction

Follow-up [[tickets/T-13701-name-ent-record-types]] reconstructs39F30 independently of the failed menu substitutions. All fields use existing types; no shared declaration/layout changes are proposed.

| Target evidence | Existing typed view |
|---|---|
| D_800E7D24 through D_800E7D30 | NameMetricWork.flags[0..12], base D_800E7D10, offset0x14 |
| D_801237A8 and D_801237B4 with selected-record stride0xF00 | NameRecordF00.flags[0] and flags[12], base D_80123120, offset0x688 |
| Loads/stores at actor offsets0x2201 and0x2203, then pointer+=0x44, eight iterations | DisplayActor44 actors128..135, fieldsunk_01 andunk_03 |
| Fixed bytes D_80121135/137, D_80121179/17B, D_801211BD/1BF, D_80121201/203 | actors137..140, fieldsunk_01 andunk_03, corresponding to flags8..11 |
| Fixed bytes D_801210F1/F3 | actor136, fieldsunk_01 andunk_03, corresponding to flag12 |

Matching func_80139CB4 independently uses NameMetricWork and NameRecordF00 for the selected record's name fields. Matching func_801324AC uses DisplayActor44 with the same0x44 stride. The original39F30 first draws thirteen icons in flag order0..7,12,8..11, then three four-entry indicator columns and the flag12 indicator. Eight actors are updated in a rolled loop; the remaining five actors are updated individually. In both cache and stored-record branches, zero flag sets actor state4 and bit0x80, while nonzero flag clears bit0x80. The stored-record branch reloads flag12 immediately after its indicator callback and retains that value through actor updates.

A complete coherent typed C draft (no unknown types, unset-register placeholders, fake padding or forced registers) compiles in the full translation unit and scores769 resolved rows. The first long actor loop is compiler-unrolled whereas the original stays rolled. A pointer-end condition using the actor array scores746; using the work-flags address endpoint scores989. These bounded probes establish no byte match. No sibling transfer follows without a reusable matching source form.

Evidence: build/name13700/typed39F30-body.c, typed39F30.c, typed39F30.diff, actor-end39F30.c and address-end39F30.c. Reproduction: build/name13700/name13704.py. The scratch forward declaration void func_80139F30(void) is required before its earlier caller in the same TU; without it, IDO sees an incompatible implicit-int declaration. Production INCLUDE_ASM remains unchanged. The distant draft is kept in ignored scratch instead of adding a hundreds-of-rows NON_MATCHING body to production.

## Types-first pass (T-17900)

T-20900 update: `NameRecord44`, `NameActorHead44`, `NameAssetActor44` are `DisplayActor44`; `NameMotionActor44` is `DisplayFixActor44` (`x`, `y`, `phase`, `flags` are `unk_24`, `unk_28`, `unk_07`, `unk_03`); `NameMotionWord` is `Fix32`; `NameDate` is `GsDateBits` (its `day`/`month` names were swapped); `D_800E7D10` is declared as `Work800E7D10` (main_api.h), so the `NameMetricWork` casts are field accesses ([[data-types]], "Shared-type unification").

Wave-3 types-first pass ([[tickets/T-17900-types-first-name_ent]], branch `w3-name-ent-types`, method of
[[speedup-research-2026-10-16]]): recover NAME_ENT's shared types, redraft every unmatched function with them, then
cut packages. 32 functions (30,504 bytes) were unmatched at `w2-good`, 9 of them guarded drafts. Drafts, driver and
README: `drafts/T-17900/`.

### Type map

| Type / global | Where | Evidence |
| --- | --- | --- |
| `u8 D_8014D060[16]` | `NAME_ENT.h` (new) | cleared and counted four at a time up to `D_8014D070`, then copied into `NameMetricWork.flags` (`func_80140F90`, `func_80141820`) |
| `u8 D_8014D158[16]` | `NAME_ENT.h` (new) | the second 16-byte flag table of `func_80140F90`, walked up to `D_8014D168` |
| `u8 D_8014D0F4[10][2]` | `NAME_ENT.h` (new) | sprite (u, v) of each decimal digit, indexed by `value % 10` (`func_80139740`); the label covers 0x64 bytes, the rest is zero |
| `u8 D_8014CC74[8]`, `u8 D_8014CC7C[8]` | `NAME_ENT.h` (sized) | name-entry buffers 0 and 1, sized like their definitions in `80132000.c`; the fold now reads `D_8014CC7A` as `D_8014CC74[6]` |
| `NameMetricWork` at `D_800E7D10` | m2c context only | `main_api.h` declares an `s32`; matched NAME_ENT code reads it as `((NameMetricWork *)&D_800E7D10)->f`, so the drafts do too (an override for one overlay is what CODING_STANDARDS 8a advises against) |
| `NameRecordF00[]` at `D_80123120`, `DisplayActor44[]` at `D_8011ECD0` | m2c context only | `u8[]` in `main_api.h`; the drafts read `((NameRecordF00 *)D_80123120)[i]` and `((DisplayActor44 *)D_8011ECD0)[k]` |

Not changed: the kana page tables `D_8014CD28`..`D_8014CDAC` (`s32` scalars plus the `NameKanaPages44` view; each
label is its own data-island line and matched code uses each one); the `SpriteSlot24` records at `D_801220D0`
(matched code uses the scalar labels `D_801220E0`..`D_80122106`); `D_8014CC84` (`[12]` in its definition, but
`D_8014CC8E`/`D_8014CC8F` inside it are addressed by name, so it stays unsized and out of the fold).

### Redraft

`drafts/T-17900/redraft.py` (from `drafts/T-17700/redraft.py`) scores a `plain` draft (m2c with the new header) and a
`typed` draft (folded asm, the views above, the T-16550/T-16560/T-17700 rewrites, then `tools/draft_fixes.py`).
New rewrites for NAME_ENT: the object view of `D_800E7D10`; record byte offsets without the view cast written as
fields (`*(s8 *)((unsigned char *)D_8011ECD0 + (6529))` is `((DisplayActor44 *)D_8011ECD0)[96].unk_01`, the same for
`D_80123120` and `D_801217D0`); a no-argument prototype of `func_80054E6C` in the m2c context (m2c took its callers'
leftover loop registers as arguments and printed `M2C_ERROR`, so `func_80140F90` and `func_80141820` had no
compiling draft); `s32` for a `void` definition that the unit calls earlier with no prototype (`func_80139F30`).
`drafts/T-17900/hand.py` scores the guarded bodies in `src` the same way.

| Measure (full-unit diff rows) | value |
| --- | --- |
| unguarded functions, not matched here | 21 |
| better than the census / same / worse | 14 (19,372 bytes) / 7 / 0 |
| sum of rows, census to now | 7,056 to 6,107 |
| median rows | 271 to 195 |
| typed beat plain / lost | 12 / 4 of 19 that compiled both ways; typed also compiled the 2 with no plain draft |
| guarded (9) | the hand bodies in `src` stay the best start; the census had scored a generic draft for 3 of them |
| matched | `func_80139740`, `func_8013EE7C` (904 bytes) |

Largest gains: `func_8013CB44` 434 to 115 (the `NameMetricWork` view), `func_8014068C` 282 to 195,
`func_80140F90` 847 to 674 and `func_80141820` 501 to 470 (now compiling without repair), `func_80143580` 1,168 to
1,067, `func_8013B1D8` 326 to 258, `func_8013A9C8` 330 to 272. The record-field form of the display records does
not change scores (same addresses), it only makes the drafts readable. No draft scored 0.

### Idioms and near misses

- `func_80139740` is a port of the matched main digit drawer `func_8004B19C` with `e = D_8014D0F4[x % 10]`. The
  column offset must be the `for` counter (`for (n = 0;; n += 8)`); with `n = 0;` before the loop the
  `move s2,zero` comes before the hoisted base addresses.
- `func_8013EE7C` is the matched `func_8013EBEC` without its cases 4 and 5.
- `func_80149B08` (`drafts/T-17900/NAME_ENT/func_80149B08/hand.c`, 35 differing instructions, all after the second call): the port of the matched
  sibling `func_801486F8` differs only around the load of record 96's +0x26 (`D_80120676`), copied to records 97
  and 98. The original loads it first, into `a3`; IDO loads it next to its first use, into `v0`, and every
  constant moves one register. A local read first, or stores through the `NameRecord44` view, change nothing.
- `prologue_line_start` (54 rows, `hand.c`): the original reloads the index `D_8014D180` into `a3` before each
  table read; IDO uses `t6`/`v0`/`t0`. A `u8` view of the selector does not change it.
- `func_80142A80` draws the lines of a song with `set_kanji_string`; its T-17900 drafts write those strings as
  their rodata labels, so the song text is not copied into the repository.

### Packages

Cut by the wave-3 package scripts from the committed tree (17 functions, 11,316 bytes). The other 13 unmatched
functions are not in a package: `func_80138C1C`, `func_80138F60`, `func_801398EC`, `func_80139F30`,
`func_8013A9C8`, `func_8013B1D8`, `func_8013B994`, `func_8013BB24`, `func_8013CB44`, `func_8014068C`,
`func_80140F90`, `func_80141820`, `func_80143580`.

| Package | Ticket | File | Functions |
| --- | --- | --- | --- |
| w3-name-ent-p01 | T-17901 | `80142A80.c` | `func_80142A80`, `func_80142E24`, `func_8014472C` (3,116 B) |
| w3-name-ent-p02 | T-17902 | `80132000.c` | `func_80132320`, `func_80134B94`, `func_80134248`, `func_80134110` (3,608 B) |
| w3-name-ent-p03 | T-17903 | `80139740.c` | `func_80140B20`, `func_8013BE18`, `func_8013DF88`, `func_8013AF78`, `func_8013D2AC` (2,252 B) |
| w3-name-ent-p04 | T-17904 | `80144AD0.c` | `prologue_line_start`, `func_801460D8`, `func_80145FB0`, `func_80149B08`, `prologue_line_run` (2,340 B) |
