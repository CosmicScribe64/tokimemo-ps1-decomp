---
id: T-3050
title: Run the per-object migration on the whole tree after wave 2
status: Ready
assignee:
created: 2026-10-09
updated: 2026-10-09
links: ["[[tickets/T-0500-per-file-game-rodata-data-bss-split]]", "[[build-system]]", "[[source-files]]"]
---

## Goal

Move every remaining overlay and every `src/main` file to one C file per original object with `tools/split_objects.py` (T-0500), once the wave-2 batches that edit `src/ovl/<NAME>.c` and `src/main/*.c` are merged.

## Acceptance criteria

- [ ] On the merged tree: `tools/docker.sh sh -c 'python3 configure.py && ninja'` OK (so `asm/` matches the yaml files), then `tools/docker.sh python3 tools/split_objects.py --all`, then `rm -rf asm build` and `tools/docker.sh sh -c 'python3 configure.py && ninja'` (no `-k`): 27 of 27 sha1 OK.
- [ ] `tools/docker.sh python3 tools/split_objects.py --all --dry-run` reports `0 to write, 0 to remove, yaml unchanged, 0 prototypes` for every unit.
- [ ] `tools/docker.sh ninja progress`: the same done counts as before the migration.
- [ ] Commit the moved sources, yaml files, `config/labels/`, headers (prototypes) and `objdiff.json` in one commit per unit or one for all; no `asm/` or `build/`.

## Notes

A trial on a copy of the T-0500 branch (all 27 units, before wave 2) built 27 of 27 OK and was idempotent. If the script stops with "not a union of objects", a file boundary in the yaml disagrees with `config/objects/<UNIT>.txt`; if it stops on a file-scope definition used by two objects, move the definition by hand (or make it an extern in the header) and re-run. New C written after the migration goes into the per-object files directly.

## Comments
