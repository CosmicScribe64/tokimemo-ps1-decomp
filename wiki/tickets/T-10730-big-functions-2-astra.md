---
id: T-10730
title: Big functions 2 (astra)
status: Done
assignee: Codex (a15)
created: 2026-10-12
updated: 2026-10-13
links: ["[[decompile-workflow]]", "[[matching-notes]]"]
---

## Goal

Match the assigned large functions in main objects 8007C030 and 8006CB30,
DATE/80132000 and EVENT/8010FE10. Recover reusable types and source idioms.

## Acceptance criteria

- [x] At least four hours of largest-first matching work, approximately one hour per hard function.
- [x] Every register miss investigated with regdiff; accepted matches verified in Docker.
- [x] Eligible near misses below about 30 differing lines retained with ticketed NON_MATCHING guards; prototype-blocked DATE match handed off as a verified patch.
- [x] Reusable layouts and idioms recorded in shared headers and decompile-workflow.
- [x] Final header/global checks, 27-unit build, progress and inline standards/spec review recorded.

## Notes

Scope and procedure: scratchpad/wave11-brief.md and scratchpad/astra15-list.txt.
Only the four assigned C files and authorized header/wiki additions are edited.
No commits, branches, stash, reset, checkout, merges or pushes. Scratch is a15/.

## Comments

- 2026-10-12: Started with the 9,460-byte main func_8007EF48. The work list is
  grouped by file; retain its order while taking the largest function first.

- First dialogue pass: m2c 2,222 instructions versus 2,168 original; repaired
  table types, pointer arguments, voice branches and selector data ownership
  reach 2,168. Scratch frame scan finds all offsets, but scheduling and register
  differences remain. regdiff reports are retained in scratchpad/a15/.
- Assembly evidence: func_80081190 returns zero for an empty string and the
  final set_kanji_string result otherwise; func_8007EF48 forwards it to
  k_disp_start. Existing shared void declaration needs correction when the
  defining handler is integrated.

- First-hour checkpoint: func_8007EF48 is retained guarded, with 2,168
  instructions and three scheduling pairs in the standalone draft. New
  DialogueWork view removes the load/store ordering miss; all work-area fields
  compile through it. Default build remains 27/27. No match is counted yet.
- EVENT state layout recovered as EventStateView, including Rec34 table and
  word bitfield. DATE local GameState pointer reaches original instruction
  count (1,441), but temporary-register/operand-order differences remain.
  Text formatter's punctuation arrays and outer parser loop are reconstructed.
- Two matches verified together by the complete 27/27 build: `read_bustup`
  and `func_8006F524`, +3,156 text bytes. `func_80082764` is retained with
  two commutative operand-order misses. DialogueTables, DATE/EVENT choice
  records and selector frame/constant idioms are documented.
- DATE `func_8013272C` now has nine differing instructions, no structural or
  frame differences. Place-byte unsigned reload locals and high-mask-first
  assignment were decisive. The remaining boolean hoist is an explicit
  allocation aid; a longer permuter pass is investigating it.
- Third match: `func_8006FB4C`, 722 instructions / 2,888 text bytes, verified
  by integrated regdiff and all 27 checksums. The 0xA8 frame uses a 40-byte
  count buffer (eleven used) and 32-byte explicit padding, marked FAKE.
  Indexed records, plain-char outputs and a signed result avoid cast tricks.
- DATE's nine-instruction draft is saved in `scratchpad/a15/date-pending-block.txt`.
  Full header validation detects the pre-existing void declaration. A narrow
  prototype exception was requested; the default source stays INCLUDE_ASM
  while that request is pending.

- Fourth function verified in isolation: DATE `func_8013272C`, 1,441 words
  / 5,768 bytes including four trailing padding bytes. The complete DATE overlay matches SHA1
  `b5fb83738364a317a590735578f5878077fcda3d`. DateFlag17 and DateSelectionView
  remove the remaining register/operand differences. The concrete body and
  one-line void-to-s32 prototype patch are in `scratchpad/a15/date-match-pending.diff`.
  The append-only exception is still pending, so this result is excluded from
  integrated counts and the source retains INCLUDE_ASM.

- Inline review started against the uncommitted diff from HEAD, applying
  CODING_STANDARDS.md and the batch spec. No reviewer subagents (brief rule).
  Standards findings resolved: new dispatcher uses aggregate views; overlay
  types are appended under guards; unused DATE boolean hoist removed; all
  retained padding/cast/scheduling aids have FAKE comments and T-10730.
  Full-file NON_MATCHING compilation succeeds. Resolved funcdiff has eight
  differing lines for func_8007EF48 and six for func_80082764, below the
  requested threshold. The latter adds one load-scheduling pair to its two
  standalone operand-order pairs. No broad draft is installed.

- Final checks after the last header addition: sync_protos --check-branch
  passes (zero new disagreements, 17 known); migrate_globals --check passes;
  full include/src header validation passes; 50 PS1 ABI checks pass; default
  ninja completes all 27 checksums. Grand total: 5179/6958 functions,
  967560/2279368 bytes, 42.4%. Scoped dupes found no additional copy plans.
- Inline Standards review: PASS for the retained scope. Default and guarded
  source builds verified; FAKE aids documented; no SDK redefinitions, C99,
  duplicate main declarations, generated-asm edits, tool changes or raw-data
  changes. Overlay header diffs are appended guarded declarations only.
- Inline Spec review: PASS for the time-boxed retained scope. All 21 functions
  iterated; three integrated matches / 6,044 bytes; two guarded near misses;
  fourth match / 5,768 bytes verified in isolated DATE checksum and full
  header/source compatibility checks. Its one-line prototype exception is
  deferred under the brief's append-only rule, not silently applied. The
  prepared patch is concrete and checked. No unresolved retained-code review
  findings; zero T-0018 rows. Remaining source-shape work is in the handoff.
- Tooling observation: regdiff strips other functions, so a standalone match
  does not prove full-file equivalence. DATE exposed cross-procedure unsigned
  global comparison handling in cvt_pass; the aggregate view fixes this case.
  No tooling was modified. All edits remain uncommitted for orchestration.

- Batch closed after more than four hours (04:24–08:28 UTC), with inline
  review and all required checks complete. Retained changes have no open
  review findings. DATE integration remains a separately documented pending
  exception; the checked patch is ready for the orchestrator. Final retained
  result: three matches / 6,044 bytes; two guarded drafts (eight/six lines).
