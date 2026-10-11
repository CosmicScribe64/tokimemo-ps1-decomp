---
id: T-9770
title: "Wave 9: list 7"
status: Done
assignee: Codex wave-9 agent 7
created: 2026-10-11
updated: 2026-10-11
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[register-allocation-wave7]]"]
---

## Goal

Match as many functions as possible from wave9-list-7.txt in branch w9-7.
Work for at least four hours, with an initial draft pass followed by deeper
control-flow, type, expression, declaration and permuter attempts.

## Acceptance criteria

- [x] Only assigned C files and authorized declarations/documentation edited.
- [x] Matches verified with Docker funcdiff --resolve and unit SHA1 checks.
- [x] Remaining drafts reverted; deep attempts and blockers recorded.
- [x] Final ninja passes 27/27 units, header/global guards and branch prototype check.
- [x] Inline standards review completed and findings resolved.

## Notes

Session began 2026-10-11 14:59:42 UTC. Assignment: 173 functions / 101,540 bytes
in 35 files. Shared brief read before matching; no commits or subagents.
Scratch: /private/tmp/claude-501/-Users-lila-Documents-Projects-tokimemo-decomp/43264b98-bb59-4c1b-ad99-a2084d55fcf0/scratchpad/w9-7/.

## Comments

- 2026-10-11: Claimed In Progress; read wiki index, workflow, standards, shared
  brief, local instructions and register-allocation notes. Baseline configure/build started.

- Baseline build uses ninja -j 4 to limit contention on the shared Docker host.
  Stopped only this worktree's initial build container and resumed incrementally.
  Initial progress query ran before assembly generation completed and reported
  missing sizes; this is not yet a tooling bug.

- Initial quick pass covered the first 15 entries. No matches yet: most drafts
  require pointer/aggregate/declaration repair; three compiled near misses are
  OPTION func_80139F8C (18 edit lines), func_80139878 (22), func_8013A550 (58).
- OPTION func_80139F8C: 24 full-width selector/value/return/control-flow variants
  did not improve the raw draft; a 300-second permuter is running on that draft.
- Model-loader repair: explicit function-pointer cast emits jalr, so discarded.
  Testing the documented 3-argument func_8009B430 override, generated with sync_protos.
  Full builds are run only at checkpoints, separate from temporary source trials.

- Actual 300-second permuter searches completed through the scratch-only tool
  workaround: OPTION func_80139F8C best score 140; initModelingData_init best 196.
  Neither produced score zero. Model-loader direct-call search tried 36 variants,
  best 13 resolved edit lines; both production drafts and the trial override were
  restored. Initial setup/import failures are excluded from search evidence.
- All 173 m2c drafts are now saved; remaining quick build/diffs are continuing.
  Next sweep repairs unknown members as byte-offset accesses and adds missing
  declarations through the permitted main API generator / overlay append-only path.

- Stable checkpoint after the quick pass: Docker ninja -j 4 completed all 27
  SHA1 checks, headers OK and globals OK. No trial C remains at this checkpoint.
  A generated u32[16] declaration for the 8-byte-row D_800B5970 table is retained
  for the pending repaired lookup experiments.

- Initial draft/build/diff pass finished all 173 functions with no gains. A second
  type/declaration pass is in progress. Prepared bounded deep searches for TT's
  TILE packet, NAME_ENT's corrected old-value snapshot, OPTION fade/wrap helpers,
  the main 8-byte table lookup, and EVENT's ten constant stores. Historical
  register-shape inventory completed; open rows are triage evidence only.

- GEKO func_8013B3C0: Docker funcdiff --resolve MATCH, +336 bytes. The
  documented sibling func_8013CC20 idiom fixes the final instruction swap:
  scalar byte store then canonical member read for func_800847B8. Retained SDK
  RECT with a marked FAKE unused scalar for its +0x2C stack home; the byte-store
  scheduling idiom is also marked FAKE. Starting full linked-unit checkpoint.

