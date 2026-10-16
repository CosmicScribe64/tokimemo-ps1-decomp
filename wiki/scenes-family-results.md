---
type: research
updated: 2026-10-16
---

# GEKO and DATE2 source-family work

Scope [[tickets/T-13900-scenes-families]], children [[tickets/T-13901-geko-families]] and [[tickets/T-13902-date2-families]]. Base 0e670a13. Owned source only GEKO and DATE2 C and overlay headers. Prior broad sweep GEKO 80133C50 scored 164 and failed bitfield compilation; no DATE2 candidate was in that sweep. Reuse Bunka consistent aggregate pointers, bitfield widths and state ordering where supported by actual target assembly. Experiments retained in build/scenes13900.

DATE2 func_8013820C: direct case stores replace the m2c shared goto (217 -> 2 rows); corrected overlay-local D_8013B5FC to s16 (target sh) removes final difference. D_8013B5D0 is s16 from its target store. Literal full-TU default proof reports MATCH, 768 linked bytes; DATE2 SHA1 OK. Evidence build/scenes13900/first-proof.log and first-build.log. GEKO 801367EC remains 44 rows with direct aggregate and 50 with pointer; no win counted.

GEKO func_80141374: replacing the m2c duplicated loop cursor with the single s16 induction variable from the target closes 39 -> 0 rows. Uses the existing GameState name array. Full-TU default literal MATCH 416 bytes and GEKO SHA1 OK (build/scenes13900/geko-proof.log, geko-build.log). Existing matching source unchanged. The recovered month table / label table / large string buffer compile but remain nonmatching; discarded production candidates, retained scratch evidence.

GEKO dispatch/indicator family: func_801351F8 (328 bytes) and func_8013882C (596 bytes) both literal MATCH; GEKO SHA1 OK, build/scenes13900/dispatch-proof.log and dispatch-build.log. The established copied function-table form plus explicit used selector fixes the former stack offset. Direct global flag updates remove the m2c byte temporary/redundant moves in both functions (24 -> 2 -> 0 and 24 -> 0). No FAKE source forms. Current accepted total four functions / 2108 linked bytes.

## Bounded negative ledger

All scores below are full translation-unit resolved diagnostic diff rows, not linked match proofs. Rejected sources remain only in ignored build/scenes13900; no guarded drafts or speculative declarations entered production.

| Candidate | Evidence and best bounded outcome |
| --- | --- |
| GEKO 801367EC, 872 list bytes | Existing GameState fields plus missing overlay byte declaration: direct 44, consistent pointer 50. String argument scheduling remains; no repeated broad sweep. |
| GEKO 80144198, 216 list bytes | Actual twelve-halfword month table reconstructed. Typed s32 year/month, direct returns, K&R definition probes: 81 -> 76 -> 74. Stack placement/register shape unresolved. |
| GEKO 80137580, 176 list bytes | Fresh direct source 58; then prior T-10910 copy-chain ledger located and read. No allocator sweep repeated; existing guarded source unchanged. |
| GEKO 80141870, 268 list bytes | Existing aggregate normalization: 81. Control-flow/result recovery needed. |
| GEKO 80134228, 244 list bytes | Existing aggregate typed draft 39; direct globals/postincrement probe 40. Not promoted. |
| GEKO 8013B10C, 388 list bytes | Recovered actual 42-entry call table and tested forward void declaration: 63. Boolean/call result and byte-view effects remain; experimental prototype removed. |
| GEKO 80133AA0, 432 list bytes | Target lh shows overlay byte declaration is too narrow at this call site. Halfword plus reloads 81, s32 fallthrough result 67. Experimental header change removed; existing matching C untouched. |
| GEKO 80133598, 452 list bytes | Recovered Rec38 flag and halfword accesses; one saved status byte gives 22, direct destination-field amount gives 4 (only two stack-byte offsets). Plausible quotient/amount locals give 8/18; no padding or artificial local accepted. |
| DATE2 801385CC, 756 list bytes | Four 21-byte labels, x/y/height arrays and four halfword widths reconstructed. Labels/layout/natural-loop probes 207 -> 199. Needs real local/layout/ABI reconstruction; speculative width declaration removed. |
| DATE2 801388C0, 380 list bytes | Actual GsRecF90 paired-record flag and label width array: 120. No source promoted. |
| DATE2 801330D4, 388 list bytes | Fresh draft exposes missing local score array and stride-indexed character attributes. Compile failure retained; no codegen conclusion. |
| DATE2 80136860, 560 list bytes | Aggregate typed draft 192. Timer and reward control-flow reconstruction still needed. |
| DATE2 801395C8, 2648 list bytes | Observed loop bound 81, byte stride31 and 0xA30 frame support actual 81x31 string buffer. First typed matrix 679; shared source locals and switch loops reduce to432. Existing integer globals hold pointer tables; casts used only in scratch. Needs original local expression lifetimes and call ordering, not padding guesses. |

