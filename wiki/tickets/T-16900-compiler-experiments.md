---
id: T-16900
title: "Compiler experiments: selector-copy rule and ugen survey"
status: Done
assignee: claude-opus (blissful-fermi-43d29a)
created: 2026-10-15
updated: 2026-10-16
links: [[[matching-breakthroughs]], [[uopt-register-allocation]], [[toolchain]], [[tickets/T-11570-w1-ovl-name_ent-80139740]], tools/cvt_pass.py, tools/regdiff.py]
---

## Goal

Two time-boxed experiments that decide whether more compiler work pays for the remaining matching:

1. The signed narrow selector-copy rule from [[tickets/T-11570-w1-ovl-name_ent-80139740]] (item 4 of
   [[matching-breakthroughs]]): does a uniform change to `tools/cvt_pass.py` (widening after narrow signed global loads,
   preserving a local copy of a global, or both) match the 7 NAME_ENT selector-copy drafts without changing a matched
   function?
2. A regdiff survey of the drafts 21 to 100 diff lines from matching (`drafts/T-14500/best-quality-catalog.jsonl`, no TT):
   which pass (uopt or ugen) makes the first register difference? Decides whether a 3-day ugen trace is worth doing.

No `src/` file changes on this branch. The scratch copies of the pass and the frozen drafts stay outside the repository.

## Acceptance criteria

- [x] Experiment 1: the 7 targets and the matched controls frozen; widening, copy preservation and both tested separately.
- [x] Experiment 1: any variant that helps gets the full matched regression (clean rebuild, 27/27 sha1, zero changed
      matched functions). Success: at least 3 of 7 targets match with 0 matched functions changed; otherwise a
      negative result.
- [x] Experiment 2: regdiff over the 21-100 drafts; share of first register differences in ugen temporaries ($t6-$t9,
      $at). Verdict: ugen trace if 25% or more.
- [x] Results in this ticket and in [[uopt-register-allocation]]; log entry.

## Notes

Verdicts: experiment 1 meets the bar, but only just (3 of 7 targets match, 0 matched functions changed). The rule
that does it fires in no matched function, so the zero is no evidence for it. Experiment 2 is below the bar (23.6%
ugen against 25%): no ugen trace.

### Experiment 1: signed narrow selector-copy rule

