---
id: T-13902
title: DATE2 bounded family reconstruction
status: Done
assignee: Codex
created: 2026-10-15
updated: 2026-10-15
links: ["[[scenes-family-results]]"]
---

## Goal

Recover matching C within owned GEKO/DATE2 scopes from 0e670a13. Original checkout is read only. Parent: T-13900.

## Acceptance criteria

- Literal full-TU default linked-byte proof and affected overlay SHA1 for every accepted function.
- Preserve existing matching bodies; Docker only; no assembly or build-tool changes.
- Record rejected candidates and review against CODING_STANDARDS.md before Done.

## Notes

Prioritize sibling source forms and >=800-byte functions, then stateless helpers or type reconstruction. Do not repeat the broad default sweep. No claim of 80% completion from this bounded lane.

## Comments

- 2026-10-15: Claimed before experiments; read index, standards, local setup, Bunka/ETC results and prior negative ledger.

DATE2 func_8013820C: direct case stores replace the m2c shared goto (217 -> 2 rows); corrected overlay-local D_8013B5FC to s16 (target sh) removes final difference. D_8013B5D0 is s16 from its target store. Literal full-TU default proof reports MATCH, 768 linked bytes; DATE2 SHA1 OK. Evidence build/scenes13900/first-proof.log and first-build.log. GEKO 801367EC remains 44 rows with direct aggregate and 50 with pointer; no win counted.

- 2026-10-15: Independent standards and spec reviews of 0e670a13...1e2eb462 pass, no actionable findings. All accepted functions re-proved literal MATCH in final-proof.log, affected GEKO/DATE2 SHA1 OK in final-build.log. Header/global/prototype checks pass (0 new / 15 known). All 102 pre-existing C bodies across touched files unchanged. Bounded pass complete; game-wide 80% remains integration work. See [[scenes-family-results]] for accepted sizes, complete negative ledger and follow-up targets.
