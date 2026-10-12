---
type: concept
updated: 2026-10-11
sources: ["tools/register_shapes.py", "tools/test_register_shapes.py", "wiki/data/t0018-cases.md"]
---

# Register allocation after wave 7

Ticket [[tickets/T-9410-register-allocation-shapes-after-wave-7]]. Workflow
idioms: [[decompile-workflow]]. Earlier investigation: [[matching-notes]],
T-9000. All experiments use IDO 5.3, `-cckr`, the cvt pass and frame pass.
Register differences: run `tools/regdiff.py` first (T-10400, [[decompile-workflow]]); it names the live range, priority and rule behind them.

## Inventory

Run `tools/docker.sh python3 tools/register_shapes.py` after configuring the
assembly. Optional `--json PATH` emits each row's line, unit, function, source,
assembly path, features and state. `--root` and `--rows` accept alternate inputs.
T-9600: pass a function name, optionally with --unit UNIT (or UNIT:function), to
filter the rows and print its merged shape. Missing original assembly is skipped
with a stderr note naming the unit, function and expected path; these rows are
excluded from counts. Unresolved row identities still fail explicitly.

Baseline before T-9410: 1,088 rows, 718 distinct `(unit, function)` pairs;
344 already C, 374 still assembly. These are symptom clusters, not proven
compiler causes. Exclusive classification uses ordered rules in the script;
the specialized DATE product and 0x800/0x40 buckets additionally require asm
evidence. Broader labels use the row descriptions. JSON also records instruction
count, multiply count, immediate constants and GameState symbol references.

| Shape | Historical rows | Distinct functions | Open functions |
|---|---:|---:|---:|
| Other register/promotion | 385 | 189 | 70 |
| Selector/compare chain | 178 | 142 | 60 |
| Constant allocation/sharing | 177 | 128 | 82 |
| Pointer/base/CSE | 107 | 77 | 51 |
| Scheduling/order | 83 | 64 | 41 |
| Frame/spill | 52 | 33 | 23 |
| Temporary numbering/order | 43 | 38 | 24 |
| Multiply/stride | 40 | 32 | 12 |
| Fade/girl | 15 | 7 | 4 |
| DATE repeated products | 4 | 4 | 4 |
| 0x800/0x40 constants | 3 | 3 | 2 |
| ENDING remainder | 1 | 1 | 1 |

Rows are historical and retained, including duplicates. Distinct-function
classification merges all its row descriptions before applying the rules, so
individual rows can land in a different bucket from their merged function.
Family annotations count only their first named function, not all names in the
prose. In particular, the ENDING row describes several sibling functions; the
one-row count does not mean only one such function exists. State comes from
`INCLUDE_ASM`, including guarded drafts; it is not an independent byte check.

After the five T-9410 gains: 346 row-anchored functions are C and 372 remain
open. Only two of the five gains have their own historical rows. The open
selector bucket falls to 59 and ENDING remainder to zero; the other counts
stay unchanged. Of the 372 open functions, 186 reference GameState addresses:
36 are in the selector bucket and 22 in pointer/base/CSE. This overlapping
subset is computed from original symbols within `0x800E6280..0x800E7D0F`.

## Experiments

- `normal_date_girl_suddenin`: 576 distinct variants. All permutations of the
  two three-byte chains, same/separate source lines, explicit `s32`, `u32`,
  `u8`, `s16`, `u16`, `register s32/u32` temporaries and one/two constant locals.
  Best still differs in five instruction edits: the first chain lacks the
  original `li v1; move v0,v1` copy and uses v1 for stores that need v0.
  `normal_date_bggirl_fadein` adds 41 variants with a local value of four types,
  pointer/scalar reads and zero through four redundant uses. Best still has
  six instruction edits around the value/base/mask registers and chain copy.
- TT `func_80134874`: 180 variants of K&R/ANSI definitions, void/s32 return,
  declaration order, register pointers, typed constant locals placed before or
  inside the branch, redundant pointer tests, and same-line statements. No
  match; this restored draft also has tail scheduling differences, so this is
  not a controlled proof about its single constant-register discrepancy.
