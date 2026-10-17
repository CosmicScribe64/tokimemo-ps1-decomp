---
id: T-20820
title: "migrate_globals: keep lines backed by a byte-match necessity proof (counter-tail family)"
status: In Progress
assignee: tr-keep-proof agent
created: 2026-10-17
updated: 2026-10-17
links: ["[[decompile-workflow]]", "[[game-state]]", "[[data-types]]", "[[data/t0018-cases]]", "[[keep-lines]]"]
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

- [ ] `tools/keepproof.py` with `check`, `census` and `rows`; tests `tools/test_keepproof.py`.
- [ ] `migrate_globals.py`: backward-compatible `keep <file> <symbol> proof=<func>:<rows_without>:<asm_sha1_8> <reason>`;
      `--check` accepts the hoisted-pair evidence or a proof field whose function is plain C and whose asm sha1
      matches; `--verify-proofs` recompiles (necessity and sufficiency). Tests in `tools/test_migrate_globals.py`.
- [ ] `wiki/keep-lines.md`: proposed rule text for the lead, linked from [[game-state]] and [[data-types]].
- [ ] Docs in [[decompile-workflow]]; real-tree validation with numbers (census false-positive rate).
- [ ] No `src/` keep line added (gate: lead approval of [[keep-lines]]); full tool suite passes.

## Notes

Worktree `tokimemo-wt/tr-keep-proof`, branch `tr-keep-proof` (from `w2-good`). Touches only `tools/`, `wiki/`.
After approval a wave-4 package applies the line to the four TACO functions.

## Comments
