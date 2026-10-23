---
id: T-25032
title: Bounded read-only MAIN candidate qualification
status: In Progress
assignee: Codex
created: 2026-10-22
updated: 2026-10-22
links: ["[[plan-remaining]]"]
---

## Goal

Read-only selection from current remaining MAIN functions, excluding prior T-25028/30 twenty-four, contributor ownership, TEL36724, all8004F870 functions,8006D7EC and strCallback. Release edit ownership800563F0.c. At most12functions/15minutes; stop on a demonstrated unrepaired semantic/control/ABI/type gap and specify one minimal proving experiment. No builds, source sweeps, workers or Astra.

## Acceptance criteria

- [ ] Current remaining membership verified before selection.
- [ ] Newest useful body and negative ledger checked against target/callers.
- [ ] Actionable candidate or explicitly scope-limited negative table preserved.

## Notes

Source-only previous checkpoint37bf807db preserved separately. Root owns integration; isolated worktree only, no push or root-checkout edits.

## Early actionable stop

Current main rechecked44a495b88. Five focused candidates confirmed remaining and compared to newest useful ledger/body: palette_load_vram,make_color_bar,mod_trans_vram,mod_set,func_80046AC8. Two raw prefixes476C0/61A3C read only for provenance, not qualified; audit stops below12functions.

Nominated func_80046AC8: newest T-24501 draft still declares s16 extent, computes tile extent then branches on narrowed sign. Target80047308..314 and80047398..3A4 branch on full word with SH only in delay slot; same pattern later arms. T-24501 already flags but retains this inherited narrowing. Exact semantic counterexample, current callers and one-type proving experiment documented at drafts/T-25032/README.md. Check for an existing private corrected variant before compiling; preserve corrected source regardless score. No builds/source tests or coverage claim. Root reviews candidate and chooses implementation ownership.