Fresh drafts for GEKO 80132D04 (800), 80139E1C (808), 8013E950 (856), 801420E4 (1260), 80144270 (1184), DATE2 80133AF0 (3680), 801348F0 (2048), 801350A4 (3480) were inspected for source families and missing types but not claimed matched or exhausted. GEKO 80143F48 needs the actual packed date/activity fields and copied byte table. No broad automatic mutation farm ran. These are useful follow-up targets for actual layout recovery; this bounded pass does not establish a compiler impossibility or exhaustion of either scope.

## Final handoff

| Unit | Function | Literal linked bytes | Commit |
| --- | --- | ---: | --- |
| DATE2 | func_8013820C | 768 | 1cd70b47 |
| GEKO | func_80141374 | 416 | b234c5e5 |
| GEKO | func_801351F8 | 328 | 1e2eb462 |
| GEKO | func_8013882C | 596 | 1e2eb462 |

Total **four functions / 2108 literal linked bytes**. List/instruction sizes can include embedded rodata and differ from the literal function bytes above. Final proof build/scenes13900/final-proof.log, affected overlay checks final-build.log; header/global/prototype logs pass with 0 new / 15 known disagreements. Body-audit.log confirms all **102 pre-existing C bodies** in three touched files are text-identical to 0e670a13 (19 DATE2, 44 + 39 GEKO). No shared canonical-header changes, no generated assembly/build-tool changes, no new FAKE forms and no disc data staged. This pass verifies the two affected overlays, not all 27 binaries. The overall 80% target remains ongoing with the integration chat.

Independent standards and spec reviews of 0e670a13...1e2eb462 found no actionable findings. Standards review checked CODING_STANDARDS.md, source and proof logs; spec review checked ownership, verification and preservation. Original checkout remained read only. Branch codex/scenes-13900 is ready for parent integration; no PR or push performed.

## Types-first pass (T-17700)

[[tickets/T-17700-types-first-geko]] recovered GEKO's shared types before function matching, redrafted the 35
unmatched GEKO functions with them and cut the rest into eight packages (branch `w3-geko-types`, from `w2-good`).
Drafts and drivers: `drafts/T-17700/` (README there). About 1 hour 45 minutes of agent time in all.

### Type map

All declarations are in `include/ovl/GEKO.h` (block `OVL_GEKO_TYPES_T17700`); matched code did not use any of the
symbols, so GEKO's sha1 stayed OK without source changes.

| Type | Symbol(s) | Used by (unmatched) | Evidence |
| --- | --- | --- | --- |
| `GekoMonthDays` (`s16 day[12]`, 0x18) | `D_80144D10`, `D_80144D28`, `D_80147838`, `D_80147C54` | `func_80132B24`, `func_80133024`, `func_80142C9C`, `func_80144198` | days per month (31, 28, ...); copied to the stack in six words, February patched to 29 for leap years |
| `GekoThresholds` (existing, `s16[11]`) | `D_80144CF8` | `func_80132B24` | same values as `D_80144CE0` of matched `func_80132A4C`; five words and a halfword copied |
| `GekoNames12` (`u8 name[12][20]`, 0xF0) | `D_80144D8C` | `func_80133C50` | twelve SJIS strings, a 0xF0 copy loop, `name[D_800E6374 >> 12]` passed to strcpy |
| `GekoWords11` (0x2C), `GekoWords2` (0x8) | `D_801451B8`, `D_801451E4`, `D_801451EC`, `D_801451F4` | `func_80136C20` | the `KangeiWords11`/`KangeiWords2` tables of the KANGEI sibling `func_8013454C` |
| `GekoFnTbl42` (`void (*f[42])()`) | `D_80146630` | `func_8013B10C` | 42 code pointers copied in a 0xA8 loop, called through `D_800E738A` |
| `GekoShorts10` (0x14) | `D_801472C4`, `D_801472D8`, `D_801472EC` | `func_801406CC` | three coordinate rows indexed by one loop counter |
| `GekoShorts6` (0xC) | `D_8014760C`, `D_80147618` | `func_801420E4` | 0xC copied each; the draft reads the copies as halfwords (`s16 sp4E` .. `sp56`) |
| `GekoBytes21`/`24`/`36`/`132`/`209`, `GekoText626` | `D_80147764`, `D_80147850`, `D_80147958`, `D_80147970`, `D_80147988`, `D_80147BFC`, `D_80147C24`, `D_80147C3C`, `D_80147C6C` | `func_80142C9C`, `func_80143F48`, `func_80144270` | byte and SJIS tables; the size is the size the copy moves (0x15 copies end with one `lbu`/`sb`) |
| `s16 D_801463E0[12]`, `s16 D_80146400[16]` | (relocations name `D_801463D8`, `D_801463F8`) | `func_8013E22C`, `func_80139E1C`, `func_8013E950` | per-record halfwords of the `Rec24` records 4..13, indexed with `i - 4` |

