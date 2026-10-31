---
id: T-26000
title: Adopt IDO 4.1's block-copy state rule in tools/ugen_pass.py
status: Done
assignee:
status: Done
assignee: w17-ugen41 (Opus)
created: 2026-10-23
updated: 2026-10-26
links: `[tickets/T-25608-gate-main-func_8006B0C8`, [[ido-52-evaluation]], [[toolchain]]]
---

## Goal

Replace the block-copy state rule in `tools/ugen_pass.py` with the one IDO 4.1's ugen follows on all 30 test cases (T-25608 check, [[ido-52-evaluation]] "T-25608: 4.1 ugen on the rule candidates"): a procedure starts in `B`, an indirect store sets `F`, a conditional branch comparing two registers sets `B`, nothing else changes the state. The T-25608 rule that matched main func_8006B0C8 is a narrower fit that disagrees with 4.1 in 8 synthetic shapes (none in matched code).

## Acceptance criteria

- [x] main `strCallback` (T-25303) and `func_8005938C` (T-25147), whose FAKE dead loops were written for the old detector, re-matched under the 4.1 rule (or shown to need another FAKE).
- [x] The 4.1 rule implemented uniformly with unit tests (the 30 cases as synthetic tests), all 27 sha1 unchanged.
- [x] CODING_STANDARDS.md 7a documentation updated ([[toolchain]], the pass docstring).

## Notes

Prototype: `drafts/T-25608/ido41/ugen_pass_41rule.py` (on the w16-ido41 branch, merged as notes).

## Comments
- 2026-10-24 (orchestrator): an Opus agent is on it (workflow wf_3d2d513f-e21, worktree w17-ugen41): implement the 4.1 rule, 30 synthetic tests, re-match strCallback and func_8005938C, two reviews, merge.

### 2026-10-23, w17-ugen41 (Opus): implemented, In Review

- `tools/ugen_pass.py`: the 4.1 rule (`copy_states`: B at entry, F at `Uistr`/`Uistv`, B at a `Ufjp`/`Utjp` whose compare has a non-constant right operand, `Ulnot` and `Urstr` skipped, a branch with no condition record raises PassError). The T-25608 lock (`_is_signed_blt_pos_imm_jump`, the armed/sticky state) is gone. Tests: `tools/test_ugen_pass.py`, the 30 cases as synthetic ucode (all match 4.1; the old rule misses the 8 that T-25608 listed) plus the rule's parts. End to end, the real uopt output of the 30 case files gives 4.1's state in all 30.
- Census (full rebuild, 566 procedures with a `Umov`): against the T-25608 rule only main `strCallback` (F to B) and `func_8005938C` (FFF to BBF) change; main fails, 26/27. `func_8006B0C8` stays BBBB.
- Re-steered without new FAKEs: `strCallback` test as `if ((D_800B5900 & 1) && D_80125C5C != 0)` (no `odd` local), `func_8005938C` test as `if (r->active && (r->flags & 0x80))`. Before, uopt wrote `odd != 0` and `r->active != 0` as `ldc 0; x; neq`, a register compare that reset B after the arm. Both arms stay, re-marked FAKE T-26000 (75 and 62 rows without them). Both bodies also match under the old rule.
- Obsolete FAKEs removed: the empty tests of main `func_80054FC0` (T-25105) and TACO `func_80145650` (T-25136) only fed the old lock; both MATCH without them.
- Finding (IDO 4.1, [[ido-52-evaluation]] "T-26000"): 4.1 also sets F at a branch on a value (`and`, `ilod`, bit fields, `shr`, ...; 5 new case files). With the respelled tests 4.1 gives F for `strCallback` and FFF for `func_8005938C` without the arms. Not modelled: it would put `func_8006B0C8`'s compaction copy (after a one-bit field test) in F, where the original is B. Follow-up candidate: measure the 4-byte copy's state in 4.1 and stored compares, then decide whether the value-branch F can be added.
- Proof: clean rebuild 27/27 sha1 OK; `tools.test_ugen_pass` OK (24 tests); full suite 11 failures + 1 error, the same as main (test_build_deps, test_protoverride); check_headers, migrate_globals --check, sync_protos --check-branch OK; funcdiff `--resolve --version default` MATCH for func_8006B0C8, strCallback, func_8005938C, func_80054FC0, func_80145650.

