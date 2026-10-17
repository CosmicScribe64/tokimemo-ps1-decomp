---
type: concept
updated: 2026-10-17
sources: ["tools/keepproof.py", "tools/migrate_globals.py", "tools/aggregate_audit.py", "config/migrate_globals.txt", "wiki/game-state.md", "wiki/data/t0018-cases.md"]
---

# Keep lines backed by a byte-match necessity proof (T-20820)

Proposed rule for the lead. Nothing in `src/`, `include/` or `CODING_STANDARDS.md` changes until it is approved; the tools below are built and tested, and no `keep` line exists in `config/migrate_globals.txt`.

## The question

[[game-state]] says every unit reaches the frame counter and the other `GameState` fields through `D_800E6280`, because the original has no separate symbol for them (T-7010: 407 read-modify-write pairs kept in order, 0 hoisted). A `keep` line gives one C file its old separate symbol back and `migrate_globals.py --check` accepts it only when the original object has an as1 "hoisted pair" on that address.

Six notes from three work packages (T-19002, T-19013, T-19019) hit a case the hoisted pair cannot show. In TACO `func_8014616C`, `func_80146754`, `func_801484E4` and `func_80149538` the original reads the frame counter 0x800E7384 for a chain of compares and reads it again, as a second uopt variable, for the final `+= 1`. With `GameState` fields IDO joins both into one register web (`func_8014616C` is 165 rows off); with a separate `D_800E7384` for the tail alone `func_8014616C` and `func_80149538` match byte for byte, and the other two get close. Three closers asked the lead whether a `keep` line may cite a live-range argument. This page replaces that question with a check.

## Proposed rule

A `keep` line (a separate symbol for one field of an aggregate, valid in one C file) needs one of two kinds of evidence. `migrate_globals.py --check` verifies both.

1. **Hoisted pair** (T-7010, unchanged): `keep <src file> <symbol> <reason>`; the original object has a load of that address hoisted above a store to another field of the aggregate.
2. **Necessity proof** (new): `keep <src file> <symbol> proof=<func>:<rows_without>:<asm_sha1_8> <reason>`, one line per function. It claims four things:
   - `<func>` is plain C in the file: defined, not `INCLUDE_ASM`, not behind a `NON_MATCHING` guard;
   - with the symbol folded into the aggregate (`D_800E7384` written as `D_800E6280.unk_1104.w`, everything else unchanged) the unit's `<func>` differs from the original in `<rows_without>` > 0 instruction rows (`funcdiff` with relocations resolved);
   - as committed, with the separate symbol, `<func>` links to the original's bytes (`funcdiff_verify.exact_match`, the literal-byte proof of a funcdiff MATCH);
   - `<asm_sha1_8>` is the first 8 hex digits of the sha1 over the instruction words of the function's original asm (`migrate_globals.asm_sha1`, addresses and names excluded).

   `--check` verifies the cheap part without compiling: the function is plain C in the file and the sha1 matches (a stale sha1 or a guarded function is an error). `--verify-proofs` recompiles and repeats necessity and sufficiency for every proof line. The symbol may be used only inside the functions its proof lines name; `--check` lists any other use, which must be written as the field. `tools/keepproof.py check UNIT:func --symbol D_X` runs both compiles and prints the finished line.

Decisions for the lead:

- **Marking.** Recommended: each use of a kept symbol carries a `FAKE` comment (CODING_STANDARDS section 7) that names the function's proof line and the ticket, because a second name for a field is not what the original author plausibly wrote; the proof says it is the only form known to compile to the original. The alternative is a plain comment (section 9), on the argument that the original asm itself shows two variables.
- **Where the rule lives.** If approved: one sentence in `CODING_STANDARDS.md` section 8 ("a `keep` line needs a hoisted pair or a necessity proof, wiki/keep-lines.md") and the update of the "Rule for the future" paragraph of [[game-state]]. This page has the details.
- **Declaration.** `--check` requires the kept symbol to be declared in `include/main_api.h` (`extern s32 D_800E7384;`, its old type). The package that applies the line adds the declaration back; `sync_protos.py` no longer warns about a declared symbol that has a `keep` line.

## Why a proof and not a rule about live ranges

The shape of the four functions can be searched for in the original asm. `keepproof.py census` does it for all 6,958 functions of the tree (6,348 matched). The pattern is: a first load of an aggregate address, a later load of the same address with no store in between, arithmetic on the reload stored back, and the first value read between the two loads; "held" means that the first value is still read after a call (a callee-saved copy or a spill), the literal `func_8014616C` form.

| | functions | of those matched |
|---|---|---|
| pattern present | 371 (299 matched, 72 unmatched with 83,108 B) | 299, all plain C |
| pattern on the counter 0x800E7384 | 52 (33 matched, 19 unmatched with 27,948 B) | 33, all plain C |
| held across a call | 22 (11 matched, 11 unmatched with 14,368 B) | 11, all plain C |
| whole tree: matched functions with a keep, a volatile access or an absolute address of an aggregate field in their C | 0 of 6,348 | |

