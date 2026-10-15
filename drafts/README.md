# Decompilation drafts and preserved work

These are source-only working archives, published so reconstruction work is not lost. **An archived draft is not a verified match.** Only promoted production C that passes the default linked-byte proof and binary checks contributes to coverage.

## Complete initial census

[T-14500](T-14500/) contains an outcome for every one of the 1,066 functions unmatched at frozen baseline `48362009`: 29 zero-score candidates, 908 other compiling candidates, and 129 failed initial attempts. Failed attempts remain explicit; this is not a claim that every function has a usable C draft.

- `catalog.jsonl`: immutable initial outcome index.
- Unit/function directories: generated or retained bodies, declarations, contexts, diagnostics and provenance where available.
- `baseline/`: original source/header snapshots.
- `repairs/` and `repairs-unaligned/`: independent alternatives, preserving the original attempt.
- `quality-audit.jsonl` and `quality-summary.json`: checkpoint of selected alternatives and unresolved reconstruction flags. These may lag a running repair pass; consult the corresponding wiki entry/commit.

“Compiling-audit-clear” only means the input compiles and does not trigger the listed residual patterns. It does not prove semantic equivalence or byte matching. Reproduction uses the existing project Docker workflow and the documented baseline, not host toolchain installs.

## Focused work

- `T-14501`: proof and normalization helpers retained from the promotion lane.
- `T-14800`, `T-14900`: ETC/RPG structural matching evidence and retained alternatives.
- `T-15200`: large EVENT dispatcher and related handler reconstructions, still nonmatching.
- `T-15300`: unfinished TACO reconstruction, contexts, headers and diagnostics.
- `T-15400`, `T-15500`: BUNKASAI encoding/bitfield alternatives and reproduction scripts.
- `T-15600`: RENSYU/SHOUGATU work and unresolved RPG delay-slot probes.
- `preserved-worktrees-2026-10-15`: older uncommitted source patches, complete modified files, base commits and external-tooling research, preserved without changing the original worktrees.

The archives deliberately exclude disc/game data, compiled binaries, generated assembly, disposable compiler intermediates and caches. Some scripts contain original checkout paths and require adapting those paths or checking out their recorded baseline. Per-ticket wiki records describe accepted matches and limitations.
