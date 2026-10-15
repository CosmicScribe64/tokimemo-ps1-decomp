---
id: T-13900
title: GEKO and DATE2 source families
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

- 2026-10-15: Independent standards and spec reviews of 0e670a13...1e2eb462 pass, no actionable findings. All accepted functions re-proved literal MATCH in final-proof.log, affected GEKO/DATE2 SHA1 OK in final-build.log. Header/global/prototype checks pass (0 new / 15 known). All 102 pre-existing C bodies across touched files unchanged. Bounded pass complete; game-wide 80% remains integration work. See [[scenes-family-results]] for accepted sizes, complete negative ledger and follow-up targets.