- TT `func_8014CC04`: 40 variants of addition association/order and typed index
  temporaries. Best remains the original one-instruction operand-order gap.
- ENDING `func_80138278`: 144 attempted variants, including header-incompatible
  return-type variants that failed compilation and are not negative codegen
  evidence. A dead dividend test before the remainder matches. One or two uses
  work with u32/s32/register u32 remainder locals. The same idiom matched
  `func_80138FC8`; both pass real function diffs and the overlay SHA1.
  Cleaning the sibling scene chains also matches `func_801391E4`.
  `func_8013855C` gets within two branch-operand swaps; an additional 16-form
  search finds `s16 one = 1` (declared then assigned, C89) for the two equality
  tests. The successful candidate was number 15, and is marked FAKE. Four
  ENDING functions pass real diffs and the overlay SHA1. `func_80137BB4`
  remains open with register and control-flow differences in the draft.
- Main `func_8005AE60`: 26 variants. A `u8` or `u32` selector local after the
  call matches without dummy operations. A local GameState pointer and the
  scalar `*(u8 *)&member` view also matched experimentally. The landed form
  uses a `u8 state` local; signed locals did not match.
- SHOUGATU `func_80137DE4`: 32 variants of local selector type, return type and
  extra uses did not match (best seven instruction edits). The main selector
  success is not a universal rule.
- TACO `func_8014B8B4`: 38 variants of the two-counter loops and recovered
  `TcActor` fields, with typed constant locals, both initialization orders,
  dead tests, duplicated overwritten stores and same-line grouping. Best
  remains five instruction edits: 0x800/0x40 load order plus the 0x80 load's
  scheduling. Do not claim this draft isolates only one instruction pair.
- DATE `func_801366A4`: 46 compiling variants with local index/stride types,
  `register`, operand order, `(i+i)*3`/`i*3*2`, redundant uses and same-line
  grouping. The original has six `multu` instructions; the variants still
  reduce/combine the repeated products. No match, best 57 instruction edits.
  Initial runs failed due to stale draft declarations; only the run using
  the existing `D_800CA22C`/`D_800CA21C` arrays counts as codegen evidence.

Scratch scripts and detailed diffs are in the shared tooling7 scratchpad's
`r7-regs/` directory (probe.py, tt.py, end.py, siblings.py, selector.py,
pointer.py and their logs). No original assembly or game data is added to git.
Further scratch scripts: constants.py, date.py (valid output date-final.log),
fade.py, sho.py, siblings-clean.py and 3855-more.py.

## Limits

No cluster has been proved impossible from C. Finite spelling searches and
failed compiler options cannot prove that claim. These experiments support
leaving specific shapes unresolved, not a universal impossibility statement.
No new build pass is proposed: successful transformations depend on source
structure and would not justify a uniform register permutation.

## Verification and review

Five functions gained, 2,768 bytes: main `func_8005AE60` (200); ENDING
`func_80138278` (740), `func_8013855C` (596), `func_80138FC8` (540),
`func_801391E4` (692). Each real C definition passes `funcdiff.py --resolve`.
The final clean `rm -rf asm build`, Docker configure and ninja build passed all
27 SHA1 checks. `build/headers.ok` and `build/globals.ok` exist;
`sync_protos.py --check-branch` exits zero. Progress is 4,734/6,958 functions,
729,612/2,279,368 bytes (baseline 4,729 functions, 726,844 bytes). Thus the
previously matched code remains byte-identical in all linked units.

`tools/test_register_shapes.py`: three synthetic tests cover asm features and
boundaries, row/family parsing, duplicate rows, overlay identity, guarded drafts,
missing assembly and unresolved names. `git diff --check` passes. Inline
standards/spec review completed against the shared brief and
`CODING_STANDARDS.md`: no unresolved findings. All matching tricks are marked;
there are no header/config/compiler-pass changes and no raw assets added.
