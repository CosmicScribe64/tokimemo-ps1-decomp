---
id: T-27002
title: Build the phase 2 rename tool (tools/rename.py)
status: Done
assignee: opus (w19-rename)
created: 2026-10-24
updated: 2026-10-24
links: ROADMAP.md (phase 2), [[rename-tool]], [[tooling]], [[build-system]], drafts/T-25900/audit.md, drafts/T-27002/
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
  merge; tables in `drafts/T-27002/`): three clean builds under `nice -n 19`, 2 jobs, each 27/27
  sha1 OK, 6,952/6,952 functions, `build/renames.ok`, `symbols.ok` and `globals.ok` passing. Trial 2
  found that `tools/object_layout.py` derived the unit from the folder name (fixed). Trial 3 renamed
  the GameState base (12,247 tokens) and one member (1,759 uses by the gcc oracle). Details:
  [[rename-tool]] ("Trial"). The tool branch itself: clean build 27/27; tool suite in the commit log.
- 2026-10-24 review round 1 (not approved, four blocking findings), fixed:
  replay crashed on a chain of 3 or more names followed by a local or field row (replay now builds
  chains from the ledger only); type and field rows now carry their file scope, so proofs 2 and 3
  map an old type name only where that type is visible (an overlay-private `FnTbl10` no longer
  fails proof 3 for GYOZI, SHOUGATU and TAIIKU); the HYPOTHESIS mark of a field goes on the renamed
  member's own line; two members of one type in one table no longer trip the offset assert; the
  kanban card, the index line and the broken phase 2 link are fixed. Also from the non-blocking
  list: m2c placeholder shapes only (`speed`, `var_count` pass), replay picks rows by the scoped
  search `check` makes and runs symbols before locals, a local row found through a later function
  rename works, apply says so when check fails after the edits, a ledger note keeps its `#`.
  Checked on a scratch clone of the real tree: the reviewer's VALEN type, replay and Entry8
  member cases pass.
- Follow-ups still open from the review (tickets to open): the merge3.sh replay (G1); a
  `wiki.py catalog` column with current names (audit 3.8.7); marking or moving the matching-era
  tools that hardcode names (3.8.8); symtab in `sync_protos.py` and `dupes.py`; folding
  `obin_renames.txt` into the ledger (after the pilot); the CODING_STANDARDS section 4 amendment
  (user approval); `check` for old member names at use sites (today IDO reports them); main renames
  inside EVENT's range against EVENT-own names.
- 2026-10-24 review round 2 (two blocking findings, two non-blocking), fixed in `tools/rename.py`:
  B1 replay walks a member chain from the name the struct holds (the oldest, a middle one or the
  last): that row renames the declaration, the rows of the other names only follow the gcc
  reports (`uses_only`), and a struct that declares two names of the chain stops the replay. B2
  replay moves what a stale branch left under an old name: objects, island lines and C files under an
  old folder (`dir`), island lines and a C file under an old file name (`file`), and an `#include` of
  an old header name; yaml lines, `INCLUDE_*` paths and config keys follow, and `check` reports the
  same leftovers. When the old and the new path both hold a file, replay stops and names both. Chains
  of three names now work for file, folder and header rows too (they ran the newest earlier name
  first). N4 the symbol-line dedupe keeps the splat attributes of the lines it drops (a conflicting
  value is listed). N5 a table note keeps its `#`, like a ledger note. Replay prints its notes.
  Tests: 12 new in `tools/test_rename.py` (one per finding plus the chain, both-exist and
  no-op cases); full tool suite OK (1,637 tests). Trial 4: [[rename-tool]].
- 2026-10-24 review round 3 (approved, no blocking findings): B1, B2, N4 and N5 hold, and the branch
  renames nothing in `src/` or `include/`. The full suite
  `tools/docker.sh python3 -m unittest discover -s tools -p 'test_*.py'` passes (1,637 tests, OK,
  3 skipped, none of them in `tools/test_rename.py`), and a separate run of `tools/test_rename.py`
  passes. Non-blocking, left as follow-ups: (1) a member use inside an uncalled macro body in a
  header (for example `#define GS_MODE2(g) ((g).modeA)` that no C file invokes) is neither patched
  by the gcc oracle nor counted in the "member uses named X remain" note, because that count scans
  only the C files that use the type (the doc already says gcc cannot see code it does not compile);
  (2) when the symbol-line dedupe adds attributes to a kept line that already ends in `;` plus a
  space, the result has a double space (`text_merge = 0x80041180;  // size:0x10 type:func`), which
  splat parses fine; (3) `wiki.py lint` reports 3 broken links to `tickets/T-27000-phase-2` in
  pages that come from w2-good (spec/index, spec/o-taco, T-27028), not in this branch's changes.
