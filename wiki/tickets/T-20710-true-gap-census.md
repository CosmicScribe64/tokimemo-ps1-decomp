---
id: T-20710
title: "True compiler-gap census and uniform allocator rule search"
status: In Review
assignee: bs-gap-census agent
created: 2026-10-17
updated: 2026-10-17
links: ["[[ido-52-evaluation]]", "[[uopt-register-allocation]]", "[[speedup-research-2026-10-16]]", "[[data/t0018-cases]]", "[[toolchain]]"]
---

## Goal

(1) Census: list the still-unmatched non-TT functions whose remaining difference is registers or instruction order
only, after the known source recipes have been tried (register-cheatsheet, T-20600 recipes, T-20610 truth tests,
T-20620 line grouping, implicit-int views through `regdiff`/`funcdiff --proto`). For each: the first divergence (uopt
colour, ugen temporary or as1 order), the allocator decision involved, and a minimal C reproducer where feasible.
Saved as `wiki/data/true-gaps.md`.

(2) Rule search: with the T-9800 traced uopt and the T-20610 offline method (dump cfe ucode once, replay, recompile
only files whose uopt input changes), look for one uniform tie-break or ordering difference in the allocator (or in
the 5.2/4.1 components) that explains several census cases and changes 0 matched functions. The eight T-9800
candidates are not repeated.

## Acceptance criteria

- [x] `wiki/data/true-gaps.md` with one row per census function (first divergence, decision, reproducer if any)
- [x] rule search results with counts (targets gained, matched functions changed) in the ticket and in
      [[uopt-register-allocation]] and [[ido-52-evaluation]]
- [x] IDO 5.2/4.1 only from the recorded URLs, sha256 checked, kept in the gitignored `tools/local-compilers/`
- [x] a build step only if it is one uniform rule, evidenced, keeps every matched function byte-identical: none
      qualifies, none added
- [x] clean rebuild 27/27, check_headers, migrate_globals --check, sync_protos --check-branch

## Notes

Started 2026-10-17 01:48 UTC, worktree `bs-gap-census` from `w2-good`, time box about 3 hours.

## Results

**Census** ([[data/true-gaps]]). 87 guarded non-TT drafts swept with `funcdiff --resolve`: 9 `reg`, 8 `order`,
3 `reg+order`, 15 `near` (at most two unpaired instructions), 52 other. `regdiff -v` on the 55 with at most six
unpaired instructions (32,308 bytes) gives the first divergence: uopt 28 (range set 15, priority 7, colour order 4,
tie 2), as1 order 12, ugen 11 (block-copy state 5, temporary numbering 6), operand order 1, local layout 1, recipe
known but not applied (busy files) 2. Two drafts have no recipe pass recorded (GEKO `func_80134228`, `func_80135AC8`).

**uopt rule search.** Only 2 of the 28 uopt decisions are ties, both first-use order of hoisted values (the case DATE
`func_80136F2C` solved in C). No tie-break candidate beyond the eight T-9800 ones can gain more than 2, and those
change 661 to 1,724 matched functions; no new uopt variant was built.

**IDO 4.1 components** (archives re-downloaded, sha256 equal to [[ido-52-evaluation]]'s table, gitignored
`tools/local-compilers/`, run only in Docker). One pass at a time on the build pipeline (K&R, cvt pass,
asm-processor), 5.3 control identical in code for all 47 units:
- 4.1's ugen on the 55: TACO `func_8013B2A0` MATCH; TACO `func_801472D4`, `func_8015B1B8`, main `func_80054C74` get
  all of the original's registers (only 4.1's `lwl`/`lwr` expansion left); DATE `func_801518B8` and TAIIKU
  `func_80140A90` get the original's copy state; 36 drafts get worse.
- The cause: the T-19001 block-copy state. Six reproducers in `tools/ido_eval_cases/ugen_copy_state/`: 5.3 and 5.2
  put a struct copy into state F after a loop or a conditional block, 4.1 does not (all three do after an indirect
  store). The original's 15 matched end-pointer copies are all B (unchanged under 4.1's ugen); its 5 F copies are in
  unmatched functions after field stores.
- 4.1's ugen on the matched set (offline: ugen input of all 431 IDO files dumped once, 5.3's ugen + frame pass vs
  4.1's ugen, both through 5.3's as1): 1,307 of 6,337 C functions change (unaligned expansions 387, in-place
  narrowing 342, other registers 387, ...). Not usable as a whole.
- 4.1's as1: no gain on the census (18 of 21 drafts unchanged, 3 worse) and on the matched set it equals 5.3's as1 in 6,336 of 6,337 C functions (main
  `func_80046684`: two independent pairs in the other order). A first run with 5.3's
  `pic0` option record showed a VALEN `func_80134030` match that was an option artifact (4.1's as1 then leaves
  delay slots empty in 1,054 matched functions).
- 5.2's ugen and as1 give exactly 5.3's lines on the six block-copy cases.

**Best candidate, not adopted:** "ugen does not enter block-copy state F at a block start; only an indirect store
does" (IDO 4.1's behaviour). Gains: the copy registers of 6 census functions (one full match with 4.1's ugen).
Matched set: 15 of 15 end-pointer copies consistent; 35 matched functions have F copies under 5.3, 17 of them
identical under 4.1's ugen, 18 confounded by 4.1's other differences. Missing: an implementation in 5.3's ugen (no
ugen source; a binasm pass would have to renumber every later ugen temporary), its regression on those 35
functions and the 27 units, and the C of the original's 5 F copies.

Scratch harness and data (not committed; session scratchpad `t20710/`, moved out of `build/` before the clean
rebuild): altcc.py (one IDO pass swapped for 4.1's or 5.2's), ugdump.py (ugen input of every IDO file), ugcmp.py and
ascmp.py (offline 4.1-vs-5.3 comparison per C function), copyscan.py (block copies of the original asm),
census_rows.json.

## Comments
