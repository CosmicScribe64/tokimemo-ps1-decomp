---
id: T-3300
title: Tooling: fix bugs reported by wave 2
status: Done
assignee: agent (t2-fixes)
created: 2026-10-09
updated: 2026-10-09
links: ["[[decompile-workflow]]", "[[matching-notes]]", "[[versions]]", "[[tickets/T-1330-tooling-m2c-context-and-permuter]]", "[[tickets/T-3200-catalog-game-versions]]"]
---

## Goal

Fix the tooling bugs the wave-2 agents reported, each with a regression test: m2c overlay lookup, m2c `lui`/`%lo` constants, four `funcdiff.py` problems, K&R definitions in `srcscan.py`, archives in `identify_version.py`.

## Acceptance criteria

- [x] `m2c.py` takes the unit from `--unit`, a path, a `UNIT:` prefix or the INCLUDE_ASM's C file, and refuses ambiguous names.
- [x] The `lui` + `%lo` constant is correct; cause found (upstream m2c) and recorded in [[matching-notes]].
- [x] `funcdiff.py`: `L` names, stale/failed build, host message, `--resolve`.
- [x] `srcscan.py` accepts K&R definitions.
- [x] `identify_version.py` reads zip, 7z and chd (all five archives of `versions/` identified).
- [x] Unit tests for each; `wiki/decompile-workflow.md` updated.
- [x] `game/` drop-in folder: `tools/prepare_disc.py`, ninja step, README Building, docs (scope extension from the user).
- [x] Clean build 27 of 27 OK, headers OK, progress unchanged.

## Notes

`tools/Dockerfile` gets p7zip-full and mame-tools (pinned apt versions) in a separate `RUN` at the end, so it merges cleanly with other Dockerfile edits. Tested with `IMG=tokimemo-decomp-t2-fixes`.

## Comments
- 2026-10-09 inline review against CODING_STANDARDS.md: no C or build-graph change; Python 3 with docstrings, tests next to the tools, nothing game-specific committed; `versions/` archives were read from the main checkout through a read-only mount and not copied; the scratch CHD (494 MB) was deleted.
- 2026-10-09 final inline review against CODING_STANDARDS.md (sections 10, 11, 12, 13), after merging main (native image, T-1321, T-3320, T-3330): no game data committed (`game/*` and `disc/` are ignored; the test archives were copied into game/ only for the builds and removed); scripts are Python 3 with docstrings, take paths as arguments, exit non-zero on failure; apt packages pinned (`p7zip-full=16.02+dfsg-8`, `mame-tools=0.242+dfsg.1-1`, both checked on amd64 and arm64 Ubuntu 22.04); no C change, so the match rule is untouched; commits start with `T-3300:`. Findings fixed during review: split rule did not touch all outputs (perpetual rebuild), unreadable image crashed with `struct.error`. No open findings. Proof on the native arm64 image from only the Best 7z in game/: `rm -rf disc asm build; configure; ninja` gives 27 of 27 OK, and all 19 `tools/test_*.py` pass.