- Linked checkpoint for the GEKO initializer: Docker ninja -j 4 completes
  27/27 unit SHA1 checks, headers OK and globals OK. The repaired sweep covered
  another 58 drafts after the first ten; no gain in that sweep. All rejected
  sources and header trials were restored before the checkpoint.

- OPTION func_80139878: bounded 850-second search covered 64 temporary-type,
  return-type, clamp assignment, if/else and chain-store variants. Best remains
  four resolved instruction edits (0x80 constant V1 versus T9). Restored every
  rejected trial; actual five-minute permutation now follows that best draft.

- OPTION fade permutation completed its actual 300 seconds, weighted score 45
  unchanged. No match; production source restored. EVENT func_8010A790 now gets
  a bounded scalar-view, local-width, result-width and control-flow search.

- EVENT func_8010A790: all 60 scalar getter/local/result/control variants
  completed in 528 seconds. The symbol byte alias improves four edits to two
  (LUI AT before versus after the final increment). An actual five-minute
  permutation follows; all source/global trials are restored between searches.

- EVENT permutation reaches zero in 40 search seconds and passes real object
  diff plus unit SHA1. Sixteen pruning checks over 226 seconds show the duplicate
  store is unnecessary; the byte alias and empty condition still match.
  A final condition-simplification check precedes retention.

- Retained EVENT func_8010A790 (+80 bytes). The duplicate store is removed;
  five condition probes require the three-term empty condition. Byte alias and
  condition are marked FAKE. Generated u8 D_800B0E72 through sync_protos;
  final fresh resolved MATCH and EVENT linked SHA1 pass. Total: 2 / 416 bytes.

- Main func_8005B040 draft MATCH after 29 variants / 354 seconds: a two-word
  pointer row preserves the original skipped T9 temporary. Checking ordinary
  unsigned cast versus the halfword alias before retaining this 180-byte gain;
  planned sibling func_8005B0F4 follows only after the first shape is verified.

- Lookup canonical `(u16)` cast also MATCH; retained plain word-pair C without
  FAKE residue. Fresh resolved verification passes; main SHA1 now runs.
  Scope check: sibling func_8005B0F4 is in an owned file but outside the 173-entry
  assignment, so its explicit retry waits until the assigned-list timebox ends.

- Main lookup retained and main linked SHA1 passes. Batch total 3 functions /
  596 bytes. Canonical `(u16)` GameState cast with a u32 word-pair pointer and
  generated 16-word table fixes the historical skipped-temporary family.

- Next ranked deep attempt: NAME_ENT func_80134110. The rewritten draft
  explicitly returns X for early bounds failure, and Y once loaded, preserving
  the target's observed V0 result. Testing signed result/local widths, byte
  views and structured/goto/early-exit forms before permutation.

- NAME bounds: separate X/Y/result locals add phi copies and keep Y in A1.
  Prepared a result-reuse search using the returned variable for Y's tests and
  division; signed-width failure constants and declaration order are included.

- NAME bounds remains 26 edits after 24 first-stage variants / 188 seconds
  and 46 result-reuse variants / 401 seconds. Original V0 lifetime differs from
  IDO's A0/A1 return phi; restored source and recorded fresh T-0018 evidence.
  Duplicate copier now runs with all matched donors and only the 173 assigned
  unmatched targets; normal --files plus --check guards still apply. Scratch
  prefilter avoids planning unrelated targets without changing project tools.

- Final bounds JSON tally corrects the provisional second-stage count to 48
  variants / 409 seconds (72 variants / 597 seconds total); result remains
  26 edits and source restored.

- Assigned scoped duplicate scan: zero plans/copies; five skipped self-groups
  say source is not in C. Tooling finding: dupes.load_funcs takes matched state
  from asm directory names; stale trial matchings create five phantom donors,
  while all three freshly verified retained definitions still have only
  nonmatchings asm files. Treat this report as advisory; report the stale-state
  bug rather than changing project tools. Official progress reads active C.

