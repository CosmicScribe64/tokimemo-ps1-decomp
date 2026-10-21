---
id: T-24929
title: "T-24929 uopt zcopy scope: propagate global copies but not local induction copies"
status: Backlog
assignee:
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

- [ ] One uniform rule (a shim or a traced-uopt patch) that reproduces the original's scope,
      every currently matched function unchanged (regression run), and ETC func_8013CCE4
      matching in plain C.
- [ ] No per-file options, no per-function build-step changes.

## Comments
