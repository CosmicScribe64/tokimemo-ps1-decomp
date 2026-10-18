# T-24009: ENDING scheduler closer

No match and no production-source change. `func_8013A2E8` in `src/ovl/ENDING/801396E0.c` retains its T-23002 guard. Its target is 4,648 instruction bytes, or 1,162 instructions. The default build uses assembly; a default MATCH does not count as C progress.

## New evidence

The existing unused `s32 pad` slot can be retyped as `char *`, assigned the first string immediately before the flag chain, read by an empty `if (pad) {}`, and passed to the first string-copy call. The full-unit resolved score becomes 10 (baseline 12). This is an allocation experiment requiring a FAKE comment, not recovered original source.

The candidate fills the original load-delay slot with `lui a1` and aligns all 1,162 instructions. Eight words still differ (16 signed diagnostic rows): the string low address occupies the first branch delay instead of the second flag mask; the first call uses the buffer low address in its delay; the following base reload and branch shift; the second flag mask stays after the arm. Score 10 is the weighted/aligned scorer result, not a count of differing words or printed rows.

This is useful evidence against treating the load-delay nop as unavoidable in the full function: a source lifetime can bypass that symptom without shrinking the function. It does not invalidate the earlier as1 whole-function threshold evidence, and it does not establish a complete source solution.

## Bounded probes

76 sequential full-unit probes, one compiler at a time, Docker only:

- Baseline and explicit `(void)` discards: 12. Casting the last 1..9 `func_8013B640` calls to void-return views: 20, 24, 27, 42, 347, 353, 337, 340, 343. Empty cursor tests after tail calls: 389.
- First-string pointer/unsigned-word local plus an empty truthiness test: 10. Explicit pointer `== 0` / `!= 0` often restores 12. `register` does not help.
- Second-mask precomputation, empty flag tests around the first arm, or a buffer-address local: no score below 10; some allocate new ranges and score 660 or more.
- All six contiguous line-groupings of the four-line first-string setup/first call region: 10. Pointer argument casts and `+ 0` / element-zero spellings: 10.
- Empty tests of the pointer's low half, high half, sign or low bits: 10. Empty pointed-byte reads: 12. Equality/inequality with the destination buffer: 141.

No binasm deletion, `.loc` removal, toolchain patch, per-function flag, or inline-assembly experiment was repeated or added. The earlier T-23002 observations remain in the ticket and matching notes.

## Reproduction

Run `tools/docker.sh python3 drafts/T-24009/probe.py` from the repository root. It writes only ignored `build/t24009/repro/` output and reports baseline 12 / pointer-lifetime 10. To see the candidate's exact words, run `tools/docker.sh python3 tools/funcdiff.py --unit ENDING --built build/t24009/repro/pointer-lifetime/guarded.o --resolve func_8013A2E8`.

The existing production guard was retained because the candidate remains nonmatching and adds a new allocation device. Independent review is tracked by `wiki/tickets/T-24009-ending-scheduler-closer.md`.
