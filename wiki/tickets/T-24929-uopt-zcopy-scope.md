---
id: T-24929
title: "T-24929 uopt zcopy scope: propagate global copies but not local induction copies"
status: In Review
assignee: claude
created: 2026-10-20
updated: 2026-10-20
links: ["[[build-step-gaps]]", "[[as1-scheduler-rule]]", "[[plan-remaining]]", "[[tickets/T-24925-func-8013cce4-8013BEF0]]"]
---

## Goal

Model the original uopt's copy-propagation scope: it propagates copies of globals (the switch
temp of ETC func_8013CCE4, giving the `$v1` compares) but not copies of locals (the unrolled
loop induction `i`), where IDO 5.3's `OPTN 405 val=1` propagates both and folds the loop.

## Evidence

T-24925 (drafts/T-24925/): ucode dumps of both spellings, the `uopt_flip.py` experiment that
proves the coupling is the option value itself, and reproduction shims. ETC func_8013CCE4 stays
guarded at 3 rows until this is resolved.

## Acceptance criteria

- [x] One uniform rule (a shim or a traced-uopt patch) that reproduces the original's scope,
      every currently matched function unchanged (regression run), and ETC func_8013CCE4
      matching in plain C.
- [x] No per-file options, no per-function build-step changes.

## Comments

### 2026-10-20 claude (In Review)

- Reproduced T-24925 (plain: 3 rows on `$v0`; flip to zcopy 1: compares fixed, loop folded, and the
  `or v0,v1,zero` copy also lost).
- The zcopy-scope hypothesis is wrong. The original's loop is IDO's zcopy-off code. The three rows are
  as1 failing to put the chain's compares on the global's register when the global is redefined in a case
  body. Adopted rule `switch_compares` in `tools/cvt_pass.py`: on uopt's output, in zcopy-off procedures,
  the entry copy `lod Rg; str Rt` is propagated into the following loads of Rt up to the first
  label/call/store. It fires in 267 procedures and changes 0 matched functions; func_8013CCE4 matches in C
  (guard removed), and the clean build is 27/27.
- Rejected variants (matched functions changed): pre-uopt temp propagation 261, zstor-off 24, both 174,
  both limited to stored selectors 74, post-uopt propagation in every procedure 275. No uopt z-option
  (0x191-0x19a) stops the loop fold under zcopy 1. Table in [[matching-notes]] T-24929.
- Other guarded functions: no diff row count changes under the rule (25 measured).
- Checks: check_headers, migrate_globals --check, sync_protos --check-branch OK; tools/test_cvt_pass 51 OK.
  Full tool suite: the 11 test_build_deps failures and 1 test_protoverride error are present at the base commit
  too (not caused by this change).
- Needs the code review (CODING_STANDARDS.md) before Done.