### 2026-10-23, w17-ugen41 (Opus): review fixes

- Blocking (standards, section 7): the four never-true FAKE arms are cut down to `if ((u32)G < 0) { *(u8 *)0 = 0; }`; the `while ((s32)G < 1)` loop compared against a constant and did nothing under the rule. main `strCallback`, `func_8005938C` and MASTER `func_801344E0`, `func_801363F4` (T-25107, its comments also named the old "signed loop test") all MATCH; without the arm all four differ (MASTER checked here: end register one slot high, state B). [[playbook]] recipe and table updated.
- Evidence (the review's point on `copy_ne0_left_if`): new case `copy_store_then_ne0_left_if.c` (indirect store first). IDO 4.1 gives F there, the pass gave B, so the first version's clause "a compare with the constant on the left counts as a register compare" was wrong. `copy_store_then_eq0_left_if.c` agrees (4.1 F). Fixed: a compare sets B only when neither operand is an `Uldc`; a compare with fewer than two operands raises PassError. Census against the first version (temporary hook, full rebuild): 0 of 566 procedures change state; `func_8006B0C8` BBBB, `strCallback` F, `func_8005938C` FFF.
- Finding: 4.1 (and raw 5.3) ignore a store in an arm that ugen drops as never entered (`copy_dropped_arm_store.c` B, control `copy_live_arm_zero_store.c` F). That explains the T-25608 reading "4.1 gives B for the old `strCallback` body with the arm". The pass counts the store; the four FAKE arms rely on it. Written down as a known limit.
- Docstring: the overclaim is reworded (every matched function builds to the original bytes with this rule, some through the FAKEs listed); known limits list the value branch, the dropped-arm store, and the unmeasured stored compares, `Uxjp` and `Uijp` (also in [[toolchain]]).
- `drafts/T-25608/ido41/utrace.py`: no longer calls the removed `_is_signed_blt_pos_imm_jump` (marks `<` only with a pass that has it).
- End to end, all 39 case files with IDO 4.1 and the pass: 34 agree; the 5 that differ are the 4 value-branch cases and `copy_dropped_arm_store`, all documented.
- Dates: ticket `updated` and the [[toolchain]] census corrected to 2026-10-23; the earlier log entry keeps its 2026-10-24 date (log entries are not edited).
- Follow-up to file (needs a board card, left to the orchestrator): measure in 4.1 the 4-byte copy's state of `func_8006B0C8`, stored compares, `Uxjp` and `Uijp`; then decide whether to model F at a branch on a value and the dropped-arm store, which would let the two main arms go.

### 2026-10-24, orchestrator: reviews approved, Done

- Standards review (CODING_STANDARDS.md): approved, no blocking findings. Non-blocking, left as follow-ups: (1) in `tools/ugen_pass.py`, `_is_register_compare_jump` returns False at an `Uldc` right operand before it checks that a left operand exists, so a compare with only a constant (`ldc 0; neq; fjp`) keeps the state instead of raising PassError, contrary to the claim in [[toolchain]]; uopt does not emit this shape, so no output changes; fix by finding the left operand first and adding `[ldc(0), op('neq'), fjp()]` to `test_compare_without_operands_raises`. (2) Keeping the state at a value branch rests on one original function (`func_8006B0C8`) and costs two never-true FAKE arms (`strCallback`, `func_8005938C`); no full-build census of F at a value branch exists, so file a follow-up: the census, the 4-byte copy of `func_8006B0C8` in 4.1, stored compares, `Uxjp` and `Uijp`. (3) The notation paragraph in `tools/ido_eval_cases/ugen_copy_state/README.md` still describes the removed T-25608 detector; mark it as T-25608 history. (4) The FAKE comments of `strCallback` and `func_8005938C` should say they work because the pass counts a store in an arm ugen drops (`copy_dropped_arm_store`).
- Spec review: approved. All three acceptance criteria are met.
- Merge checks: check_headers, migrate_globals --check, sync_protos --check-branch, `tools.test_ugen_pass`, and funcdiff MATCH for `func_8006B0C8`, `strCallback`, `func_8005938C` (INCLUDE_ASM absent).

2026-10-26: Closed during kanban cleanup after Phases 1, 2, and 3 completed (27/27 binaries byte-matching, Phase 2 source audit complete, and Phase 3 Godot port and asset packs verified).
