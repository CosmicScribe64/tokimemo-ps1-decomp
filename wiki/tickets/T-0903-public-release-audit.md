---
id: T-0903
title: "Public-release audit"
status: Done
assignee:
created: 2026-10-09
updated: 2026-10-09
links: []
---

## Goal

Scan the full git history and working tree for committed game data, generated asm and host-path leaks, check .gitignore, and fix what a normal commit can fix.

## Acceptance criteria

- [x] Done when the goal holds and the inline code-review gate has no open findings.

## Notes

Audit of all 96 commits on every ref and of the working tree, 2026-10-09 (before the T-0900 to T-0903 commits were pushed anywhere).

- Large blobs: none over 60 KB at the time of the scan; the whole `.git` is 6.2 MB. Tracked files over 100 KB: none.
- Game data by path or extension (`*.bin`, `*.EXN`, `*.iso`, `*.cue`, `*.zip`, `SLPM*`, `disc/`, `asm/`, `build/`, `expected/`): never committed. The only `.s` files ever committed are the ten `src/ovl/pad/*.s` stubs (three or four `nop` lines each). Only `config/SLPM_86.053.yaml` and `.sha1` carry the name SLPM.
- Disassembly in the wiki: no page has more than a handful of instruction lines; no long hex runs; no Japanese game text in any tracked file.
- Host paths: no `/Users/`, `/private/tmp` or `/home/<user>/` in any tracked file or in any revision of any file (`git grep` over every commit). Commit author and committer are the GitHub noreply address on all 96 commits; commit messages contain no paths or addresses.
- `.gitignore` already covered `*.zip`, `*.bin`, `*.cue`, `*.iso`, `/disc/`, `/build/`, `/expected/`, `/asm/`, `/build.ninja`. Added `*.EXN`, `*.STR`, `*.xa`, `*.img`, `*.chd`, `*.7z`, `*.rar`, `*.exe`, `SLPM_*` (with `config/` and `tools/` exceptions), `*.enc`, and the CI key file names. `git ls-files -ci --exclude-standard` is empty, so no tracked file is newly ignored.
- History rewrite needed: none.

## Comments
- 2026-10-09 inline code-review gate: `.gitignore` additions tested with `git check-ignore` (config/SLPM_86.053.* stay tracked); no code changes otherwise. No open findings.