- NAME_ENT func_801341A8: fifteen explicit-old-return variants are 18 edits
  away (V0/V1 roles swapped). Interrupted only the verified own Docker
  container with SIGINT; finally restored source and overlay declarations.
  Reordered the scratch search to interleave void / s32-no-return / explicit
  return early, before finer arithmetic/control variants.

- NAME snapshot draft MATCH after six interleaved probes / 91 seconds. Void
  form with a canonical global reread after the store fixes V0/V1 allocation.
  Removed the unused search scalar and appended guarded overlay word globals;
  fresh resolved verification now checks the cleaned source.

- Cleaned NAME snapshot passes fresh resolved diff and NAME_ENT linked SHA1.
  Caller return values are unused; no FAKE. Retained total four / 756 bytes.
  Next assigned deep attempt: OPTION eight-byte row helper func_80139E3C.

- OPTION row helper corrected byte stride tests three row views, return forms,
  declaration orders and narrow-update forms. TT packet deep search is prepared
  with SDK TILE and the observed 0x80B8 render-buffer stride; void/s32 probes
  are interleaved before finer layout variants.

- OPTION table first stage: 36 probes / 233 seconds, best 67 resolved edits.
  Narrow value lifetime and redundant-store placement remain; source restored.
  TT packet variants now run with actual SDK and render-buffer types.

- Prepared EVENT func_8010A6E4 type repair: Rec34 bit-field reads and a
  root aggregate view at D_800B0860 (table starts +0x1A4). The observed word
  at D_800B0C4C and byte at root+0x3ED belong to record 11's flags. Keeps both
  original bases without reaching the field through an untyped byte offset.
- Read-only owned queue scan completed; top candidates remain assigned work.
  The already identified main table-address twin is reserved for the end of
  the assignment timebox, within the same owned C file.

- TT packet search: 192 probes / 776 seconds, best 58 resolved edits.
  SDK types and correct render-buffer stride leave pointer rotation and
  scheduling. No draft below permutation threshold. Restored C/header;
  fifth new T-0018 symptom row. EVENT record-image repair now runs.

- OPTION second table stage: 36 call-store probes / 131 seconds, best 61
  edits; source restored. Both table stages total 72 probes / 364 seconds.
- EVENT repaired draft compiles; 48 aggregate/return/read/update variants /
  288 seconds reach 47 edits. Known base-offset folding and reload loss remain.
  An actual 300-second permuter is running despite the larger edit count.

- EVENT permutation completes 300 seconds at weighted 505 without zero;
  restored C/header. Added OPTION row-helper and EVENT flag evidence: seven
  new T-0018 symptom rows total. TAIIKU's clamp probes now use a GsWord
  accumulator/high-half view, interleaving return forms and both halfword views.

- TAIIKU initial corrected-union probes reach 18 resolved edits. SDK calls,
  signed divisions and packet dimensions agree; maximum-clamp V0/V1 and
  final counter-store scheduling remain. Finish bounded variants, then run
  actual five-minute permutation on the closest draft.

- TAIIKU bounded search completed: 144 variants / 538 seconds, best 18
  edits. Restored source/header, then start actual 300-second permutation
  with the proper GsWord high-half view.

- TAIIKU permutation completes 300 seconds, weighted 330 -> 305, no zero.
  Restored C/header; eighth T-0018 row. Preliminary inline standards/spec
  review found only incidental main EOF normalization, now restored.
  Assigned GEKO flag helper starts canonical union-word/bit-field probes.

- GEKO flag helper: 72 probes / 96 seconds reach six resolved edits.
  Restored source; actual 300-second permutation now runs. Four-hour minimum
  reached at 18:59:42 UTC; remaining steps are own-file twin and final checks.

- GEKO flags MATCH +172 bytes: permutation plateaus at 230, then first
  direct-fix probe / 11 seconds matches with unsigned counter read and
  canonical field reload. Void form passes fresh resolved diff and linked
  GEKO SHA1. No FAKE. Five retained gains / 928 bytes. Own-file main twin now
  runs after the four-hour assigned-list timebox and owned-queue scan.

