---
type: handoff
updated: 2026-10-15
---

# Stopping point — 2026-10-15

The user requested a good stopping point with the work on main and current information in the wiki. The user subsequently requested compiling drafts for all remaining failures before stopping. That explicit repair work is active under T15700–T16000; the 80% goal remains incomplete and has not yet been paused. The recurring integration heartbeat is paused.

## Verified production state

- 5,945 / 6,958 game functions matched.
- **1,452,192 / 2,279,368 game-code bytes: 63.7%.**
- 80% threshold: 1,823,496 bytes; **371,304 bytes remain**.
- SDK library assembly is excluded consistently. Nonmatching drafts add no coverage.
- Latest full shared-header validation: all 27 binary checksums passed for `_schedule_init`; later RENSYU, SHOUGATU and EVENT changes passed their affected binary checks and header/global/prototype guards. Default linked proofs cover every promoted function.
- Evidence: `build/scratch/goal-schedule-init.log`, `goal-etc-repair-zero.log`, `goal-final-fresh-pair.log`, `goal-event-archive.log`, and `goal-stopping-validation.log` in the integration checkout. Relevant tickets preserve results on main even though build logs are local.

## Preserved drafts and unresolved work

Start with `drafts/README.md` and [[exhaustive-drafts]]. The frozen baseline `48362009` contained 1,066 unmatched scopes. Every one now has an explicit initial outcome: 29 zero candidates, 908 other compiling scores and 129 failures. All 29 initial zeros and the additional ETC repair zero have been promoted only after linked-byte verification.

The completed bounded repair passes select 30 zero candidates, 959 nonzero compiling alternatives and **77 scopes still lacking a compiling alternative**, with no absent initial scopes. These figures describe the frozen census, including functions subsequently matched. They are not the current production unmatched-function count. Compilation is also separate from reconstruction quality: residual artificial stack expressions, inferred layouts and placeholders are listed in the quality ledger. Every function does **not** yet have a usable reconstructed draft.

Source/body/context/provenance archives include all 33 independent DATE/type/unaligned repairs, automatic attempts, root matching alternatives, unfinished TACO and RPG probes, and the large EVENT work. Older uncommitted ENDING/wave source edits and external-tooling research were copied into `drafts/preserved-worktrees-2026-10-15` without modifying their original checkouts. Binaries, generated assembly, game data and caches are excluded. Failed and superseded source alternatives are retained, explicitly nonmatching.

The largest EVENT dispatcher remains nonmatching: T15200 reduced its reported diagnostic differences to 1,930; related handler 80118010 remains 26 diff rows / 13 register-only positions. The archive contains 29 alternatives and full contexts. No bytes were claimed for this work. The largest dialogue work and other known near misses remain documented in their original tickets; avoid repeating exhausted register-order experiments without new evidence.

## Resuming efficiently

1. Fetch main into a suitable isolated worktree. Original `/Users/lila/Documents/Projects/tokimemo-decomp` remains read-only for source integration; the integration checkout is `/Users/lila/Documents/Projects/tokimemo-wt/integration`.
2. Read this page, [[exhaustive-drafts]], the latest T14500 quality/best-result ledgers and the specific target ticket before repeating a probe.
3. Prioritize coherent repair of the 77 remaining compile failures and reconstruction flags, and transferable field/control-flow fixes. Eight additional hexadecimal-escape cases remain noted; the tested byte-preserving helper is retained. Such repairs are not automatically matches.
4. Use the existing Docker image and `tools/docker.sh` for all analysis/compiler Python. Reuse/hardlink immutable disc inputs; never copy whole disc images into new worktrees or hardlink writable generated assembly. Disk free space was approximately 17 GiB at handoff.
5. Reproduce archived drafts at their recorded baseline/context. Do not overwrite initial attempts. Promote only after explicit default linked proof and appropriate checksum/header/global/prototype checks, then push main without force.
6. If delegation is requested, use GPT-6.1 Sol medium as requested by the user. The explicitly requested compilation-repair workers are active; do not start unrelated matching experiments.

A fresh build/coverage check is `tools/docker.sh sh -c 'ninja -j2 && python3 tools/sync_protos.py --check-branch && python3 tools/progress.py'`. It does not replace per-function default literal proof for new changes.
