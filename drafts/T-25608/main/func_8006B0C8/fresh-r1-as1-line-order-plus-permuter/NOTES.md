# T-25608 fresh-start round 1, lens "as1 line order plus permuter": main func_8006B0C8

Result: MATCH (1,268 bytes, default C, clean build 27/27). The C is in `src/main/800674B0.c`. The last step is a
change to `tools/ugen_pass.py`, the block-copy state pass. The C alone leaves 82 rows under the old rule.

## Steps

1. Start: the digest's best body `fresh-r1-best.c`, 90 funcdiff rows (every access through `GameState *g`).
2. as1 order, words 133/136. The outer loop is `i = 0; while (i < n) { ...; i++; }`, with `i++` as the last
   statement, after the swap. Its increment then carries a later line than `k = i` and `j = i + 1`, and as1 places
   it as the original does. 90 to 82 rows (`as1-while-82.c`, commit 13c73c1f8). The #line trace (`trace.py`)
   showed the tie: on the `for` line the increment has the smallest line number and goes first.
3. The 82 rows left (words 228-311) were ugen temporaries one ring slot off, from the swap's second copy on.
   `ugstate.py`/`ugforce.py` (a scratch ugen shim that logs and forces the B/F state of each `Umov`) showed the
   pass choosing `BFFF` for [compaction copy, three swap copies]. Forcing all 16 combinations gave:
   `BBBB` 0 rows (MATCH), `BBBF` 31, `BFBB` 31, `BBFB` 39, `BFFF` 41 (current), `BBFF` 47, `BFFB` 49,
   any F on the first copy 109-125. The C was complete; only the copy state was wrong.
4. Why F: the pass locks F for the rest of a procedure once a signed positive-immediate `<` jump follows an
   indirect store. Here the jump is the sort loop's guard `fjp (n >= 1)` (ucode 266), and the stores are the
   month loop's bit-field stores (`istr`), with that loop's own back branch between them.
5. No C avoids it under the old rule. uopt rewrites every guard spelling to `geq n 1`: `if (0 < n) do ... while`
   (`v1.c`), `if (n > 0) do ... while` (`v2.c`) both still lock; `0 < n - i` (`v3.c`) and `i - n < 0` (`v4.c`)
   change the code (259 and 295 rows). The month stores are indexed, so they are always `istr`, and the guard
   must come after them.
6. Census (full build with the trace in `census-trace.diff`, output `census-trace.txt`, evaluated by
   `census_eval.py`, listed by `census_locks.py`): 565 procedures with a `Umov`; the lock changes a state in 14.
   In 11 of them the store is directly followed by a loop's back branch (`U Rb`). In the other 3, only stores,
   calls or parameter moves lie between (main `func_80054FC0`, the T-25105 FAKE; TEL `8013CCD0.c`; one unit not
   identified, `U Rf`). None of the 14 has a conditional jump between the store and the locking jump. Ours is
   `U Jb Rf`.
   Candidate rules against the old one (procedures whose states change):
   - C1, "the store is the last state op before the jump": 1 (breaks main `func_80054FC0`).
   - C3 "the last state op is not a jump", C6 "no fjp/tjp between the store and the jump",
     C8 "no backward fjp/tjp between": 0 each, and all three give ours `BBBB`.
7. Adopted C6, the simplest to state: `uist_armed` is set by `istr`/`istv` and cleared by every `fjp`/`tjp`, and
   the lock needs it set. Two unit tests were added to `tools/test_ugen_pass.py` (19/19 pass). Clean build 27/27, so
   no other object changed. check_headers, migrate_globals --check and sync_protos --check-branch pass.
8. Both FAKEs remain needed under the new rule: without the `goto swap; swap:` label, or without `pad[9]`,
   the function no longer matches.

## Mechanism

Block-copy state. The ugen pass locked state F at a loop guard that follows indirect stores in an earlier loop. The
original compiled the three swap copies in state B (two scratch registers spent before each copy's transfer
address, not one). Every later ugen temporary was one slot off.

## Proposal

Merge the C and the `tools/ugen_pass.py` lock rule together (wiki: toolchain "Block-copy state pass", matching-notes
"T-25608"). The rule meets CODING_STANDARDS 7a: it is uniform, documented, and tested; the census is the evidence;
there are no exceptions and no per-function switch. The only evidence is the census plus this function. No other
function in the corpus separates the old rule from the new one. If the rule is rejected, the fallback is the 82-row
guard (`as1-while-82.c`) as NON_MATCHING. I found no C spelling that avoids the lock under the old rule.

Unrelated: `python3 -m unittest discover -s tools` has 11 failures in `test_build_deps` (its fixture tree lacks
`tools/ugen_pass.py`) and 1 error in `test_protoverride`. I did not touch either test, and both fail for reasons
outside the pass logic.

## Permuter

A run on the 82-row body (`lowprio.sh`, container f-b0c8-as1-perm) ran about 40 minutes at score 40 with no gain.
I stopped it once the copy state turned out to be the cause, because it compiles through this worktree's
`tools/ugen_pass.py` and its scores would have changed with the rule.
