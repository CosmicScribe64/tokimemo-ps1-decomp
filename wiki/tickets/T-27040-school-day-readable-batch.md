---
id: T-27040
title: Apply a readable school-day gameplay batch
status: In Progress
assignee: orchestrator
created: 2026-10-24
updated: 2026-10-24
links: ["[[phase2]]", "[[spec/m-weekday]]", "[[tickets/T-27012-p2-m-weekday]]"]
---

## Goal

Make the school-day loop and its parameter-delta formula easier to use as the first gameplay reference for the Godot port. Reconcile a bounded subset of the reviewed weekday analysis with current core names and ADR prefixes, then apply through rename.py only.

## Acceptance criteria

- [ ] Body-backed function names use the ADR school_day_ prefix, O.BIN names stay unchanged.
- [ ] Formula parameters and essential locals have precise role names; arithmetic and layout stay unchanged.
- [ ] Independent table review, rename.py plan/apply/check, clean 27/27 SHA1 and 6,952 function census.
- [ ] Standards/spec code review resolved before Done; updated living spec and ledger.

## Notes

Scope is a bounded application batch. Remaining weekday data/constant/file/header proposals stay with T-27009 reconciliation. Source edits happen exclusively through tools/rename.py.

## Comments

2026-10-24: Claimed after the user clarified readiness to start the Godot port as the outcome. Current outputs independently hash 27/27; a clean regeneration will remove stale old-name assembly files before final progress validation.

2026-10-24 pre-application review: independent Sol 6.1 approved all 16 rows against definitions, callers, ADR ownership and collision checks. Nine helpers use school_day_; seven parameter/local names reflect actual normalized factors and delta pointers. No O.BIN renames or arithmetic changes. `rename.py plan` exited 0; `rename.py apply` exited 0 with built-in proofs and `renames OK (740 ledger rows)`. Clean Docker build is running; no post-change hash or census claim yet. Living weekday spec refreshed from applied ledger; added the required English one-line comment to parameter_up_down.
