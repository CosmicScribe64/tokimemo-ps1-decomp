---
id: T-20820
title: "migrate_globals: keep lines backed by a byte-match necessity proof (counter-tail family)"
status: In Review
assignee: tr-keep-proof agent
created: 2026-10-17
updated: 2026-10-17
links: ["[[decompile-workflow]]", "[[game-state]]", "[[data-types]]", "[[data/t0018-cases]]", "[[keep-lines]]", "drafts/T-20820/"]
---

## Goal

Replace the open policy question "may a `keep` line cite a live-range proof instead of a hoisted pair?" with a
checkable rule, without touching `src/` or `CODING_STANDARDS.md`. Six notes from three packages (T-19002,
T-19013, T-19019) hit one wall: the original reads the frame counter at 0x800E7384 through a second uopt
variable for the final `+= 1`; plain C reproduces that only with a separate `D_800E7384`, and
`migrate_globals --check` rejects a `keep` line without an as1 hoisted pair (T-7010). TACO `func_8014616C`
(0x5E8), `func_80146754` (0x604), `func_801484E4` (0x188) and `func_80149538` (0x2B4) are 4,136 B in 4
functions; six more unmatched functions in [[data/t0018-cases]] show the same symptom.

## Acceptance criteria

- [x] `tools/keepproof.py` with `check`, `census` and `rows`; tests `tools/test_keepproof.py`.
- [x] `migrate_globals.py`: backward-compatible `keep <file> <symbol> proof=<func>:<rows_without>:<asm_sha1_8> <reason>`;
      `--check` accepts the hoisted-pair evidence or a proof field whose function is plain C and whose asm sha1
      matches; `--verify-proofs` recompiles (necessity and sufficiency). Tests in `tools/test_migrate_globals.py`.
- [x] `wiki/keep-lines.md`: proposed rule text for the lead, linked from [[game-state]] and [[data-types]].
- [x] Docs in [[decompile-workflow]]; real-tree validation with numbers (census false-positive rate).
- [x] No `src/` keep line added (gate: lead approval of [[keep-lines]]); full tool suite passes.

## Notes

Worktree `tokimemo-wt/tr-keep-proof`, branch `tr-keep-proof` (from `w2-good`). Touches only `tools/`, `wiki/`.
After approval a wave-4 package applies the line to the four TACO functions.

## Comments

2026-10-17 (tr-keep-proof agent): built, tested, committed on branch `tr-keep-proof` (from `w2-good`). Rule proposal and all numbers: [[keep-lines]]; reports in `drafts/T-20820/`.

- **Tools.** `tools/keepproof.py check UNIT:func --symbol S [--src FILE | --from-draft] [--declare TYPE]` (necessity: unit compiled with the symbol folded by `migrate_globals.rewrite_text`, rows > 0; sufficiency: `funcdiff_verify.exact_match` on the committed plain C; prints the `keep ... proof=` line), `census` (counter-tail shape in all original asm, tags of the matched C), `rows` (drafts with each field as a separate symbol). `migrate_globals.py`: new syntax, `--check` verifies plain C and the asm sha1 (first 8 hex digits of the sha1 over the instruction words), `--verify-proofs` recompiles, one proof line per function, uses of a proof-kept symbol outside the proven functions are listed, old hoisted-pair lines unchanged. `sync_protos.py` no longer warns about a declared symbol that has a keep line.
- **Real tree.** Census over 6,958 functions (6,348 matched): the shape is in 371 (299 matched, 72 unmatched, 83,108 B); all 299 matched ones are plain C with no keep, volatile or absolute address (false-positive rate 100%, 11 of 11 for the held-across-a-call form); the necessity proof has none by construction. `check --from-draft` on scratch text of the committed drafts: TACO `func_8014616C` PROVEN (1,512 B linked equal, 165 rows without, sha `bb8762f0`), `func_80149538` PROVEN (692 B, 33 rows, `d7dd7f6c`), `func_801484E4` necessary but not sufficient (1 row left, the first spill `sw v0` for `sw t0`); `rows`: `func_80146754` 158 rows as m2c draft, 2 with the tail symbol; the six other candidates compile to identical words with separate symbols (no keep-line candidate). `migrate_globals --check` on a scratch root with the real proof line: accepted; stale sha1 and a guarded function: rejected.
- **Deviations from the spec text.** (1) The text variants are compiled with `order_search.compile_variant`, the text form of `funcdiff_verify.compile_unit` (that one reads `c.src` from disk, and `trailing_pad`/`source_extent` resolve by the real path). (2) `rows_without` is the larger of the removed and added lines of `funcdiff.diff_lines` (165 for `func_8014616C`; the notes' 242 are funcdiff lines of another draft). (3) Folding the symbol keeps the file's other absorbed names as written, so only the tested symbol changes. (4) A proof line is accepted when the proof holds or the hoisted pair holds (literal OR).
- **Limits.** The census is static and the proofs hold for the text `--from-draft`/`--src` builds, not for a committed file (none exists yet). `rows` tries one field, all fields and the last statement; other splits are untried. No `src/`, `include/`, `config/` or `CODING_STANDARDS.md` change.
- Tests: `tools/test_keepproof.py` 47, `tools/test_migrate_globals.py` 24, `tools/test_sync_protos.py` 76; full suite result in the log.