- Main own-file twin func_8005B0F4 MATCH +180 bytes. Generated u32[16]
  D_800B59B0; fresh resolved diff and linked main SHA1 pass. Six gains / 1,108
  bytes (five assigned / 928 bytes, one own-file twin / 180 bytes).
- Final own-file aggregate search: GEKO func_8013B10C, observed 42 callback
  words copied in fourteen twelve-byte trips. Test scalar/table declaration
  order and ordinary control/return forms, then final all-unit checks.

- Callback aggregate: 48 probes / 67 seconds reach ten edits. Correct copy
  and frame; signed GameState/overlay byte reads and store/load pair remain.
  Actual five-minute permuter runs, followed by unsigned-view probes.

- Callback direct follow-up: eight read probes, eight store probes and four
  scheduling probes reduce ten edits -> four -> two. Void probes fail the
  existing s32 forward declaration; retained trials preserve it. Widths fixed
  by unsigned byte views/stores; one store/load reorder remains. Final actual
  300-second permutation runs on this corrected source; final checks follow.

- Final callback permutation completes 300 seconds at base weighted 80,
  no improvement/zero. C/header restored. Total 68 direct probes (28 void
  trials rejected by the established s32 declaration) plus two actual
  300-second searches; two resolved store/load-order edits remain. Ninth
  symptom row recorded. Matching stops with six gains / 1,108 bytes.
- In Review: code frozen; final linked checks, prototype/global guards,
  ownership audit and inline standards/spec review run before Done.

## Final result and review

| Unit | Function | Bytes |
| --- | --- | ---: |
| main | func_8005B040 | 180 |
| main | func_8005B0F4 | 180 |
| GEKO | func_8013B3C0 | 336 |
| GEKO | func_80140FD0 | 172 |
| EVENT | func_8010A790 | 80 |
| NAME_ENT | func_801341A8 | 160 |
| Total | 6 functions | 1,108 |

Five assigned gains / 928 bytes; one own-file twin / 180 bytes after the
assignment timebox. Nine new T-0018 symptom rows; all unsuccessful C/header
trials restored. Session began 14:59:42 UTC; final audit milestone 19:51:58 UTC
records over four hours fifty-two minutes of work on 2026-10-11.

Final Docker verification: 523-job incremental ninja, all 27 linked binaries
SHA1 OK, headers/globals guards OK. Only checksum stamps were cleared to make
all 27 checks visible; objects/asm retained. Fresh resolved funcdiff passes
6/6. sync_protos --check-branch: zero new disagreements, 17 known, branch OK.
migrate_globals --check: globals OK. git diff --check passes.

Official progress: `grand total   4876/6958   777768/2279368  34.1%`.

Inline code-review skill applied against HEAD's uncommitted diff, using
CODING_STANDARDS.md, CONTRIBUTING.md, the wave brief and this ticket:

- Standards: pass. C89/fixed-width types, SDK RECT, canonical GameState,
  generated main externs and guarded overlay externs; FAKE reasons/ticket ids
  cover the GEKO RECT home/byte-store idiom and EVENT byte alias/empty test.
- Spec: pass. Complete 173-function draft pass, aggregate repairs, ranked
  bounded deep iterations and actual 300-second permutations; only owned C,
  authorized declarations and own documentation changed. Project tools,
  raw data and config remain unchanged; no git mutations, commits or subagents.
- Resolved findings: incidental main EOF normalization restored; newly added
  status log entry placed at append position with its text and all historical
  entries preserved. No unresolved findings.

Ownership audit: 13 changed files, no unexpected path, no staged files.
Wiki lint: ticket/card/index Done agreement and ticket link present; history
preserved. Changes remain uncommitted for the orchestrator.

Top non-T-0018 blocker: complex aggregate/type recovery in remaining m2c drafts.
Tool bugs: permute K&R discovery regex stalls before its timer; worker import
shadows stdlib queue with tools/queue.py; dupes matched flags depend on stale
asm directory names and miss actual C donors. Scratch workarounds only.
