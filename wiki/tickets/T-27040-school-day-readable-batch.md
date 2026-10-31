---
id: T-27040
title: Apply a readable school-day gameplay batch
status: Done
assignee: orchestrator
created: 2026-10-24
updated: 2026-10-24
links: ["[[phase2]]", "`spec/m-weekday`", "[[tickets/T-27012-p2-m-weekday]]"]
---

## Goal

Make the school-day loop and its parameter-delta formula easier to use as the first gameplay reference for the Godot port. Reconcile a bounded subset of the reviewed weekday analysis with current core names and ADR prefixes, then apply through rename.py only.

## Acceptance criteria

- [x] Body-backed function names use the ADR school_day_ prefix, O.BIN names stay unchanged.
- [x] Formula parameters and essential locals have precise role names; arithmetic and layout stay unchanged.
- [x] Independent table review, rename.py plan/apply/check, clean 27/27 SHA1 and 6,952 function census.
- [x] Standards/spec code review resolved before Done; updated living spec and ledger.

## Notes

Scope is a bounded application batch. Remaining weekday data/constant/file/header proposals stay with T-27009 reconciliation. Source edits happen exclusively through tools/rename.py.

## Comments

2026-10-24: Claimed after the user clarified readiness to start the Godot port as the outcome. Current outputs independently hash 27/27; a clean regeneration will remove stale old-name assembly files before final progress validation.

2026-10-24 pre-application review: independent Sol 6.1 approved all 16 rows against definitions, callers, ADR ownership and collision checks. Nine helpers use school_day_; seven parameter/local names reflect actual normalized factors and delta pointers. No O.BIN renames or arithmetic changes. `rename.py plan` exited 0; `rename.py apply` exited 0 with built-in proofs and `renames OK (740 ledger rows)`. Clean Docker build is running; no post-change hash or census claim yet. Living weekday spec refreshed from applied ledger; added the required English one-line comment to parameter_up_down.

2026-10-24 verification: clean Docker build (`nice -n 19 sh -c 'rm -rf asm build && python3 configure.py && ninja -j2 && ninja progress'`, container --cpus=2) exited 0; 27/27 SHA1 lines and independent hashlib verification 27/27. Census: 6,952/6,952 functions, 2,276,920/2,276,920 bytes. Stale pre-rename asm files were generated residue; clean regeneration restores the correct census without a tooling/source fix. Apply log /private/tmp/tokimemo-t27040-apply.log; clean log /private/tmp/tokimemo-t27040-clean.log (local only).

2026-10-24 code review: Luna Standards axis (c3c830094...6036b5a86) found zero hard violations and zero new smells; Sol 6.1 Spec axis found one P3 stale argument-name passage. Fixed at a7a3dd221 and Sol verified resolved. No unresolved findings; table approval preceded application. Final proof is recorded above.

2026-10-24: Done after both review axes, the verified documentation correction and the clean 27/27 build/census proof. Remaining weekday proposal rows are explicitly outside this bounded batch.
