---
id: T-27002
title: Build the phase 2 rename tool (tools/rename.py)
status: In Review
assignee: opus (w19-rename)
created: 2026-10-24
updated: 2026-10-24
links: [[tickets/T-27000-phase-2]], [[rename-tool]], [[tooling]], [[build-system]], drafts/T-25900/audit.md, drafts/T-27002/
---

## Goal

One safe, mechanical and provable path for every phase 2 rename (functions, globals, members,
types, locals, parameters, files, folders, headers, constants), so many agents can rename in
parallel and merges can replay each other's renames. Specification: the T-25900 audit (sections 2.3
gaps G1-G9, 3 tool design, 6 risks) and the user's decisions of 2026-10-24 (English names, O.BIN
names kept with a comment, readable file, folder and header names, hypotheses marked).

## Acceptance criteria

- [x] `tools/rename.py` with `plan`, `apply`, `check`, `replay`, `lookup`, `export` and `const`;
      rows keyed by (unit, address); ledger under `config/renames/`.
- [x] `tools/symtab.py`: one resolver for the symbol files of every unit's yaml, and the symbol-file
      checks (`tools/check_symbols.py`, ninja `build/symbols.ok`).
- [x] Prerequisite fixes: generators keep yaml names and symbol files, tools accept named symbols
      and readable folders and headers.
- [x] `tools/test_rename.py` with synthetic fixtures; the full tool suite passes.
- [x] Trial on a small unit (symbols, a member, a file, a folder, a header) builds 27/27; not merged.
- [x] Docs: module docstring, [[tooling]], [[rename-tool]], [[build-system]], [[source-files]].

## Notes

What landed (branch `w19-rename`):

- `tools/rename.py`: the table kinds `func`, `data`, `field`, `type`, `local`, `param`, `file`
  (C files, and headers with scope `include`), `dir` (overlay folders), and `const`. Collision checks,
  the gcc `-fsyntax-only` member oracle (batched per run of field rows), the proofs (inverse round
  trip, gcc error set, `sync_protos.py` views), the ledger with chains (union of connected names per
  key, branching and cycles refused), `check`, `replay`, `lookup`, `export`.
- `tools/symtab.py`: units, scope rule, placement of new names (shared file, main-only file for
  EVENT's range, `config/symbols_<NAME>.txt`, `_hypotheses` files with a HYPOTHESES header), and
  `problems()`.
- Prerequisite fixes: `check_symbols.py` checks every yaml's symbol files (`cd_read_status` moved
  to `config/symbol_addrs_main.txt`, the `func_80084C98` view marked `alias-view`); `configure.py`
  feeds `main_names.ld` from every main-only symbol file, runs the symbol check with `restat`, adds
  `build/renames.ok`; `split_objects.py` (object and label names), `data_island.py` and so
  `orphan_rodata.py` keep yaml names; `gen_overlay_configs.py` keeps a yaml's symbol files and
  does not overwrite `config/overlays/DATE_symbols.txt`; `migrate_globals.py` accepts named
  aggregate bases and keep symbols and finds an object's asm by the srcscan path rule;
  `srcscan.py` (`object_name`, `unit_dir`, `unit_of_source`, `header_units`, `unit_header`),
  `sync_protos.py`, `object_layout.py` and `type_recovery.py` map readable folders and headers back
  to the unit; `funcdiff.py` and `wiki.py` read `config/symbols_*.txt` too.
- Not changed: CODING_STANDARDS section 4 still says "rename only through the symbol file"; the
  amendment ("everything else through tools/rename.py") needs the user's approval. The merge
  script `.claude/orchestrator/bin/merge3.sh` still runs its dead `obin_renames.txt` block (G1);
  replace it with `tools/docker.sh python3 tools/rename.py replay` and `rename.py check` after
  conflict resolution. `config/obin_renames.txt` is not folded into the ledger: its 105 old names are
  still used in overlays (G2), so `check` would fail until the pilot (audit step 1) merges them with
  `--allow-alias`.

## Comments

- 2026-10-24 trial (scratch branch `w19-rename-trial`, worktree `../w19-rename-trial`, not for
  merge; tables in `drafts/T-27002/`): see [[rename-tool]] ("Trial").
