---
id: T-13901
title: GEKO bounded family reconstruction
status: In Progress
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

GEKO func_80141374: replacing the m2c duplicated loop cursor with the single s16 induction variable from the target closes 39 -> 0 rows. Uses the existing GameState name array. Full-TU default literal MATCH 416 bytes and GEKO SHA1 OK (build/scenes13900/geko-proof.log, geko-build.log). Existing matching source unchanged. The recovered month table / label table / large string buffer compile but remain nonmatching; discarded production candidates, retained scratch evidence.