Not changed: the stride-0xC record tables that `tools/type_recovery.py` ranks first (`D_80146CB4`, `D_80146A14`,
`D_80146628`, ...; matched code reads them as separate `s32` globals), the 0x34-byte records around `D_80144F3C`,
`D_80144F70`, `D_80144FA4` (defined in `80134D10.c` one `INCLUDE_RODATA` line per word), and GameState (already a
struct; the redraft folds its splat labels).

### Matches and near misses

- `func_8013E22C` (484 B) matched as a port of its EVENT clone `func_8010564C` (siblings.py ratio 1.000):
  `Rec24` records of `D_801217D0`, `D_800E6280.unk_1228[5]` for EVENT's `D_800B1C1C`, `unk_10A5[i]` for
  `D_800B188D[i]`, the side arrays as `D_801463E0[i - 4]`.
- Guarded (`NON_MATCHING`) from matched siblings or the census draft, all register or schedule differences:
  `func_80136C20` (KANGEI `func_8013454C` form with the `u8 girl` local that sets the frame; one lui lands in
  the load delay of `D_80145090` instead of the delay slot of the `D_80145088` branch, 4 funcdiff rows),
  `func_80140040` (EVENT `func_80108DD4` form, do-while second loop; the tail loads `$ra` in the branch delay
  slot, 7 rows), `func_80134228` and `func_80135AC8` (the merged store of `unk_110D` through a `u8` local;
  `%hi` in `$v1` instead of `$v0`, 16 rows each), `func_801355E8` (SHOUGATU `func_8013BDE0` switch; the
  temporaries start at `$t6` where the original starts at `$t0`, all 112 instructions off by register number).
- `func_80133024`: 40 rows by hand from the 46-row typed draft (`drafts/T-17700/GEKO/func_80133024/hand.c`);
  the month table needs four bytes of locals above it and the loop keeps `unk_03F`/`unk_040` in `$a3`/`$a2`.

### Idioms

- A local declared before a stack table moves the table in the frame even when the local lives in a register:
  `u8 girl;` first put `func_80136C20`'s tables at `0x40` like the original (without it they sat at `0x44`).
- The census's state-machine shape for `unk_110D` (read into a local, increment on the zero path, one store at
  the join) is the original's: the asm stores `unk_110D` on both paths. The `if (unk_110D == 0) { ...; unk_110D
  += 1; }` form of the matched siblings stores only on the zero path.

### Redraft

`drafts/T-17700/redraft.py` is the T-17400 driver with three changes: the fold now runs after
`tools/m2c.py`'s `fix_shared_hi` and keeps the `%hi` offset (the old order made m2c print `D_800E6280.unk0` for
GameState bytes that share a `%hi`; 11 typed drafts of T-17200/T-17400 show it), `stack-elements` (single
elements and byte steps of a copied table, and the table's name read as a value, become `sp.f[k]`), and
`gamestate-records` (`*(&D_800E6280.unk_1BC[0].F + (i * 0x38))` becomes `unk_1BC[i].F`).

| | value |
| --- | --- |
| unguarded functions | 29 (27 compile; `func_80142C9C`, `func_80143F48` do not, census kept) |
| better than the census | 14 functions, 8,804 bytes |
| same / worse | 11 / 4 (the worse four start from T-16510 or hand-repaired census drafts) |
| diff rows, sum (median) | 6,067 to 5,536 (138 to 132) |
| typed vs plain | better in 11 of 25, worse in 5; 2 drafts compile only typed |
| rewrites used | stack-table 10, stack-elements 5, gamestate-records 2, fptab 1, display-records 1, record-temp 1 |
| largest gains | `func_80144270` 491 to 200, `func_801406CC` 283 to 219, `func_80132B24` 211 to 160, `func_80133024` 94 to 46, `func_80144198` 97 to 69 |
| `tools/draft_fixes.py` | no score changed |
| score 0 | none |

The guarded bodies were scored the same way (`drafts/T-17700/hand.py`): `func_80134228` and `func_80135AC8` 16,
`func_80136C20` 4, `func_80140040` 7, `func_80137580` 30 full-unit rows.

### Packages

Eight file-disjoint packages from the orchestrator's `mklists3.py` (lists in its scratchpad,
`w3-geko-p01-list.txt` to `w3-geko-p08-list.txt`):

| Package | Ticket | Files | Functions | Bytes |
| --- | --- | --- | ---: | ---: |
| w3-geko-p01 | T-17701 | `80134D10.c` | 5 | 2,660 |
| w3-geko-p02 | T-17702 | `801338F0.c` | 2 | 1,268 |
| w3-geko-p03 | T-17703 | `801323E0.c` | 5 | 2,668 |
| w3-geko-p04 | T-17704 | `80134040.c` | 2 | 676 |
| w3-geko-p05 | T-17705 | `8013F360.c` | 4 | 2,024 |
| w3-geko-p06 | T-17706 | `80139910.c`, `8013AF80.c` | 5 | 2,524 |
| w3-geko-p07 | T-17707 | `8013DF70.c`, `80142760.c`, `80140A00.c` | 5 | 3,116 |
| w3-geko-p08 | T-17708 | `80141AF0.c` | 1 | 1,260 |
