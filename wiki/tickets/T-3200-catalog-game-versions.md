---
id: T-3200
title: Catalog game versions
status: Done
assignee: agent (s-versions)
created: 2026-10-09
updated: 2026-10-09
links: ["[[versions]]", "[[disc-layout]]", "[[executable]]", "[[overlays]]"]
---

## Goal

Catalog the nine release archives the user put in `versions/`: format, track layout, hashes, disc ID, boot exe and overlay hashes. Identify our target (SLPM_86.053) among them, establish release order and lineage, and measure how much code differs per version. Decide whether SLPM_86.053 remains the best target.

## Acceptance criteria

- [x] Per-archive catalog (format, tracks, hashes, duplicates).
- [x] Per-version SYSTEM.CNF, exe header and sha1, overlay sha1s, file-list differences.
- [x] Our target identified; release order and lineage with evidence.
- [x] Code diff against ours (identical overlays, differing functions/byte ranges).
- [x] `wiki/versions.md`, `config/versions.txt`, `tools/identify_version.py`.
- [x] Inline review against CODING_STANDARDS.md; no game data committed.

## Notes

Extraction happens in Docker into the scratchpad only; extracted copies are deleted afterwards.

## Comments
- 2026-10-09: extracted in Docker (ubuntu:24.04 throwaway container with p7zip for the 7z, project image for Python) into the scratchpad, never into a repo; extracted copies deleted afterwards. Result: 5 distinct discs in 9 archives, see [[versions]]. Rev 1 = v1.1 (Ver 1.10); Shokai differs by one PVD byte; O.BIN identical in all; SLPM_86.053 is the newest (Best). Recommendation: keep Best.
- 2026-10-09 inline review against CODING_STANDARDS.md checklist: no C or build change (matches n/a, default build untouched); scripts are Python 3 with docstrings, take paths as arguments, exit non-zero on error, run via tools/docker.sh (`tools/test_identify_version.py` 5 tests OK); only hashes and sizes committed (config/versions.txt), no game data staged (git status checked); wiki, index, log, kanban updated. Known limits recorded in [[versions]]: function counts are upper bounds (masking heuristic, data inside overlays counted as functions). No open findings.
