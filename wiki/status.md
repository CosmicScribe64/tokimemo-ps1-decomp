---
type: status
updated: 2026-10-31
---

# Status

The decompilation is finished. A Godot port and its plans were removed on 2026-10-31, and this page no longer tracks future work.

## Matching

Every game function is in C. That is 6,952 of 6,952 functions and 2,276,920 of 2,276,920 bytes, and all 27 binaries (`SLPM_86.053` and the 26 overlays) match their SHA-1. The last functions matched on 2026-10-24 (merged at `dbc017d86`). They were SHOUGATU `func_8013EA98` and TAIIKU `func_80137784` ([[tickets/T-25500-stubborn-functions]]), then main `func_800476C0`, `func_80067870`, NAME_ENT `func_80143580` and main `func_8006B0C8` ([[tickets/T-25600-finish-gate]]). `func_8006B0C8` needed a narrower block-copy rule in `tools/ugen_pass.py` ([[tickets/T-26000-ugen-block-copy-41-rule]]). Lessons from the last 100 or so matches are in [[notes/wins-2026-10]].

The SDK libraries stay in assembly and are not counted in these totals ([[psyq-sdk]]).

## Readable source (phase 2)

Phase 2 renamed and documented the matched code without changing a byte ([[phase2]], [[tickets/T-27000-phase-2]]). The closeout gate on 2026-10-25 proved all 27 binaries at source digest `687b5ce789d59f154b527264d23227b741a08bc619e4a1f8c67d617257f4c4f3`. The integration record holds:
- 437 logical C basenames;
- 1,298 semantic operations plus one local-name repair;
- 14 exact original-data includes;
- 14,449 rename ledger rows and 437 mapped views with inverse proofs;
- 43 data islands across 2,083 pieces;
- 337 path-tool tests.

An earlier audit from 2026-10-24, before the closeout, is in [[spec/phase2-completion-audit]]. Conventions are in [[phase2-naming]], `CODING_STANDARDS.md` and `GLOSSARY.md`.

On 2026-10-30, `message_window_set_title_hidden` and `portrait_read_step` were changed to take and forward the argument their callers pass. The build still matches 27/27.

## Verifying the build

```
tools/docker.sh python3 configure.py
tools/docker.sh ninja
```

A good build prints `OK` for each of the 27 binaries and for the rename, symbol, header and globals checks. `tools/docker.sh ninja progress` prints the per-file totals.

## Translations

`translations/` keeps the English text records and the translated artwork with their review history. It is a backup of that work, not a runtime asset pack, and no tool in the repository applies it to the game.
