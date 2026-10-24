---
id: T-27002
title: Build the phase 2 rename tool (tools/rename.py)
status: In Progress
assignee: opus (w19-rename)
created: 2026-10-24
updated: 2026-10-24
links: [[tickets/T-27000-phase-2]], [[rename-tool]], [[tooling]], drafts/T-25900/audit.md
---

## Goal

One safe, mechanical and provable path for every phase 2 rename (functions, globals, fields, types,
locals, parameters, files, folders, headers, constants), so many agents can rename in parallel and
merges can replay each other's renames. Specification: the T-25900 audit (sections 2.3 gaps G1-G9,
3 tool design, 6 risks) and the user's decisions of 2026-10-24 (English names, O.BIN names kept with a
comment, readable file and header names, hypotheses marked).

## Acceptance criteria

- [ ] `tools/rename.py` with `plan`, `apply`, `check`, `replay`, `lookup`, `export` and `const`;
      rows keyed by (unit, address); ledger under `config/renames/`.
- [ ] `tools/symtab.py`: one resolver for the symbol files of every unit's yaml.
- [ ] Prerequisite fixes: generators keep yaml names and symbol files, tools accept named symbols.
- [ ] `tools/test_rename.py` with synthetic fixtures; the full tool suite passes.
- [ ] Trial on a small unit (symbols, a field, a file) builds 27/27, then is reverted.
- [ ] Docs: module docstring, `wiki/tooling.md`, `wiki/rename-tool.md`.

## Notes

## Comments