Setup. Scratch copy of the pass (`cvt_variant.py`, outside the repository) that imports `tools/cvt_pass.py` and adds
rules selected by an environment variable; with no rule it is byte-for-byte the production pipeline. The 7 targets
(the guarded drafts of `func_8013D4CC`, `func_8013DAC4`, `func_8013E430`, `func_8013F0DC`, `func_8013F454`,
`func_8013FB8C`, and the natural `n` draft of `func_8013CEC8` from commit 64965d57) were frozen in a scratch copy of
`src/ovl/NAME_ENT/80139740.c`; NAME_ENT `func_80134110` (80132000.c, the T-0018 `move v0,v1` return) was an extra
probe. The notes list no "11 copy-propagation functions"; T-11570 names only these 7 (plus `func_80134110` as the same
shape). Score: resolved diff rows against the original (funcdiff's `diff_lines`); the base column reproduces the
T-11570 numbers exactly. Controls: every matched function, through `tools/uopt_eval.py regress` (per-function
comparison against the verified build) and a clean rebuild.

Rules tried:
- `ws:L`/`ws:J`, `wsc:L`/`wsc:J`: a `CVT J<-L` or `CVT J<-J` after narrow signed global loads (`LOD J`, length 1/2),
  where the ANSI front end would widen an unsigned load (`ws`) or only before a comparison (`wsc`).
- `wsp`/`wsp2`: after uopt, constant first in `==`/`!=` against a narrow signed global, the order uopt gives a widened
  (non-plain) operand. `wsp` reuses the pass's `_constant_first`; `wsp2` classifies a register operand by its most
  recent definition instead of every definition in the procedure.
- `cps`/`cpa`/`cp0`: zcopy off (`OPTN 405 0`) for procedures that store a direct narrow signed global load into a
  local (`cps`), any global load into a local (`cpa`), or for every procedure (`cp0`).
- `cpw` (coupled): only in a procedure that stores a direct load of a narrow signed global G into a local *and*
  compares G with a constant (`==`/`!=`): zcopy off, and after uopt constant first against G (`wsp2`), nowhere else.

| function | base | `cps` | `ws:L` | `wsc:L` | `ws:J`, `wsc:J`, `wsp`, `wsp2` | `ws:L`+`cps` | `wsc:L`+`cps` | `wsp2`+`cps` | `cpw` |
|---|---|---|---|---|---|---|---|---|---|
| `func_8013DAC4` | 27 | 4 | 10 | 6 | 27 | 14 | 10 | **0** | **0** |
| `func_8013F454` | 23 | 4 | 14 | 10 | 23 | 8 | 23 | **0** | **0** |
| `func_8013FB8C` | 27 | 4 | 12 | 8 | 27 | 14 | 10 | **0** | **0** |
| `func_8013D4CC` | 33 | 10 | 18 | 14 | 33 | 20 | 16 | 6 | 6 |
| `func_8013E430` | 28 | 8 | 35 | 31 | 28 | 18 | 14 | 4 | 4 |
| `func_8013F0DC` | 18 | 20 | 22 | 18 | 18 | 24 | 20 | 20 | 18 |
| `func_8013CEC8` | 42 | 14 | 126 | 51 | 42 (`wsp2`: 40) | 18 | 25 | 10 | 10 |
| `func_80134110` (extra) | 62 | 56 | 62 | 62 | 62 | 57 | 56 | 56 | 62 |

`cpa` and `cp0` give the `cps` column on the targets. Findings:
- Copy preservation alone gives the T-11570 result (4/4/4 lines); the 4 lines are the two compares,
  `beq v1,a1` against the original's `beq a1,v1`.
- A real widening record does not give the constant-first order. uopt folds `CVT J<-J`; `CVT J<-L` turns the load
  into an expression with its own temporary and makes most targets worse.
- The production `_constant_first` helper misses these compares because the constant's register also passes a call
  argument in the same procedure (mixed definitions). Classifying by the latest definition (`wsp2`) fixes that.
- Copy preservation plus constant first (`wsp2`+`cps`) matches 3 targets.

Regression of `wsp2`+`cps` (all matched C, `uopt_eval regress`): 5931 of 5950 functions identical. Two of the 19
(BUNKASAI `func_80153FA4`, `func_80158E3C`, 2 rows each: trailing nops) also differ with the unmodified pass and their
objects are byte-identical to the base compile, so they are a comparison artifact of `uopt_eval`, not a change. That
leaves 17 changed matched functions: `cps` alone changes 12 (ENDING `func_8013B510`, `func_8013B640`; NAME_ENT
`func_80133D94`, `func_80133E54`, `func_80149E3C`, `func_8014AF28`; OPTION `func_80134804`, `func_80134C00`,
`func_80134E64`, `func_801368FC`, `func_80137E38`; TAIIKU `func_801449A0`), `wsp2` alone 5 (DATE `func_8013D8B0`,
ENDING `func_80137F64`, `func_8013855C`, `func_80138A9C`, `func_80138DC8`). Both uncoupled rules fail.

Regression of `cpw`: 0 changes in the 9 files that changed before. Clean rebuild with `tools/cvt_pass.py` temporarily
replaced by a wrapper around the variant (uncommitted, reverted): 27/27 sha1 OK, globals OK. Overlay proof: with the
three drafts temporarily active in `src/ovl/NAME_ENT/80139740.c` (uncommitted, reverted), NAME_ENT.bin sha1 OK. So
`func_8013DAC4`, `func_8013F454` and `func_8013FB8C` (920 bytes) match from plain C under `cpw`.

Why this is weak evidence. The `cpw` log shows the rule fired in no procedure of the production build: no matched
function has the coupled shape. It fires in 6 procedures of the frozen target file. The zero-change result therefore
says the rule is harmless to the matched set, not that it is right. Its two halves, applied more widely, each
contradict matched code (12 and 5 functions above). The constant-first half also has known exceptions in the original
(`lh` globals constant-first 40 to 8, T-7000). Under CODING_STANDARDS 7a ("derived from the original code over a broad
sample") the rule rests on one family in one file.

Proposal (not done here): a uniform `cpw` rule in `tools/cvt_pass.py` is justified by the protocol's success criterion.
Before adopting it, count the coupled shape in the original's asm (narrow signed global in a register, a constant-first
compare of it, and a `move` copy that the code then uses), list the exceptions, and add synthetic tests. Gain if
adopted: 3 functions (920 bytes) matched, 3 more brought to 4-10 lines.

### Experiment 2: regdiff survey of the 21-100 line drafts

Selection: `drafts/T-14500/best-quality-catalog.jsonl`, score 21 to 100, no TT: 268 drafts, 262 still `INCLUDE_ASM`
in the tree (6 matched since). Each draft's stored full TU (`context.c`) was run through `tools/regdiff.py` with the
T-9800 traced uopt (copied into this session's scratch `r9-uopt/variants/trace`). 31 stored TUs no longer compiled
(asm paths moved since T-14500); they were rebuilt from the current source the way `tools/exhaustive_drafts.py` places
a draft, and 7 still fail (ETC `func_8013D32C`, `func_8013E89C`; OPTION `func_8013A09C`, `func_8013A1BC`; RPG_BAT
`func_80140E70`; SHOUGATU `func_80137664`; main `schedule_girls_gakko`). About 10 minutes of compute at 2 jobs.

| first register divergence (262 unmatched drafts) | count |
|---|---|
| ugen temporary in the draft (t6-t9, `$at`) | 59 |
| uopt priority order | 87 |
| uopt extra or missing live range | 37 |
| no trace range holds the draft register | 25 |
| uopt tie | 22 |
| draft in a uopt colour, original in a ugen temporary | 20 |
| no register difference (structure, operand order or constants only) | 5 |
| regdiff error | 7 |

Share: 59 of 250 first register divergences are ugen temporaries on the draft side, **23.6%**, against 49 of 138
(36%) in the earlier T-10400 sample. Other cuts: audit-clear drafts only 58/232 = 25.0%; ugen on either
side 79/250 = 31.6%; ugen on both sides (the only case where ugen's own order alone decides) 23/250 = 9.2%. By score
band: 21-40 lines 26.4%, 41-60 35.0%, 61-100 14.6%.

Verdict: below 25%, so no 3-day ugen trace. Where one side holds the value in a uopt colour, uopt decided whether the
value got a register, which the existing trace already shows. Only 9% are a ugen ordering question.

## Comments

- 2026-10-16 review (CODING_STANDARDS.md checklist, diff w2-good...side-int): wiki pages only ([[uopt-register-allocation]],
  [[matching-breakthroughs]], this ticket); no `src/`, `include/`, `tools/`, `config/` or `drafts/` change, no game data
  (scratch pass and frozen drafts stay outside the repository). The arithmetic of the ugen table checks: the rows sum to
  262 drafts, 250 after the 7 errors and 5 without a register difference; 59/250 = 23.6%, 58/232 = 25.0%, 79/250 = 31.6%,
  23/250 = 9.2%. Finding, fixed: the sentence comparing 23.6% with the earlier sample was garbled; reworded. The
  experiment 1 result is superseded by [[tickets/T-16910-cpw-census]] (no uniform rule) and
  [[tickets/T-16920-gamestate-1093-type]] (the 7 targets match as C through `GameState *g`); a follow-up note was added
  to [[uopt-register-allocation]]. Done.