False-positive rate of the asm shape on matched code: **299 of 299** (and 11 of 11 for the held form). Among them 47 carry a `FAKE` comment for another reason and 22 use a `GameState *g` pointer local; none has a `volatile` access to the aggregate, an absolute address inside it, or a kept symbol. Matched code with the held form exists in 11 functions; on the counter itself there are main `func_8007C8D8`, ETC `func_8013C330` and RPG_BAT `func_801529B4` (the others read the mode byte 0x800E738A), and all match with fields. A live-range argument therefore cannot be the evidence: it holds for functions that do not need the symbol.

The necessity proof has no false positives on matched code by construction: a matched function is plain C with fields that already matches, so its `rows_without` is 0 and the proof is refused (the check stops after the first compile). The cost is the compile: about 35 s for a whole `check` on this machine.

## What the tool found

Run on scratch copies of the unit files (nothing in `src/` was edited; the scratch file is the committed file with the function written as plain C and its last statement `D_800E7384 += 1;`):

| function | bytes | separate symbol for the tail | folded (plain draft) | verdict |
|---|---|---|---|---|
| TACO `func_8014616C` | 1,512 | links equal | 165 rows differ | PROVEN, `proof=func_8014616C:165:bb8762f0` |
| TACO `func_80149538` | 692 | links equal | 33 rows differ | PROVEN, `proof=func_80149538:33:d7dd7f6c` |
| TACO `func_801484E4` | 392 | 1 row differs (first spill `sw v0` against `sw t0`, with the FAKE `s32 pad[2]`; 3 rows without it) | 32 rows differ | not sufficient: the symbol is necessary but not enough |
| TACO `func_80146754` | 1,540 | not run: only the raw m2c draft exists (below: 158 rows, 2 with the tail symbol) | | no plain C yet |

Draft rows (`keepproof.py rows`, committed guarded draft or the newest archived draft, NON_MATCHING compiled):

| function | draft | rows as written | every use a separate symbol | only the last statement |
|---|---|---|---|---|
| TACO `func_8014616C` | guarded, absolute-address FAKE | 3 | 165 | **0** |
| TACO `func_80146754` | archived m2c draft (T-16560), never polished | 158 | 158 | **2** |
| TACO `func_801484E4` | guarded, volatile FAKE and `pad[2]` FAKE | 6 | 32 | **1** |
| TACO `func_80149538` | guarded, volatile FAKE | 5 | 33 | **0** |
| OMIMAI `func_8013388C` | archived typed draft (T-19012) | 298 | 298 (4 fields at once: 298) | 298 |
| main `func_800590CC` | guarded | 13 | 13 (4 fields at once: 13) | 13 |
| main `func_800591D8` | archived (T-17000) | 39 | 39 (4 fields at once: 39) | 39 |
| TACO `func_8013506C` | guarded | 16 | 16 | 16 |
| SHOUGATU `func_801347E0` | guarded | 3 | 3 (2 fields at once: 3) | 3 |
| main `csr_load_vram` | guarded | 13 | no scalar field used (array accesses only) | |

How to read it. The separate symbol helps only when one function mixes both views: the compare chain reads the field, the tail reads the symbol, and uopt then sees two variables. Written everywhere, the symbol compiles exactly like the field. For the six candidates at the bottom of the table the numbers do not move; for four of them the resolved instruction words of the draft with every field of the function as a separate symbol were compared with those of the draft as written and are identical (OMIMAI `func_8013388C` 487 words, main `func_800590CC` 71, `func_800591D8` 80, SHOUGATU `func_801347E0` 44). That is the T-7010 result again. `rows` tries one field at a time, all fields at once, and the last statement; another split of the uses is not tried.

The numbers in the proof lines are those of the scratch run; a package that commits the function recomputes them with `tools/keepproof.py check` and writes the line it prints.

## Not covered

The six other functions of [[data/t0018-cases]] whose notes say that the original reads a separate symbol (OMIMAI `func_8013388C`, main `func_800590CC`, `func_800591D8`, `csr_load_vram`, TACO `func_8013506C`, SHOUGATU `func_801347E0`) do not have the counter-tail shape (the census does not list them), and the table above shows that separate symbols do not change their drafts. They are not candidates for a proof line; their remaining rows are register allocation and scheduling that the symbol does not move. If one of them reaches a draft where the symbol makes a difference, `check` gives the same answer for it.

## Next steps (not started)

1. Lead decision on the rule and on the marking.
2. Wave-4 package for the TACO counter tail (about 1 hour): re-add `extern s32 D_800E7384;` to `include/main_api.h`, write `func_8014616C` and `func_80149538` as plain C with the tail `D_800E7384 += 1;` (`tools/keepproof.py check UNIT:func --symbol D_800E7384 --from-draft --write-scratch FILE` writes the plain C text it proves from the committed guarded draft), run `keepproof.py check` for each, add the proof lines, run `migrate_globals.py --check`, build. `func_80146754` and `func_801484E4` need work beyond the symbol.
3. TACO `func_80146754` (1,540 B) goes from 158 to 2 rows with the tail symbol alone: worth a polishing package once the rule is approved.
4. The census lists 72 unmatched functions with the shape (83,108 B; `drafts/T-20820/census.json`); the 19 on 0x800E7384 (27,948 B) are the first to try. The shape alone proves nothing, so each one needs a draft and a `check`.

See also [[decompile-workflow]] ("Keep lines with a proof") and [[tickets/T-20820-keep-proof]].
